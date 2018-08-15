#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "guimanager.h"
#include "berkeleymanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    BerkeleyManager manager;
    GuiManager::GetInstance().setManager(manager);
    GuiManager::GetInstance().setQmlEngine(engine);
    engine.rootContext() -> setContextProperty("GuiManager", &GuiManager::GetInstance());
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
