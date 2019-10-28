#ifndef GAMEFRAME_H
#define GAMEFRAME_H

#include <QMainWindow>
#include <QHash>
#include <QLabel>
#include "dialogbox.h"

class GameFrame : public QMainWindow
{
    Q_OBJECT
private:
    QString cur_file;
    int cur_line = 0;
    // tags: file, tag, line
    QHash<QString, QHash<QString, int>> tags;
    // script_buffer: file, buffer
    QHash<QString, QStringList> script_buffer;

    // In order to load tags, the program must load the file first
    void load_file(QString file);
    void load_tag(QString file);
    void jump_to_tag(QString file, QString tag);

    DialogBox *dialogBox;
    bool dialogForward = false;

    void tgs_use(QStringList& params);
    void tgs_background(QStringList& params);
    void tgs_print(QString content);
    void tgs_print_newline();
    void tgs_wait();

public:
    GameFrame(QWidget *parent = nullptr);
    ~GameFrame();

    void init();
    void run();

public slots:
    void text_forward();
};

#endif // GAMEFRAME_H
