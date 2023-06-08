#include "schema.h"

namespace sindyk
{
bool BinarySchema::parse(kiwi::ByteBuffer &bb)
{
    if (!_schema.parse(bb))
        return false;
    _schema.findDefinition("GraphicNode", _indexGraphicNode);
    _schema.findDefinition("Document", _indexDocument);
    return true;
}

bool BinarySchema::skipGraphicNodeField(kiwi::ByteBuffer &bb, uint32_t id) const
{
    return _schema.skipField(bb, _indexGraphicNode, id);
}

bool BinarySchema::skipDocumentField(kiwi::ByteBuffer &bb, uint32_t id) const
{
    return _schema.skipField(bb, _indexDocument, id);
}

float *Rect::left()
{
    return _flags[0] & 1 ? &_data_left : nullptr;
}

const float *Rect::left() const
{
    return _flags[0] & 1 ? &_data_left : nullptr;
}

void Rect::set_left(const float &value)
{
    _flags[0] |= 1;
    _data_left = value;
}

float *Rect::top()
{
    return _flags[0] & 2 ? &_data_top : nullptr;
}

const float *Rect::top() const
{
    return _flags[0] & 2 ? &_data_top : nullptr;
}

void Rect::set_top(const float &value)
{
    _flags[0] |= 2;
    _data_top = value;
}

float *Rect::right()
{
    return _flags[0] & 4 ? &_data_right : nullptr;
}

const float *Rect::right() const
{
    return _flags[0] & 4 ? &_data_right : nullptr;
}

void Rect::set_right(const float &value)
{
    _flags[0] |= 4;
    _data_right = value;
}

float *Rect::bottom()
{
    return _flags[0] & 8 ? &_data_bottom : nullptr;
}

const float *Rect::bottom() const
{
    return _flags[0] & 8 ? &_data_bottom : nullptr;
}

void Rect::set_bottom(const float &value)
{
    _flags[0] |= 8;
    _data_bottom = value;
}

bool Rect::encode(kiwi::ByteBuffer &_bb)
{
    if (left() == nullptr)
        return false;
    _bb.writeVarFloat(_data_left);
    if (top() == nullptr)
        return false;
    _bb.writeVarFloat(_data_top);
    if (right() == nullptr)
        return false;
    _bb.writeVarFloat(_data_right);
    if (bottom() == nullptr)
        return false;
    _bb.writeVarFloat(_data_bottom);
    return true;
}

bool Rect::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema)
{
    if (!_bb.readVarFloat(_data_left))
        return false;
    set_left(_data_left);
    if (!_bb.readVarFloat(_data_top))
        return false;
    set_top(_data_top);
    if (!_bb.readVarFloat(_data_right))
        return false;
    set_right(_data_right);
    if (!_bb.readVarFloat(_data_bottom))
        return false;
    set_bottom(_data_bottom);
    return true;
}

float *Point2dXY::x()
{
    return _flags[0] & 1 ? &_data_x : nullptr;
}

const float *Point2dXY::x() const
{
    return _flags[0] & 1 ? &_data_x : nullptr;
}

void Point2dXY::set_x(const float &value)
{
    _flags[0] |= 1;
    _data_x = value;
}

float *Point2dXY::y()
{
    return _flags[0] & 2 ? &_data_y : nullptr;
}

const float *Point2dXY::y() const
{
    return _flags[0] & 2 ? &_data_y : nullptr;
}

void Point2dXY::set_y(const float &value)
{
    _flags[0] |= 2;
    _data_y = value;
}

bool Point2dXY::encode(kiwi::ByteBuffer &_bb)
{
    if (x() == nullptr)
        return false;
    _bb.writeVarFloat(_data_x);
    if (y() == nullptr)
        return false;
    _bb.writeVarFloat(_data_y);
    return true;
}

bool Point2dXY::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema)
{
    if (!_bb.readVarFloat(_data_x))
        return false;
    set_x(_data_x);
    if (!_bb.readVarFloat(_data_y))
        return false;
    set_y(_data_y);
    return true;
}

float *Matrix2d::_m11()
{
    return _flags[0] & 1 ? &_data__m11 : nullptr;
}

const float *Matrix2d::_m11() const
{
    return _flags[0] & 1 ? &_data__m11 : nullptr;
}

void Matrix2d::set__m11(const float &value)
{
    _flags[0] |= 1;
    _data__m11 = value;
}

