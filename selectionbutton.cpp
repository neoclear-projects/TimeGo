#include "selectionbutton.h"
#include <QEvent>

SelectionButton::SelectionButton(QString _tag, QWidget *parent)
    : QPushButton(parent) {
    this->tag = _tag;
}

bool SelectionButton::event(QEvent *e) {
    switch (e->type()) {
    case QEvent::MouseButtonPress:
        emit pressed(this->tag);
        break;
    default:
        break;
    }
    return QPushButton::event(e);
}
