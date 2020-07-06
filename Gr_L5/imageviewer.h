#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H
#include <QImage>
#include <QMainWindow>
#include <QLabel>
#include <QScrollArea>
#include <QScrollBar>
#include <QPrinter>
#include <QFileDialog>
#include <QMessageBox>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QHBoxLayout>
#include <QDebug>
#include <QSound>
#include <QtCharts>
#include <locale>
#include <vector> // подключаем необходимую библиотеку
using namespace std;

class ImageViewer : public QMainWindow
{
    Q_OBJECT

/*Конструктор*/
public:
    ImageViewer();

/*Слоты*/
private slots:
    void open();            //открыть
    void zoomIn();          //увеличить
    void zoomOut();         //уменьшить
    void normalSize();      //нормальный размер
    void mirrorHor();       //отзеркалить(горизонталь)
    void mirrorVert();      //отзеркалить(вертикаль)
    void binar();           //бинаризация(ЧБ)
    void toGray();          //к отеннкам серого
    void toNegative();      //к негативу
    void original();        //оригинальное изображение
    void brightUp();        //яркость +
    void brightDown();      //яркость -
    void contrastUp();      //контраст +
    void contrastDown();    //контраст -

/*Методы*/
private:
    void createActions();                                       //создать событие
    void createMenus();                                         //создать меню
    void updateActions();                                       //обновить событие
    void scaleImage(double factor);                             //увеличить масштаб
    void adjustScrollBar(QScrollBar *scrollBar, double factor); //скролбар
    QImage brightRouter(QImage sup);                             //
    QImage contrastRouter(QImage sup);
    void layoutUpdate();

/*Переменные*/
    int brightStep = 0;         //яркость
    double contrastStep = 1;    //констраст
    double scaleFactor;         //масштаб

    QScrollBar *brightBar;
    QVBoxLayout *lay;
    QLabel *imageLabel;
    QScrollArea *scrollArea;
    QChartView *chartView;
    QImage photo;
    QImage photoCur;
    QChart *chart;

    QAction *openAct;
    QAction *exitAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalSizeAct;
    QAction *mirrorVAct;
    QAction *mirrorHAct;
    QAction *binarAct;
    QAction *grayAct;
    QAction *negativeAct;
    QAction *originalAct;
    QAction *brightUpAct;
    QAction *brightDownAct;
    QAction *contrastUpAct;
    QAction *contrastDownAct;

    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *work;
    QMenu *colorStuff;
};
#endif // IMAGEVIEWER_H