float *Matrix2d::_m12()
{
    return _flags[0] & 2 ? &_data__m12 : nullptr;
}

const float *Matrix2d::_m12() const
{
    return _flags[0] & 2 ? &_data__m12 : nullptr;
}

void Matrix2d::set__m12(const float &value)
{
    _flags[0] |= 2;
    _data__m12 = value;
}

float *Matrix2d::_m13()
{
    return _flags[0] & 4 ? &_data__m13 : nullptr;
}

const float *Matrix2d::_m13() const
{
    return _flags[0] & 4 ? &_data__m13 : nullptr;
}

void Matrix2d::set__m13(const float &value)
{
    _flags[0] |= 4;
    _data__m13 = value;
}

float *Matrix2d::_m21()
{
    return _flags[0] & 8 ? &_data__m21 : nullptr;
}

const float *Matrix2d::_m21() const
{
    return _flags[0] & 8 ? &_data__m21 : nullptr;
}

void Matrix2d::set__m21(const float &value)
{
    _flags[0] |= 8;
    _data__m21 = value;
}

float *Matrix2d::_m22()
{
    return _flags[0] & 16 ? &_data__m22 : nullptr;
}

const float *Matrix2d::_m22() const
{
    return _flags[0] & 16 ? &_data__m22 : nullptr;
}

void Matrix2d::set__m22(const float &value)
{
    _flags[0] |= 16;
    _data__m22 = value;
}

float *Matrix2d::_m23()
{
    return _flags[0] & 32 ? &_data__m23 : nullptr;
}

const float *Matrix2d::_m23() const
{
    return _flags[0] & 32 ? &_data__m23 : nullptr;
}

void Matrix2d::set__m23(const float &value)
{
    _flags[0] |= 32;
    _data__m23 = value;
}

float *Matrix2d::_m31()
{
    return _flags[0] & 64 ? &_data__m31 : nullptr;
}

const float *Matrix2d::_m31() const
{
    return _flags[0] & 64 ? &_data__m31 : nullptr;
}

void Matrix2d::set__m31(const float &value)
{
    _flags[0] |= 64;
    _data__m31 = value;
}

float *Matrix2d::_m32()
{
    return _flags[0] & 128 ? &_data__m32 : nullptr;
}

const float *Matrix2d::_m32() const
{
    return _flags[0] & 128 ? &_data__m32 : nullptr;
}

void Matrix2d::set__m32(const float &value)
{
    _flags[0] |= 128;
    _data__m32 = value;
}

float *Matrix2d::_m33()
{
    return _flags[0] & 256 ? &_data__m33 : nullptr;
}

const float *Matrix2d::_m33() const
{
    return _flags[0] & 256 ? &_data__m33 : nullptr;
}

void Matrix2d::set__m33(const float &value)
{
    _flags[0] |= 256;
    _data__m33 = value;
}

bool Matrix2d::encode(kiwi::ByteBuffer &_bb)
{
    if (_m11() == nullptr)
        return false;
    _bb.writeVarFloat(_data__m11);
    if (_m12() == nullptr)
        return false;
    _bb.writeVarFloat(_data__m12);
    if (_m13() == nullptr)
        return false;
    _bb.writeVarFloat(_data__m13);
    if (_m21() == nullptr)
        return false;
    _bb.writeVarFloat(_data__m21);
    if (_m22() == nullptr)
        return false;
    _bb.writeVarFloat(_data__m22);
    if (_m23() == nullptr)
        return false;
    _bb.writeVarFloat(_data__m23);
    if (_m31() == nullptr)
        return false;
    _bb.writeVarFloat(_data__m31);
    if (_m32() == nullptr)
        return false;
    _bb.writeVarFloat(_data__m32);
    if (_m33() == nullptr)
        return false;
    _bb.writeVarFloat(_data__m33);
    return true;
}

bool Matrix2d::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema)
{
    if (!_bb.readVarFloat(_data__m11))
        return false;
    set__m11(_data__m11);
    if (!_bb.readVarFloat(_data__m12))
        return false;
    set__m12(_data__m12);
    if (!_bb.readVarFloat(_data__m13))
        return false;
    set__m13(_data__m13);
    if (!_bb.readVarFloat(_data__m21))
        return false;
    set__m21(_data__m21);
    if (!_bb.readVarFloat(_data__m22))
        return false;
    set__m22(_data__m22);
    if (!_bb.readVarFloat(_data__m23))
        return false;
    set__m23(_data__m23);
    if (!_bb.readVarFloat(_data__m31))
        return false;
    set__m31(_data__m31);
    if (!_bb.readVarFloat(_data__m32))
        return false;
    set__m32(_data__m32);
    if (!_bb.readVarFloat(_data__m33))
        return false;
    set__m33(_data__m33);
    return true;
}

