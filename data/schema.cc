#include "schema.h"

namespace sindyk
{

bool BinarySchema::parse(kiwi::ByteBuffer &bb)
{
    if (!_schema.parse(bb))
        return false;
    _schema.findDefinition("PolyElement", _indexPolyElement);
    _schema.findDefinition("GraphicNode", _indexGraphicNode);
    _schema.findDefinition("Document", _indexDocument);
    return true;
}

bool BinarySchema::skipPolyElementField(kiwi::ByteBuffer &bb, uint32_t id) const
{
    return _schema.skipField(bb, _indexPolyElement, id);
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

Point2dXY *PolySegment::begin()
{
    return _data_begin;
}

const Point2dXY *PolySegment::begin() const
{
    return _data_begin;
}

void PolySegment::set_begin(Point2dXY *value)
{
    _data_begin = value;
}

Point2dXY *PolySegment::end()
{
    return _data_end;
}

const Point2dXY *PolySegment::end() const
{
    return _data_end;
}

void PolySegment::set_end(Point2dXY *value)
{
    _data_end = value;
}

bool PolySegment::encode(kiwi::ByteBuffer &_bb)
{
    if (begin() == nullptr)
        return false;
    if (!_data_begin->encode(_bb))
        return false;
    if (end() == nullptr)
        return false;
    if (!_data_end->encode(_bb))
        return false;
    return true;
}

bool PolySegment::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema)
{
    _data_begin = _pool.allocate<Point2dXY>();
    if (!_data_begin->decode(_bb, _pool, _schema))
        return false;
    _data_end = _pool.allocate<Point2dXY>();
    if (!_data_end->decode(_bb, _pool, _schema))
        return false;
    return true;
}

Point2dXY *PolyArc::center()
{
    return _data_center;
}

const Point2dXY *PolyArc::center() const
{
    return _data_center;
}

void PolyArc::set_center(Point2dXY *value)
{
    _data_center = value;
}

float *PolyArc::radius()
{
    return _flags[0] & 2 ? &_data_radius : nullptr;
}

const float *PolyArc::radius() const
{
    return _flags[0] & 2 ? &_data_radius : nullptr;
}

void PolyArc::set_radius(const float &value)
{
    _flags[0] |= 2;
    _data_radius = value;
}

float *PolyArc::radius2()
{
    return _flags[0] & 4 ? &_data_radius2 : nullptr;
}

const float *PolyArc::radius2() const
{
    return _flags[0] & 4 ? &_data_radius2 : nullptr;
}

void PolyArc::set_radius2(const float &value)
{
    _flags[0] |= 4;
    _data_radius2 = value;
}

float *PolyArc::beginAngle()
{
    return _flags[0] & 8 ? &_data_beginAngle : nullptr;
}

const float *PolyArc::beginAngle() const
{
    return _flags[0] & 8 ? &_data_beginAngle : nullptr;
}

void PolyArc::set_beginAngle(const float &value)
{
    _flags[0] |= 8;
    _data_beginAngle = value;
}

float *PolyArc::sweepAngle()
{
    return _flags[0] & 16 ? &_data_sweepAngle : nullptr;
}

const float *PolyArc::sweepAngle() const
{
    return _flags[0] & 16 ? &_data_sweepAngle : nullptr;
}

void PolyArc::set_sweepAngle(const float &value)
{
    _flags[0] |= 16;
    _data_sweepAngle = value;
}

bool PolyArc::encode(kiwi::ByteBuffer &_bb)
{
    if (center() == nullptr)
        return false;
    if (!_data_center->encode(_bb))
        return false;
    if (radius() == nullptr)
        return false;
    _bb.writeVarFloat(_data_radius);
    if (radius2() == nullptr)
        return false;
    _bb.writeVarFloat(_data_radius2);
    if (beginAngle() == nullptr)
        return false;
    _bb.writeVarFloat(_data_beginAngle);
    if (sweepAngle() == nullptr)
        return false;
    _bb.writeVarFloat(_data_sweepAngle);
    return true;
}

bool PolyArc::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema)
{
    _data_center = _pool.allocate<Point2dXY>();
    if (!_data_center->decode(_bb, _pool, _schema))
        return false;
    if (!_bb.readVarFloat(_data_radius))
        return false;
    set_radius(_data_radius);
    if (!_bb.readVarFloat(_data_radius2))
        return false;
    set_radius2(_data_radius2);
    if (!_bb.readVarFloat(_data_beginAngle))
        return false;
    set_beginAngle(_data_beginAngle);
    if (!_bb.readVarFloat(_data_sweepAngle))
        return false;
    set_sweepAngle(_data_sweepAngle);
    return true;
}

