#include "bluetoothconnection.h"
#include <QBluetoothAddress>
#include <QDebug>
#include <QDateTime>
#include "protocols.h"
#ifdef _ANDROID_OS_
#include <arpa/inet.h>
#endif
#include <QThread>

void BluetoothConnection::btea(quint32 *v, int n, const quint32 key[])
{
    uint32_t y, z, sum;
    unsigned p, rounds, e;
    if (n > 1) {          /* Coding Part */
        rounds = 6 + 52/n;
        sum = 0;
        z = v[n-1];
        do {
            sum += DELTA;
            e = (sum >> 2) & 3;
            for (p=0; p<n-1; p++) {
                y = v[p+1];
                z = v[p] += MX;
            }
            y = v[0];
            z = v[n-1] += MX;
        } while (--rounds);
    } else if (n < -1) {  /* Decoding Part */
        n = -n;
        rounds = 6 + 52/n;
        sum = rounds*DELTA;
        y = v[0];
        do {
            e = (sum >> 2) & 3;
            for (p=n-1; p>0; p--) {
                z = v[p-1];
                y = v[p] -= MX;
            }
            z = v[n-1];
            y = v[0] -= MX;
            sum -= DELTA;
        } while (--rounds);
    }
}

void BluetoothConnection::dump(const QByteArray &value)
{
    QString str;
    QString dp;
   for(int i=0;i<value.size();i++)
   {
       str = " 0x" + QString::number(value[i] & 0xFF,16);
       dp += str;
   }
   qDebug() << "dump:" << dp;
}

quint16 BluetoothConnection::crc16(const QByteArray &value)
{
    quint16 tmp;
    quint16 crc=0xffff;
    for(int i=0;i<value.size();i++)
        {
    //    i= *ptr;
        tmp=(quint16)(value[i] & 0xFF);
        crc=crc^tmp;
        for(int j=0;j<8;j++)
            {
            if(crc & 0x1)
                {
                crc>>=1;
                crc^=0xa001;
                }
            else
                crc>>=1;
            }
        }
    return(crc);
}

BluetoothConnection::BluetoothConnection(QObject *parent)
    : QObject(parent),
      mConnected(false),
      mControl(0),
      mService(0),
      mRecvCounter(0),
      foundUARTService(false),
      mState(CSDisconnected),
      mLastError(BluetoothNoError),
      mCommState(CommIdle),
      mPrevCommState(CommIdle),
      mHanded(false),
      mDevType(0),
      mBatteryLevel(0xff),
      mDevModel(0),
      mTargetModel(0),
      protocol(1),
      lastRecv(0)
{
    mStatics["session_name"] = "";
    mStatics["session_address"] = "";
    mStatics["session_rx_size"] = 0;
    mStatics["session_tx_size"] = 0;
    mStatics["session_time"] = QDateTime::currentDateTime();
    mStatics["session_rx"] = 0;
    mStatics["session_tx"] = 0;
    mStatics["session_rx_error"] = 0;
    mStatics["session_tx_error"] = 0;
    timer = new QTimer(this);
    timer->start(50);
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(onTimerTriggered()));
    QObject::connect(this,SIGNAL(recevFrame(QByteArray)),this,SLOT(frameReceved(QByteArray)));
}

BluetoothConnection::~BluetoothConnection()
{
    timer->stop();
    delete timer;
}

bool BluetoothConnection::connected() const
{
    return mConnected;
}

void BluetoothConnection::connect(const QBluetoothDeviceInfo &deviceInfo)
{
    foundUARTService = false;
    qDebug() << "on bluetootheconnection::connect";

    if (mControl) {
        mControl->disconnectFromDevice();
        delete mControl;
        mControl = 0;
    }

    mControl = new QLowEnergyController(deviceInfo, this);
    QObject::connect(mControl, SIGNAL(serviceDiscovered(QBluetoothUuid)),
                     this, SLOT(serviceDiscovered(QBluetoothUuid)));
    QObject::connect(mControl, SIGNAL(discoveryFinished()),
                     this, SLOT(serviceScanDone()));
    QObject::connect(mControl, SIGNAL(error(QLowEnergyController::Error)),
                     this, SLOT(controllerError(QLowEnergyController::Error)));
    QObject::connect(mControl, SIGNAL(connected()),
                     this, SLOT(deviceConnected()));
    QObject::connect(mControl, SIGNAL(disconnected()),
                     this, SLOT(deviceDisconnected()));

    mStatics["session_name"] = deviceInfo.name();
    mStatics["session_address"] = deviceInfo.address().toString();
    qDebug() << "connect to device!";

    mState = CSConnecting;
    mPrevCommState = CommIdle;
    mCommState = CommIdle;
    mHanded = false;

    Q_EMIT stateChanged(mState);
    Q_EMIT staticsChanged(mStatics);

    mControl->connectToDevice();
}

