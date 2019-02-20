/*
 * Author: Königsreiter Simon, Klimont Joel, Lampalzer Alexander, Glavanits Marcel
 * Class: 5BHIF
 * Date: 14.02.2019
 */

#include "drawablemapwidget.h"

#include "mainwindow.h"
#include <algorithm>

#include <QStringList>
#include <QPainter>
#include <QApplication>


DrawableMapWidget::DrawableMapWidget(QWidget *parent) : QWidget(parent)
{
    resetPic();
}

void DrawableMapWidget::paintEvent(QPaintEvent*)
{
    QPainter painter{this};
    painter.drawPixmap(0, 0, pic);
}

QPoint DrawableMapWidget::latLonToPoint(double lat, double lon)
{
    return QPoint((lon + 180.0) * 4.0,
                  (lat - 90.0) * -4.0);
}



void DrawableMapWidget::connectAirports(Airport fromAP, Airport toAP, QPainter &painter)
{
    auto directDistance = abs(fromAP.lon - toAP.lon);
    auto roundDistance = 360.0 - directDistance;

    auto fromPoint = latLonToPoint(fromAP.lat, fromAP.lon);
    auto toPoint = latLonToPoint(toAP.lat, toAP.lon);

    if (directDistance < roundDistance)
    {
       painter.drawLine(fromPoint, toPoint);
    }
    else
    {
       auto leftSide = latLonToPoint((fromAP.lat + toAP.lat) / 2, -180.0);
       auto rightSide = latLonToPoint((fromAP.lat + toAP.lat) / 2, 180.0);

       if (fromAP.lon > 0)
       {
           painter.drawLine(fromPoint, rightSide);
           painter.drawLine(leftSide, toPoint);
       }
       else
       {
           painter.drawLine(fromPoint, leftSide);
           painter.drawLine(rightSide, toPoint);
       }
    }
}

void DrawableMapWidget::drawIatas(Airport from, Airport to, QPainter &painter) {
    painter.setPen(Qt::white);
    QPoint toPnt = latLonToPoint(to.lat, to.lon);
    toPnt.setX(toPnt.x() - 15);
    toPnt.setY(toPnt.y() - 7);

    QPoint fromPnt = latLonToPoint(from.lat, from.lon);
    fromPnt.setX(fromPnt.x() - 15);
    fromPnt.setY(fromPnt.y() - 7);

    painter.drawText(fromPnt, from.iata.toStdString().c_str());
    painter.drawText(toPnt, to.iata.toStdString().c_str());
}

void DrawableMapWidget::connectTheDots(std::vector<std::vector<int>> routes, QString airline)
{
    resetPic();
    DbManager dbm{};
    auto airports = dbm.airports;
    int alliance_id = dbm.getAllianceForAirline(airline);
    QPainter painter{&pic};
    QPen selected_airline{Qt::red, 3};
    QPen alliance_flight{Qt::blue, 3};
    QPen every_flight{Qt::gray, 3};
    //painter.setPen(QPen{QBrush{QColor{82, 82, 255}}, 3});
    painter.setPen(every_flight);

    for (auto route : routes) {
        for (size_t i{0}; i < route.size() - 1; i++) {
            Airport fromAP = airports[route[i]];
            Airport toAP = airports[route[i + 1]];

            if (dbm.getAirlineForRoute(fromAP.id, toAP.id) == airline.simplified().trimmed().toStdString()) {
                painter.setPen(selected_airline);
            } else if (alliance_id == dbm.getAllianceForAirline(dbm.getAirlineForRoute(fromAP.id, toAP.id).c_str())) {
                painter.setPen(alliance_flight);
            } else {
                painter.setPen(every_flight);
            }

            connectAirports(fromAP, toAP, painter);

            drawIatas(fromAP, toAP, painter);
        }
    }

    update();
}

void DrawableMapWidget::resetPic()
{
    QPixmap l_pic{1440, 720};
    DbManager dbm;
    QPainter painter(&l_pic);
    QPixmap map(":/static/Earthmap.jpg");
    map = map.scaled(1440, 720 , Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    painter.drawPixmap(0, 0, map);

    painter.setPen(QPen{QBrush{QColor{0, 255, 0, 200}}, 3});

    auto airports = dbm.airports;
    for (auto airport : airports) {
        painter.drawPoint(latLonToPoint(airport.lat, airport.lon));
    }

    pic = l_pic;
}




