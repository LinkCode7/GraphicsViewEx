#ifndef SINDY_MATH_H
#define SINDY_MATH_H

#include <stdint.h>

#include <string>

#ifndef SINDY_ZERO
    #define SINDY_ZERO 0.0000001 // 1e-7
#endif

namespace sindy
{
inline double getAbs(double dValue)
{
    if (dValue > 0)
        return dValue;
    else
        return -dValue;
}

// 比较两个浮点数：0表示相同 1表示前面大 -1表示后面大
inline int compare(const double src1, const double src2, double tol = SINDY_ZERO)
{
    double dblSub = src1 - src2;
    if (getAbs(dblSub) <= tol)
        return 0; // 相同
    else if (dblSub > tol)
        return 1;
    else
        return -1;
}

// 四舍五入
double roundFloat(double value, uint8_t num);

// 四舍五入并去除尾部无效的0
std::string simplifyFloat(double value, uint8_t num);
// 去除尾部的0，"1.4142100" -> "1.41421"
void trimInvalid0(std::string& strFloat);

uint64_t powTen(uint8_t num);

} // namespace sindy

#endif // !SINDY_MATH_H
