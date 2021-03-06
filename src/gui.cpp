#include "gui.h"
#include "file.h"
#include <iostream>
#include <QString>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QIcon>
using namespace std;

GUI::GUI(int argc, char** argv) {
    qDebug() << "Application started" << endl;
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("cpp", &this->workerThread);
    engine.load(QUrl("qrc:/qml/main.qml"));
    QObject* qml = engine.rootObjects().first();

    QObject::connect(qml, SIGNAL(signalRemoveCurrentTask(QString, QString)),
                     &this->workerThread, SLOT(onRemoveCurrentTask(QString, QString)));

    QObject::connect(qml, SIGNAL(signalMoveNewTasksToCurrent()),
                     &this->workerThread, SLOT(onMoveNewTasksToCurrent()));

    QObject::connect(qml, SIGNAL(signalAddNewTasks(QVariant, QString)),
                     &this->workerThread, SLOT(onAddNewTasks(QVariant, QString)));

    QObject::connect(qml,  SIGNAL(signalStartCurrentTasks()),
                     &this->workerThread, SLOT(onStartCurrentTasks()));

    QObject::connect(qml, SIGNAL(signalStopCurrentTasks()),
                     &this->workerThread, SLOT(onStopCurrentTasks()));

    QObject::connect(qml, SIGNAL(signalWorkerInvoke(QString, QVariant)),
                     &this->workerThread, SLOT(onWorkerInvoke(QString, QVariant)));

    this->workerThread.signalStartWorker();
    QObject::connect(&app, SIGNAL(aboutToQuit()), this, SLOT(onAboutToQuit()));

    app.setWindowIcon(QIcon("main.png"));
    app.exec();
//    emit this->workerThread.signalStopWorker();
//    this->workerThread.wait();
}

GUI::~GUI() {}

void GUI::onAboutToQuit() {
    emit this->workerThread.signalStopWorker();
//    this->workerThread.wait();
}

//void GUI::setMouseCursor(int type) {
//    this->app->setOverrideCursor(QCursor(static_cast<Qt::CursorShape>(type)));
//}

