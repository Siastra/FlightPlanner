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

std::tuple<int, int> DrawableMapWidget::latLonToImg(int lat, int lon)
{
    return std::make_tuple<int, int>((lat - 90) * -4, (lon + 180) * 4);
}

std::tuple<int, int> DrawableMapWidget::latLonToImg(std::tuple<int, int, int> inp)
{
    return latLonToImg(std::get<1>(inp), std::get<2>(inp));
}

void DrawableMapWidget::connectTheDots(std::vector<std::vector<int> > routes)
{
    resetPic();
    auto airports = DbManager{}.getLatLongOfAllAirports();
    QPainter painter{&pic};
    painter.setPen(QPen{QBrush{QColor{82, 82, 255}}, 3});

    for (auto route : routes) {
        for (size_t i{1}; i < route.size(); i++) {
            auto from_tuple = latLonToImg(airports[route[i - 1] - 1]);
            auto to_tuple = latLonToImg(airports[route[i] - 1]);

            QPoint from{std::get<1>(from_tuple), std::get<0>(from_tuple)};
            QPoint to{std::get<1>(to_tuple), std::get<0>(to_tuple)};

            painter.drawLine(from, to);
        }
    }

    painter.setPen(Qt::black);
    for (auto route : routes) {
        for (auto airport : route) {
            auto tpl = latLonToImg(airports[airport - 1]);
            QPoint pnt{std::get<1>(tpl) - 15, std::get<0>(tpl) - 7};

            painter.drawText(pnt, DbManager{}.getIataForID(airport - 1).c_str());
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

    auto airports = dbm.getLatLongOfAllAirports();
    for (auto airport : airports) {
        auto img_coord = latLonToImg(airport);
        int lat = std::get<0>(img_coord);
        int lon = std::get<1>(img_coord);
        painter.drawPoint(QPoint{lon, lat});
    }

    pic = l_pic;
}




