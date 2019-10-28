#include "dialogbox.h"
#include <QTime>
#include <QCoreApplication>
#include <QEvent>

DialogBox::DialogBox(QWidget *parent)
    : QLabel(parent) {

}

DialogBox::~DialogBox() {

}

void DialogBox::add_text(QString text) {
    for (int i = 0; i < text.size(); i++) {
        meta += text[i];
        this->setText(meta);
        QTime t;
        t.start();
        while(t.elapsed() < 60)
            QCoreApplication::processEvents();
        this->show();
    }
}

void DialogBox::new_line() {
    meta += "\n    ";
    this->setText(meta);
    this->show();
}

void DialogBox::reset() {
    this->meta = this->indent;
}

bool DialogBox::event(QEvent *e) {
    switch (e->type()) {
    case QEvent::MouseButtonPress:
        emit pressed();
        break;
    default:
        break;
    }
    return QLabel::event(e);
}
