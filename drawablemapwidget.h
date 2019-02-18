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
#include "airport.h"

class DrawableMapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DrawableMapWidget(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent* e);
    std::tuple<double, double> latLonToImg(double lat, double lon);
    std::tuple<double, double> latLonToImg(std::tuple<double, double, double> inp);

    void connectTheDots(std::vector<std::vector<int>> routes);

    QPoint latLonToPoint(double lat, double lon);
signals:

public slots:

private:
    QPixmap pic;

    void connectAirports(Airport from, Airport to, QPainter &painter);
    void drawIatas(Airport from, Airport to, QPainter &painter);
    void resetPic();
};

#endif // DRAWABLEMAPWIDGET_H
