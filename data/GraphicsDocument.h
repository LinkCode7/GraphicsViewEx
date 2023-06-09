#ifndef GRAPHICS_DOCUMENT_H
#define GRAPHICS_DOCUMENT_H
#include <string>
#include <unordered_map>

#include "SaveFlags.h"

class IGeGraphic;

class Document
{
public:
    Document() {}
    Document(Document const&) = delete;

    void reset();
    void calculateNewId();

    // 添加、删除图形
    void addGraphic(IGeGraphic* pGraphic);
    bool removeGraphic(IGeGraphic* pGraphic);

    // 选中图形
    void                            clearSelectedGraphics() { return _selected.clear(); }
    void                            addSelectedGraphics(IGeGraphic* pGraphic) { return _selected.emplace_back(pGraphic); }
    const std::vector<IGeGraphic*>* selectedGraphics() const { return &_selected; }

    // 获取所有图形
    void getObjects(std::vector<IGeGraphic*>& arrObject);

    // 版本号
    void        createVersion(std::string const& str) { _createVersion = str; }
    std::string createVersion() const { return _createVersion; }
    void        lastOpenVersion(std::string const& str) { _lastOpenVersion = str; }
    std::string lastOpenVersion() const { return _lastOpenVersion; }

private:
    uint _newId = 0;

    std::string _createVersion;
    std::string _lastOpenVersion;

    std::vector<IGeGraphic*>              _selected;
    std::unordered_map<uint, IGeGraphic*> _mapId2Geometry;
};

#endif // !GRAPHICS_DOCUMENT_H
