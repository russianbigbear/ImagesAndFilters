#include "mainwindow.h"
#include <QApplication>
#include "imageviewer.h"

 int main(int argc, char *argv[])
 {
     setlocale(LC_ALL, "rus");
     QApplication app(argc, argv);
     ImageViewer imageViewer;
     imageViewer.show();

     return app.exec();
 }
