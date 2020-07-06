#include "imageviewer.h"

/*Конструктор*/
ImageViewer::ImageViewer()
{
    imageLabel = new QLabel;
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    QVBoxLayout *vBoxLayout = new QVBoxLayout;
    QWidget *widget = new QWidget();
    widget->setLayout(vBoxLayout);

    chart=new QChart();
    chart->legend()->hide();
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    vBoxLayout->addWidget(scrollArea);
    vBoxLayout->addWidget(chartView);

    setCentralWidget(widget);

    createActions();
    createMenus();

    setWindowTitle(tr("Фоторедактор 1.0"));
    resize(800, 1000);
}

/*Увеличить констрастность*/
void ImageViewer::contrastUp(){
    contrastStep*=1.1;
    photoCur = contrastRouter(brightRouter(photo));
    imageLabel->setPixmap(QPixmap::fromImage(photoCur));
    layoutUpdate();
}

/*Уменьшить констрастность*/
void ImageViewer::contrastDown(){
    contrastStep/=1.1;
    photoCur = contrastRouter(brightRouter(photo));
    imageLabel->setPixmap(QPixmap::fromImage(photoCur));
    layoutUpdate();
}

/*Распределитель для Контраста*/
QImage ImageViewer::contrastRouter(QImage sup){

    double avgR = 0;        //красный
    double avgB = 0;        //синий
    double avgG = 0;        //зеленый
    double countPix = 0;    //количество пикселей

    //считаем пиксели
    for(int i = 0; i < sup.width(); i++)
    {
        for(int j = 0; j < sup.height(); j++)
        {
            QColor pix = QColor(sup.pixel(i, j));
            avgR += pix.red();
            avgG += pix.green();
            avgB += pix.blue();
            countPix++;
        }
    }

    avgR /= countPix;
    avgB /= countPix;
    avgG /= countPix;

    for(int i=0;i<sup.width();i++)
    {
        for(int j=0;j<sup.height();j++)
        {
            QColor pix = QColor(sup.pixel(i,j));
            int red = int((pix.red()-avgR)*contrastStep+avgR);
            if(red>255)
                red = 255;
            if(red<0)
                red = 0;
            int green = int((pix.green()-avgG)*contrastStep+avgG);
            if(green>255)
                green = 255;
            if(green<0)
                green = 0;
            int blue = int((pix.blue()-avgB)*contrastStep+avgB);
            if(blue>255)
                blue=255;
            if(blue<0)
                blue=0;
            sup.setPixelColor(i,j,QColor(red,green,blue));
        }
    }
    return sup;
}

/*Увеличить яркость*/
void ImageViewer::brightUp(){
    brightStep += 10;
    photoCur = contrastRouter(brightRouter(photo));
    imageLabel->setPixmap(QPixmap::fromImage(photoCur));
    layoutUpdate();
}

/*Уменьшить яркость*/
void ImageViewer::brightDown(){
    brightStep -= 10;
    photoCur = contrastRouter(brightRouter(photo));
    imageLabel->setPixmap(QPixmap::fromImage(photoCur));
    layoutUpdate();
}

/*Распределитель для Яркости*/
QImage ImageViewer::brightRouter(QImage sup)
{
    for(int i=0;i<sup.width();i++)
    {
        for(int j=0;j<sup.height();j++)
        {
            QColor pix = QColor(sup.pixel(i,j));
            int red = int(pix.red()+brightStep);
            if(red>255)
                red = 255;
            if(red<0)
                red = 0;
            int green = int(pix.green()+brightStep);
            if(green>255)
                green = 255;
            if(green<0)
                green = 0;
            int blue = int(pix.blue()+brightStep);
            if(blue>255)
                blue=255;
            if(blue<0)
                blue=0;
            sup.setPixelColor(i,j,QColor(red,green,blue));
        }
    }
    return sup;
}

/*Увеличить изображение*/
void ImageViewer::zoomIn()
{
    scaleImage(1.25);
}

/*Уменьшить изображение*/
void ImageViewer::zoomOut()
{
    scaleImage(0.75);
}

/*Обновление слоя*/
void ImageViewer::layoutUpdate(){
    int counter = 0;
    QLineSeries* series = new QLineSeries();
    for(int k=0; k<256; k++)
    {
        counter=0;
        for(int i=0;i<photoCur.width();i++)
        {
            for(int j=0;j<photoCur.height();j++)
            {
                QColor pix = QColor(photoCur.pixel(i,j));
                int Y = int(pix.red()*0.299+pix.green()*0.5876+pix.blue()*0.114);
                if(Y==k)
                {
                    counter++;
                }
            }
        }
        series->append(k,counter);
    }
    chart->removeAllSeries();
    chart->addSeries(series);
}

