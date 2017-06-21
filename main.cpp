#include "widget.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setWindowTitle("AnalysisLogFiles v1.2");
    w.setWindowIcon(QIcon(QPixmap(":/logo.png")));
    w.show();

    return a.exec();
}
