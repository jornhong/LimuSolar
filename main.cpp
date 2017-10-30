#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "bluetoothdevicesmodel.h"
#include "jsextends.h"
#include "bluetoothconnection.h"
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <QSettings>
#include <QQuickStyle>
#include <QFontDatabase>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    app.setOrganizationName("Limutech");
    app.setOrganizationDomain("limutech.com");
    app.setApplicationName("SolarMobile");

    QQmlApplicationEngine engine;
    qmlRegisterType<BluetoothDevicesModel>("com.limutech.solar",1,0,"BluetoothDevicesModel");
    qmlRegisterType<NavAction>("com.limutech.solar",1,0,"NavAction");
    qmlRegisterType<BluetoothConnection>("com.limutech.solar",1,0,"BluetoothConnection");
    qmlRegisterType<JSExtends>("com.limutech.solar",1,0,"JSExtends");

    QSettings settings;
    QString lang = settings.value("sys/lang").toString();
    if(lang.isEmpty())
    {
        settings.setValue("sys/lang","en");
        lang = "en";
    }
    if(lang == QString("cn"))
    {
        int id = QFontDatabase::addApplicationFont(":/fonts/wqy-microhei.ttc");
        QString family = QFontDatabase::applicationFontFamilies(id).at(0);
        QFont microhei(family);
        QGuiApplication::setFont(microhei);
    }

    JSExtendsPtr jsExtends = new JSExtends();
    jsExtends->selectLanguage(lang);

    engine.addImportPath("qrc:/qml");
    engine.rootContext()->setContextProperty("jsExt",jsExtends);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();
}


