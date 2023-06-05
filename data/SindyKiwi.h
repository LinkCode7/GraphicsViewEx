#ifndef SINDY_KIWI_H
#define SINDY_KIWI_H

#include <string>
#include <vector>

#include "qrect.h"

namespace kiwi
{
class MemoryPool;
class String;
} // namespace kiwi

namespace sindyk
{
class Rect;
class Document;
class GraphicNode;
class Point2dXY;
class Matrix2d;
} // namespace sindyk

class SindyKiwi
{
public:
    SindyKiwi();
    ~SindyKiwi();

    inline kiwi::MemoryPool& pool() { return *_pool; };
    inline sindyk::Document& message() { return *_message; };
    inline void              currentNode(sindyk::GraphicNode* node) { _node = node; }

    kiwi::String fromString(const std::string& str);

    QRectF        toRect(sindyk::Rect* rect);
    sindyk::Rect* fromRect(QRectF const& rect);

    QPointF            toPoint(sindyk::Point2dXY* pt);
    sindyk::Point2dXY* fromPoint(QPointF const& pt);

    sindyk::Matrix2d* fromMatrix(QTransform const& mat);
    QTransform        toMatrix(sindyk::Matrix2d* mat);

    void getPoints(std::vector<QPointF>& points);
    void setPoints(std::vector<QPointF> const& points);

private:
    kiwi::MemoryPool*    _pool;
    sindyk::Document*    _message;
    sindyk::GraphicNode* _node;
};

#endif // ! SINDY_KIWI_H