void BluetoothConnection::disconnect()
{
    if (mControl) {
        mControl->disconnectFromDevice();
        //delete mControl;
        //mControl = 0;

    }
    mState = CSDisconnected;
    mCommState = CommIdle;
    mPrevCommState = CommIdle;
    Q_EMIT stateChanged(mState);
}

void BluetoothConnection::sendData(const QVariantList &data)
{
    QByteArray byteData;
    foreach(const QVariant &d,data)
    {
        quint8 byte = (quint8)(d.toInt() & 0xff);
        byteData.append(byte);
    }
    writeDataToUart(byteData);
}

QVariantList BluetoothConnection::genTargetKey(const QVariantList &uuid) const
{
    quint32 uuids[2];
    quint8 *i = (quint8 *)&uuids[0];
    QVariantList outBytes;
    foreach(const QVariant &d,uuid)
    {
        *(i++) = (quint8)(d.toInt() & 0xff);
    }

#if defined(_IOS_OS_) || defined(_ANDROID_OS_)
    uuids[0] = ntohl(uuids[0]);
    uuids[1] = ntohl(uuids[1]);
#endif

    btea(&uuids[0],2,privateKey);
    i = (quint8 *)&uuids[0];
#if defined(_IOS_OS_) || defined(_ANDROID_OS_)
    uuids[0] = ntohl(uuids[0]);
    uuids[1] = ntohl(uuids[1]);
#endif

    for(int j=0;j<8;j++)
    {
        outBytes.append((*(i++)) & 0xFF);
    }
    return outBytes;
}

QByteArray BluetoothConnection::slipEncode(const QByteArray &in) const
{
    QByteArray inBytes;
    inBytes.append(0xC0);
    for(int i=0;i<in.size();i++)
    {
        uchar byte = (in[i] & 0xFF);
        switch(byte)
        {
        case 0xC0:
            inBytes.append(0xDB);
            inBytes.append(0xDC);
            break;
        case 0xDB:
            inBytes.append(0xDB);
            inBytes.append(0xDD);
            break;
        default:
            inBytes.append(byte);
        }
    }
    inBytes.append(0xC0);
    return inBytes;
}

QByteArray BluetoothConnection::slipDecode(const QByteArray &in) const
{
    QByteArray outBytes;
    bool ESCOcurse=false;
    for(int i=0;i<in.size();i++)
    {
        uchar byte = (in[i] & 0xFF);
        if(byte == 0xDB)
        {
            ESCOcurse = true;
        }else{
            if((byte == 0xDC)&&ESCOcurse)
            {
                outBytes.append(0xC0);
            }else if((byte == 0xDD)&&ESCOcurse)
            {
                outBytes.append(0xDB);
            }else{
                outBytes.append(byte);
            }
            ESCOcurse = false;
        }
    }
    return outBytes;
}

QByteArray BluetoothConnection::writeFrame(quint8 cmd, const QByteArray &data) const
{
    QByteArray frame;
    frame.append((char)cmd);
    frame.append((char)data.size());
    frame.append(data);
    quint16 crc = BluetoothConnection::crc16(frame);
    frame.append(crc >> 8);
    frame.append(crc & 0x00FF);
    return slipEncode(frame);
}

QVariantMap BluetoothConnection::statics() const
{
    return mStatics;
}

BluetoothConnection::ConnectionState BluetoothConnection::state() const
{
    return mState;
}

BluetoothConnection::Error BluetoothConnection::lastError() const
{
    return mLastError;
}

