#include <QDir>
#include <QCoreApplication>
#include <QPluginLoader>
#include <QJsonArray>
#include <QStandardPaths>
#include <QDebug>
#include "pluginsframework.h"
#include "plugininterface.h"


PluginsFramework::PluginsFramework(QObject *parent):QAbstractItemModel(parent)
{
    //pluginsDir = QDir::currentPath();
    QDir extPluginsDir = QDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/plugins");
    if(!extPluginsDir.exists())
    {
        QDir::root().mkpath(extPluginsDir.absolutePath());
    }
    pluginsSearchDirs.append(QDir(qApp->applicationDirPath()));
    pluginsSearchDirs.append(extPluginsDir);
    mRoleNames[nameRole] = "name";
    mRoleNames[authorRole] = "author";
    mRoleNames[versionRole] = "version";
    mRoleNames[fileNameRole] = "fileName";
    mRoleNames[filePathRole] = "filePath";
    mRoleNames[entryRole] = "entry";
    mRoleNames[iconRole] = "icon";
    mRoleNames[instanceRole] = "instance";
    mRoleNames[descriptsRole] = "descripts";
    mRoleNames[protocolRole] = "protocol";
    mRoleNames[modalsRole] = "modals";

    /*#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#endif
    pluginsDir.cd("plugins");*/
    //initPlugins();
}

void PluginsFramework::loadPlugin(QObject *plugin)
{
    //ControllerPlugin *pluginObject = qobject_cast<ControllerPlugin *>(plugin);
    //QVariant a;
    //a.setValue(pluginObject);
}

QObject *PluginsFramework::loadPlugin(const QString &fileName)
{
    QPluginLoader loader(fileName);
    QObject *plugin = loader.instance();
    return plugin;
}

QString PluginsFramework::getControllerPlugin(QString model)
{
    QString fileName;
    QStringList modelsItem;
    foreach (QVariantMap item, pluginsInfo) {
        if(item["device"].toString() == "controller")
        {
            modelsItem = item["models"].value<QStringList>();
            foreach(QString m,modelsItem)
            {
                if(m == model)
                {
                    fileName = item.value("absolutePath").toString();
                }
            }
        }
    }
    return fileName;
}

QVariantMap PluginsFramework::getPluginInfo(const QString &modal) const
{
    QVariantMap mapInfo;
    QStringList modalItems;
    qDebug() << "plugin infos:" << pluginsInfo;

    foreach (QVariantMap item, pluginsInfo) {
        modalItems = item["models"].value<QStringList>();
        foreach(QString m,modalItems)
        {
            qDebug() << "compacting:" << m;
            if(m == modal)
            {
                qDebug() << "matched!";
                mapInfo = item;
            }
        }
    }
    return mapInfo;
}

int PluginsFramework::rowCount(const QModelIndex &parent) const
{
    return mPlugins.size();
}

int PluginsFramework::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QModelIndex PluginsFramework::index(int row, int column, const QModelIndex &parent) const
{
    if((row >= 0) && row < mPlugins.size())
    {
        return createIndex(row,column);
    }
    return QModelIndex();
}

QModelIndex PluginsFramework::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

QVariant PluginsFramework::data(const QModelIndex &index, int role) const
{
    return mPlugins[index.row()][role];
}

QVariantMap PluginsFramework::get(int row) const
{
    QVariantMap res;
    QModelIndex index = this->index(row,0);
    if(index.isValid())
    {
        foreach(const int &key,mRoleNames.keys())
        {
            res[mRoleNames.value(key)] = this->data(index,key);
        }
    }
    return res;
}

QModelIndex PluginsFramework::indexControllModal(int protocol, int modal) const
{
    int mRow = -1;
   for(int i=0;i<mPlugins.size();i++)
   {
       int pluginProtocol = mPlugins.at(i).value(protocolRole).toInt();
       qDebug() << "searching plugin:" << QString::number(pluginProtocol);
       if(protocol == pluginProtocol )
       {
           qDebug() << "protocol is valid!";
           if(modal <= 0)
           {
               mRow = i;
               break;
           }else{
               QList<int> modals = mPlugins.at(i).value(modalsRole).value<QList<int>>();
               foreach(const int &m,modals)
               {
                   if(m == modal)
                   {
                       mRow = i;
                       break;
                   }
               }
           }
       }
   }
   if(mRow >= 0)
   {
       return index(mRow,0);
   }else{
       return QModelIndex();
   }
}

