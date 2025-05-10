/**
 * Project name: ICP Project 2024/2025
 *
 * @file udp_manager.cpp
 * @author xcervia00
 *
 * @brief Implementation of UDP network communication utilised as FSM input
 *
 */

#include "udp_manager.h"

#include <QObject>
#include <QNetworkDatagram>
#include <QUdpSocket>
#include <QHostAddress>
#include <QAbstractSocket>
#include <QDataStream>
#include <QRegularExpression>
#include <cstdint>
#include <random>

#include "mvc_interface.h"
#include "view/editorwindow.h"

FsmNetworkManager::FsmNetworkManager(QObject *parent)
    : QObject(parent)
{
    if(parent != nullptr)
    {
        this->ownerObject = (FsmInterface*)qobject_cast<EditorWindow*>(parent);
    }

    // New Socket
    udpSocket = new QUdpSocket(this);

    // Received packet ==> process signal
    connect(udpSocket, &QUdpSocket::readyRead, this, &FsmNetworkManager::processReceivedPacket);
}

bool FsmNetworkManager::parseInput(const QByteArray &data, QString &outName, QString &outValue)
{
    if(data.isEmpty())
        return false;
        
    auto separatorName = data.indexOf('\0');
    if(separatorName < 0 || separatorName >= data.size()-1){
        // The input packet has no nulbytes ==> can't parse
        return false;
    }

    auto separatorValue = data.indexOf('\0', separatorName+1);
    if(separatorValue < 0 || separatorValue >= data.size()){
        // The input packet has no nulbytes ==> can't parse
        return false;
    }

    outName = QString(data.left(separatorName));
    outValue = QString(data.mid(separatorName+1, separatorValue - separatorName - 1));

    return true;
}

FsmNetworkManager::~FsmNetworkManager()
{
    if(isListening)
    {
        cancelServer();
    }
    else if(isConnected)
    {
        cancelClient();
    }
    else if(isActive())
    {
        stopListening();
    }
}

bool FsmNetworkManager::startListening(const QHostAddress &address, quint16 port)
{
    // Is socket set?
    if(udpSocket == nullptr)
        return false;

    // Are we already listening?
    if(this->isActive())
        return false;

    if(!udpSocket->bind(address, port, QAbstractSocket::ReuseAddressHint))
        return false; // Failed to bind socket

    // Success!
    return true;
}

void FsmNetworkManager::processReceivedPacket()
{

    while(this->isActive() && udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());

        NetworkEndpoint sender;

        auto bytesRx = udpSocket->readDatagram(datagram.data(), datagram.size(), &sender.address, &sender.port);
        
        // Failed to obtain data ==> ignore
        if(bytesRx <= 0)
            continue;

        // Ignore unknown unregistered clients
        if(isListening && !clientAddresses.contains(sender) && MSG_TYPE(datagram.data()) != UDP_CONNECT)
        {
            continue;
        }

        qInfo() << "Network: Received input from " << sender.address.toString() << " via " << sender.port << " of type" << MSG_TYPE(datagram.data());

        // Respond depending on message
        switch(MSG_TYPE(datagram.data()))
        {
            case UDP_MESSAGE_TYPE::UDP_CONNECT:
                this->messageConnect(sender, datagram);
                break;
            case UDP_MESSAGE_TYPE::UDP_DISCONNECT:
                this->messageDisconnect(sender, datagram);
                break;
            case UDP_MESSAGE_TYPE::UDP_INPUT:
                this->messageInput(sender, datagram);
                break;
            case UDP_MESSAGE_TYPE::UDP_INTER_STATE:
                this->messageInterState(sender, datagram);
                break;
            case UDP_MESSAGE_TYPE::UDP_SYNC_REQUEST:
                this->messageSyncRequest(sender, datagram);
                break;
            case UDP_MESSAGE_TYPE::UDP_SYNC_EXECUTE:
                this->messageSyncExecute(sender, datagram);
                break;
            default:
                break;
        }
    }
}

void FsmNetworkManager::stopListening()
{
    if(udpSocket != nullptr && udpSocket->state() != QAbstractSocket::UnconnectedState) 
    {
        udpSocket->close();
    }
}

bool FsmNetworkManager::isActive()
{
    return udpSocket != nullptr && udpSocket->state() == QAbstractSocket::BoundState;
}

NETWORK_MANAGER_STATE FsmNetworkManager::getState() const
{
    return this->state;
}

bool FsmNetworkManager::getInitiation()
{
    return this->startedExternally;
}

void FsmNetworkManager::setAddress(const QHostAddress &address, quint16 port)
{
    this->serverAddress.address = address;
    this->serverAddress.port = port;
}

const NetworkEndpoint &FsmNetworkManager::getServerInfo() const
{
    return serverAddress;
}

