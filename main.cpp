#include "mainwindow.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <qfontdatabase.h>
#include <string>
#include "src/logger.h"

#include<QTextStream>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.setMouseTracking(true);
    w.resize(1600, 720);
    w.showFullScreen();//全屏显示
    w.maxInit();
    //为了安卓删除最大最小关闭按钮，framelesswindow.cpp添加了maxInit函数。
    //无边框后，随便点一个地方拖就会拖动，这个在安卓里会变成bug。
    //禁用了framelesswindow.cpp366-368行
    return a.exec();
}
