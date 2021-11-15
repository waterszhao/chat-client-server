#include "widget.h"
#include "ui_widget.h"
#include <QThread>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->btn_send->setEnabled(false);
    connect(ui->btn_connect,&QPushButton::clicked,[=](){
        serverAddress = ui->le_address->text();
        serverPort = ui->le_port->text().toUShort();
        chatName = ui->le_name->text();

        ui->btn_send->setEnabled(true);
        ui->le_address->setEnabled(false);
        ui->le_port->setEnabled(false);
        ui->le_name->setEnabled(false);
        ui->btn_connect->setEnabled(false);
        ts.connectToHost(serverAddress,serverPort);
        QThread::msleep(300);
        QString firstMessage = QString("---------%1 join the chat room---------").arg(chatName);
        ts.write(firstMessage.toUtf8());
    });

    connect(ui->btn_disconnect,&QPushButton::clicked,[=](){
        ui->btn_send->setEnabled(false);
        ui->le_address->setEnabled(true);
        ui->le_port->setEnabled(true);
        ui->le_name->setEnabled(true);
        ui->btn_connect->setEnabled(true);
        QString firstMessage = QString("---------%1 leave the chat room---------").arg(chatName);
        ts.write(firstMessage.toUtf8());
        ts.disconnectFromHost();
    });

    connect(ui->btn_send,&QPushButton::clicked,[=](){
        QString text = QString("%1 : %2").arg(chatName).arg(ui->le_message->text());
        ts.write(text.toUtf8());
        ui->le_message->setText("");
        ui->btn_send->setEnabled(false);
    });

    connect(ui->le_message,&QLineEdit::textChanged,[=](){
        ui->btn_send->setEnabled(!ui->btn_connect->isEnabled()&&!(ui->le_message->text()==""));
    });
    ui->btn_send->setShortcut(Qt::Key_Return|Qt::Key_Return);
    connect(&ts,&QTcpSocket::readyRead,[=](){
        ui->listWidget->addItem(ts.readAll());
    });
}

Widget::~Widget()
{
    delete ui;
}