void FsmNetworkManager::messageDisconnect(const NetworkEndpoint &sender, const QByteArray &data)
{
    // Incorrect data size
    if(data.size() != sizeof(UDP_MESSAGE_TYPE::UDP_DISCONNECT))
        return;

    // Client
    if(isConnected)
    {   
        static_cast<EditorWindow*>(this->ownerObject)->networkClientStop();

    } // Server
    else if(isListening)
    {
        this->clientAddresses.remove(sender);
    }
}

void FsmNetworkManager::messageConnect(const NetworkEndpoint &sender, const QByteArray &data)
{
    // Incorrect data size
    if(data.size() != sizeof(UDP_MESSAGE_TYPE::UDP_CONNECT))
        return;

    // Receiver must be server
    if(!isListening)
        return;

    // Register the sender
    this->clientAddresses.insert(sender);
}

void FsmNetworkManager::messageSyncRequest(const NetworkEndpoint &sender, const QByteArray &data)
{
    if(data.size() != sizeof(UDP_MESSAGE_TYPE::UDP_SYNC_REQUEST))
        return;
    
    // Only server may respond
    if(!isListening)
        return;

    // Construct the message
    QString msg;
    QTextStream stream(&msg, QIODevice::WriteOnly);
    stream << static_cast<uint8_t>(UDP_MESSAGE_TYPE::UDP_SYNC_EXECUTE);

    // Load current representation to stream
    this->ownerObject->saveStream(stream);
    stream.flush();

    // Remove duplicite whitespaces
    msg.replace(QRegularExpression("[ \t]+"), " ");
    MSG_TYPE(msg.data()) = static_cast<uint8_t>(UDP_MESSAGE_TYPE::UDP_SYNC_EXECUTE);

    // Send the representation back to sender (without duplicite whitespaces)
    this->udpSocket->writeDatagram(msg.toLocal8Bit(), sender.address, sender.port);
}

void FsmNetworkManager::messageSyncExecute(const NetworkEndpoint &sender, const QByteArray &data)
{
    qInfo() << "Network: Syncing with remote server at " << sender.address.toString() << " via " << sender.port;

    if(sender != serverAddress)
        return;

    // Client only
    if(!isConnected)
        return;

    // At least one byte of content
    if((unsigned long)data.size() <= sizeof(UDP_MESSAGE_TYPE::UDP_SYNC_EXECUTE))
        return;

    // Ignore message type byte
    QTextStream stream(data.mid(sizeof(UDP_MESSAGE_TYPE::UDP_SYNC_EXECUTE)), QIODevice::ReadOnly);
    stream.flush();

    // Load the representation from the packet
    this->ownerObject->loadStream(stream);
}

void FsmNetworkManager::messageInput(const NetworkEndpoint &sender, const QByteArray &data)
{
    // Must be some content
    if((unsigned long)data.size() < sizeof(UDP_MESSAGE_TYPE::UDP_INPUT)+4)
        return;

    QString eventName;
    QString eventValue;

    // Parse into event
    if(!parseInput(data.mid(sizeof(UDP_MESSAGE_TYPE::UDP_INPUT)), eventName, eventValue))
    {
        return;
    }    

    // Client
    if(isConnected)
    {
        this->ownerObject->inputEvent(eventName, eventValue);

    } // Server
    else if(isListening)
    {
        QByteArray msg;
        msg.append(static_cast<int>(UDP_MESSAGE_TYPE::UDP_INPUT));
        msg.append(eventName.toLocal8Bit());
        msg.append('\0');
        msg.append(eventValue.toLocal8Bit());
        msg.append('\0');
        
        // Send message to all clients
        SERVER_FOR_ALL(
            // Don't resend back to the sender
            if(client != sender)
            {
                this->udpSocket->writeDatagram(msg, client.address, client.port);
            }
        );

        this->ownerObject->inputEvent(eventName, eventValue);
    }
}

void FsmNetworkManager::messageInterState(const NetworkEndpoint &sender, const QByteArray &data)
{
    (void)sender;

    if(data.size() != sizeof(UDP_MESSAGE_TYPE::UDP_INTER_STATE) + sizeof(UDP_BOOL::SIZE))
        return;

    // Must be client or server
    if(!isConnected && !isListening)
        return;

    // Stop or start interpretation based on the input
    if(MSG_BOOL_VAL(data.data()) == UDP_BOOL::TRUE)
    {
        if(isConnected)
        {
            this->startedExternally = true;
        }

        this->ownerObject->startInterpretation();
    }
    else if(MSG_BOOL_VAL(data.data()) == UDP_BOOL::FALSE)
    {
        if(isConnected)
        {
            this->startedExternally = false;
        }

        this->ownerObject->stopInterpretation();
    }
}