/*Бинаризация*/
void ImageViewer::binar()
{
    for(int i=0;i<photoCur.width();i++)
    {
        for(int j=0;j<photoCur.height();j++)
            photoCur.setPixelColor(i,j,(QColor(photoCur.pixel(i,j)).black() > 127 ? "black":"white"));
    }
    imageLabel->setPixmap(QPixmap::fromImage(photoCur));
    layoutUpdate();
}

/*К оттенкам серого*/
void ImageViewer::toGray()
{
    // Y=0,299R + 0,5876G + 0,114B
    for(int i=0;i<photoCur.width();i++)
    {
        for(int j=0;j<photoCur.height();j++)
        {
            QColor pix = QColor(photoCur.pixel(i,j));
            int Y = int(pix.red()*0.299+pix.green()*0.5876+pix.blue()*0.114);
            photoCur.setPixelColor(i,j,QColor(Y,Y,Y));
        }
    }
    imageLabel->setPixmap(QPixmap::fromImage(photoCur));
    layoutUpdate();
}

/*Негатив*/
void ImageViewer::toNegative()
{

    for(int i=0;i<photoCur.width();i++)
    {
        for(int j=0;j<photoCur.height();j++)
        {
            QColor pix = QColor(photoCur.pixel(i,j));
            photoCur.setPixelColor(i,j,QColor(255-pix.red(),255-pix.green(),255-pix.blue()));
        }
    }
    imageLabel->setPixmap(QPixmap::fromImage(photoCur));
    layoutUpdate();
}

/*К оригинальному изображению*/
void ImageViewer::original(){
    photoCur = photo;
    brightStep = 0;
    contrastStep=1;
    imageLabel->setPixmap(QPixmap::fromImage(photoCur));
    layoutUpdate();
}

/*Нормальный размер*/
void ImageViewer::normalSize()
{
    imageLabel->adjustSize();
    scaleFactor = 1.0;
}

/*Отразить вертикаль*/
void ImageViewer::mirrorVert()
{
    imageLabel->setPixmap(QPixmap::fromImage(imageLabel->pixmap()->toImage().mirrored(true,false)));
}

/*Отразить горизонталь*/
void ImageViewer::mirrorHor()
{
    imageLabel->setPixmap(QPixmap::fromImage(imageLabel->pixmap()->toImage().mirrored(false,true)));
}

/*Масштабирование*/
void ImageViewer::scaleImage(double factor)
{
    Q_ASSERT(imageLabel->pixmap());
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    zoomInAct->setEnabled(scaleFactor < 3.0);
    zoomOutAct->setEnabled(scaleFactor > 0.333);
}

/*Работа с прокруткой*/
void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}

