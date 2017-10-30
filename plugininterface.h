#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H
#include <QObject>

class PluginInterface
{
public:
    virtual bool load()=0;
    virtual bool initResource()=0;
    virtual void selectLanguage(QString lang)=0;
};

#define PluginInterface_iid "org.limutech.solar.PluginInterface"
Q_DECLARE_INTERFACE(PluginInterface, PluginInterface_iid)

#endif // PLUGININTERFACE_H
