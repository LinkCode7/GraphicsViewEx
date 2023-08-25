#ifndef SAVE_FLAGS_H
#define SAVE_FLAGS_H

#define DOCUMENT_SAVE_FILE_NAME "sindy.txt"

using uint = unsigned int;

// 文档内容改变标志
class SaveFlags
{
public:
    SaveFlags() {}
    SaveFlags(SaveFlags const&) = delete;
    SaveFlags& operator=(SaveFlags const&) = delete;

    // 需要序列化的标志，可细分到每个字段
    enum Flag
    {
        // 文档
        eDocVersion = 0, // 版本

        // 画布
        eViewInfo,

        // 基类数据
        eIGeGraphicInfo,  // 普通信息
        eIGeGraphicMat,   // 变换信息
        eIGePointSetInfo, // 点集[1,n]

        // 子类
        eGeBoxInfo,          // 包围盒
        eGeSquarePointsInfo, // 方形点集
        eGePolygonInfo,      // 多边形数据
        eChipInfo,

        // 其它共性的
        eGraphicSymbolInfo, // 符号点

        eAllCount
    };

    void add(Flag flag) { _bits[flag] = true; }
    void remove(Flag flag) { _bits[flag] = false; }
    bool has(Flag flag) const { return _bits[flag]; }

    void full(bool is)
    {
        for (auto i = 0; i < eAllCount + 1; ++i)
            _bits[i] = is;
    }

private:
    bool _bits[eAllCount + 1] = {};
};

class GeBox;
class GeSymbolPoint;
class GePolyline;
class IGeGraphic;
class GeSquarePoints;
class IGePointSet;
class IGePoint;
class GePolylineIndex;
class GeSegment;
class GePolygon;

// 访问者
class VisitGraphics
{
public:
    virtual void visit(IGeGraphic* pItem) = 0;
    virtual void visit(IGePoint*)         = 0;
    virtual void visit(IGePointSet*)      = 0;

    virtual void visit(GeBox*)          = 0;
    virtual void visit(GeSymbolPoint*)  = 0;
    virtual void visit(GePolyline*)     = 0;
    virtual void visit(GeSquarePoints*) = 0;
    virtual void visit(GePolygon*)      = 0;
};

#endif // !SAVE_FLAGS_H
