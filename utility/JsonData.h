#ifndef SINDY_JSON_DATA_H
#define SINDY_JSON_DATA_H

#include <string>

#include "../iguana/iguana/json_reader.hpp"
#include "../iguana/iguana/json_writer.hpp"

namespace sindy
{
void writeContents(std::string const &filename, std::string const &text);

void parseFromJson(std::string const &contents);
void saveAsJson();

class JsonString
{
    std::vector<std::string> _arrry;

public:
    JsonString() {}

    template <typename T>
    JsonString(T const &object)
    {
        pushObject(object);
    }

    template <typename T>
    inline void pushObject(T const &object)
    {
        _arrry.emplace_back(makeJson(object));
    }
    inline void push(std::string const &str) { _arrry.emplace_back(str); }

    inline std::vector<std::string> &arrayRef() { return _arrry; }

    template <typename T>
    static std::string makeJson(T const &object)
    {
        std::string str;
        iguana::to_json(object, str);
        return str;
    }

    static std::string makeArray(std::vector<JsonString> const &objects)
    {
        JsonString object;
        for (auto const &str : objects)
            object._arrry.insert(object._arrry.end(), str._arrry.begin(), str._arrry.end());
        return object.make();
    }

    inline std::string make() const
    {
        auto size = _arrry.size();
        if (size == 0)
            return {};
        std::string json("[");
        for (auto i = 0; i < size; ++i)
        {
            json += _arrry[i];
            if (i != size - 1)
                json.push_back(',');
        }
        json.push_back(']');
        return json;
    }
};

inline bool findInt(iguana::jobject const &keyValues, std::string const &key, int &value)
{
    auto it = keyValues.find(key);
    if (it == keyValues.end())
        return false;
    value = it->second.to_int();
    return true;
}
inline bool findDouble(iguana::jobject const &keyValues, std::string const &key, double &value)
{
    auto it = keyValues.find(key);
    if (it == keyValues.end())
        return false;
    value = it->second.to_double();
    return true;
}
inline bool findString(iguana::jobject const &keyValues, std::string const &key, std::string &value)
{
    auto it = keyValues.find(key);
    if (it == keyValues.end())
        return false;
    value = it->second.to_string();
    return true;
}

std::string stringify(iguana::jvalue const &value);
void        _stringify(iguana::jvalue const &value, std::string &json);

} // namespace sindy

#endif // !SINDY_JSON_DATA_H
