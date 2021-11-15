#include "widget.h"
#include "ui_widget.h"
#include <QTimer>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(&server,&QTcpServer::newConnection,this,&Widget::addClient);

    QTimer * timer = new QTimer;
    connect(timer,&QTimer::timeout,[=](){
        for (int i = 0; i < list.size(); i++) {
            if (list[i]->state() == QTcpSocket::UnconnectedState){
                list.removeAt(i);
                i--;
            }
        }
        ui->lbl_tips->setText(QString("%1 clients are online in the moment.").arg(list.size()));
    });
    timer->start(3000);

    connect(ui->lbl_port,&QLineEdit::textChanged,[=](){
            ui->btn_connect->setEnabled(!(ui->lbl_port->text() == ""));
    });
}

Widget::~Widget()
{
    delete ui;

}


void Widget::on_btn_connect_clicked()
{
    if (server.listen(QHostAddress::Any,ui->lbl_port->text().toUInt())){
        qDebug() << "服务器启动成功，开始监听" ;
    }
    ui->lbl_port->setEnabled(false);
    ui->btn_connect->setEnabled(false);
}

void Widget::addClient()
{
    QTcpSocket* client = server.nextPendingConnection();
    list.append(client);
    connect(client,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
}

void Widget::onReadyRead()
{

    for (int i = 0; i < list.size(); i++) {
        if (list[i]->bytesAvailable()){
            QByteArray array = list[i]->readAll();

            ui->listWidget->addItem(array);
            sendMessage(&array);
        }
    }
}

void Widget::sendMessage(QByteArray *array)
{
    for (int i = 0; i < list.size(); i++) {
        list[i]->write(*array);
    }
}


