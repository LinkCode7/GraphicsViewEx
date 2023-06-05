#ifndef SAVE_FLAGS_H
#define SAVE_FLAGS_H

#define DOCUMENT_SAVE_FILE_NAME "GraphicsDocument.txt"

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
        eDocVersion = 0, // 文档版本

        eViewInfo, // 画布信息

        eIGeGraphicInfo, // 基类中的普通信息
        eIGeGraphicMat,  // 基类中的变换信息

        eIGePointInfo,    // 点
        eIGePointSetInfo, // 点集

        // 子类
        eGeAimInfo, // 定位点
        eGeBoxInfo, // 包围盒
        eChipInfo,
        eGeSquarePointsInfo, // 方形点集
        // eGePolylineIndexInfo, // 带编号的连续线段
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
class GeAim;
class GePolyline;
class IGeGraphic;
class GeSquarePoints;
class IGePointSet;
class IGePoint;
class GePolylineIndex;
class GeSegment;

// 访问者
class VisitGraphics
{
public:
    virtual void visit(IGeGraphic* pItem) = 0;
    virtual void visit(IGePoint*)         = 0;
    virtual void visit(IGePointSet*)      = 0;

    virtual void visit(GeBox*)           = 0;
    virtual void visit(GeAim*)           = 0;
    virtual void visit(GePolyline*)      = 0;
    virtual void visit(GePolylineIndex*) = 0;
    virtual void visit(GeSquarePoints*)  = 0;
    virtual void visit(GeSegment*)       = 0;
};

#endif // !SAVE_FLAGS_H
