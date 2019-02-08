#ifndef DRAWABLEMAPWIDGET_H
#define DRAWABLEMAPWIDGET_H

#include <QWidget>
#include "dbmanager.h"
#include "mainwindow.h"
#include <QApplication>

class DrawableMapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DrawableMapWidget(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent* e);
    std::tuple<int, int> latLonToImg(int lat, int lon);
    std::tuple<int, int> latLonToImg(std::tuple<int, int> inp);

signals:

public slots:

private:
};

#endif // DRAWABLEMAPWIDGET_H
