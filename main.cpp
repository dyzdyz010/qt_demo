// Qt app entry point and QML type registration.
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include "formcontroller.h"
#include "formmodel.h"
#include "rulecontroller.h"
#include "ruleeditorcontroller.h"
#include "rulemodel.h"
#include "listvaluemodel.h"

int main(int argc, char* argv[])
{
    QQuickStyle::setStyle("Basic");
    QGuiApplication app(argc, argv);

    qmlRegisterType<FormModel>("qt_demo", 1, 0, "FormModel");
    qmlRegisterType<FormController>("qt_demo", 1, 0, "FormController");
    qmlRegisterType<RuleModel>("qt_demo", 1, 0, "RuleModel");
    qmlRegisterType<RuleController>("qt_demo", 1, 0, "RuleController");
    qmlRegisterType<RuleEditorController>("qt_demo", 1, 0, "RuleEditorController");
    qmlRegisterType<ListValueModel>("qt_demo", 1, 0, "ListValueModel");

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("qt_demo", "Main");

    return app.exec();
    // return QGuiApplication::exec();
}
