#ifndef GRAPHIC_UTILITY_H
#define GRAPHIC_UTILITY_H

class QRectF;
class QPointF;
#include <string>

#include "stdint.h"

#ifndef PI
    #define PI 3.14159265358979323846    // pi
    #define PI2 6.28318530717958623200   // pi*2
    #define PI_2 1.57079632679489661923  // pi/2
    #define PI_4 0.785398163397448309616 // pi/4
#endif

namespace sindy
{
void extent2Rect(QPointF const& ptMin, QPointF const& ptMax, QRectF& rect);
void extent2Rect(double minX, double minY, double maxX, double maxY, QRectF& rect);
void point2Rect(double x, double y, QRectF& rect, double halfLength);

void addTestEntity();
void addChipToScene();

// 数据流转十六进制字符串
std::string stream2HexString(uint8_t* data, size_t length);
// 十六进制字符串转数据流
void hexString2Stream(std::string const& hexString, uint8_t*& data, size_t& length);

bool readContents(std::string const& filename, std::string& text);

} // namespace sindy

#endif // !GRAPHIC_UTILITY_H
