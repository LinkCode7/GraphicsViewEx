#include "kiwi.h"

namespace sindyk
{
#ifndef INCLUDE_SINDYK_H
    #define INCLUDE_SINDYK_H

class BinarySchema
{
public:
    bool                      parse(kiwi::ByteBuffer &bb);
    const kiwi::BinarySchema &underlyingSchema() const { return _schema; }
    bool                      skipGraphicNodeField(kiwi::ByteBuffer &bb, uint32_t id) const;
    bool                      skipChipField(kiwi::ByteBuffer &bb, uint32_t id) const;
    bool                      skipDocumentField(kiwi::ByteBuffer &bb, uint32_t id) const;

private:
    kiwi::BinarySchema _schema;
    uint32_t           _indexGraphicNode = 0;
    uint32_t           _indexChip        = 0;
    uint32_t           _indexDocument    = 0;
};

enum class NodeType : uint32_t
{
    eBox      = 1,
    eChip     = 2,
    ePoint    = 3,
    ePolyline = 4,
};

class Rect;
class Point2dXY;
class Matrix2d;
class GraphicNode;
class Chip;
class Document;

class Rect
{
public:
    Rect() { (void)_flags; }

    float *      left();
    const float *left() const;
    void         set_left(const float &value);

    float *      top();
    const float *top() const;
    void         set_top(const float &value);

    float *      right();
    const float *right() const;
    void         set_right(const float &value);

    float *      bottom();
    const float *bottom() const;
    void         set_bottom(const float &value);

    bool encode(kiwi::ByteBuffer &bb);
    bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
    uint32_t _flags[1]    = {};
    float    _data_left   = {};
    float    _data_top    = {};
    float    _data_right  = {};
    float    _data_bottom = {};
};

class Point2dXY
{
public:
    Point2dXY() { (void)_flags; }

    float *      x();
    const float *x() const;
    void         set_x(const float &value);

    float *      y();
    const float *y() const;
    void         set_y(const float &value);

    bool encode(kiwi::ByteBuffer &bb);
    bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
    uint32_t _flags[1] = {};
    float    _data_x   = {};
    float    _data_y   = {};
};

class Matrix2d
{
public:
    Matrix2d() { (void)_flags; }

    float *      _m11();
    const float *_m11() const;
    void         set__m11(const float &value);

    float *      _m12();
    const float *_m12() const;
    void         set__m12(const float &value);

    float *      _m21();
    const float *_m21() const;
    void         set__m21(const float &value);

    float *      _m22();
    const float *_m22() const;
    void         set__m22(const float &value);

    float *      _dx();
    const float *_dx() const;
    void         set__dx(const float &value);

    float *      _dy();
    const float *_dy() const;
    void         set__dy(const float &value);

    bool encode(kiwi::ByteBuffer &bb);
    bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
    uint32_t _flags[1]  = {};
    float    _data__m11 = {};
    float    _data__m12 = {};
    float    _data__m21 = {};
    float    _data__m22 = {};
    float    _data__dx  = {};
    float    _data__dy  = {};
};

class GraphicNode
{
public:
    GraphicNode() { (void)_flags; }

    NodeType *      type();
    const NodeType *type() const;
    void            set_type(const NodeType &value);

    int32_t *      rgba64();
    const int32_t *rgba64() const;
    void           set_rgba64(const int32_t &value);

    int32_t *      id();
    const int32_t *id() const;
    void           set_id(const int32_t &value);

    Matrix2d *      mat();
    const Matrix2d *mat() const;
    void            set_mat(Matrix2d *value);

    Rect *      rect();
    const Rect *rect() const;
    void        set_rect(Rect *value);

    Point2dXY *      pt();
    const Point2dXY *pt() const;
    void             set_pt(Point2dXY *value);

    kiwi::Array<Point2dXY> *      points();
    const kiwi::Array<Point2dXY> *points() const;
    kiwi::Array<Point2dXY> &      set_points(kiwi::MemoryPool &pool, uint32_t count);

    bool encode(kiwi::ByteBuffer &bb);
    bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
    uint32_t               _flags[1]    = {};
    NodeType               _data_type   = {};
    Matrix2d *             _data_mat    = {};
    Rect *                 _data_rect   = {};
    Point2dXY *            _data_pt     = {};
    kiwi::Array<Point2dXY> _data_points = {};
    int32_t                _data_rgba64 = {};
    int32_t                _data_id     = {};
};

class Chip
{
public:
    Chip() { (void)_flags; }

    NodeType *      type();
    const NodeType *type() const;
    void            set_type(const NodeType &value);

    int32_t *      x();
    const int32_t *x() const;
    void           set_x(const int32_t &value);

    int32_t *      y();
    const int32_t *y() const;
    void           set_y(const int32_t &value);

    int32_t *      color();
    const int32_t *color() const;
    void           set_color(const int32_t &value);

    kiwi::Array<Point2dXY> *      stuff();
    const kiwi::Array<Point2dXY> *stuff() const;
    kiwi::Array<Point2dXY> &      set_stuff(kiwi::MemoryPool &pool, uint32_t count);

    bool encode(kiwi::ByteBuffer &bb);
    bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
    uint32_t               _flags[1]   = {};
    NodeType               _data_type  = {};
    kiwi::Array<Point2dXY> _data_stuff = {};
    int32_t                _data_x     = {};
    int32_t                _data_y     = {};
    int32_t                _data_color = {};
};

class Document
{
public:
    Document() { (void)_flags; }

    kiwi::String *      create_version();
    const kiwi::String *create_version() const;
    void                set_create_version(const kiwi::String &value);

    kiwi::String *      last_open_version();
    const kiwi::String *last_open_version() const;
    void                set_last_open_version(const kiwi::String &value);

    Matrix2d *      mat();
    const Matrix2d *mat() const;
    void            set_mat(Matrix2d *value);

    int32_t *      background();
    const int32_t *background() const;
    void           set_background(const int32_t &value);

    kiwi::Array<GraphicNode> *      graphics();
    const kiwi::Array<GraphicNode> *graphics() const;
    kiwi::Array<GraphicNode> &      set_graphics(kiwi::MemoryPool &pool, uint32_t count);

    kiwi::Array<Chip> *      chips();
    const kiwi::Array<Chip> *chips() const;
    kiwi::Array<Chip> &      set_chips(kiwi::MemoryPool &pool, uint32_t count);

    bool encode(kiwi::ByteBuffer &bb);
    bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
    uint32_t                 _flags[1]               = {};
    kiwi::String             _data_create_version    = {};
    kiwi::String             _data_last_open_version = {};
    Matrix2d *               _data_mat               = {};
    kiwi::Array<GraphicNode> _data_graphics          = {};
    kiwi::Array<Chip>        _data_chips             = {};
    int32_t                  _data_background        = {};
};

#endif

} // namespace sindyk