/*Создание событий в меню*/
void ImageViewer::createActions()
{
    openAct = new QAction(tr("&Открыть..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    exitAct = new QAction(tr("&Выйти"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    zoomInAct = new QAction(tr("Увеличить &на (25%)"), this);
    zoomInAct->setShortcut(tr("Ctrl++"));
    zoomInAct->setEnabled(false);
    connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

    zoomOutAct = new QAction(tr("Уменьшить &на (25%)"), this);
    zoomOutAct->setShortcut(tr("Ctrl+-"));
    zoomOutAct->setEnabled(false);
    connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

    normalSizeAct = new QAction(tr("&Стандартный размер"), this);
    normalSizeAct->setShortcut(tr("Ctrl+S"));
    normalSizeAct->setEnabled(false);
    connect(normalSizeAct, SIGNAL(triggered()), this, SLOT(normalSize()));

    mirrorVAct = new QAction(tr("Отразить по &вертикали"), this);
    mirrorVAct->setShortcut(tr("Ctrl+V"));
    mirrorVAct->setEnabled(false);
    connect(mirrorVAct, SIGNAL(triggered()), this, SLOT(mirrorVert()));

    mirrorHAct = new QAction(tr("Отразить по &горизонтали"), this);
    mirrorHAct->setShortcut(tr("Ctrl+H"));
    mirrorHAct->setEnabled(false);
    connect(mirrorHAct, SIGNAL(triggered()), this, SLOT(mirrorHor()));

    binarAct = new QAction(tr("&Бинаризация"), this);
    binarAct->setShortcut(tr("Ctrl+B"));
    binarAct->setEnabled(false);
    connect(binarAct, SIGNAL(triggered()), this, SLOT(binar()));

    grayAct= new QAction(tr("&К оттенкам серому"), this);
    grayAct->setShortcut(tr("Ctrl+T"));
    grayAct->setEnabled(false);
    connect(grayAct, SIGNAL(triggered()), this, SLOT(toGray()));

    negativeAct = new QAction(tr("&Негатив"), this);
    negativeAct->setShortcut(tr("Ctrl+N"));
    negativeAct->setEnabled(false);
    connect(negativeAct, SIGNAL(triggered()), this, SLOT(toNegative()));

    originalAct = new QAction(tr("&К оригиналу"), this);
    originalAct->setShortcut(tr("Ctrl+R"));
    originalAct->setEnabled(false);
    connect(originalAct, SIGNAL(triggered()), this, SLOT(original()));

    brightUpAct= new QAction(tr("&Увеличить яркость"), this);
    brightUpAct->setShortcut(tr("Up"));
    brightUpAct->setEnabled(false);
    connect(brightUpAct, SIGNAL(triggered()), this, SLOT(brightUp()));

    brightDownAct= new QAction(tr("&Уменьшить яркость"), this);
    brightDownAct->setShortcut(tr("Down"));
    brightDownAct->setEnabled(false);
    connect(brightDownAct, SIGNAL(triggered()), this, SLOT(brightDown()));

    contrastUpAct= new QAction(tr("&Увеличить контрастность"), this);
    contrastUpAct->setShortcut(tr("Right"));
    contrastUpAct->setEnabled(false);
    connect(contrastUpAct, SIGNAL(triggered()), this, SLOT(contrastUp()));

    contrastDownAct= new QAction(tr("&Уменьшить контрастность"), this);
    contrastDownAct->setShortcut(tr("Left"));
    contrastDownAct->setEnabled(false);
    connect(contrastDownAct, SIGNAL(triggered()), this, SLOT(contrastDown()));
}

/*Меню*/
void ImageViewer::createMenus()
{
    fileMenu = new QMenu(tr("&Файл"), this);
    fileMenu->addAction(openAct);
    fileMenu->addSeparator();
    fileMenu->addAction(originalAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    viewMenu = new QMenu(tr("&Размер"), this);
    viewMenu->addAction(zoomInAct);
    viewMenu->addAction(zoomOutAct);
    viewMenu->addAction(normalSizeAct);

    work=new QMenu(tr("&Опции"), this);
    work->addAction(mirrorVAct);
    work->addAction(mirrorHAct);

    colorStuff = new QMenu(tr("&Цвет"), this);
    colorStuff->addAction(binarAct);
    colorStuff->addAction(grayAct);
    colorStuff->addAction(negativeAct);
    colorStuff->addSeparator();
    colorStuff->addAction(brightUpAct);
    colorStuff->addAction(brightDownAct);
    colorStuff->addSeparator();
    colorStuff->addAction(contrastUpAct);
    colorStuff->addAction(contrastDownAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(viewMenu);
    menuBar()->addMenu(work);
    menuBar()->addMenu(colorStuff);
}

/*Открытие файла*/
void ImageViewer::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть файл"), QDir::currentPath());

    if (!fileName.isEmpty()) {
        QImage image(fileName);
        if (image.isNull()) {
            QMessageBox::information(this, tr("Просмотр изображения"), tr("Не удалось загрузить %1.").arg(fileName));
            return;
        }
        imageLabel->setPixmap(QPixmap::fromImage(image));
        photo = imageLabel->pixmap()->toImage();
        photoCur=photo;
        scaleFactor = 1.0;

        binarAct->setEnabled(true);
        mirrorVAct->setEnabled(true);
        mirrorHAct->setEnabled(true);
        grayAct->setEnabled(true);
        negativeAct->setEnabled(true);
        originalAct->setEnabled(true);
        brightDownAct->setEnabled(true);
        brightUpAct->setEnabled(true);
        zoomInAct->setEnabled(true);
        zoomOutAct->setEnabled(true);
        normalSizeAct->setEnabled(true);
        contrastDownAct->setEnabled(true);
        contrastUpAct->setEnabled(true);   
        imageLabel->adjustSize();

        layoutUpdate();
    }
}
