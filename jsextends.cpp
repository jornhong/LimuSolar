#include "jsextends.h"
#include <QQuickItem>
#include <QUuid>
#include <QQmlEngine>
#include <QQmlProperty>
#include <QDir>
#include <QTextStream>
#include <QProcessEnvironment>
#include <QProcess>
#include <QGuiApplication>
#include "plugininterface.h"
#include <QKeyEvent>
#include <QWindow>
#include "ios/toast.h"


#ifdef _ANDROID_OS_
    #include "jni.h"
    #include <QtAndroidExtras/QAndroidJniObject>
#endif



NavAction::~NavAction()
{

}

void NavAction::setAccepted(bool v)
{
    mAccepted = v;
}

bool NavAction::accepted() const
{
   return mAccepted;
}

NavAction::NavigateActions NavAction::action() const
{
    return mAction;
}

void NavAction::setAction(NavAction::NavigateActions act)
{
    mAction = act;
}

QVariantMap NavAction::opts() const
{
    return mOpts;
}

void NavAction::setOpts(const QVariantMap &opts)
{
   mOpts = opts;
}


QString JSExtends::formatMSecToTimeFormat(quint64 msecs)
{
   QDateTime dateTime = QDateTime::fromTime_t(msecs/1000);
   if(msecs >= (24*3600*1000))
   {
       return dateTime.toUTC().toString("dd Day hh:mm:ss");
   }
   return dateTime.toUTC().toString("hh:mm:ss");
}

QString JSExtends::formatDateTime(const QDateTime &datetime, QString formatString)

{
    return datetime.toString(formatString);
}

QString JSExtends::intToHex(long v, int fieldWidth)
{
    QString res = QString("%1").arg(v,fieldWidth,16,QLatin1Char('0'));
    return res;
}

QString JSExtends::createUUID()
{
    //DownloadPlugin *plugin = new DownloadPlugin();
    return QUuid::createUuid().toString();
}

void JSExtends::showDialog(QQuickItem *item)
{
    if(mMainWin)
    {
        qDebug() << "showing dialog!";
        QObject *dialogMask = mMainWin->findChild<QObject *>("dialogMask");
        QObject *dialogArea = mMainWin->findChild<QObject *>("dialogArea");
        if(dialogMask && dialogArea)
        {
            QQmlProperty::write(item, "parent"
                            , QVariant::fromValue<QObject*>(dialogArea));
            QQmlProperty::write(item, "visible"
                            ,true);
            dialogMask->setProperty("state","visibled");
            mBlur = -0.5;
            currentDialog = item;
            emit blurChanged(mBlur);
        }
    }
}

void JSExtends::closeDialog()
{
    if(mMainWin)
    {
        QObject *dialogMask = mMainWin->findChild<QObject *>("dialogMask");
        QObject *dialogArea = mMainWin->findChild<QObject *>("dialogArea");
        if(dialogMask && dialogArea)
        {
            dialogMask->setProperty("state","closed");
            mBlur = 0.0;
            emit blurChanged(mBlur);
        }
    }
    currentDialog = NULL;
}

void JSExtends::restartApp()
{
#ifdef _ANDROID_OS_
    //QAndroidJniObject::callStaticMethod<void>("com/limutech/solar/Activity",
    //                                   "doRestart");
    QAndroidJniObject::callStaticMethod<void>("com/limutech/solar/Activity",
                                       "restart");
#endif
}
void JSExtends::vibrate(int duration)
{
#ifdef _ANDROID_OS_
    QAndroidJniObject::callStaticMethod<void>("com/limutech/solar/Activity",
                                       "vibrate","(I)V",duration);
#endif
}

QString JSExtends::getVersionName()
{
    QString version = "1.0";
#ifdef _ANDROID_OS_
   QAndroidJniObject stringVersion = QAndroidJniObject::callStaticObjectMethod("com/limutech/solar/Activity",
                                       "getVersion","()Ljava/lang/String;");
   version = stringVersion.toString();
#else
    return "1.0";
#endif
//    return stringVersion.toString();
    return version;

}

int JSExtends::getVersion()
{
    int version = 1;
#ifdef _ANDROID_OS_
   jint jversion = QAndroidJniObject::callStaticMethod<jint>("com/limutech/solar/Activity",
                                       "getVersionCode");
   version = jversion;
#else
    return 1;
#endif
    return version;

}

void JSExtends::setMainTitle(const QString &title)
{
    QQmlProperty::write(mMainWin, "mainTitle"
                    , title);

}

void JSExtends::setSubTitle(const QString &title)
{
    QQmlProperty::write(mMainWin, "subTitle"
                        , title);
}

void JSExtends::showNotify(const QString &msg)
{
    mNotificationMsg = msg;
    emit notificationChanged();
}

QString JSExtends::getCurrentLANG() const
{
   return mLANG;
}

void JSExtends::selectLanguage(QString lang)
{

    mLANG = lang;
    settings.setValue("sys/lang",mLANG);
    selectLanguage();
}

void JSExtends::selectLanguage()
{
    if(mLANG == QString("cn"))
    {
        qDebug() << "on init cn translator!!!!!!";
            if (translator_cn->load(":/cn.qm")) {
                qDebug() << "on install cn translator!!!!!!";
                qApp->installTranslator(translator_cn);
            }
    }
    if(mLANG == QString("en"))
    {
        qApp->removeTranslator(translator_cn);
    }
    pluginFramework->selectLanguage(mLANG);
}