NodeType *GraphicNode::type()
{
    return _flags[0] & 1 ? &_data_type : nullptr;
}

const NodeType *GraphicNode::type() const
{
    return _flags[0] & 1 ? &_data_type : nullptr;
}

void GraphicNode::set_type(const NodeType &value)
{
    _flags[0] |= 1;
    _data_type = value;
}

uint32_t *GraphicNode::argb()
{
    return _flags[0] & 2 ? &_data_argb : nullptr;
}

const uint32_t *GraphicNode::argb() const
{
    return _flags[0] & 2 ? &_data_argb : nullptr;
}

void GraphicNode::set_argb(const uint32_t &value)
{
    _flags[0] |= 2;
    _data_argb = value;
}

int32_t *GraphicNode::id()
{
    return _flags[0] & 4 ? &_data_id : nullptr;
}

const int32_t *GraphicNode::id() const
{
    return _flags[0] & 4 ? &_data_id : nullptr;
}

void GraphicNode::set_id(const int32_t &value)
{
    _flags[0] |= 4;
    _data_id = value;
}

Matrix2d *GraphicNode::mat()
{
    return _data_mat;
}

const Matrix2d *GraphicNode::mat() const
{
    return _data_mat;
}

void GraphicNode::set_mat(Matrix2d *value)
{
    _data_mat = value;
}

Rect *GraphicNode::rect()
{
    return _data_rect;
}

const Rect *GraphicNode::rect() const
{
    return _data_rect;
}

void GraphicNode::set_rect(Rect *value)
{
    _data_rect = value;
}

kiwi::Array<Point2dXY> *GraphicNode::points()
{
    return _flags[0] & 32 ? &_data_points : nullptr;
}

const kiwi::Array<Point2dXY> *GraphicNode::points() const
{
    return _flags[0] & 32 ? &_data_points : nullptr;
}

kiwi::Array<Point2dXY> &GraphicNode::set_points(kiwi::MemoryPool &pool, uint32_t count)
{
    _flags[0] |= 32;
    return _data_points = pool.array<Point2dXY>(count);
}

bool GraphicNode::encode(kiwi::ByteBuffer &_bb)
{
    if (type() != nullptr)
    {
        _bb.writeVarUint(1);
        _bb.writeVarUint(static_cast<uint32_t>(_data_type));
    }
    if (argb() != nullptr)
    {
        _bb.writeVarUint(2);
        _bb.writeVarUint(_data_argb);
    }
    if (id() != nullptr)
    {
        _bb.writeVarUint(3);
        _bb.writeVarInt(_data_id);
    }
    if (mat() != nullptr)
    {
        _bb.writeVarUint(4);
        if (!_data_mat->encode(_bb))
            return false;
    }
    if (rect() != nullptr)
    {
        _bb.writeVarUint(5);
        if (!_data_rect->encode(_bb))
            return false;
    }
    if (points() != nullptr)
    {
        _bb.writeVarUint(6);
        _bb.writeVarUint(_data_points.size());
        for (Point2dXY &_it : _data_points)
            if (!_it.encode(_bb))
                return false;
    }
    _bb.writeVarUint(0);
    return true;
}

bool GraphicNode::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema)
{
    uint32_t _count;
    while (true)
    {
        uint32_t _type;
        if (!_bb.readVarUint(_type))
            return false;
        switch (_type)
        {
            case 0:
                return true;
            case 1:
            {
                if (!_bb.readVarUint(reinterpret_cast<uint32_t &>(_data_type)))
                    return false;
                set_type(_data_type);
                break;
            }
            case 2:
            {
                if (!_bb.readVarUint(_data_argb))
                    return false;
                set_argb(_data_argb);
                break;
            }
            case 3:
            {
                if (!_bb.readVarInt(_data_id))
                    return false;
                set_id(_data_id);
                break;
            }
            case 4:
            {
                _data_mat = _pool.allocate<Matrix2d>();
                if (!_data_mat->decode(_bb, _pool, _schema))
                    return false;
                break;
            }
            case 5:
            {
                _data_rect = _pool.allocate<Rect>();
                if (!_data_rect->decode(_bb, _pool, _schema))
                    return false;
                break;
            }
            case 6:
            {
                if (!_bb.readVarUint(_count))
                    return false;
                for (Point2dXY &_it : set_points(_pool, _count))
                    if (!_it.decode(_bb, _pool, _schema))
                        return false;
                break;
            }
            default:
            {
                if (!_schema || !_schema->skipGraphicNodeField(_bb, _type))
                    return false;
                break;
            }
        }
    }
}

