#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <QTextStream>
#include <QDateTime>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_input_clicked()
{
    source_path=QFileDialog::getOpenFileName(this,tr("文件"),"");
    QFile source(source_path);

    if(!source.open(QFile::ReadOnly|QFile::Text))
    {
        //QMessageBox::warning(this,tr("Error"),
                                 //tr("read file error"));
        log_info+=QDateTime::currentDateTime().toString();
        log_info=log_info+" 打开文件 "+source_path+" 失败\n";
    }
    else
    {
        log_info+=QDateTime::currentDateTime().toString();
        log_info=log_info+" 打开文件 "+source_path+" 成功\n";
    }

    QTextStream file(&source);
    this->ui->textEdit_source->setFont(QFont("微软雅黑",15));
    this->ui->textEdit_source->setText(file.readAll());
    this->ui->textEdit_info->setText(log_info);
    //this->ui->textEdit->setFont(QFont("微软雅黑",20));
    //this->ui->textEdit_source->setText(file.readAll());
    source.close();
}

void Widget::on_pushButton_lex_clicked()
{
    LexAnalyzer lex_a;
    //qDebug()<<path_source;
    int n=lex_a.analyze(source_path.toStdString());

    if(n<0)
    {
        log_info+=QDateTime::currentDateTime().toString();
        log_info+=" 词法分析错误 ,打开文件失败\n";
        this->ui->textEdit_info->setText(log_info);
        //QMessageBox::warning(this,tr("Error"),tr("Error"));
    }
    else if(n>0)
    {
        log_info=log_info+QDateTime::currentDateTime().toString()+" 词法分析错误 ";
        log_info=log_info+"error in line "+QString::number(n)+"\n";
        this->ui->textEdit_info->setText(log_info);
        //QMessageBox::warning(this,tr("Error"),tr("Error"));
    }
    //词法分析的显示
    else
    {
        QFile target(LEX_FILE);
        if(!target.open(QFile::ReadOnly|QFile::Text))
        {
            log_info=log_info+QDateTime::currentDateTime().toString();
            log_info+=" 打开词法分析结果失败\n";
            this->ui->textEdit_info->setText(log_info);
        }
            //QMessageBox::warning(this,tr("Error"),
                                //tr("read file error"));
        QTextStream file_t(&target);
        this->ui->textEdit_lex->setFont(QFont("微软雅黑",15));
        this->ui->textEdit_lex->setText(file_t.readAll());
        target.close();
        log_info=log_info+QDateTime::currentDateTime().toString()+" 词法分析完成\n";
        this->ui->textEdit_info->setText(log_info);

        QFile symbol("symbol.txt");
        if(!symbol.open(QFile::ReadOnly|QFile::Text))
        {
            log_info=log_info+QDateTime::currentDateTime().toString();
            log_info+=" 打开符号表失败\n";
            this->ui->textEdit_info->setText(log_info);
        }
                //QMessageBox::warning(this,tr("Error"),
                                     //tr("read file error"));
        QTextStream file_s(&symbol);
        this->ui->textEdit_table->setFont(QFont("微软雅黑",20));
        this->ui->textEdit_table->setText(file_s.readAll());
        symbol.close();

        log_info=log_info+QDateTime::currentDateTime().toString()+" 符号表初始化完成\n";
        this->ui->textEdit_info->setText(log_info);
    }

}

void Widget::on_pushButton_gram_clicked()
{
    grammer gram_a;
    int n=gram_a.analyze(LEX_FILE);
    if(n<0)
    {
        log_info+=QDateTime::currentDateTime().toString();
        log_info+=" 语法分析错误 ,打开文件失败\n";
        this->ui->textEdit_info->setText(log_info);
        QMessageBox::warning(this,tr("Error"),tr("Error"));
    }
    else if(n>0)
    {
       log_info=log_info+QDateTime::currentDateTime().toString()+" 词法分析错误 ";
       log_info=log_info+"error in line "+QString::number(n)+"\n";
       this->ui->textEdit_info->setText(log_info);
       QMessageBox::warning(this,tr("Error"),tr("Error"));
    }
//语法分析的显示
    else
    {
        QFile target(GRAM_FILE);
        if(!target.open(QFile::ReadOnly|QFile::Text))
        {
            log_info=log_info+QDateTime::currentDateTime().toString();
            log_info+=" 打开语法分析结果失败\n";
            this->ui->textEdit_info->setText(log_info);
            QMessageBox::warning(this,tr("Error"),tr("Error"));
        }
        QTextStream file(&target);
        this->ui->textEdit_gram->setFont(QFont("微软雅黑",10));
        this->ui->textEdit_gram->setText(file.readAll());
        target.close();
        log_info=log_info+QDateTime::currentDateTime().toString()+" 语法分析完成\n";
        this->ui->textEdit_info->setText(log_info);
        QFile symbol("symbol.txt");
        if(!symbol.open(QFile::ReadOnly|QFile::Text))
        {
            log_info=log_info+QDateTime::currentDateTime().toString();
            log_info+=" 打开符号表结果失败\n";
            this->ui->textEdit_info->setText(log_info);
            QMessageBox::warning(this,tr("Error"),tr("Error"));
        }
        QTextStream file_s(&symbol);
        this->ui->textEdit_table->setFont(QFont("微软雅黑",20));
        this->ui->textEdit_table->setText(file_s.readAll());
        log_info=log_info+QDateTime::currentDateTime().toString();
        log_info+=" 符号表更新完成\n";
        this->ui->textEdit_info->setText(log_info);
        symbol.close();
    }
}

void Widget::on_pushButton_format_clicked()
{
    grammer *gram_a = new grammer;
    gram_a->analyze(LEX_FILE);
    Generate *gene_a=new Generate(gram_a->grammerTree);
    int n=gene_a->analyse();
    qDebug()<<n;
    if(n<0)
    {
        log_info=log_info+QDateTime::currentDateTime().toString()+" 中间代码生成错误, ";
        log_info=log_info+"文件打开失败\n";
        this->ui->textEdit_info->setText(log_info);
        QMessageBox::warning(this,tr("Error"),tr("Error"));
    }
    else
    {
        Fourformat f;
        f.getfourformat("generate.txt");
        QFile gene_file("format.txt");
        if(!gene_file.open(QFile::ReadOnly|QFile::Text))
        {
            ;
        }
        QTextStream file_s(&gene_file);
        this->ui->textEdit_format->setFont(QFont("微软雅黑",20));
        this->ui->textEdit_format->setText(file_s.readAll());
        gene_file.close();


        log_info=log_info+QDateTime::currentDateTime().toString()+" 中间代码生成完成\n";
        this->ui->textEdit_info->setText(log_info);
    }
}



void Widget::on_pushButton_runall_clicked()
{
    on_pushButton_input_clicked();
    on_pushButton_lex_clicked();
    on_pushButton_gram_clicked();
    on_pushButton_format_clicked();
}
