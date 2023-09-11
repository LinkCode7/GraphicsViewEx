#ifndef SINDY_POINT_H
#define SINDY_POINT_H

#define SINDY_DEFAULT_POINT_VAL 0.0

#define SINDY_PRECISION 1e-8

#ifndef PI
    #define PI 3.14159265358979323846    // pi
    #define PI2 6.28318530717958623200   // pi*2
    #define PI_2 1.57079632679489661923  // pi/2
    #define PI_4 0.785398163397448309616 // pi/4
#endif

#define RADIAN_0 0.0
#define RADIAN_90 PI_2
#define RADIAN_180 PI
#define RADIAN_270 4.71238898038468967400
#define RADIAN_360 PI2

#include <cmath>

namespace sindy
{
class Point2d
{
    double _data[2] = {SINDY_DEFAULT_POINT_VAL, SINDY_DEFAULT_POINT_VAL};

public:
    static const Point2d kOrigin;
    static const Point2d kXAxis;
    static const Point2d kYAxis;

    Point2d() {}
    Point2d(double x, double y)
    {
        _data[0] = x;
        _data[1] = y;
    }

    inline double x() const { return _data[0]; }
    inline void   x(double x) { _data[0] = x; }

    inline void   y(double y) { _data[1] = y; }
    inline double y() const { return _data[1]; }

    inline Point2d& operator=(const Point2d& other)
    {
        if (this == &other)
            return *this;
        _data[0] = other._data[0];
        _data[1] = other._data[1];
        return *this;
    }

    inline bool equal(Point2d const& pt, double tol) const
    {
        return (fabs(_data[0] - pt._data[0]) <= tol && fabs(_data[1] - pt._data[1]) <= tol) ? true : false;
    }
    inline bool operator==(const Point2d& pt) const
    {
        if (fabs(this->_data[0] - pt._data[0]) <= SINDY_PRECISION && fabs(this->_data[1] - pt._data[1]) <= SINDY_PRECISION)
            return true;
        return false;
    }
    inline bool operator!=(const Point2d& pt) const { return !(*this == pt); }

    inline Point2d getAdd(double x, double y) const { return Point2d(_data[0] + x, _data[1] + y); }
    inline Point2d operator+(const Point2d& pt) const { return Point2d(_data[0] + pt._data[0], _data[1] + pt._data[1]); }
    inline Point2d operator+=(const Point2d& pt)
    {
        *this = (*this + pt);
        return *this;
    }
    inline Point2d operator+=(double value)
    {
        _data[0] += value;
        _data[1] += value;
        return *this;
    }

    inline Point2d operator-(const Point2d& pt) const { return Point2d(_data[0] - pt._data[0], _data[1] - pt._data[1]); }
    inline Point2d operator-=(const Point2d& pt)
    {
        *this = (*this - pt);
        return *this;
    }
    inline Point2d operator-=(double value)
    {
        _data[0] -= value;
        _data[1] -= value;
        return *this;
    }

    inline Point2d scale(double scale) const { return {_data[0] * scale, _data[1] * scale}; }
    inline double  operator*(const Point2d& pt) const { return _data[0] * pt._data[0] + _data[1] * pt._data[1]; }

    inline double dot(Point2d const& pt) { return (_data[0] * pt._data[0] + _data[1] * pt._data[1]); };

    // 叉积
    inline double        crossProduct(const Point2d& pt) const { return _data[0] * pt._data[1] - _data[1] * pt._data[0]; }
    inline static double crossProduct(const Point2d& a, const Point2d& b, const Point2d& c, const Point2d& d)
    {
        return Point2d(b - a).crossProduct(Point2d(c - d));
    }

    inline static double crossProduct(double x1, double y1, double x2, double y2) { return x1 * y2 - y1 * x2; }
    inline static double crossProduct(double x1, double y1, double x2, double y2, double x3, double y3)
    {
        return crossProduct(x2 - x1, y2 - y1, x3 - x2, y3 - y2);
    }

    // 求向量ab、bc的叉乘
    inline static double crossProduct(const Point2d& a, const Point2d& b, const Point2d& c)
    {
        return crossProduct(a.x(), a.y(), b.x(), b.y(), c.x(), c.y());
    }

    inline double length() const { return sqrt(_data[0] * _data[0] + _data[1] * _data[1]); };

    // 单位向量
    inline Point2d normalize() const
    {
        if (_data[0] == 0 && _data[1] == 0)
            return Point2d(0, 0);
        double len = length();
        return {_data[0] / len, _data[1] / len};
    }

    // 得到将this绕center旋转指定弧度后的点
    inline Point2d rotate(Point2d const& center, double radian) const
    {
        double cos1 = cos(radian);
        double sin1 = sin(radian);
        double dx   = _data[0] - center._data[0];
        double dy   = _data[1] - center._data[1];
        double x    = dx * cos1 + dy * -sin1;
        double y    = dx * sin1 + dy * cos1;
        return Point2d(center._data[0] + x, center._data[1] + y);
    }

    // 求向量this->center的弧度[-PI, PI]，笛卡尔系坐标轴
    inline double angle(Point2d const& center) const { return atan2(_data[1] - center.y(), _data[0] - center.x()); }
    // 求向量this->center与kXAxis的弧度[0, PI * 2]
    inline double angleX(Point2d const& center = Point2d(0.0, 0.0))
    {
        double result = angle(center);
        if (result < 0.0)
            result = PI2 + result;
        return result;
    }

    inline double distance(Point2d const& pt) const
    {
        return sqrt(pow(fabs(_data[0] - pt._data[0]), 2) + pow(fabs(_data[1] - pt._data[1]), 2));
    }
    inline double powDist(Point2d const& pt) const
    {
        return pow(fabs(_data[0] - pt._data[0]), 2) + pow(fabs(_data[1] - pt._data[1]), 2);
    }
};

} // namespace sindy

#endif // !SINDY_POINT_H
