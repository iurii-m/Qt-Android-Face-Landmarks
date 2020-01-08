#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "Filter.h"
#include <QDebug>
#include <QSplashScreen>
#include <QPainter>
#include <QMainWindow>

int main(int argc, char *argv[])
{
//test

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterType<Filter>("FilterLib", 1, 0, "Filter");
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));


    return app.exec();
}
