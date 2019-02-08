#include "drawablemapwidget.h"

#include "mainwindow.h"
#include <algorithm>

#include <QStringList>
#include <QPainter>
#include <QApplication>

DrawableMapWidget::DrawableMapWidget(QWidget *parent) : QWidget(parent)
{
}

void DrawableMapWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QPixmap map(":/static/Earthmap.jpg");

    painter.drawPixmap(0, 0, map);

    painter.setPen(QPen{QBrush{QColor{0,255,0, 200}}, 2});

    auto airports = static_cast<MainWindow*>(qApp->activeWindow())->dbm.getLatLongOfAllAirports();
    for (std::tuple<int, int> airport : airports) {
        auto img_coord = latLonToImg(airport);
        int lat = std::get<0>(img_coord);
        int lon = std::get<1>(img_coord);

        painter.drawPoint(QPoint{lon, lat});
    }
}

std::tuple<int, int> DrawableMapWidget::latLonToImg(int lat, int lon)
{
    return std::make_tuple<int, int>((lat - 90) * -2, (lon + 180) * 2);
}

std::tuple<int, int> DrawableMapWidget::latLonToImg(std::tuple<int, int> inp)
{
    return latLonToImg(std::get<0>(inp), std::get<1>(inp));
}

