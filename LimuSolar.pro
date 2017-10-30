TEMPLATE = app

QT += qml quick bluetooth svg xml quickcontrols2 sql #androidextras
CONFIG += c++11 qtquickcompiler

TRANSLATIONS+=cn.ts

SOURCES += main.cpp \
    bluetoothdevicesmodel.cpp \
    jsextends.cpp \
    bluetoothconnection.cpp \
    pluginsframework.cpp \
    ios/toast.mm

RESOURCES += qml.qrc \
    qmlresource.qrc \
    lmd.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML2_IMPORT_PATH = $${_PRO_FILE_PWD}/qml
QML_IMPORT_PATH = $${_PRO_FILE_PWD_}/qml

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    bluetoothdevicesmodel.h \
    jsextends.h \
    bluetoothconnection.h \
    pluginsframework.h \
    plugininterface.h \
    protocols.h \
    ios/toast.h

#DISTFILES += \
#    android/AndroidManifest.xml \
#    android/gradle/wrapper/gradle-wrapper.jar \
#    android/gradlew \
#    android/res/values/libs.xml \
#    android/build.gradle \
#    android/gradle/wrapper/gradle-wrapper.properties \
#    android/gradlew.bat

android {
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
    QT += androidextras
    DEFINES += _ANDROID_OS_
    OTHER_FILES += android/src/com/limutech/solar/Activity.java
}

#ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_EXTRA_LIBS =
}

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    android/src/com/limutech/solar/UpdateManager.java \
    android/src/com/limutech/solar/AppUpdate.java \
    android/src/com/limutech/solar/AppUpdateService.java \
    android/src/com/limutech/solar/Displayer.java \
    android/src/com/limutech/solar/ResponseParser.java \
    android/src/com/limutech/solar/Version.java \
    android/src/com/limutech/solar/internal/NetworkUtil.java \
    android/src/com/limutech/solar/internal/ResponseCallback.java \
    android/src/com/limutech/solar/internal/SimpleJSONParser.java \
    android/src/com/limutech/solar/internal/VerifyTask.java \
    android/src/com/limutech/solar/internal/VersionDialogListener.java \
    android/src/com/limutech/solar/internal/VersionPersistent.java \
    android/src/com/limutech/solar/MyApplication.java \
    OptionForm.ui.qml \
    Option.qml \
    ios/Info.plist

lupdate_only{
    SOURCES = *.qml \
                qml/LmControls/*.qml
}


ios{

    LIBS += -framework Foundation -framework CoreFoundation -framework UIKit
    QMAKE_INFO_PLIST += ios/Info.plist
    DEFINES += _IOS_OS_
    ios_icon.files = $$files($$PWD/ios/Icon*.png)
    QMAKE_BUNDLE_DATA += ios_icon
    OBJECTIVE_HEADERS +=
    OBJECTIVE_SOURCES += \
            ios/UIView+Toast.m \
            ios/toast.mm
}


