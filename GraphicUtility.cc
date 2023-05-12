#include "GraphicUtility.h"

#include <QGraphicsScene>

#include "./graphics/BoxGraphic.h"
#include "./graphics/ChipGraphic.h"
#include "./graphics/PointGraphic.h"

extern QGraphicsScene* g_pScene;

void sindy::extent2Rect(double minX, double minY, double maxX, double maxY, QRectF& rect)
{
    rect = {minX, minY, maxX - minX, maxY - minY};
    // rect = {minX, maxY, maxX - minX, maxY - minY};
}

void sindy::point2Rect(double x, double y, QRectF& rect, double tol)
{
    rect = {x - tol, y - tol, tol * 2, tol * 2};
}

void sindy::addTestEntity()
{
    if (!g_pScene) return;

    g_pScene->addItem(new BoxGraphic(-100, -100, 300, 200));

    g_pScene->addItem(new PointGraphic(100, 100));
}

void sindy::addChipToScene()
{
    QImage image;
    image.load("image/qt4logo.png");

    // Populate g_pScene
    int xx     = 0;
    int nitems = 0;
    for (int i = -11000; i < 11000; i += 110) {
        ++xx;
        int yy = 0;
        for (int j = -7000; j < 7000; j += 70) {
            ++yy;
            qreal x = (i + 11000) / 22000.0;
            qreal y = (j + 7000) / 14000.0;

            QColor         color(image.pixel(int(image.width() * x), int(image.height() * y)));
            QGraphicsItem* item = new ChipGraphic(color, xx, yy);
            item->setPos(QPointF(i, j));
            g_pScene->addItem(item);

            ++nitems;
        }
    }
}
