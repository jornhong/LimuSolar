#ifndef BLUETOOTHDEVICESMODEL_H
#define BLUETOOTHDEVICESMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include <QQmlParserStatus>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QLowEnergyController>
#include <QLowEnergyService>

class BluetoothDevicesModel : public QAbstractItemModel,public QQmlParserStatus
{
    Q_OBJECT
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
public:
    explicit BluetoothDevicesModel(QObject *parent = 0);
    enum deviceRoles{addressRole=Qt::UserRole+1,nameRole,servicesRole,deviceRole};
    Q_ENUMS(deviceRoles)
    Q_ENUMS(QBluetoothDeviceDiscoveryAgent::Error)
    Q_INTERFACES(QQmlParserStatus)


    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    int columnCount(const QModelIndex &parent=QModelIndex()) const;
    QModelIndex index(int row, int column, const QModelIndex &parent=QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    virtual QHash<int,QByteArray> roleNames() const;

    void setRunning(bool running);
    bool running() const;


private:
    QList<QHash<int,QVariant>> mRecords;
    QHash<int,QByteArray> mRoleNames;
    //DeviceInfo mCurrentDevice;
    QBluetoothDeviceDiscoveryAgent *mDeviceDiscoveryAgent;
    QLowEnergyDescriptor mNotificationDesc;
    bool mRunning;
    void refresh();
protected:
    virtual void classBegin();
    virtual void componentComplete();

signals:
    void runningChanged(const bool&);
    void scanError(QBluetoothDeviceDiscoveryAgent::Error error);
    void scanFinish();

public slots:

private slots:
    void addDevice(const QBluetoothDeviceInfo&);
    void scanFinished();
    void deviceScanError(QBluetoothDeviceDiscoveryAgent::Error);
};

typedef BluetoothDevicesModel* BluetoothDevicesModelPtr;
Q_DECLARE_METATYPE(BluetoothDevicesModelPtr)
Q_DECLARE_METATYPE(QBluetoothDeviceDiscoveryAgent::Error)

#endif // BLUETOOTHDEVICESMODEL_H
