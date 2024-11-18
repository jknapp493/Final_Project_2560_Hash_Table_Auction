//Jackson Knapp Group 13 main.cpp
#include "mainwindow.h" //STOCK STANDARD FROM START. INITIAL CONFIG FOR A WINDOW. -Jknapp

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
