#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <QTextStream>
#include <QDateTime>
#include "lexanalyzer.h"
//#include "grammer.h"
#define LEX_FILE "target.txt"
#define GRAM_FILE "target2.txt"
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    QString log_info;//日志信息
    QString source_path;

private slots:
    void on_pushButton_input_clicked();

    void on_textEdit_info_textChanged();

    void on_pushButton_lex_clicked();

    void on_pushButton_gram_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
