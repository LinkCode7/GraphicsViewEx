#ifndef GRAPHIC_UTILITY_H
#define GRAPHIC_UTILITY_H

class QRectF;

#ifndef M_PI
    #define M_PI 3.14159265358979323846  // pi
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

void extent2Rect(double minX, double minY, double maxX, double maxY, QRectF& rect);
void point2Rect(double x, double y, QRectF& rect, double tol);

void addTestEntity();
void addChipToScene();

}  // namespace sindy

#endif  // !GRAPHIC_UTILITY_H
