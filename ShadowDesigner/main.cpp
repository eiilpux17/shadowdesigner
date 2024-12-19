#include "widget.h"

#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_Use96Dpi);
    QApplication a(argc, argv);
    a.setStyleSheet("file:///:/style.qss");
    Widget w;
    w.show();
    return a.exec();
}
