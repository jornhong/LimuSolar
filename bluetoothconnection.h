#ifndef BLUETOOTHCONNECTION_H
#define BLUETOOTHCONNECTION_H

#include <QObject>
#include <QBluetoothDeviceInfo>
#include <QQmlParserStatus>
#include <QLowEnergyController>
#include <QLowEnergyService>
#include <QLowEnergyDescriptor>
#include <QBluetoothUuid>
#include <QTimer>
#include <QDateTime>


#define SERVICE_UUID 0xfff0
#define HANDSHARK_TIMEOUT 6000

#define SLIP_RECV_TIMEOUT 500

#define DELTA 0x9e3779b9
#define MX (((z>>5^y<<2) + (y>>3^z<<4)) ^ ((sum^y) + (key[(p&3)^e] ^ z)))


class BluetoothConnection : public QObject,public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
    Q_PROPERTY(ConnectionState state READ state NOTIFY stateChanged)
    Q_PROPERTY(QVariantMap statics READ statics NOTIFY staticsChanged)
    Q_PROPERTY(Error lastError READ lastError NOTIFY errorChanged)
public:
    static void btea(quint32 *v,int n,quint32 const key[4]);
    static void dump(const QByteArray &value);
    static quint16 crc16(const QByteArray &value);
    explicit BluetoothConnection(QObject *parent = 0);
    ~BluetoothConnection();
    enum Error{BluetoothNoError,BluetoothConnectionError,BluetoothServiceNotFoundError,BluetoothCharateristicError,BluetoothDeviceInvalidError,BluetoothUnknowError};
    enum ConnectionState{CSDisconnected,CSConnecting,CSConnected,CSDiscoveringService,CSServiceDiscovered,CSHandshark,CSHandsharked,CSError};
    enum CommunicationState{CommIdle,CommInit,CommHanding,CommWaitingAck,CommHanded,CommError,CommEnd};
    bool connected() const;
    Q_INVOKABLE void connect(const QBluetoothDeviceInfo &deviceInfo);
    Q_INVOKABLE void disconnect();
    Q_INVOKABLE void sendData(const QVariantList &data);
    Q_INVOKABLE QVariantList genTargetKey(const QVariantList &uuid) const;

    QByteArray slipEncode(const QByteArray &in) const;
    QByteArray slipDecode(const QByteArray &in) const;
    QByteArray writeFrame(quint8 cmd,const QByteArray &data) const;
    QVariantMap statics() const;
    ConnectionState state() const;
    Error lastError() const;
    Q_ENUMS(Error)
    Q_ENUMS(ConnectionState)
    bool writeToUART(const QByteArray &data);
    bool writeDataToUart(const QByteArray &data);

private:
    bool mConnected;
    QLowEnergyController *mControl;
    QLowEnergyService *mService;
    bool foundUARTService;
    QTimer *timer;
    QVariantMap mStatics;
    QByteArray mRecvBuffer;
    int mRecvCounter;

    CommunicationState mCommState;
    CommunicationState mPrevCommState;
    QDateTime tempDateTime;

    quint32 privateKey[4]={0x40476491,0x79520980,0x11627080,0x28559885};
    quint32 mSessionKey[2];

    bool mHanded;  //是否握手完成
    quint8 protocol;

    quint8 mDevType;   //0:unknow,1:IRDevice,2:directDevice
    quint8 mBatteryLevel; //0xFF:not support,otherwise:battery level
    quint8 mDevModel; //0x00:unknow;
    quint8 mTargetModel; //0x00:unknow;

    ConnectionState mState;
    Error mLastError;
    quint64 lastRecv;

private slots:
    void serviceDiscovered(const QBluetoothUuid &gatt);
    void serviceScanDone();
    void controllerError(QLowEnergyController::Error error);
    void deviceConnected();
    void deviceDisconnected();

    void frameReceved(const QByteArray &frame);

    void serviceStateChanged(QLowEnergyService::ServiceState s);
    void recvUARTData(const QLowEnergyCharacteristic &c,
                              const QByteArray &value);
    void onTimerTriggered();

protected:
    virtual void classBegin();
    virtual void componentComplete();

signals:
    void connectedChanged(const bool &);
    void staticsChanged(QVariantMap &);
    void disconnected();
    void stateChanged(const ConnectionState &state);
    void errorChanged(const Error &error);
    void recevFrame(const QByteArray &frame);
    void userFrame(const QVariantList &frame);
    void protocolChanged(int protocol);

public slots:
};

#endif // BLUETOOTHCONNECTION_H
