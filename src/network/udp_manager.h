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
#include <QNetworkDatagram>
#include <QUdpSocket>
#include <QHostAddress>
#include <QAbstractSocket>

#define DEFAULT_UDP_PORT 60000
#define DEFAULT_ADDRESS QHostAddress::LocalHost

/**
 * @brief Class for listening to input from network and converting it to format that FsmModel (interpreter) understands
 */
class FsmNetworkManager: public QObject
{
    Q_OBJECT

    protected:
        QUdpSocket *m_udpSocket; ///< UDP socket used by the receiver

        /**
         * @brief Parses input packet into two output strings
         * @param data The data of the packet
         * @param outName The name of the event sent through network (first string)
         * @param outValue The string value of the event sent through network (second string)
         * @return Returns true if parsing was successful, otherwise false
         */
        bool parsePacket(const QByteArray &data, QString &outName, QString &outValue);
    public:
        /**
         * @brief Constructor for UDP message receiver
         * @param parent The parent object
         */
        FsmNetworkManager(QObject *parent = nullptr);

        /**
         * @brief Destructor of UDP message receiver
         */
        ~FsmNetworkManager();

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

    signals:
        // Signal fired when a message was successfully received
        void udpMessageReceived(const QString &name, const QString &value);
    private slots:
        // Processed received packets
        void processReceivedPacket();

};

#endif