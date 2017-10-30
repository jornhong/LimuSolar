#include "bluetoothdevicesmodel.h"
#include <QDebug>
#include <QBluetoothLocalDevice>

BluetoothDevicesModel::BluetoothDevicesModel(QObject *parent)
        :QAbstractItemModel(parent),
          mRunning(false)
{

        mRoleNames[nameRole] = "name";
        mRoleNames[addressRole] = "address";
        mRoleNames[deviceRole] = "device";

        QBluetoothLocalDevice device;
        if(device.hostMode() == QBluetoothLocalDevice::HostPoweredOff)
        {
                device.powerOn();
        }

        mDeviceDiscoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);

        connect(mDeviceDiscoveryAgent, SIGNAL(deviceDiscovered(const QBluetoothDeviceInfo&)),
                this, SLOT(addDevice(const QBluetoothDeviceInfo&)));

        connect(mDeviceDiscoveryAgent, SIGNAL(error(QBluetoothDeviceDiscoveryAgent::Error)),
                this, SLOT(deviceScanError(QBluetoothDeviceDiscoveryAgent::Error)));

        connect(mDeviceDiscoveryAgent, SIGNAL(error(QBluetoothDeviceDiscoveryAgent::Error)),
                this, SIGNAL(scanError(QBluetoothDeviceDiscoveryAgent::Error)));

        connect(mDeviceDiscoveryAgent, SIGNAL(finished()), this, SLOT(scanFinished()));
        connect(mDeviceDiscoveryAgent, SIGNAL(finished()), this, SIGNAL(scanFinish()));
}

int BluetoothDevicesModel::rowCount(const QModelIndex &parent) const
{
        Q_UNUSED(parent)
        return mRecords.size();

}

int BluetoothDevicesModel::columnCount(const QModelIndex &parent) const
{
        Q_UNUSED(parent)
        return 1;

}

QModelIndex BluetoothDevicesModel::index(int row, int column, const QModelIndex &parent) const
{
        Q_UNUSED(parent)
        return createIndex(row,column);
}

QModelIndex BluetoothDevicesModel::parent(const QModelIndex &child) const
{
        Q_UNUSED(child)
        return QModelIndex();
}

QVariant BluetoothDevicesModel::data(const QModelIndex &index, int role) const
{
        return mRecords[index.row()][role];
}

QHash<int, QByteArray> BluetoothDevicesModel::roleNames() const
{
        return mRoleNames;
}


void BluetoothDevicesModel::setRunning(bool running)
{
        if(running)
        {
                refresh();
        }else{
                if(mDeviceDiscoveryAgent->isActive())
                {
                        mDeviceDiscoveryAgent->stop();
                }
        }
        if(running != mRunning)
        {
                mRunning = running;
                Q_EMIT runningChanged(mRunning);
        }
}

bool BluetoothDevicesModel::running() const
{
        return mRunning;
}

void BluetoothDevicesModel::refresh()
{
        if(mDeviceDiscoveryAgent->isActive())
        {
                mDeviceDiscoveryAgent->stop();
        }
        beginResetModel();
        mRecords.clear();
        endResetModel();
        qDebug() << "starting searching....";
        mDeviceDiscoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
        if(mDeviceDiscoveryAgent->isActive())
        {
                qDebug() << "scanner actived!";
        }else{
                qDebug() << "scanner inactived!";
        }
}

void BluetoothDevicesModel::classBegin()
{

}

void BluetoothDevicesModel::componentComplete()
{
        if(mRunning)
        {
                refresh();
        }
}

void BluetoothDevicesModel::addDevice(const QBluetoothDeviceInfo &device)
{
        qDebug() << "adding devices...";
        if (device.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration) {
                beginInsertRows(QModelIndex(),mRecords.size(),mRecords.size());
                QHash<int,QVariant> row;
                row[addressRole] = device.address().toString();
                row[nameRole] = device.name();
                row[deviceRole] = QVariant::fromValue<QBluetoothDeviceInfo>(device);
                mRecords.append(row);
                endInsertRows();
        }
}

void BluetoothDevicesModel::scanFinished()
{
        qDebug() << "scan finish!";
        mRunning = false;
        emit runningChanged(mRunning);
}

void BluetoothDevicesModel::deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error)
{
        qDebug() << "scan error:" << error;
        mRunning = false;
        //    emit runningChanged(mRunning);
        if(error == QBluetoothDeviceDiscoveryAgent::PoweredOffError)
        {
                QBluetoothLocalDevice device;
                device.powerOn();
        }
}



