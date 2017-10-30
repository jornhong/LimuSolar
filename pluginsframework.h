#ifndef PLUGINSFRAMEWORK_H
#define PLUGINSFRAMEWORK_H
#include <QStringList>
#include <QDir>
#include <QList>
#include <QVariantMap>
#include <QObject>
#include <QAbstractItemModel>

class PluginsFramework : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum PluginRole{nameRole=Qt::UserRole+1,authorRole,entryRole,iconRole,protocolRole,deviceNameRole,
                    instanceRole,fileNameRole,filePathRole,versionRole,modalsRole,autoRole,descriptsRole};
    Q_ENUMS(PluginRole)

    explicit PluginsFramework(QObject *parent=0);
    void loadPlugin(QObject *plugin);
    static QObject *loadPlugin(const QString &fileName);
    QList<QVariantMap> getPluginsInfo()
    {
        return pluginsInfo;
    }
    QString getControllerPlugin(QString model);
    QVariantMap getPluginInfo(const QString &modal) const;

    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    int columnCount(const QModelIndex &parent=QModelIndex()) const;
    QModelIndex index(int row, int column, const QModelIndex &parent=QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    Q_INVOKABLE QVariantMap get(int row) const;
    QModelIndex indexControllModal(int protocol,int modal=-1) const;

    virtual QHash<int,QByteArray> roleNames() const;
    void initPlugins();


    Q_INVOKABLE bool instance(int row);
    Q_INVOKABLE void selectLanguage(QString lang);



    ~PluginsFramework();

private:
    QStringList filenames;
    QList<QDir> pluginsSearchDirs;
    QList<QVariantMap> pluginsInfo;
    QHash<int,QByteArray> mRoleNames;
    QList<QHash<int,QVariant>> mPlugins;

protected:
signals:

public slots:
};

typedef PluginsFramework* PluginsFrameworkPtr;
Q_DECLARE_METATYPE(PluginsFrameworkPtr)

#endif // PLUGINSFRAMEWORK_H