bool BluetoothConnection::writeToUART(const QByteArray &data)
{
    if(!mService)
    {
        qDebug() << "not service found!";
        return false;
    }
    const QLowEnergyCharacteristic uart = mService->characteristic(
                QBluetoothUuid((QBluetoothUuid::CharacteristicType)0xfff1));
    if (!uart.isValid()) {
        qDebug() << "uart characteristic not found!";
        return false;
    }
    //qDebug() << "write to uart:" << data << " thread:" << QThread::currentThreadId();
    mService->writeCharacteristic(uart,data);
    quint64 txSize = mStatics["session_tx_size"].toLongLong();
    txSize += data.size();
    quint64 txCount = mStatics["session_tx"].toLongLong();
    ++txCount;
    mStatics["session_tx_size"] = txSize;
    mStatics["session_tx"] = txCount;
    Q_EMIT staticsChanged(mStatics);
    return true;
}

bool BluetoothConnection::writeDataToUart(const QByteArray &data)
{
    QByteArray frame;
    frame.append(data);
    quint16 crc = BluetoothConnection::crc16(frame);
    frame.append(crc >> 8);
    frame.append(crc & 0x00FF);
    return writeToUART(slipEncode(frame));

}


void BluetoothConnection::serviceDiscovered(const QBluetoothUuid &gatt)
{

    QBluetoothUuid::ServiceClassUuid softSerial =(QBluetoothUuid::ServiceClassUuid) SERVICE_UUID;
    if (gatt == QBluetoothUuid(softSerial)) {
        qDebug() << "get service!!!!!";
        foundUARTService = true;
    }

}

void BluetoothConnection::serviceScanDone()
{
    if(mService)
    {
        delete mService;
        mService = 0;
    }

    //! [Filter HeartRate service 2]
    if (foundUARTService) {
        QBluetoothUuid::ServiceClassUuid softSerial =(QBluetoothUuid::ServiceClassUuid) SERVICE_UUID;
        qDebug() << "connect to service ....";
        mService = mControl->createServiceObject(
                    QBluetoothUuid(softSerial), this);

    }else{
        qDebug() << "uart service not found!";
    }
    if(!mService)
    {
        mLastError = BluetoothServiceNotFoundError;
        mState = CSError;
        Q_EMIT errorChanged(mLastError);
        Q_EMIT stateChanged(mState);
        disconnect();
        return;
    }



    QObject::connect(mService, SIGNAL(stateChanged(QLowEnergyService::ServiceState)),
                     this, SLOT(serviceStateChanged(QLowEnergyService::ServiceState)));
    QObject::connect(mService, SIGNAL(characteristicChanged(QLowEnergyCharacteristic,QByteArray)),
                     this, SLOT(recvUARTData(QLowEnergyCharacteristic,QByteArray)));

    mStatics["session_time"] = QDateTime::currentDateTime();
    mStatics["session_rx_size"] = 0;
    mStatics["session_tx_size"] = 0;
    mStatics["session_rx"] = 0;
    mStatics["session_tx"] = 0;
    mStatics["session_rx_error"] = 0;
    mStatics["session_tx_error"] = 0;
    mService->discoverDetails();
    Q_EMIT staticsChanged(mStatics);

}

void BluetoothConnection::controllerError(QLowEnergyController::Error error)
{
    switch(error)
    {
    case QLowEnergyController::ConnectionError:
        mLastError = BluetoothConnectionError;
        Q_EMIT errorChanged(mLastError);
        // Q_EMIT this->error(BluetoothConnectionError);
        break;
    default:
        mLastError = BluetoothUnknowError;
        Q_EMIT errorChanged(mLastError);
        break;
    }
    mState = CSError;
    Q_EMIT stateChanged(mState);

}

void BluetoothConnection::deviceConnected()
{
    mState = CSConnected;
    Q_EMIT stateChanged(mState);
    mControl->discoverServices();

}

void BluetoothConnection::deviceDisconnected()
{
    qDebug() << "device is disconnected......................";
    mConnected = false;
    mState = CSDisconnected;
    Q_EMIT disconnected();
    Q_EMIT stateChanged(mState);
    Q_EMIT connectedChanged(mConnected);
}

