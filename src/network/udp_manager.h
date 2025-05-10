/**
 * Project name: ICP Project 2024/2025
 *
 * @file udp_manager.h
 * @author xcervia00
 *
 * @brief Implementation of UDP network communication utilised as FSM input
 *
 */


#ifndef UDP_MANAGER_H_
#define UDP_MANAGER_H_

#include <QObject>
#include <QSet>
#include <QNetworkDatagram>
#include <QUdpSocket>
#include <QHostAddress>
#include <QAbstractSocket>

#include "mvc_interface.h"

// Extract parameter from byte array
#define MSG_EXTRACT_PARAM(msg, param) (((uint8_t*)(msg))[UDP_PARAM_POSITIONS::param])
// Extract message bool value from byte message
#define MSG_BOOL_VAL(msg) MSG_EXTRACT_PARAM((msg), BOOL)
// Extract message type from byte message
#define MSG_TYPE(msg) MSG_EXTRACT_PARAM((msg), TYPE)

enum NETWORK_MANAGER_STATE : uint8_t
{
    SERVER,
    CLIENT,
    NONE
};

/**
 * @brief Defines the locations of network parameters
 */
enum UDP_PARAM_POSITIONS : uint8_t
{
    TYPE = 0,
    BOOL = 1,
};

/**
 * @brief Representation of boolean in UDP message
 */
enum UDP_BOOL : uint8_t
{
    FALSE = 0,
    TRUE = 1,
    SIZE
};

/**
 * @brief Types of UDP messages
 */
enum UDP_MESSAGE_TYPE : uint8_t
{
    UDP_CONNECT, // Client wants to connect to server
    UDP_DISCONNECT, // Client wants to disconnect from server
    UDP_INPUT, // Input event
    UDP_INTER_STATE, // Interpretation state
    UDP_SYNC_REQUEST, // Request for sync
    UDP_SYNC_EXECUTE, // Synchronize the client
};

// Default port used by UDP manager
#define DEFAULT_UDP_PORT 60000
// Default address used by UDP manager
#define DEFAULT_ADDRESS QHostAddress::LocalHost

// Constructs a datagram
#define CONSTRUCT_DATAGRAM(var, ...) QByteArray var; do{QDataStream __udp_stream__(&var, QIODevice::WriteOnly); __udp_stream__ << __VA_ARGS__;}while(0)

// Perform code on all clients
#define SERVER_FOR_ALL(...) do{for(auto &client : this->clientAddresses){__VA_ARGS__}}while(0)

/**
 * @brief Structure used for representing both address and its ip
 */
struct NetworkEndpoint
{   
    QHostAddress address; ///< The IPv4 address of an endpoint
    quint16 port; ///< The port of an endpoint

    NetworkEndpoint(const QHostAddress& addr = DEFAULT_ADDRESS, quint16 p = DEFAULT_UDP_PORT)
    : address(addr), port(p) {}

    // Comparison == op.
    bool operator==(const NetworkEndpoint& other) const {
        return address == other.address && port == other.port;
    }

    // Comparison != op.
    bool operator!=(const NetworkEndpoint& other) const {
        return address != other.address || port != other.port;
    }
};

/**
 * @brief Custom hash calculation for addresses
 * @param endpoint The endpoint address used
 * @param seed Random seed
 * @return The hash
 */
inline uint qHash(const NetworkEndpoint& endpoint, uint seed = 0) {
    return qHash(endpoint.address, seed) ^ qHash(endpoint.port, seed << 1);
};

/**
 * @brief Class for listening to input from network and converting it to format that FsmModel (interpreter) understands
 */
class FsmNetworkManager: public QObject
{
    Q_OBJECT

    protected:
        NETWORK_MANAGER_STATE state = NONE; ///< The current state

        QUdpSocket *udpSocket; ///< UDP socket used by the receiver

        NetworkEndpoint serverAddress; ///< The server address this manager is connected to
        QSet<NetworkEndpoint> clientAddresses; ///< The list of addresses the server updates

        bool isListening = false; ///< Is the manager currently listening for client Input
        bool isConnected = false; ///< Is the manager currently connected to a server

        bool startedExternally = false;

        FsmInterface * ownerObject = nullptr; ///< Pointer to the owning interface

    public:
        /**
         * @brief Constructor for UDP message receiver
         * @param parent The parent object ==> must be set
         */
        FsmNetworkManager(QObject *parent = nullptr);

