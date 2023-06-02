#include "schema.h"

namespace sindyk
{
bool BinarySchema::parse(kiwi::ByteBuffer &bb)
{
    if (!_schema.parse(bb))
        return false;
    _schema.findDefinition("GraphicNode", _indexGraphicNode);
    _schema.findDefinition("Chip", _indexChip);
    _schema.findDefinition("Document", _indexDocument);
    return true;
}

bool BinarySchema::skipGraphicNodeField(kiwi::ByteBuffer &bb, uint32_t id) const
{
    return _schema.skipField(bb, _indexGraphicNode, id);
}

bool BinarySchema::skipChipField(kiwi::ByteBuffer &bb, uint32_t id) const
{
    return _schema.skipField(bb, _indexChip, id);
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

float *Matrix2d::_m21()
{
    return _flags[0] & 4 ? &_data__m21 : nullptr;
}

const float *Matrix2d::_m21() const
{
    return _flags[0] & 4 ? &_data__m21 : nullptr;
}

void Matrix2d::set__m21(const float &value)
{
    _flags[0] |= 4;
    _data__m21 = value;
}

float *Matrix2d::_m22()
{
    return _flags[0] & 8 ? &_data__m22 : nullptr;
}

const float *Matrix2d::_m22() const
{
    return _flags[0] & 8 ? &_data__m22 : nullptr;
}

void Matrix2d::set__m22(const float &value)
{
    _flags[0] |= 8;
    _data__m22 = value;
}

float *Matrix2d::_dx()
{
    return _flags[0] & 16 ? &_data__dx : nullptr;
}

const float *Matrix2d::_dx() const
{
    return _flags[0] & 16 ? &_data__dx : nullptr;
}

void Matrix2d::set__dx(const float &value)
{
    _flags[0] |= 16;
    _data__dx = value;
}

float *Matrix2d::_dy()
{
    return _flags[0] & 32 ? &_data__dy : nullptr;
}

const float *Matrix2d::_dy() const
{
    return _flags[0] & 32 ? &_data__dy : nullptr;
}

void Matrix2d::set__dy(const float &value)
{
    _flags[0] |= 32;
    _data__dy = value;
}

bool Matrix2d::encode(kiwi::ByteBuffer &_bb)
{
    if (_m11() == nullptr)
        return false;
    _bb.writeVarFloat(_data__m11);
    if (_m12() == nullptr)
        return false;
    _bb.writeVarFloat(_data__m12);
    if (_m21() == nullptr)
        return false;
    _bb.writeVarFloat(_data__m21);
    if (_m22() == nullptr)
        return false;
    _bb.writeVarFloat(_data__m22);
    if (_dx() == nullptr)
        return false;
    _bb.writeVarFloat(_data__dx);
    if (_dy() == nullptr)
        return false;
    _bb.writeVarFloat(_data__dy);
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
    if (!_bb.readVarFloat(_data__m21))
        return false;
    set__m21(_data__m21);
    if (!_bb.readVarFloat(_data__m22))
        return false;
    set__m22(_data__m22);
    if (!_bb.readVarFloat(_data__dx))
        return false;
    set__dx(_data__dx);
    if (!_bb.readVarFloat(_data__dy))
        return false;
    set__dy(_data__dy);
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

int32_t *GraphicNode::rgba64()
{
    return _flags[0] & 2 ? &_data_rgba64 : nullptr;
}

const int32_t *GraphicNode::rgba64() const
{
    return _flags[0] & 2 ? &_data_rgba64 : nullptr;
}

void GraphicNode::set_rgba64(const int32_t &value)
{
    _flags[0] |= 2;
    _data_rgba64 = value;
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

Point2dXY *GraphicNode::pt()
{
    return _data_pt;
}

const Point2dXY *GraphicNode::pt() const
{
    return _data_pt;
}

void GraphicNode::set_pt(Point2dXY *value)
{
    _data_pt = value;
}

kiwi::Array<Point2dXY> *GraphicNode::points()
{
    return _flags[0] & 64 ? &_data_points : nullptr;
}

const kiwi::Array<Point2dXY> *GraphicNode::points() const
{
    return _flags[0] & 64 ? &_data_points : nullptr;
}

kiwi::Array<Point2dXY> &GraphicNode::set_points(kiwi::MemoryPool &pool, uint32_t count)
{
    _flags[0] |= 64;
    return _data_points = pool.array<Point2dXY>(count);
}

bool GraphicNode::encode(kiwi::ByteBuffer &_bb)
{
    if (type() != nullptr)
    {
        _bb.writeVarUint(1);
        _bb.writeVarUint(static_cast<uint32_t>(_data_type));
    }
    if (rgba64() != nullptr)
    {
        _bb.writeVarUint(2);
        _bb.writeVarInt(_data_rgba64);
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
    if (pt() != nullptr)
    {
        _bb.writeVarUint(6);
        if (!_data_pt->encode(_bb))
            return false;
    }
    if (points() != nullptr)
    {
        _bb.writeVarUint(7);
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
                if (!_bb.readVarInt(_data_rgba64))
                    return false;
                set_rgba64(_data_rgba64);
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
                _data_pt = _pool.allocate<Point2dXY>();
                if (!_data_pt->decode(_bb, _pool, _schema))
                    return false;
                break;
            }
            case 7:
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

NodeType *Chip::type()
{
    return _flags[0] & 1 ? &_data_type : nullptr;
}

const NodeType *Chip::type() const
{
    return _flags[0] & 1 ? &_data_type : nullptr;
}

void Chip::set_type(const NodeType &value)
{
    _flags[0] |= 1;
    _data_type = value;
}

int32_t *Chip::x()
{
    return _flags[0] & 2 ? &_data_x : nullptr;
}

const int32_t *Chip::x() const
{
    return _flags[0] & 2 ? &_data_x : nullptr;
}

void Chip::set_x(const int32_t &value)
{
    _flags[0] |= 2;
    _data_x = value;
}

int32_t *Chip::y()
{
    return _flags[0] & 4 ? &_data_y : nullptr;
}

const int32_t *Chip::y() const
{
    return _flags[0] & 4 ? &_data_y : nullptr;
}

void Chip::set_y(const int32_t &value)
{
    _flags[0] |= 4;
    _data_y = value;
}

int32_t *Chip::color()
{
    return _flags[0] & 8 ? &_data_color : nullptr;
}

const int32_t *Chip::color() const
{
    return _flags[0] & 8 ? &_data_color : nullptr;
}

void Chip::set_color(const int32_t &value)
{
    _flags[0] |= 8;
    _data_color = value;
}

kiwi::Array<Point2dXY> *Chip::stuff()
{
    return _flags[0] & 16 ? &_data_stuff : nullptr;
}

const kiwi::Array<Point2dXY> *Chip::stuff() const
{
    return _flags[0] & 16 ? &_data_stuff : nullptr;
}

kiwi::Array<Point2dXY> &Chip::set_stuff(kiwi::MemoryPool &pool, uint32_t count)
{
    _flags[0] |= 16;
    return _data_stuff = pool.array<Point2dXY>(count);
}

bool Chip::encode(kiwi::ByteBuffer &_bb)
{
    if (type() != nullptr)
    {
        _bb.writeVarUint(1);
        _bb.writeVarUint(static_cast<uint32_t>(_data_type));
    }
    if (x() != nullptr)
    {
        _bb.writeVarUint(2);
        _bb.writeVarInt(_data_x);
    }
    if (y() != nullptr)
    {
        _bb.writeVarUint(3);
        _bb.writeVarInt(_data_y);
    }
    if (color() != nullptr)
    {
        _bb.writeVarUint(4);
        _bb.writeVarInt(_data_color);
    }
    if (stuff() != nullptr)
    {
        _bb.writeVarUint(5);
        _bb.writeVarUint(_data_stuff.size());
        for (Point2dXY &_it : _data_stuff)
            if (!_it.encode(_bb))
                return false;
    }
    _bb.writeVarUint(0);
    return true;
}

bool Chip::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema)
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
                if (!_bb.readVarInt(_data_x))
                    return false;
                set_x(_data_x);
                break;
            }
            case 3:
            {
                if (!_bb.readVarInt(_data_y))
                    return false;
                set_y(_data_y);
                break;
            }
            case 4:
            {
                if (!_bb.readVarInt(_data_color))
                    return false;
                set_color(_data_color);
                break;
            }
            case 5:
            {
                if (!_bb.readVarUint(_count))
                    return false;
                for (Point2dXY &_it : set_stuff(_pool, _count))
                    if (!_it.decode(_bb, _pool, _schema))
                        return false;
                break;
            }
            default:
            {
                if (!_schema || !_schema->skipChipField(_bb, _type))
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

Matrix2d *Document::mat()
{
    return _data_mat;
}

const Matrix2d *Document::mat() const
{
    return _data_mat;
}

void Document::set_mat(Matrix2d *value)
{
    _data_mat = value;
}

int32_t *Document::background()
{
    return _flags[0] & 8 ? &_data_background : nullptr;
}

const int32_t *Document::background() const
{
    return _flags[0] & 8 ? &_data_background : nullptr;
}

void Document::set_background(const int32_t &value)
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

kiwi::Array<Chip> *Document::chips()
{
    return _flags[0] & 32 ? &_data_chips : nullptr;
}

const kiwi::Array<Chip> *Document::chips() const
{
    return _flags[0] & 32 ? &_data_chips : nullptr;
}

kiwi::Array<Chip> &Document::set_chips(kiwi::MemoryPool &pool, uint32_t count)
{
    _flags[0] |= 32;
    return _data_chips = pool.array<Chip>(count);
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
    if (mat() != nullptr)
    {
        _bb.writeVarUint(3);
        if (!_data_mat->encode(_bb))
            return false;
    }
    if (background() != nullptr)
    {
        _bb.writeVarUint(4);
        _bb.writeVarInt(_data_background);
    }
    if (graphics() != nullptr)
    {
        _bb.writeVarUint(5);
        _bb.writeVarUint(_data_graphics.size());
        for (GraphicNode &_it : _data_graphics)
            if (!_it.encode(_bb))
                return false;
    }
    if (chips() != nullptr)
    {
        _bb.writeVarUint(6);
        _bb.writeVarUint(_data_chips.size());
        for (Chip &_it : _data_chips)
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
                _data_mat = _pool.allocate<Matrix2d>();
                if (!_data_mat->decode(_bb, _pool, _schema))
                    return false;
                break;
            }
            case 4:
            {
                if (!_bb.readVarInt(_data_background))
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
            case 6:
            {
                if (!_bb.readVarUint(_count))
                    return false;
                for (Chip &_it : set_chips(_pool, _count))
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