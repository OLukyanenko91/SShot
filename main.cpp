#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include "src/Model/Model.hpp"
#include "src/Screen/Screen.hpp"
#include "src/Screen/Screenshot.hpp"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/res/main.qml"));

    QQuickStyle::setStyle("Material");
    qmlRegisterType<NModel::CModel>("Components", 1, 0, "Model");
    qRegisterMetaType<NScreen::CScreenshot>("NScreen::CScreenshot");

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