kiwi::String *Document::create_version()
{
    return _flags[0] & 1 ? &_data_create_version : nullptr;
}

const kiwi::String *Document::create_version() const
{
    return _flags[0] & 1 ? &_data_create_version : nullptr;
}

void Document::set_create_version(const kiwi::String &value)
{
    _flags[0] |= 1;
    _data_create_version = value;
}

kiwi::String *Document::last_open_version()
{
    return _flags[0] & 2 ? &_data_last_open_version : nullptr;
}

const kiwi::String *Document::last_open_version() const
{
    return _flags[0] & 2 ? &_data_last_open_version : nullptr;
}

void Document::set_last_open_version(const kiwi::String &value)
{
    _flags[0] |= 2;
    _data_last_open_version = value;
}

Matrix2d *Document::matView()
{
    return _data_matView;
}

const Matrix2d *Document::matView() const
{
    return _data_matView;
}

void Document::set_matView(Matrix2d *value)
{
    _data_matView = value;
}

uint32_t *Document::background()
{
    return _flags[0] & 8 ? &_data_background : nullptr;
}

const uint32_t *Document::background() const
{
    return _flags[0] & 8 ? &_data_background : nullptr;
}

void Document::set_background(const uint32_t &value)
{
    _flags[0] |= 8;
    _data_background = value;
}

kiwi::Array<GraphicNode> *Document::graphics()
{
    return _flags[0] & 16 ? &_data_graphics : nullptr;
}

const kiwi::Array<GraphicNode> *Document::graphics() const
{
    return _flags[0] & 16 ? &_data_graphics : nullptr;
}

kiwi::Array<GraphicNode> &Document::set_graphics(kiwi::MemoryPool &pool, uint32_t count)
{
    _flags[0] |= 16;
    return _data_graphics = pool.array<GraphicNode>(count);
}

bool Document::encode(kiwi::ByteBuffer &_bb)
{
    if (create_version() != nullptr)
    {
        _bb.writeVarUint(1);
        _bb.writeString(_data_create_version.c_str());
    }
    if (last_open_version() != nullptr)
    {
        _bb.writeVarUint(2);
        _bb.writeString(_data_last_open_version.c_str());
    }
    if (matView() != nullptr)
    {
        _bb.writeVarUint(3);
        if (!_data_matView->encode(_bb))
            return false;
    }
    if (background() != nullptr)
    {
        _bb.writeVarUint(4);
        _bb.writeVarUint(_data_background);
    }
    if (graphics() != nullptr)
    {
        _bb.writeVarUint(5);
        _bb.writeVarUint(_data_graphics.size());
        for (GraphicNode &_it : _data_graphics)
            if (!_it.encode(_bb))
                return false;
    }
    _bb.writeVarUint(0);
    return true;
}

bool Document::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema)
{
    uint32_t _count;
    while (true)
    {
        uint32_t _type;
        if (!_bb.readVarUint(_type))
            return false;
        switch (_type)
        {
            case 0:
                return true;
            case 1:
            {
                if (!_bb.readString(_data_create_version, _pool))
                    return false;
                set_create_version(_data_create_version);
                break;
            }
            case 2:
            {
                if (!_bb.readString(_data_last_open_version, _pool))
                    return false;
                set_last_open_version(_data_last_open_version);
                break;
            }
            case 3:
            {
                _data_matView = _pool.allocate<Matrix2d>();
                if (!_data_matView->decode(_bb, _pool, _schema))
                    return false;
                break;
            }
            case 4:
            {
                if (!_bb.readVarUint(_data_background))
                    return false;
                set_background(_data_background);
                break;
            }
            case 5:
            {
                if (!_bb.readVarUint(_count))
                    return false;
                for (GraphicNode &_it : set_graphics(_pool, _count))
                    if (!_it.decode(_bb, _pool, _schema))
                        return false;
                break;
            }
            default:
            {
                if (!_schema || !_schema->skipDocumentField(_bb, _type))
                    return false;
                break;
            }
        }
    }
}

} // namespace sindyk