void BluetoothConnection::frameReceved(const QByteArray &frame)
{
    QVariantList vl;

    if(frame.size() < 4)
    {
        return;
    }
    //qDebug() << "on frameReceved:";
    //dump(frame);

    switch(frame[0])
    {
    case HandShark:
        qDebug() << "on HandShark";
        quint32 *sk =(quint32 *) &frame.data()[2];
        quint32 sk0 = 0;
        quint32 sk1 = 0;

        qDebug("orign session key 0:%x",mSessionKey[0]);
        qDebug("orign session key 0:%x",mSessionKey[1]);
        qDebug("receive session key 0:%x",sk[0]);
        qDebug("receive session key 0:%x",sk[1]);

#if defined(_IOS_OS_) || defined(_ANDROID_OS_)
        sk0 = ntohl(sk[0]);
        sk1 = ntohl(sk[1]);
#endif
        qDebug("ntohl session key 0:%x",sk0);
        qDebug("ntohl session key 0:%x",sk1);

        if((sk0 == mSessionKey[0])&&(sk1 == mSessionKey[1]))
        {
            qDebug() << "...........................:" << frame.size();
            mHanded = true;
            if(frame.size() >= 13) //protocol indicator cause
            {
                emit protocolChanged((int)frame[10]);
            }else{
                emit protocolChanged(1);
            }

        }else{
            qDebug() << "skey invalid!!!!!!!!!!!!!!!!!!!!!!!!!";
        }
        break;
    }
    if(frame[0] >= 0x20)  //device to endpointer command
    {
        for(int i=0;i<frame.size();i++)
        {
            vl.append((unsigned int)(frame[i] & 0xFF));
        }
        emit userFrame(vl);
    }
}

void BluetoothConnection::serviceStateChanged(QLowEnergyService::ServiceState s)
{
    switch (s) {
    case QLowEnergyService::ServiceDiscovered:
    {
        const QLowEnergyCharacteristic uart = mService->characteristic(
                    QBluetoothUuid((QBluetoothUuid::CharacteristicType)0xfff1));

        if (!uart.isValid()) {
            mLastError = BluetoothCharateristicError;
            mState = CSError;
            Q_EMIT errorChanged(mLastError);
            Q_EMIT stateChanged(mState);
            disconnect();
            return;
        }

        const QLowEnergyDescriptor m_notificationDesc = uart.descriptor(
                    QBluetoothUuid::ClientCharacteristicConfiguration);
        if (!m_notificationDesc.isValid()) {
            mLastError = BluetoothCharateristicError;
            mState = CSError;
            Q_EMIT errorChanged(mLastError);
            Q_EMIT stateChanged(mState);
            disconnect();
            return;
        }
        mService->writeDescriptor(m_notificationDesc, QByteArray::fromHex("0100"));
        mPrevCommState = mCommState;
        qDebug() << "..................................";
        qDebug() << "uart found........................";
        mCommState = CommInit;
        mState = CSServiceDiscovered ;
        Q_EMIT stateChanged(mState);
        break;
    }
    default:
        //nothing for now
        break;
    }

}

void BluetoothConnection::recvUARTData(const QLowEnergyCharacteristic &c, const QByteArray &value)
{
    //qDebug() << "on recvUARTData";
    quint64 frameCount = mStatics["session_rx"].toLongLong();
    quint64 errorCount = mStatics["session_rx_error"].toLongLong();

    quint64 recvTime = QDateTime::currentMSecsSinceEpoch();
    if((recvTime - lastRecv) > SLIP_RECV_TIMEOUT)
    {
        mRecvCounter = 0;
        mRecvBuffer.clear();
        qDebug() << "clear recvcounter";
    }

    //qDebug() << "mRecvCounter = " << QString::number(mRecvCounter);

    lastRecv = recvTime;


    for(int i=0;i<value.size();i++)
    {
        ++mRecvCounter;
        mRecvBuffer.append(value[i] & 0xFF);
        if((value[i] & 0xFF) == 0xC0)
        {
            if(mRecvCounter > 1) //process last END
            {
                //qDebug() << "on slip ending RecvBuffer Size:" << QString::number(mRecvBuffer.size()) << " buffer[0]:" << QString::number(mRecvBuffer[0] & 0xFF,16);
                /*最小包包含SLIP_END(2BYTE) + CMD(1BYTE) + DATA_SIZE(1BYTE) + CRC(2BYTE）*/
                //dump(mRecvBuffer);
                if((mRecvBuffer.size() >= 6)&&((quint8)mRecvBuffer[0] == 0xC0))
                {
                    QByteArray frameData = slipDecode(mRecvBuffer.mid(1,mRecvBuffer.size()-2)); //去掉SLIP_END然后转码SLIP包
                    //quint16 crc = ((quint16)((frameData[frameData.size()-2])&0xFF << 8) + frameData[frameData.size()-1];
                    quint16 crcH = (frameData[frameData.size()-2] & 0xFF) << 8;
                    quint16 crc = crcH + (frameData[frameData.size()-1] & 0xFF);
                    //qDebug() << "on frame:" << frameData << " crc:0x" << QString::number(crc,16);
                    quint16 calcCrc = BluetoothConnection::crc16(frameData.mid(0,frameData.size()-2));
                    //qDebug() << "cal crc:0x" << QString::number(calcCrc,16);
                    if(crc == calcCrc)
                    {
                        ++frameCount;
                        Q_EMIT recevFrame(frameData);
                    }else{
                        qDebug() << "crc error";
                        qDebug("crc must be %x but %x",calcCrc,crc);
                        ++errorCount;
                    }
                }else{
                    ++errorCount;
                }
                mRecvCounter=0;
                mRecvBuffer.clear();
            }else{
                //qDebug() << "on frame start";
            }
        }
    }

    quint64 rxSize = mStatics["session_rx_size"].toLongLong();
    rxSize += value.size();
    mStatics["session_rx_size"] = rxSize;
    mStatics["session_rx"] = frameCount;
    mStatics["session_rx_error"] = errorCount;
    Q_EMIT staticsChanged(mStatics);

}

