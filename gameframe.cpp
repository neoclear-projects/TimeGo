#include "gameframe.h"
#include <QFile>
//#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include <QPalette>
#include <QCoreApplication>
#include <QTextCodec>

GameFrame::GameFrame(QWidget *parent)
    : QMainWindow(parent){
}

GameFrame::~GameFrame() {

}

void GameFrame::jump_to_tag(QString file, QString tag) {
    if (!this->script_buffer.count(file))
        throw "GameFrame::jump_to_tag: Unable to find file";
    this->cur_file = file;
    this->cur_line = this->tags[file][tag];
}

void GameFrame::load_file(QString file) {
    QFile f(file);
    if (f.open(QFile::ReadOnly)) {
        QTextStream buf(&f);
        buf.setCodec(QTextCodec::codecForName("UTF-8"));

        QString content = buf.readAll();
        qDebug() << content << endl;
        if (content.isEmpty())
            return;
        if (content.contains("\r\n"))
            this->script_buffer[file] = content.split("\r\n");
        else
            this->script_buffer[file] = content.split("\n");
    }
}

void GameFrame::load_tag(QString file) {
    int line_num = 0;
    for (const auto& line : this->script_buffer[file]) {
        if (line.startsWith("@")) {
            QString tag_name = line.mid(1, line.size() - 1);
            this->tags[file][tag_name] = line_num;
        }
        line_num++;
    }
}

void GameFrame::init() {
    load_file("app.tgs");
    load_tag("app.tgs");
    jump_to_tag("app.tgs", "main");
}

void GameFrame::run() {
    while (cur_line < script_buffer[cur_file].size()) {
        QString line = this->script_buffer[this->cur_file][this->cur_line];
        if (line.startsWith("@") ||
                line.startsWith("#") ||
                line.isEmpty()) {
            // Do nothing
        } else if (line.startsWith("!")) {
            QStringList commands = line.split(" ");
            if (commands[1] == "use")
                tgs_use(commands);
            else if (commands[1] == "background")
                tgs_background(commands);
            else if (commands[1] == "return")
                break;
            else if (commands[1] == "n")
                tgs_print_newline();
            else if (commands[1] == "p")
                tgs_wait();
            else if (commands[1] == "quit")
                exit(0);
            else if (commands[1] == "dialog_font_size") {
                QFont font;
                font.setPointSize(commands[2].toInt());
                this->dialogBox->setFont(font);
            } else if (commands[1] == "select")
                tgs_select(commands);
        } else
            tgs_print(line);
        this->cur_line++;
    }
}

void GameFrame::tgs_use(QStringList& params) {
    if (params[2] == "BuiltinDialogBox") {
        this->dialogBox = new DialogBox(this);
        this->dialogBox->setObjectName(tr("dialogBox"));
        int x = this->width() / 6;
        int y = this->height() * 3 / 4;
        int w = this->width() * 2 / 3;
        int h = this->height() / 4;
        this->dialogBox->setGeometry(QRect(x, y, w, h));
        this->dialogBox->setAlignment(Qt::AlignLeft);
//        this->DialogBox->setStyleSheet("background-color: rgb(0, 255, 0)");
        this->dialogBox->show();
    }
}

void GameFrame::tgs_background(QStringList& params) {
    QString image = params[2];
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window,
                     QBrush(QPixmap(image)));
    this->setPalette(palette);
//    this->repaint();
}

void GameFrame::tgs_print(QString content) {
    this->dialogBox->add_text(content);
}

void GameFrame::tgs_print_newline() {
    this->dialogBox->new_line();
}

void GameFrame::tgs_wait() {
    connect(dialogBox, &DialogBox::pressed,
            this, &GameFrame::text_forward);
    while (!this->dialogForward) {
        QCoreApplication::processEvents();
    }
    this->dialogForward = false;
    this->dialogBox->reset();
    disconnect(dialogBox, &DialogBox::pressed,
               this, &GameFrame::text_forward);
}

void GameFrame::text_forward() {
    this->dialogForward = true;
}

void GameFrame::tgs_select(QStringList& params) {
    for (int i = 2; i < params.size(); i += 3) {
        QString label_name = params[i];
        QString pos = params[i + 1];
        QString tag = params[i + 2];
        SelectionButton *but = new SelectionButton(tag, this);
        this->selection_button[label_name] = but;
        but->setGeometry(pos.split(",")[0].toInt(),
                         pos.split(",")[1].toInt(),
                         pos.split(",")[2].toInt(),
                         pos.split(",")[3].toInt());
        but->setText(label_name);
        but->show();
        connect(but, &SelectionButton::pressed,
                this, &GameFrame::selection_jump);
    }
    while (!this->dialogForward) {
        QCoreApplication::processEvents();
    }
    this->dialogForward = false;
}

void GameFrame::selection_jump(QString _tag) {
    jump_to_tag(this->cur_file, _tag);
    for (const auto& it : this->selection_button) {
        disconnect(it, &SelectionButton::pressed,
                   this, &GameFrame::selection_jump);
        delete it;
    }
    selection_button.clear();
    this->dialogForward = true;
}
