#include "gameframe.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QIcon>
//#include <QCoreApplication>
#include <QTextCodec>

void setFrame(GameFrame &frame, QString key, QString value) {
    if (key == "#") {
        return;
    } else if (key == "resolution") {
        int w = value.split("x")[0].toInt();
        int h = value.split("x")[1].toInt();
        frame.setFixedSize(w, h);
    } else if (key == "opacity") {
        frame.setWindowOpacity(value.toDouble());
    } else if (key == "title") {
        frame.setWindowTitle(value);
    } else if (key == "window_icon") {
        frame.setWindowIcon(QIcon(value));
    }
}

int main(int argc, char *argv[])
{
    QApplication time(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    // Load stylesheet
    QFile style_file("style.qss");
    style_file.open(QFile::ReadOnly);
    QString style_sheet = QObject::tr(style_file.readAll());

    time.setStyleSheet(style_sheet);

    GameFrame frame;

    QFile spec_file("app.spec");
    if (spec_file.open(QFile::ReadOnly)) {
        QTextStream spec(&spec_file);
        QString line;
        while (spec.readLineInto(&line)) {
            char key[128], value[128];
            sscanf(line.toStdString().c_str(), "%s%s", key, value);
            setFrame(frame, QObject::tr(key), QObject::tr(value));
        }
    }

    frame.show();
    frame.init();
    frame.run();

    return time.exec();
}
