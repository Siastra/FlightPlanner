/*
 * Author: Königsreiter Simon, Klimont Joel, Lampalzer Alexander, Glavanits Marcel
 * Class: 5BHIF
 * Date: 14.02.2019
 */

#ifndef DRAWABLEMAPWIDGET_H
#define DRAWABLEMAPWIDGET_H

#include <QWidget>
#include "dbmanager.h"
#include <QApplication>

class DrawableMapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DrawableMapWidget(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent* e);
    std::tuple<int, int> latLonToImg(int lat, int lon);
    std::tuple<int, int> latLonToImg(std::tuple<int, int, int> inp);

    void connectTheDots(std::vector<std::vector<int>> routes);

signals:

public slots:

private:
    QPixmap pic;

    void resetPic();
};

#endif // DRAWABLEMAPWIDGET_H