PolySegment *PolyElement::segment()
{
    return _data_segment;
}

const PolySegment *PolyElement::segment() const
{
    return _data_segment;
}

void PolyElement::set_segment(PolySegment *value)
{
    _data_segment = value;
}

PolyArc *PolyElement::arc()
{
    return _data_arc;
}

const PolyArc *PolyElement::arc() const
{
    return _data_arc;
}

void PolyElement::set_arc(PolyArc *value)
{
    _data_arc = value;
}

bool PolyElement::encode(kiwi::ByteBuffer &_bb)
{
    if (segment() != nullptr)
    {
        _bb.writeVarUint(1);
        if (!_data_segment->encode(_bb))
            return false;
    }
    if (arc() != nullptr)
    {
        _bb.writeVarUint(2);
        if (!_data_arc->encode(_bb))
            return false;
    }
    _bb.writeVarUint(0);
    return true;
}

bool PolyElement::decode(kiwi::ByteBuffer &_bb, kiwi::MemoryPool &_pool, const BinarySchema *_schema)
{
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
                _data_segment = _pool.allocate<PolySegment>();
                if (!_data_segment->decode(_bb, _pool, _schema))
                    return false;
                break;
            }
            case 2:
            {
                _data_arc = _pool.allocate<PolyArc>();
                if (!_data_arc->decode(_bb, _pool, _schema))
                    return false;
                break;
            }
            default:
            {
                if (!_schema || !_schema->skipPolyElementField(_bb, _type))
                    return false;
                break;
            }
        }
    }
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

uint32_t *GraphicNode::id()
{
    return _flags[0] & 8 ? &_data_id : nullptr;
}

const uint32_t *GraphicNode::id() const
{
    return _flags[0] & 8 ? &_data_id : nullptr;
}

void GraphicNode::set_id(const uint32_t &value)
{
    _flags[0] |= 8;
    _data_id = value;
}

kiwi::String *GraphicNode::name()
{
    return _flags[0] & 16 ? &_data_name : nullptr;
}

const kiwi::String *GraphicNode::name() const
{
    return _flags[0] & 16 ? &_data_name : nullptr;
}

void GraphicNode::set_name(const kiwi::String &value)
{
    _flags[0] |= 16;
    _data_name = value;
}

uint32_t *GraphicNode::flags()
{
    return _flags[0] & 32 ? &_data_flags : nullptr;
}

const uint32_t *GraphicNode::flags() const
{
    return _flags[0] & 32 ? &_data_flags : nullptr;
}

void GraphicNode::set_flags(const uint32_t &value)
{
    _flags[0] |= 32;
    _data_flags = value;
}

kiwi::Array<uint32_t> *GraphicNode::polyIndexes()
{
    return _flags[0] & 64 ? &_data_polyIndexes : nullptr;
}

const kiwi::Array<uint32_t> *GraphicNode::polyIndexes() const
{
    return _flags[0] & 64 ? &_data_polyIndexes : nullptr;
}

kiwi::Array<uint32_t> &GraphicNode::set_polyIndexes(kiwi::MemoryPool &pool, uint32_t count)
{
    _flags[0] |= 64;
    return _data_polyIndexes = pool.array<uint32_t>(count);
}

kiwi::Array<PolyElement> *GraphicNode::polyElements()
{
    return _flags[0] & 128 ? &_data_polyElements : nullptr;
}

const kiwi::Array<PolyElement> *GraphicNode::polyElements() const
{
    return _flags[0] & 128 ? &_data_polyElements : nullptr;
}

kiwi::Array<PolyElement> &GraphicNode::set_polyElements(kiwi::MemoryPool &pool, uint32_t count)
{
    _flags[0] |= 128;
    return _data_polyElements = pool.array<PolyElement>(count);
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
    return _flags[0] & 512 ? &_data_points : nullptr;
}

const kiwi::Array<Point2dXY> *GraphicNode::points() const
{
    return _flags[0] & 512 ? &_data_points : nullptr;
}

kiwi::Array<Point2dXY> &GraphicNode::set_points(kiwi::MemoryPool &pool, uint32_t count)
{
    _flags[0] |= 512;
    return _data_points = pool.array<Point2dXY>(count);
}

uint32_t *GraphicNode::symbolType()
{
    return _flags[0] & 1024 ? &_data_symbolType : nullptr;
}

const uint32_t *GraphicNode::symbolType() const
{
    return _flags[0] & 1024 ? &_data_symbolType : nullptr;
}

