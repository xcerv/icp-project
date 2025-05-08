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

bool FsmNetworkManager::parsePacket(const QByteArray &data, QString &outName, QString &outValue)
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

FsmNetworkManager::FsmNetworkManager(QObject *parent)
    : QObject(parent)
{
    // New Socket
    m_udpSocket = new QUdpSocket(this);

    // Received packet ==> process signal
    connect(m_udpSocket, &QUdpSocket::readyRead, this, &FsmNetworkManager::processReceivedPacket);
}

FsmNetworkManager::~FsmNetworkManager()
{
    if (m_udpSocket != nullptr && m_udpSocket->state() != QAbstractSocket::UnconnectedState) 
    {
        m_udpSocket->close();
    }
}

bool FsmNetworkManager::startListening(const QHostAddress &address, quint16 port)
{
    // Is socket set?
    if(m_udpSocket == nullptr)
        return false;

    // Are we already listening?
    if(this->isActive())
        return false;

    if(!m_udpSocket->bind(address, port, QAbstractSocket::ReuseAddressHint))
        return false; // Failed to bind socket

    // Success!
    return true;
}

void FsmNetworkManager::processReceivedPacket()
{
    if(m_udpSocket == nullptr)
        return;

    while(m_udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(m_udpSocket->pendingDatagramSize());

        QHostAddress senderAddress;
        quint16 senderPort;

        auto bytesRx = m_udpSocket->readDatagram(datagram.data(), datagram.size(), &senderAddress, &senderPort);
        
        // Failed to obtain data ==> ignore
        if(bytesRx < 0)
            return;

        QString eventName;
        QString eventValue;

        if(parsePacket(datagram.data(), eventName, eventValue))
        {
            // Is valid packet ==> inform controller
            emit udpMessageReceived(eventName, eventValue);
        }
    }
}

void FsmNetworkManager::stopListening()
{
    if(this->isActive())
        m_udpSocket->close();
}

bool FsmNetworkManager::isActive()
{
    return m_udpSocket != nullptr && m_udpSocket->state() == QAbstractSocket::BoundState;;
}