void FsmNetworkManager::actionInterState(bool interpret)
{
    uint8_t udp_bool = interpret ? UDP_BOOL::TRUE : UDP_BOOL::FALSE;
    CONSTRUCT_DATAGRAM(msg, UDP_INTER_STATE << udp_bool);

    // Client
    if(isConnected)
    {
        this->startedExternally = false;
        this->udpSocket->writeDatagram(msg, serverAddress.address, serverAddress.port);

    } // Server
    else if(isListening)
    {
        this->actionSyncExecute();

        SERVER_FOR_ALL(
            this->udpSocket->writeDatagram(msg, client.address, client.port);
        );
    }
}

void FsmNetworkManager::actionInput(const QString &name, const QString &value)
{
    QByteArray msg;
    msg.append(static_cast<int>(UDP_MESSAGE_TYPE::UDP_INPUT));
    msg.append(name.toLocal8Bit());
    msg.append('\0');
    msg.append(value.toLocal8Bit());
    msg.append('\0');

    // Client
    if(isConnected)
    {   
        this->udpSocket->writeDatagram(msg, serverAddress.address, serverAddress.port);

    } // Server
    else if(isListening)
    {       
        // Send message to all clients
        SERVER_FOR_ALL(
            this->udpSocket->writeDatagram(msg, client.address, client.port);
        );
    }
}

void FsmNetworkManager::actionForceDisconnect(const NetworkEndpoint &target)
{
    if(isListening || isConnected)
    {   
        CONSTRUCT_DATAGRAM(msg, UDP_DISCONNECT);

        // Send the disconnect message
        this->udpSocket->writeDatagram(msg, target.address, target.port);

        // if(isListening){
        //     // Unlink target from set
        //     this->clientAddresses.remove(target);
        // }
    }
}

void FsmNetworkManager::actionSyncRequest()
{
    // Client only
    if(!isConnected)
        return;

    CONSTRUCT_DATAGRAM(msg, UDP_SYNC_REQUEST);
    this->udpSocket->writeDatagram(msg, serverAddress.address, serverAddress.port);
}

void FsmNetworkManager::actionSyncExecute()
{
    // Server only
    if(!isListening)
        return;

    // Optimisation ==> do nothing if there are no clients
    if(this->clientAddresses.isEmpty())
        return;

    QString msg;
    QTextStream stream(&msg, QIODevice::WriteOnly);
    stream << static_cast<uint8_t>(UDP_MESSAGE_TYPE::UDP_SYNC_EXECUTE);

    // Load current representation to stream
    this->ownerObject->saveStream(stream);
    stream.flush();

    // Remove duplicite whitespaces
    msg.replace(QRegularExpression("[ \t]+"), " ");
    MSG_TYPE(msg.data()) = static_cast<uint8_t>(UDP_MESSAGE_TYPE::UDP_SYNC_EXECUTE);
    auto bytes = msg.toLocal8Bit();

    // Send the representation back to all clients (without duplicite whitespaces)
    SERVER_FOR_ALL(
        this->udpSocket->writeDatagram(bytes, client.address, client.port);
    );
}

void FsmNetworkManager::actionConnect()
{
    // Client only
    if(!isConnected)
        return;

    CONSTRUCT_DATAGRAM(msgConnect, UDP_CONNECT);
    // Send the connect message
    this->udpSocket->writeDatagram(msgConnect, serverAddress.address, serverAddress.port);

    CONSTRUCT_DATAGRAM(msgSync, UDP_SYNC_REQUEST);
    // Send the sync request
    this->udpSocket->writeDatagram(msgSync, serverAddress.address, serverAddress.port);
}

void FsmNetworkManager::enableClient()
{
    if(serverAddress.address == QHostAddress::AnyIPv4)
    {
        this->ownerObject->throwError(ERROR_NETWORK_GENERIC, "Network: Client can't connect to \'Any\'");
        return;
    }

    // The source port is selected dynamically between 60001 and MAX
    std::random_device dev;
    std::mt19937 rng;
    rng.seed(dev());
    std::uniform_int_distribution<quint16> dist(DEFAULT_UDP_PORT, UINT16_MAX);

    auto randomPort = static_cast<quint16>(dist(rng));
    this->startListening(serverAddress.address, randomPort);
    this->isConnected = true;
    this->state = NETWORK_MANAGER_STATE::CLIENT;
    
    // Register at server
    this->actionConnect();
}

void FsmNetworkManager::cancelClient()
{
    // Let server know client is cancelled
    if(isConnected)
    {
        this->actionForceDisconnect(serverAddress);
    }

    this->stopListening();
    this->isConnected = false;
    this->state = NETWORK_MANAGER_STATE::NONE;
}

void FsmNetworkManager::enableServer()
{
    this->startListening(serverAddress.address, serverAddress.port);
    this->isListening = true;
    this->state = NETWORK_MANAGER_STATE::SERVER;
}

void FsmNetworkManager::cancelServer()
{
    // Server was on
    if(isListening)
    {
        SERVER_FOR_ALL(
            this->actionForceDisconnect(client);
        );
    }
    this->clientAddresses.clear();
    this->stopListening();
    this->isListening = false;
    this->state = NETWORK_MANAGER_STATE::NONE;
}