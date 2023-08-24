#include "mainwindow.h"
#include "allocatedialog.h"

#include <QApplication>
#include <fstream>
#include <iostream>

#include <QDir>

int main(int argc, char *argv[])
{

    //QDir::setCurrent(QDir::currentPath() + "/../QtProject");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
