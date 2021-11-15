#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_btn_connect_clicked();
    void addClient();
    void onReadyRead();
    void sendMessage(QByteArray *array);

private:
    Ui::Widget *ui;
    QTcpServer server;
    QList<QTcpSocket*> list;
};
#endif // WIDGET_H
