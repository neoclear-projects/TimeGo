#ifndef DIALOGBOX_H
#define DIALOGBOX_H

#include <QObject>
#include <QWidget>
#include <QLabel>

class DialogBox : public QLabel
{
    Q_OBJECT

private:
    QString meta = "    ";
    const QString indent = "    ";

signals:
    void pressed();

public:
    DialogBox(QWidget *parent = nullptr);
    ~DialogBox();

    void add_text(QString text);
    void new_line();
    void reset();

protected:
    bool event(QEvent *e);
};

#endif // DIALOGBOX_H
