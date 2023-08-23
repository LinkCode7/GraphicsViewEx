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
    bool                      skipDocumentField(kiwi::ByteBuffer &bb, uint32_t id) const;

private:
    kiwi::BinarySchema _schema;
    uint32_t           _indexGraphicNode = 0;
    uint32_t           _indexDocument    = 0;
};

enum class NodeType : uint32_t
{
    eBox           = 1,
    eChip          = 2,
    eAim           = 3,
    ePolyline      = 4,
    ePolylineIndex = 5,
    eSquarePoints  = 6,
    eSegment       = 7,
    eArc           = 8,
    eBezier        = 9,
    ePolygon       = 10,
};

class Rect;
class Point2dXY;
class Matrix2d;
class GraphicNode;
class Document;

class Rect
{
public:
    Rect() { (void)_flags; }

    float       *left();
    const float *left() const;
    void         set_left(const float &value);

    float       *top();
    const float *top() const;
    void         set_top(const float &value);

    float       *right();
    const float *right() const;
    void         set_right(const float &value);

    float       *bottom();
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

    float       *x();
    const float *x() const;
    void         set_x(const float &value);

    float       *y();
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

    float       *_m11();
    const float *_m11() const;
    void         set__m11(const float &value);

    float       *_m12();
    const float *_m12() const;
    void         set__m12(const float &value);

    float       *_m13();
    const float *_m13() const;
    void         set__m13(const float &value);

    float       *_m21();
    const float *_m21() const;
    void         set__m21(const float &value);

    float       *_m22();
    const float *_m22() const;
    void         set__m22(const float &value);

    float       *_m23();
    const float *_m23() const;
    void         set__m23(const float &value);

    float       *_m31();
    const float *_m31() const;
    void         set__m31(const float &value);

    float       *_m32();
    const float *_m32() const;
    void         set__m32(const float &value);

    float       *_m33();
    const float *_m33() const;
    void         set__m33(const float &value);

    bool encode(kiwi::ByteBuffer &bb);
    bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
    uint32_t _flags[1]  = {};
    float    _data__m11 = {};
    float    _data__m12 = {};
    float    _data__m13 = {};
    float    _data__m21 = {};
    float    _data__m22 = {};
    float    _data__m23 = {};
    float    _data__m31 = {};
    float    _data__m32 = {};
    float    _data__m33 = {};
};

class GraphicNode
{
public:
    GraphicNode() { (void)_flags; }

    NodeType       *type();
    const NodeType *type() const;
    void            set_type(const NodeType &value);

    uint32_t       *argb();
    const uint32_t *argb() const;
    void            set_argb(const uint32_t &value);

    kiwi::String       *id();
    const kiwi::String *id() const;
    void                set_id(const kiwi::String &value);

    Matrix2d       *mat();
    const Matrix2d *mat() const;
    void            set_mat(Matrix2d *value);

    uint32_t       *flags();
    const uint32_t *flags() const;
    void            set_flags(const uint32_t &value);

    Rect       *rect();
    const Rect *rect() const;
    void        set_rect(Rect *value);

    kiwi::Array<Point2dXY>       *points();
    const kiwi::Array<Point2dXY> *points() const;
    kiwi::Array<Point2dXY>       &set_points(kiwi::MemoryPool &pool, uint32_t count);

    bool encode(kiwi::ByteBuffer &bb);
    bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
    uint32_t               _flags[1]    = {};
    NodeType               _data_type   = {};
    kiwi::String           _data_id     = {};
    Matrix2d              *_data_mat    = {};
    Rect                  *_data_rect   = {};
    kiwi::Array<Point2dXY> _data_points = {};
    uint32_t               _data_argb   = {};
    uint32_t               _data_flags  = {};
};

class Document
{
public:
    Document() { (void)_flags; }

    kiwi::String       *create_version();
    const kiwi::String *create_version() const;
    void                set_create_version(const kiwi::String &value);

    kiwi::String       *last_open_version();
    const kiwi::String *last_open_version() const;
    void                set_last_open_version(const kiwi::String &value);

    Matrix2d       *matView();
    const Matrix2d *matView() const;
    void            set_matView(Matrix2d *value);

    uint32_t       *background();
    const uint32_t *background() const;
    void            set_background(const uint32_t &value);

    kiwi::Array<GraphicNode>       *graphics();
    const kiwi::Array<GraphicNode> *graphics() const;
    kiwi::Array<GraphicNode>       &set_graphics(kiwi::MemoryPool &pool, uint32_t count);

    bool encode(kiwi::ByteBuffer &bb);
    bool decode(kiwi::ByteBuffer &bb, kiwi::MemoryPool &pool, const BinarySchema *schema = nullptr);

private:
    uint32_t                 _flags[1]               = {};
    kiwi::String             _data_create_version    = {};
    kiwi::String             _data_last_open_version = {};
    Matrix2d                *_data_matView           = {};
    kiwi::Array<GraphicNode> _data_graphics          = {};
    uint32_t                 _data_background        = {};
};

#endif

} // namespace sindyk
