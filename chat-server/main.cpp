#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setFont(QFont("DejaVu Sans Mono",12));
    Widget w;
    w.show();
    return a.exec();
}
