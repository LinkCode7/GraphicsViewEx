#ifndef GRAPHIC_UTILITY_H
#define GRAPHIC_UTILITY_H

class QRectF;
class QPointF;
#include <string>

#include "stdint.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846 // pi
#endif

namespace sindy
{
// 角度转弧度
constexpr double angle2radian(double angle)
{
    return M_PI / 180 * angle;
}
// 弧度转角度
constexpr double radian2angle(double radian)
{
    return 180 / M_PI * radian;
}

void extent2Rect(QPointF const& ptMin, QPointF const& ptMax, QRectF& rect);
void extent2Rect(double minX, double minY, double maxX, double maxY, QRectF& rect);
void point2Rect(double x, double y, QRectF& rect, double halfLength);

void addTestEntity();
void addChipToScene();

// 数据流转十六进制字符串
std::string stream2HexString(uint8_t* data, size_t length);
// 十六进制字符串转数据流
void hexString2Stream(std::string const& hexString, uint8_t*& data, size_t& length);

} // namespace sindy

#endif // !GRAPHIC_UTILITY_H