        /**
         * @brief Destructor of UDP message receiver
         */
        virtual ~FsmNetworkManager();

        /**
         * @brief Begins to listen with UDP packet
         * @param address The address to listen on (localhost by default)
         * @param port The port to listen on (60000 by default)
         * @return Returns true if began listening after the function was called, otherwise false
         * @note Uses SO_REUSEADDR option
         */
        bool startListening(const QHostAddress &address = DEFAULT_ADDRESS, quint16 port = DEFAULT_UDP_PORT);

        /**
         * @brief Stops listening (nop if wasn't listening)
         */
        void stopListening();

        /**
         * @brief Is socket currently listening?
         * @return Returns true if it is listening, otherwise false
         */
        bool isActive();

        /**
         * @brief Get current state of the network manager
         * @return Returns the state (client, server, none)
         */
        NETWORK_MANAGER_STATE getState() const;

        
        /**
         * @brief Did this manager initiate network communication
         * @return True if it initiated, otherwise false
         */
        bool getInitiation();

        /**
         * @brief Sets the server address (either for listening or for receiving)
         * @param address The address to set
         * @param port The port to set
         */
        void setAddress(const QHostAddress &address, quint16 port);

        /**
         * @brief Returns information about the server
         * @return NetworkEndpoint info (address and port)
         */
        const NetworkEndpoint &getServerInfo() const;

        /* 
         =======================
         =   Received message
         =======================
        */

        /**
         * @brief Disconnection request
         * @param sender Who send this message
         * @param data Data of the message
         */
        void messageDisconnect(const NetworkEndpoint &sender, const QByteArray &data);
        /**
         * @brief Connection request
         * @param sender Who send this message
         * @param data Data of the message
         */
        void messageConnect(const NetworkEndpoint &sender, const QByteArray &data);
        /**
         * @brief Request to sync with sender
         * @param sender Who send this message
         * @param data Data of the message
         */
        void messageSyncRequest(const NetworkEndpoint &sender, const QByteArray &data);
        /**
         * @brief Sync with the sender
         * @param sender Who send this message
         * @param data Data of the message
         */
        void messageSyncExecute(const NetworkEndpoint &sender, const QByteArray &data);

        /**
         * @brief Input event 
         * @param sender Who send this message
         * @param data Data of the message
         */
        void messageInput(const NetworkEndpoint &sender, const QByteArray &data);

        /**
         * @brief Interpreter state change
         * @param sender Who send this message
         * @param data Data of the message
         */
        void messageInterState(const NetworkEndpoint &sender, const QByteArray &data);

        /* 
         ======================
         =   Network Actions
         ======================
        */

        /**
         * @brief Starts/Stops interpretation of remote server/all clients
         * @param state The state to set (true if start, false if stop)
         */
        void actionInterState(bool interpret);
        /**
         * @brief Sends input event to given server/all clients
         * @param name The name of the input event
         * @param value The value of the input
         */
        void actionInput(const QString &name, const QString &value);

        /**
         * @brief Force disconnects a client
         */
        void actionForceDisconnect(const NetworkEndpoint &target);

        /**
         * @brief Requests sync from server
         */
        void actionSyncRequest();

        /**
         * @brief Explicitly syncs all the clients
         */
        void actionSyncExecute();

        /**
         * @brief Client registers to server
         */
        void actionConnect();

        /* 
         =================================
         =   Network connection handling
         =================================
        */

        /**
         * @brief Client cancels connection
         */
        void cancelClient();
        /**
         * @brief Client enables connection to server 
         */
        void enableClient();

        /**
         * @brief Server is set to enabled (listening)
         */
        void enableServer();

        /**
         * @brief Server is set to disabled (not listening anymore)
         */
        void cancelServer();

    signals:
        // Signal fired when a message was successfully received
        void udpMessageReceived(const QString &name, const QString &value);
    private slots:
        // Processed received packets
        void processReceivedPacket();

    protected:
        /**
         * @brief Parses input packet into two output strings
         * @param data The data of the packet
         * @param outName The name of the event sent through network (first string)
         * @param outValue The string value of the event sent through network (second string)
         * @return Returns true if parsing was successful, otherwise false
         */
        bool parseInput(const QByteArray &data, QString &outName, QString &outValue);

};

#endif