quint8 d1=0;

void BluetoothConnection::onTimerTriggered()
{
    QByteArray data;
    quint32 tempKeys[2];
    quint8 *cp;
    if(!mControl)
    {
        mCommState = CommEnd;
        return;
    }
    /*if(mControl->state() == QLowEnergyController::UnconnectedState)
    {
        mCommState = CommEnd;
        return;
    }*/

    switch(mCommState)
    {
    case CommIdle:
        break;
    case CommInit:
        data.clear();

        srand(QDateTime::currentDateTime().toTime_t());
        mSessionKey[0] = qrand();
        mSessionKey[1] = qrand();
        //mSessionKey[0] = 0x11111111;
        //mSessionKey[1] = 0x11111111;
        tempKeys[0] = mSessionKey[0];
        tempKeys[1] = mSessionKey[1];

        qDebug("send session key 0:%x",mSessionKey[0]);
        qDebug("send session key 1:%x",mSessionKey[1]);

        btea(&tempKeys[0],2,privateKey);
        cp = (quint8 *)&tempKeys[0];
        qDebug("encrypt session key 0:%x",tempKeys[0]);
        qDebug("encrypt session key 1:%x",tempKeys[1]);
        for(int i=7;i>=0;i--)
        {
            data.append(*(cp+i));
        }

        //qDebug() << "tempKeys 0 :" << "0x" << QString::number(tempKeys[0],16);
        //qDebug() << "tempKeys 1 :" << "0x" << QString::number(tempKeys[1],16);

        writeToUART(writeFrame(HandShark,data));

        tempDateTime = QDateTime::currentDateTime();
        mPrevCommState = mCommState;
        mState = CSHandshark;
        mCommState = CommWaitingAck;
        Q_EMIT stateChanged(mState);
        break;
    case CommWaitingAck:
        if(mHanded)
        {
            mPrevCommState = mCommState;
            mCommState = CommHanded;
        }else{
            if(QDateTime::currentDateTime().msecsTo(tempDateTime) < -HANDSHARK_TIMEOUT)
            {
                mPrevCommState = mCommState;
                mLastError = BluetoothDeviceInvalidError;
                mState = CSError;
                mCommState = CommError;
                Q_EMIT stateChanged(mState);
            }
        }
        break;
    case CommHanded:
        qDebug() << "handle success!!!!!!!!!!!!!!!!!!!!";
        mPrevCommState = mCommState;
        mCommState = CommIdle;
        mState = CSHandsharked;
        Q_EMIT stateChanged(mState);
        mConnected = true;
        Q_EMIT connectedChanged(mConnected);
        break;
    case CommError:
        disconnect();
        mPrevCommState = mCommState;
        mCommState = CommIdle;
        break;
    case CommEnd:
        break;
    }
}

void BluetoothConnection::classBegin()
{

}

void BluetoothConnection::componentComplete()
{

}


