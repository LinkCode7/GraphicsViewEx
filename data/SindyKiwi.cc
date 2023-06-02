#include "SindyKiwi.h"

#include "qmatrix.h"
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

sindyk::Matrix2d* SindyKiwi::fromMatrix(QMatrix const& mat)
{
    auto out = _pool->allocate<sindyk::Matrix2d>();
    out->set__m11(mat.m11());
    out->set__m12(mat.m12());
    out->set__m21(mat.m21());
    out->set__m22(mat.m22());
    out->set__dx(mat.dx());
    out->set__dy(mat.dy());
    return out;
}

QMatrix SindyKiwi::toMatrix(sindyk::Matrix2d* mat)
{
    return QMatrix{*mat->_m11(), *mat->_m12(), *mat->_m21(), *mat->_m22(), *mat->_dx(), *mat->_dy()};
}

void SindyKiwi::setPoints(QVector<QPointF> const& points)
{
    auto                            size     = points.size();
    kiwi::Array<sindyk::Point2dXY>& arrPoint = _node->set_points(*_pool, size);

    for (auto i = 0; i < size; ++i)
    {
        arrPoint[i].set_x(points[i].x());
        arrPoint[i].set_y(points[i].y());
    }
}

void SindyKiwi::getPoints(QVector<QPointF>& points)
{
    const kiwi::Array<sindyk::Point2dXY>& arrPoint = *_node->points();

    auto size = arrPoint.size();
    for (size_t i = 0; i < size; ++i)
    {
        points.push_back({*arrPoint[i].x(), *arrPoint[i].y()});
    }
}