#include "JsonData.h"

#include <fstream>
#include <sstream>

#define SPLIT_CHAR '\n'
#include "../graphics/GePolygon.h"

#ifndef PI
    #define PI 3.14159265358979323846    // pi
    #define PI2 6.28318530717958623200   // pi*2
    #define PI_2 1.57079632679489661923  // pi/2
    #define PI_4 0.785398163397448309616 // pi/4
#endif

namespace sindy
{
void writeContents(std::string const& filename, std::string const& text)
{
    std::fstream fs;
    fs.open(filename, std::ios::out);
    fs.write(text.c_str(), text.size());
    fs.close();
}

void parseFromJson(std::string const& contents)
{
    size_t cursor = 0;
    size_t length = contents.size();

    while (cursor < length)
    {
        int64_t index = contents.find(SPLIT_CHAR, cursor);
        if (index < 0)
            index = length;

        auto count = index - cursor;
        auto line  = contents.substr(cursor, count);
        cursor     = index + 1;

        // parse
        iguana::jvalue values;
        iguana::parse(values, line);

        auto&& arr = values.to_array();
        for (auto const& value : arr)
        {
            bool is = value.is_object();
            is      = is;
        }
    }
}

void saveAsJson()
{
    using namespace sindy;
    PolySegment element0({0, 0}, {50, 0});
    PolyArc     element1({50, 0}, 50, PI, PI);
    PolySegment element2({100, 0}, {100, 100});
    PolySegment element3({100, 100}, {0, 0});

    auto json = JsonString::makeArray({element0, element1, element2, element3});
    parseFromJson(json);

    // 一个外轮廓
    auto json2 = JsonString::makeArray({Polygon({4}), element0, element1, element2, element3});
    // 一个外轮廓，一个内洞
    auto json3 = JsonString::makeArray(
        {Polygon({4, 4}), element0, element1, element2, element3, element0, element1, element2, element3});
}

std::string sindy::stringify(iguana::jvalue const& value)
{
    std::string json;
    _stringify(value, json);
    return json;
}

void sindy::_stringify(iguana::jvalue const& value, std::string& json)
{
    if (value.is_int())
    {
        json += std::to_string(value.to_int());
    }
    else if (value.is_double())
    {
        json += std::to_string(value.to_double());
    }
    else if (value.is_string())
    {
        json.push_back('"');
        json += value.to_string();
        json.push_back('"');
    }
    else if (value.is_bool())
    {
        if (value.to_bool())
            json += "\"true\"";
    }
    else if (value.is_null())
    {
    }
    else if (value.is_array())
    {
        auto arr  = value.to_array();
        auto size = arr.size();
        if (size == 0)
            return;

        json.push_back('[');
        for (auto i = 0; i < size; ++i)
        {
            if (i > 0)
                json.push_back(',');
            _stringify(value, json);
        }
        json.push_back(']');
    }
    else if (value.is_object())
    {
        auto key_value = value.to_object();
        if (key_value.empty())
            return;

        bool first = true;
        json.push_back('{');
        for (auto const& [key, value] : key_value)
        {
            if (first)
                first = false;
            else
                json.push_back(',');

            json.push_back('"');
            json += key;
            json.push_back('"');

            json.push_back(':');
            _stringify(value, json);
        }

        json.push_back('}');
    }
}

} // namespace sindy