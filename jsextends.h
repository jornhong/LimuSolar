#ifndef JSEXTENDS_H
#define JSEXTENDS_H

#include <QObject>
#include <QDateTime>
#include <QQmlComponent>
#include <QQuickItem>
#include <QTranslator>
#include <QSettings>
#include "pluginsframework.h"
#ifdef _IOS_OS_
//#import <Foundation/Foundation.h>
//#import <Foundation/NSString.h>
#endif

#ifdef RELEASE_DEFS

#endif

class NavAction:public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool accepted READ accepted WRITE setAccepted)
    Q_PROPERTY(int action READ action CONSTANT)
    Q_PROPERTY(QVariantMap opts READ opts CONSTANT)
public:
    enum NavigateActions{NVKey_Back,NVKey_Home,NVKey_Menu,NV_Main_Privous,NV_SubPrivous};
    Q_ENUMS(NavigateActions)
    ~NavAction();
    void setAccepted(bool v);
    bool accepted() const;
    NavigateActions action() const;
    void setAction(NavigateActions act);
    QVariantMap opts() const;
    void setOpts(const QVariantMap &opts);
private:
    bool mAccepted;
    NavigateActions mAction;
    QVariantMap mOpts;
};

typedef NavAction* NavActionPtr;
Q_DECLARE_METATYPE(NavActionPtr)

class JSExtends:public QObject
{
    Q_OBJECT
    Q_PROPERTY(float blur MEMBER mBlur NOTIFY blurChanged)
    Q_PROPERTY(QObject* appWin MEMBER mMainWin)
    Q_PROPERTY(PluginsFrameworkPtr pluginsModel READ getPlugins CONSTANT)
    Q_PROPERTY(QString notification READ notification WRITE setNotification NOTIFY notificationChanged)
    Q_PROPERTY(int stackDepth READ stackDepth WRITE setStackDepth NOTIFY stackDepthChanged)
    Q_PROPERTY(int mainStackDepth READ mainStackDepth WRITE setMainStackDepth NOTIFY mainStackDepthChanged)

    Q_PROPERTY(QString currentLANG READ getCurrentLANG NOTIFY languageChanged)
public:
    Q_INVOKABLE QString formatMSecToTimeFormat(quint64 msecs);
    Q_INVOKABLE static QString formatDateTime(const QDateTime &datetime,QString formatString="yyyy-MM-dd hh:mm:ss");
    Q_INVOKABLE QString intToHex(long v,int fieldWidth=2);
    Q_INVOKABLE static QString createUUID();
    Q_INVOKABLE void showDialog(QQuickItem *item);
    Q_INVOKABLE void closeDialog();
    Q_INVOKABLE void setMainTitle(const QString &title);
    Q_INVOKABLE void setSubTitle(const QString &title);
    Q_INVOKABLE void showNotify(const QString &msg);
    Q_INVOKABLE QString getCurrentLANG() const;
    Q_INVOKABLE void selectLanguage(QString lang);
    Q_INVOKABLE void selectLanguage();
    Q_INVOKABLE void setFullscreen(bool is);

    Q_INVOKABLE QVariant getSetting(const QString &name);
    Q_INVOKABLE void setSetting(const QString &name,QVariant v);

    //inject to java activity method
    Q_INVOKABLE void vibrate(int duration=500);
    Q_INVOKABLE QString getVersionName();
    Q_INVOKABLE int getVersion();
    Q_INVOKABLE void restartApp();

    Q_INVOKABLE int indexControllModal(int protocol,int modal=0) const;
    Q_INVOKABLE QVariantMap getPlugin(int row) const;

    int stackDepth();
    void setStackDepth(int depth);

    int mainStackDepth();
    void setMainStackDepth(int depth);


    JSExtends(QObject *parent=0);
    ~JSExtends();
    void setMainWindowsPtr(QObject *win);
    Q_INVOKABLE void postAction(const int &action, const QVariantMap &opts=QVariantMap());
    PluginsFrameworkPtr getPlugins() const;
    Q_INVOKABLE void loadPlugins();  //load plugins list
    Q_INVOKABLE bool loadPlugin(int row); //instance a plugin
    Q_INVOKABLE void toast(const QString &s);
    Q_INVOKABLE void sendKey();



    QString notification() const;
    void setNotification(const QString &notification);

private:
    float mBlur=0.0;
    bool dialogShowed=false;
    QQuickItem *currentDialog=NULL;
    QObject *mMainWin= NULL;
    void processDefaultActions(const NavActionPtr action);
    PluginsFramework *pluginFramework=0;
    int mStackDepth;
    int mMainStackDepth;
    QString mLANG;
    QSettings settings;
    QTranslator* translator_cn = new QTranslator();

    QString mNotificationMsg;
private slots:
    void updateAndroidNotification();

signals:
    void blurChanged(float);
    void actionEvent(const NavActionPtr action);
    void notificationChanged();
    void stackDepthChanged(int);
    void mainStackDepthChanged(int);
    void languageChanged(const QString &);
};

typedef JSExtends* JSExtendsPtr;

Q_DECLARE_METATYPE(JSExtendsPtr)

#endif // JSEXTENDS_H
