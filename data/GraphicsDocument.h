#ifndef GRAPHICS_DOCUMENT_H
#define GRAPHICS_DOCUMENT_H
#include <string>
#include <unordered_map>

#include "SaveFlags.h"

class IGeGraphic;

class Document
{
    Document() {}
    Document(Document const&) = delete;

public:
    static inline Document& instance()
    {
        static Document doc;
        return doc;
    }

    void reset();
    void calculateNewId();

    void addGraphic(IGeGraphic* pGraphic);
    bool removeGraphic(IGeGraphic* pGraphic);

    // 获取所有图形
    void getObjects(std::vector<IGeGraphic*>& arrObject);

    void        createVersion(std::string const& str) { _createVersion = str; }
    std::string createVersion() const { return _createVersion; }

    void        lastOpenVersion(std::string const& str) { _lastOpenVersion = str; }
    std::string lastOpenVersion() const { return _lastOpenVersion; }

private:
    uint _newId = 0;

    std::string _createVersion;
    std::string _lastOpenVersion;

    std::unordered_map<uint, IGeGraphic*> _mapId2Geometry;
};

#endif // !GRAPHICS_DOCUMENT_H
