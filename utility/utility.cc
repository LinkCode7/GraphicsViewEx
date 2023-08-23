#include "utility.h"

#include <fstream>
#include <iomanip>
#include <sstream>

#include "../graphics/GeAim.h"
#include "../graphics/GeBox.h"
#include "../graphics/GePolygon.h"
#include "../graphics/GePolyline.h"
#include "../graphics/GePolylineIndex.h"
#include "../graphics/GeSegment.h"
#include "../graphics/GeSquarePoints.h"
#include "../graphics/chip.h"
#include "../view/GraphicsArchive.h"

void sindy::extent2Rect(double minX, double minY, double maxX, double maxY, QRectF& rect)
{
    rect = {minX, minY, maxX - minX, maxY - minY};
    // rect = {minX, maxY, maxX - minX, maxY - minY};
}

void sindy::extent2Rect(QPointF const& ptMin, QPointF const& ptMax, QRectF& rect)
{
    rect = {ptMin.x(), ptMin.y(), ptMax.x() - ptMin.x(), ptMax.y() - ptMin.y()};
}

void sindy::point2Rect(double x, double y, QRectF& rect, double halfLength)
{
    rect = {x - halfLength, y - halfLength, halfLength * 2, halfLength * 2};
}

void sindy::addTestEntity()
{
    auto pScene = GeArchive().scene();
    if (!pScene)
        return;

    pScene->addItem(new GeBox({100, 100}, {300, 200}));

    pScene->addItem(new GeAim({200, -100}));

    pScene->addItem(new GeSegment({-100, -100}, {100, 100}));

    std::initializer_list<QPointF> list = {{-186.41, 55.28},  {-115.21, 55.28}, {-93.21, 0},     {-71.21, 55.28},
                                           {0, 55.28},        {-57.6, 89.44},   {-35.6, 144.72}, {-93.21, 110.56},
                                           {-150.81, 144.72}, {-128.81, 89.44}, {-186.41, 55.28}};

    pScene->addItem(new GePolyline(list));
    pScene->addItem(new GePolylineIndex(list));
    pScene->addItem(new GeSquarePoints(list));
}

void sindy::addChipToScene()
{
    QImage image;
    image.load("image/qt4logo.png");

    auto pScene = GeArchive().scene();

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
            pScene->addItem(item);

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

bool sindy::readContents(std::string const& filename, std::string& text)
{
    std::fstream fs;
    fs.open(filename, std::ios::in);
    if (!fs.is_open())
        return false;

    std::stringstream ss;
    ss << fs.rdbuf();
    fs.close();

    text = ss.str();
    return true;
}

void writeFromStream(char const* buff, char const* filename, size_t size)
{
    FILE* fp = fopen(filename, "wb+");
    if (!fp)
        return;

    fwrite(buff, 1, size, fp);
    fclose(fp);
}

void readToStream(const char* filename)
{
    FILE* fp = fopen(filename, "rb");
    if (!fp)
        return;

    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);

    char* buff = new char[size];
    memset(buff, 0, size);

    fseek(fp, 0, SEEK_SET);
    size_t nRead = fread(buff, sizeof(char), size, fp);
    fclose(fp);
    if (false && nRead > 0)
    {
        // 将二进制流打印成16进制字符串
        for (unsigned int i = 0; i < nRead; i++)
        {
            printf("%02X ", (unsigned char)buff[i]);
            if (i % 16 == 15)
                printf("\n");
        }
    }

    // 字节序反转<stdlib.h>：_byteswap_ulong

    writeFromStream(buff, "NewFileName", size);
    delete[] buff;
}
