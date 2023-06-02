#include "utility.h"

#include <QGraphicsScene>
#include <iomanip>
#include <sstream>

#include "../graphics/BoxGraphic.h"
#include "../graphics/PointGraphic.h"
#include "../graphics/PolylineGraphic.h"
#include "../graphics/chip.h"

extern QGraphicsScene* g_pScene;

void sindy::extent2Rect(double minX, double minY, double maxX, double maxY, QRectF& rect)
{
    rect = {minX, minY, maxX - minX, maxY - minY};
    // rect = {minX, maxY, maxX - minX, maxY - minY};
}

void sindy::extent2Rect(QPointF const& ptMin, QPointF const& ptMax, QRectF& rect)
{
    rect = {ptMin.x(), ptMin.y(), ptMax.x(), ptMax.y()};
}

void sindy::point2Rect(double x, double y, QRectF& rect, double tol)
{
    rect = {x - tol, y - tol, tol * 2, tol * 2};
}

void sindy::addTestEntity()
{
    if (!g_pScene)
        return;

    g_pScene->addItem(new BoxGraphic({100, 100}, {300, 200}));

    g_pScene->addItem(new PointGraphic({200, -100}));

    auto poly = new PolylineGraphic{{0, 55.28},       {71.2, 55.28},  {93.2, 0},      {115.21, 55.28}, {186.41, 55.28}, {128.81, 89.44},
                                    {150.81, 144.72}, {93.2, 110.56}, {35.6, 144.72}, {57.6, 89.44},   {0, 55.28}};
    g_pScene->addItem(poly);
}

void sindy::addChipToScene()
{
    QImage image;
    image.load("image/qt4logo.png");

    // Populate g_pScene
    int xx     = 0;
    int nitems = 0;
    for (int i = -11000; i < 11000; i += 110)
    {
        ++xx;
        int yy = 0;
        for (int j = -7000; j < 7000; j += 70)
        {
            ++yy;
            qreal x = (i + 11000) / 22000.0;
            qreal y = (j + 7000) / 14000.0;

            QColor         color(image.pixel(int(image.width() * x), int(image.height() * y)));
            QGraphicsItem* item = new Chip(color, xx, yy);
            item->setPos(QPointF(i, j));
            g_pScene->addItem(item);

            ++nitems;
        }
    }
}

// 数据流转十六进制字符串
std::string sindy::stream2HexString(uint8_t* data, size_t length)
{
    std::ostringstream ss;
    ss << std::setfill('0') << std::hex;
    for (auto i = 0; i < length; ++i)
        ss << std::setw(2) << static_cast<unsigned int>(data[i]);
    return ss.str();
}

// 十六进制字符串转数据流
void sindy::hexString2Stream(std::string const& hexString, uint8_t*& data, size_t& length)
{
    auto size = hexString.size();
    length    = size / 2;
    data      = new uint8_t[length];

    for (size_t i = 0; i < size; i += 2)
    {
        std::string   byte_str = hexString.substr(i, 2);
        unsigned char byte     = static_cast<unsigned char>(stoi(byte_str, nullptr, 16));
        data[i / 2]            = byte;
    }
}
