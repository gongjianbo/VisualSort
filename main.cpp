#include <QApplication>
#include "BubbleSimple.h"
#include "MainUI.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    BubbleSimple window;
    window.show();
    //MainUI window;
    //window.show();
    return app.exec();
}