QHash<int, QByteArray> PluginsFramework::roleNames() const
{
    return mRoleNames;
}

/*ControllerPluginPtr PluginsFramework::allocControllerPlugin(QString fileName)
{
    ControllerPluginPtr controllerPlugin;
    QPluginLoader loader(fileName);
    QObject *plugin = loader.instance();
    controllerPlugin = qobject_cast<ControllerPluginPtr>(plugin);
    return controllerPlugin;
}*/

PluginsFramework::~PluginsFramework()
{
}

void PluginsFramework::initPlugins()
{
    qDebug() << "on plugins framework:initPlugins";
    beginResetModel();
    mPlugins.clear();
    bool replaceOldVersion=false;
    bool pluginsExists=false;
    bool existsIndex=0;
    foreach(QDir pluginsDir,pluginsSearchDirs)
    {
        qDebug() << "current plugin dir = " << pluginsDir.absolutePath();
        foreach (QString fileName,pluginsDir.entryList(QDir::Files))
        {
            qDebug() << "get plugin files:" << fileName;
            QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
            //QObject *plugin = loader.instance();
            QJsonObject metaData = loader.metaData().value("MetaData").toObject();
            QJsonValue pluginAuthor = metaData.value("author");


            if(pluginAuthor != QJsonValue::Undefined)
            {
                QHash<int,QVariant> item;
                QString pluginName = metaData.value("name").toString();
                qDebug() << "plugin name:" << pluginName;
                pluginsExists = false;
                replaceOldVersion = false;

                item[authorRole] = pluginAuthor;
                item[nameRole] = metaData.value("name").toString();
                item[fileNameRole] = fileName;
                item[filePathRole] = pluginsDir.absoluteFilePath(fileName);
                item[entryRole] = metaData.value("entry").toString();
                item[iconRole] = metaData.value("icon").toString();
                item[protocolRole] = metaData.value("protocol").toInt();
                item[versionRole] = metaData.value("version").toDouble();
                item[autoRole] = metaData.value("auto").toBool();
                item[descriptsRole] = metaData.value("descript").toVariant();

                QList<int>arrayItem;

                if(metaData.value("models").isArray())
                {
                    foreach (QJsonValue v, metaData.value("models").toArray()) {
                        arrayItem << v.toInt();
                    }
                }
                item[modalsRole] = QVariant::fromValue<QList<int>>(arrayItem);
                for(int i=0;i<mPlugins.size();i++)
                {
                    const QHash<int,QVariant> &r = mPlugins.at(i);
                    if((r.value(nameRole).toString().compare(pluginName,Qt::CaseInsensitive)) == 0)
                    {
                        pluginsExists = true;
                        existsIndex = i;
                        if(r.value(versionRole).toDouble() < item.value(versionRole).toDouble())
                        {
                            replaceOldVersion = true;
                        }
                    }
                }
                if(pluginsExists)
                {
                    if(replaceOldVersion)
                    {
                        mPlugins.removeAt(existsIndex);
                    }else{
                        continue;
                    }
                }
                mPlugins.append(item);
            }
        }}
    endResetModel();
}

bool PluginsFramework::instance(int row)
{
    QModelIndex index = this->index(row,0);
    if(index.isValid())
    {
        QString absolutePath = this->data(index,filePathRole).toString();
        QPluginLoader loader(absolutePath);
        QObject *object = loader.instance();
        PluginInterface *plugin = qobject_cast<PluginInterface *>(object);
        plugin->load();
        plugin->initResource();
        mPlugins[index.row()][instanceRole] = QVariant::fromValue<QObject *>(object);
        Q_EMIT dataChanged(index,index);
    }
    return true;
}

void PluginsFramework::selectLanguage(QString lang)
{
    for(int i=0;i<rowCount();i++)
    {
        if(mPlugins[i][instanceRole].isValid())
        {
            QObject *object = mPlugins[i][instanceRole].value<QObject *>();
            PluginInterface *plugin = qobject_cast<PluginInterface *>(object);
            plugin->selectLanguage(lang);
        }
    }
}




