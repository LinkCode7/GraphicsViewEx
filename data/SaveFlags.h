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

    enum Flag
    {
        eDocVersion = 0, // 文档版本

        eViewInfo, // 画布信息

        eBasicGraphicInfo, // 基类中的普通信息
        eBasicGraphicMat,  // 基类中的变换信息

        // 子类
        eBoxGraphicInfo,
        eChipInfo,
        ePointGraphicInfo,
        ePolylineGraphic,
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

class BoxGraphic;
class PointGraphic;
class PolylineGraphic;
class BasicGraphic;

// 访问者
class VisitGraphics
{
public:
    virtual void visit(BasicGraphic* pItem) = 0;
    virtual void visit(BoxGraphic*)         = 0;
    virtual void visit(PointGraphic*)       = 0;
    virtual void visit(PolylineGraphic*)    = 0;
};

#endif // !SAVE_FLAGS_H