void GraphicNode::set_symbolType(const uint32_t &value)
{
    _flags[0] |= 1024;
    _data_symbolType = value;
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
    if (mat() != nullptr)
    {
        _bb.writeVarUint(3);
        if (!_data_mat->encode(_bb))
            return false;
    }
    if (id() != nullptr)
    {
        _bb.writeVarUint(4);
        _bb.writeVarUint(_data_id);
    }
    if (name() != nullptr)
    {
        _bb.writeVarUint(5);
        _bb.writeString(_data_name.c_str());
    }
    if (flags() != nullptr)
    {
        _bb.writeVarUint(6);
        _bb.writeVarUint(_data_flags);
    }
    if (polyIndexes() != nullptr)
    {
        _bb.writeVarUint(7);
        _bb.writeVarUint(_data_polyIndexes.size());
        for (uint32_t &_it : _data_polyIndexes)
            _bb.writeVarUint(_it);
    }
    if (polyElements() != nullptr)
    {
        _bb.writeVarUint(8);
        _bb.writeVarUint(_data_polyElements.size());
        for (PolyElement &_it : _data_polyElements)
            if (!_it.encode(_bb))
                return false;
    }
    if (rect() != nullptr)
    {
        _bb.writeVarUint(9);
        if (!_data_rect->encode(_bb))
            return false;
    }
    if (points() != nullptr)
    {
        _bb.writeVarUint(10);
        _bb.writeVarUint(_data_points.size());
        for (Point2dXY &_it : _data_points)
            if (!_it.encode(_bb))
                return false;
    }
    if (symbolType() != nullptr)
    {
        _bb.writeVarUint(11);
        _bb.writeVarUint(_data_symbolType);
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
                _data_mat = _pool.allocate<Matrix2d>();
                if (!_data_mat->decode(_bb, _pool, _schema))
                    return false;
                break;
            }
            case 4:
            {
                if (!_bb.readVarUint(_data_id))
                    return false;
                set_id(_data_id);
                break;
            }
            case 5:
            {
                if (!_bb.readString(_data_name, _pool))
                    return false;
                set_name(_data_name);
                break;
            }
            case 6:
            {
                if (!_bb.readVarUint(_data_flags))
                    return false;
                set_flags(_data_flags);
                break;
            }
            case 7:
            {
                if (!_bb.readVarUint(_count))
                    return false;
                for (uint32_t &_it : set_polyIndexes(_pool, _count))
                    if (!_bb.readVarUint(_it))
                        return false;
                break;
            }
            case 8:
            {
                if (!_bb.readVarUint(_count))
                    return false;
                for (PolyElement &_it : set_polyElements(_pool, _count))
                    if (!_it.decode(_bb, _pool, _schema))
                        return false;
                break;
            }
            case 9:
            {
                _data_rect = _pool.allocate<Rect>();
                if (!_data_rect->decode(_bb, _pool, _schema))
                    return false;
                break;
            }
            case 10:
            {
                if (!_bb.readVarUint(_count))
                    return false;
                for (Point2dXY &_it : set_points(_pool, _count))
                    if (!_it.decode(_bb, _pool, _schema))
                        return false;
                break;
            }
            case 11:
            {
                if (!_bb.readVarUint(_data_symbolType))
                    return false;
                set_symbolType(_data_symbolType);
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

uint32_t *Document::maxId()
{
    return _flags[0] & 16 ? &_data_maxId : nullptr;
}

const uint32_t *Document::maxId() const
{
    return _flags[0] & 16 ? &_data_maxId : nullptr;
}

void Document::set_maxId(const uint32_t &value)
{
    _flags[0] |= 16;
    _data_maxId = value;
}

kiwi::Array<GraphicNode> *Document::graphics()
{
    return _flags[0] & 32 ? &_data_graphics : nullptr;
}

const kiwi::Array<GraphicNode> *Document::graphics() const
{
    return _flags[0] & 32 ? &_data_graphics : nullptr;
}

kiwi::Array<GraphicNode> &Document::set_graphics(kiwi::MemoryPool &pool, uint32_t count)
{
    _flags[0] |= 32;
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
    if (maxId() != nullptr)
    {
        _bb.writeVarUint(5);
        _bb.writeVarUint(_data_maxId);
    }
    if (graphics() != nullptr)
    {
        _bb.writeVarUint(6);
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
                if (!_bb.readVarUint(_data_maxId))
                    return false;
                set_maxId(_data_maxId);
                break;
            }
            case 6:
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