void JSExtends::setFullscreen(bool is)
{
    if(is)
    {
       mMainWin->setProperty("visibility",QWindow::FullScreen);
    }else{
        mMainWin->setProperty("visibility",QWindow::AutomaticVisibility);
    }
}

QVariant JSExtends::getSetting(const QString &name)
{
    return settings.value(name);
}

void JSExtends::setSetting(const QString &name, QVariant v)
{
    settings.setValue(name,v);
}


int JSExtends::indexControllModal(int protocol, int modal) const
{
    QModelIndex index = pluginFramework->indexControllModal(protocol,modal);
    if(index.isValid())
    {
        qDebug() << "get plugin";
        return index.row();
    }else{
        qDebug() << "not plugin found!";
        return -1;
    }
}

QVariantMap JSExtends::getPlugin(int row) const
{
    return pluginFramework->get(row);
}

int JSExtends::stackDepth()
{
    return mStackDepth;
}

void JSExtends::setStackDepth(int depth)
{
   mStackDepth = depth;
   emit stackDepthChanged(depth);
}

int JSExtends::mainStackDepth()
{
    return mMainStackDepth;
}

void JSExtends::setMainStackDepth(int depth)
{
   mMainStackDepth = depth;
   emit mainStackDepthChanged(mMainStackDepth);
}

JSExtends::JSExtends(QObject *parent)
    :QObject(parent),mStackDepth(0),mMainStackDepth(0),
      pluginFramework(0)
{
    pluginFramework = new PluginsFramework();
    mLANG = "en";
    connect(this, SIGNAL(notificationChanged()), this, SLOT(updateAndroidNotification()));
}

JSExtends::~JSExtends()
{
   if(pluginFramework)
   {
       delete pluginFramework;
       pluginFramework = 0;
   }
}

void JSExtends::setMainWindowsPtr(QObject *win)
{
    mMainWin = win;
}

void JSExtends::postAction(const int &action, const QVariantMap &opts)
{
    qDebug() << "on post action" << QString::number(mStackDepth) << ":" << QString::number(mMainStackDepth);
    if(currentDialog != NULL)
    {
        switch(action)
        {
        case NavAction::NVKey_Back:
            QMetaObject::invokeMethod(currentDialog,"reject");
            break;
        }
        return;
    }
    auto actionObj = new NavAction();
    actionObj->setAccepted(true);
    actionObj->setAction((NavAction::NavigateActions)action);
    actionObj->setOpts(opts);
    qRegisterMetaType<NavActionPtr>("NavAction");
    emit actionEvent(actionObj);
    if(actionObj->accepted())
    {
        processDefaultActions(actionObj);
    }

    if(mStackDepth > 0)
    {
        actionObj->setAccepted(true);
        actionObj->setAction(NavAction::NV_SubPrivous);
        actionObj->setOpts(opts);
        qDebug() << "emit nv sub privous event";
        emit actionEvent(actionObj);
        if(actionObj->accepted())
        {
            processDefaultActions(actionObj);
        }
    }else if(mMainStackDepth > 0)
    {
        actionObj->setAccepted(true);
        actionObj->setAction(NavAction::NV_Main_Privous);
        actionObj->setOpts(opts);
        emit actionEvent(actionObj);
        if(actionObj->accepted())
        {
            processDefaultActions(actionObj);
        }
    }
    /*actionObj->setAccepted(true);
    actionObj->setAction(NavAction::N);*/

    actionObj->deleteLater();
}

PluginsFrameworkPtr JSExtends::getPlugins() const
{
   return pluginFramework;
}

void JSExtends::loadPlugins()
{
    pluginFramework->initPlugins();
}

bool JSExtends::loadPlugin(int row)
{
    /*QModelIndex index = pluginFramework->index(row,0);
    if(index.isValid())
    {
    }*/
    return true;
}

void JSExtends::toast(const QString &s)
{
#ifdef _ANDROID_OS_
    QAndroidJniObject javaNotification = QAndroidJniObject::fromString(s);
    QAndroidJniObject::callStaticMethod<void>("com/limutech/solar/Activity",
                                       "displayMessage",
                                       "(Ljava/lang/String;)V",
                                       javaNotification.object<jstring>());
#endif
#ifdef _IOS_OS_
    makeIOSToast(s);
#endif
}

void JSExtends::sendKey()
{


        QKeyEvent event(QEvent::KeyPress, Qt::Key_A, Qt::NoModifier);
        if(mMainWin)
            qApp->sendEvent(mMainWin, &event);
}

QString JSExtends::notification() const
{
    return mNotificationMsg;

}

void JSExtends::setNotification(const QString &notification)
{
    if(mNotificationMsg == notification)
    {
        return;
    }
    mNotificationMsg = notification;
    emit notificationChanged();
}

void JSExtends::processDefaultActions(const NavActionPtr action)
{
    switch(action->action())
    {
    case NavAction::NVKey_Back:
        qApp->quit();
        break;
    default:
        break;
    }
}

void JSExtends::updateAndroidNotification()
{
    qDebug() << "on android notifyer!";
#ifdef _ANDROID_OS_
    QAndroidJniObject javaNotification = QAndroidJniObject::fromString(mNotificationMsg);
    QAndroidJniObject::callStaticMethod<void>("com/limutech/solar/Activity",
                                       "notify",
                                       "(Ljava/lang/String;)V",
                                       javaNotification.object<jstring>());
#endif


}
