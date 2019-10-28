#ifndef SELECTIONBUTTON_H
#define SELECTIONBUTTON_H

#include <QObject>
#include <QWidget>
#include <QPushButton>

class SelectionButton : public QPushButton
{
    Q_OBJECT
private:
    QString tag;

public:
    explicit SelectionButton(QString _tag, QWidget *parent = nullptr);

signals:
    void pressed(QString _tag);

protected:
    bool event(QEvent *e);

};

#endif // SELECTIONBUTTON_H
