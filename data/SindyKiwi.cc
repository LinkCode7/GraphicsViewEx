#include "SindyKiwi.h"

#include "../graphics/GePolygon.h"
#include "qtransform.h"
#include "schema.h"

SindyKiwi::SindyKiwi() : _message(new sindyk::Document()), _pool(new kiwi::MemoryPool()), _node(nullptr)
{
}
SindyKiwi ::~SindyKiwi()
{
    delete _message;
    delete _pool;
}

kiwi::String SindyKiwi::fromString(const std::string& str)
{
    char* c_str = _pool->allocate<char>(str.size() + 1);
    memcpy(c_str, str.c_str(), str.size());
    return kiwi::String(c_str);
}

sindyk::Rect* SindyKiwi::fromRect(QRectF const& rect)
{
    auto out = _pool->allocate<sindyk::Rect>();
    out->set_left(rect.left());
    out->set_top(rect.top());
    out->set_right(rect.right());
    out->set_bottom(rect.bottom());
    return out;
}
QRectF SindyKiwi::toRect(sindyk::Rect* rect)
{
    return QRectF{*rect->left(), *rect->top(), *rect->right(), *rect->bottom()};
}

sindyk::Point2dXY* SindyKiwi::fromPoint(QPointF const& pt)
{
    auto out = _pool->allocate<sindyk::Point2dXY>();
    out->set_x(pt.x());
    out->set_y(pt.y());
    return out;
}
QPointF SindyKiwi::toPoint(sindyk::Point2dXY* pt)
{
    return QPointF{*pt->x(), *pt->y()};
}

sindyk::Matrix2d* SindyKiwi::fromMatrix(QTransform const& mat)
{
    auto out = _pool->allocate<sindyk::Matrix2d>();
    out->set__m11(mat.m11());
    out->set__m12(mat.m12());
    out->set__m13(mat.m13());
    out->set__m21(mat.m21());
    out->set__m22(mat.m22());
    out->set__m23(mat.m23());
    out->set__m31(mat.m31());
    out->set__m32(mat.m32());
    out->set__m33(mat.m33());
    return out;
}

QTransform SindyKiwi::toMatrix(sindyk::Matrix2d* mat)
{
    return QTransform{*mat->_m11(), *mat->_m12(), *mat->_m13(), *mat->_m21(), *mat->_m22(),
                      *mat->_m23(), *mat->_m31(), *mat->_m32(), *mat->_m33()};
}

void SindyKiwi::setPoints(std::vector<QPointF> const& points)
{
    auto                            size     = points.size();
    kiwi::Array<sindyk::Point2dXY>& arrPoint = _node->set_points(*_pool, size);

    for (auto i = 0; i < size; ++i)
    {
        arrPoint[i].set_x(points[i].x());
        arrPoint[i].set_y(points[i].y());
    }
}

void SindyKiwi::getPoints(std::vector<QPointF>& points)
{
    const kiwi::Array<sindyk::Point2dXY>& arrPoint = *_node->points();

    auto size = arrPoint.size();
    for (size_t i = 0; i < size; ++i)
    {
        points.push_back({*arrPoint[i].x(), *arrPoint[i].y()});
    }
}

void SindyKiwi::setPolygon(GePolygon* polygon)
{
    // 每个闭合图形的边的数量
    auto const& arrIndex = polygon->indexes();

    auto                   size    = arrIndex.size();
    kiwi::Array<uint32_t>& indexes = _node->set_polyIndexes(*_pool, size);
    for (auto i = 0; i < size; ++i)
    {
        indexes[i] = arrIndex[i];
    }

    // 边
    auto const& arrElement = polygon->elements();

    size                                       = arrElement.size();
    kiwi::Array<sindyk::PolyElement>& elements = _node->set_polyElements(*_pool, size);
    for (auto i = 0; i < size; ++i)
    {
        if (auto pLine = dynamic_cast<sindy::PolySegment*>(arrElement[i].get()); pLine)
        {
            auto seg = _pool->allocate<sindyk::PolySegment>();

            auto begin = _pool->allocate<sindyk::Point2dXY>();
            begin->set_x(pLine->_begin.x);
            begin->set_y(pLine->_begin.y);
            seg->set_begin(begin);
            auto end = _pool->allocate<sindyk::Point2dXY>();
            end->set_x(pLine->_end.x);
            end->set_y(pLine->_end.y);
            seg->set_end(end);
            elements[i].set_segment(seg);
        }
        else if (auto pArc = dynamic_cast<sindy::PolyArc*>(arrElement[i].get()); pArc)
        {
            auto arc = _pool->allocate<sindyk::PolyArc>();

            auto center = _pool->allocate<sindyk::Point2dXY>();
            center->set_x(pArc->_center.x);
            center->set_y(pArc->_center.y);
            arc->set_center(center);
            arc->set_radius(pArc->_radius);
            arc->set_radius2(pArc->_radius2);
            arc->set_beginAngle(pArc->_beginAngle);
            arc->set_sweepAngle(pArc->_sweepAngle);
            elements[i].set_arc(arc);
        }
        else
        {
            std::cout << "unsupported type\n";
            assert(0);
        }
    }
}
