#include "SindyMath.h"

namespace sindy
{
double roundFloat(double value, uint8_t num)
{
    if (num == 0)
        return value;

    double srcZS    = floor(value);
    double lngMulti = pow((double)10, num);

    value -= srcZS;
    value *= lngMulti;
    double src = floor(value);
    if (value - src >= 0.5)
    {
        double dblXS = (src + 1) / lngMulti;
        return srcZS + dblXS;
    }

    double dblXS = src / lngMulti;
    return srcZS + dblXS;
}

std::string simplifyFloat(double value, uint8_t num)
{
    if (value < 0.0)
    {
        auto result = simplifyFloat(-value, num);
        if (result != "0")
            result = '-' + result;
        return result;
    }

    // 分离整数、小数
    uint64_t iFloor = floor(value);
    double   dec    = value - iFloor;

    uint64_t pow = powTen(num);

    // 四舍五入
    double decNew = dec * pow;

    // 通过float精度转换实现四舍五入
    uint64_t floorNew = (double)(decNew + 0.5);

    // 可能出现进位至整数部分
    if (floorNew == pow)
    {
        floorNew = 0;
        ++iFloor;
    }

    // 整数部分
    std::string strFloor = std::to_string(iFloor);
    if (floorNew <= 0)
        return strFloor;

    std::string& ret = strFloor;
    // 小数部分，需要处理前置位补0和后置位去0操作。如保留三位小数时，小数部分是60，但显示为0.06
    std::string strDec = std::to_string(floorNew);

    int lenDec = strDec.size();
    if (lenDec == num)
    {
        ret.push_back('.');
        ret += strDec;
        trimInvalid0(ret);
    }
    else
    {
        ret.push_back('.');
        lenDec = num - lenDec;
        while (--lenDec >= 0)
        {
            ret.push_back('0');
        }
        trimInvalid0(strDec);
        ret += strDec;
    }

    return ret;
}

void trimInvalid0(std::string& strFloat)
{
    int end = strFloat.size();
    for (int i = end - 1; i >= 0; --i)
    {
        if (strFloat[i] != '0')
            break;
        --end;
    }
    if (end == strFloat.size())
        return;

    strFloat = std::string(strFloat.begin(), strFloat.begin() + end);
}

uint64_t powTen(uint8_t num)
{
    uint64_t multiplier = 1;
    switch (num)
    {
        case 0:
            break;
        case 1:
            multiplier = 10;
            break;
        case 2:
            multiplier = 100;
            break;
        case 3:
            multiplier = 1000;
            break;
        case 4:
            multiplier = 10000;
            break;
        case 5:
            multiplier = 100000;
            break;
        case 6:
            multiplier = 1000000;
            break;
        case 7:
            multiplier = 10000000;
            break;
        case 8:
            multiplier = 100000000;
            break;
        case 9:
            multiplier = 1000000000;
            break;
        case 10:
            multiplier = 10000000000;
            break;
        default:
            for (auto i = 0; i < num; ++i)
            {
                multiplier *= 10;
            }
            break;
    }
    return multiplier;
}

} // namespace sindy
