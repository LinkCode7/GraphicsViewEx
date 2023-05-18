#include "PolylineGraphic.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <iostream>

#include "../GraphicUtility.h"

PolylineGraphic::PolylineGraphic(double minX, double minY, double maxX, double maxY)
{
    m_points.push_back(QPointF(minX, minY));
    m_points.push_back(QPointF(minX, maxY));
    m_points.push_back(QPointF(maxX, maxY));
    m_points.push_back(QPointF(maxX, minY));
    m_points.push_back(QPointF(minX, minY));
};

PolylineGraphic::PolylineGraphic(const QPointF &ptBegin) : m_points{ptBegin} {};

QRectF PolylineGraphic::boundingRect() const
{
    return QPolygonF(m_points).boundingRect();
};
QPainterPath PolylineGraphic::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
};
void PolylineGraphic::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    painter->drawPolyline(QPolygonF(m_points));
};

void PolylineGraphic::addPoint(const QPointF &ptNow)
{
    std::cout << m_points.size() << "(" << ptNow.x() << "," << ptNow.y() << ")" << std::endl;
    m_points.push_back(ptNow);
};

void PolylineGraphic::setLastPt(const QPointF &ptNow)
{
    if (m_points.size() < 2) {
        m_points.push_back(ptNow);
    }
    else {
        m_points[m_points.size() - 1] = ptNow;
    }
};

bool PolylineGraphic::closePolyLine(const QPointF &point)
{
    if (m_points.size() <= 3) {
        return false;
    }
    else if (!checkCross(point)) {  // 可以闭合
        // addPoint(point);
        m_points[m_points.size() - 1] = m_points[0];
        return true;
    }
};

bool PolylineGraphic::checkCross(const QPointF &point)
{
    if (m_points.size() <= 2) return false;

    QPointF beginPt = m_points[0];
    QPointF lastPt  = m_points[m_points.size() - 2];
    for (int i = 1; i < m_points.size() - 1; ++i) {
        QPointF endPt = m_points[i];
        if (collisionDect(lastPt, point, beginPt, endPt)) {
            if (lastPt == endPt) {
                if (onLineDect(point, beginPt, endPt)) return true;
            }
            else if (crossDect(lastPt, point, beginPt, endPt)) {
                return true;
            }
        }
        beginPt = endPt;
        continue;
    }
    return false;
};
bool PolylineGraphic::collisionDect(const QPointF &p1, const QPointF &p2, const QPointF &p3, const QPointF &p4)
{
    // xy轴投影法
    double x1max = std::max(p1.x(), p2.x());
    double x1min = std::min(p1.x(), p2.x());
    double y1max = std::max(p1.y(), p2.y());
    double y1min = std::min(p1.y(), p2.y());

    double x2max = std::max(p3.x(), p4.x());
    double x2min = std::min(p3.x(), p4.x());
    double y2max = std::max(p3.y(), p4.y());
    double y2min = std::min(p3.y(), p4.y());

    bool checkX = false;
    bool checkY = false;

    if ((x1max > x2min && x1max < x2max) || (x1min > x2min && x1min < x2max) || (x2max > x1min && x2max < x1max) || (x2max > x1min && x2max < x1max))
        checkX = true;
    if ((y1max > y2min && y1max < y2max) || (y1min > y2min && y1min < y2max) || (y2max > y1min && y2max < y1max) || (y2max > y1min && y2max < y1max))
        checkY = true;

    return checkX && checkY;
};
bool PolylineGraphic::crossDect(const QPointF &p1, const QPointF &p2, const QPointF &p3, const QPointF &p4)
{
    // (p1p3×p1p2) × (p1p4×p1p2) < 0
    auto   p1p3  = QPointF(p3.x() - p1.x(), p3.y() - p1.y());
    auto   p1p2  = QPointF(p2.x() - p1.x(), p2.y() - p1.y());
    auto   p1p4  = QPointF(p4.x() - p1.x(), p4.y() - p1.y());
    double left  = p1p3.x() * p1p2.y() - p1p3.y() * p1p2.x();
    double right = p1p4.x() * p1p2.y() - p1p4.y() * p1p2.x();
    double res1  = left * right;

    // (p3p1×p3p4) × (p3p2×p3p4) < 0
    auto p3p1   = QPointF(p1.x() - p3.x(), p1.y() - p3.y());
    auto p3p4   = QPointF(p4.x() - p3.x(), p4.y() - p3.y());
    auto p3p2   = QPointF(p2.x() - p3.x(), p2.y() - p3.y());
    left        = p3p1.x() * p3p4.y() - p3p1.y() * p3p4.x();
    right       = p3p2.x() * p3p4.y() - p3p2.y() * p3p4.x();
    double res2 = left * right;

    // 小于零跨立，大于零不跨立
    return (res1 < 0 && res2 < 0) ? true : false;
};
bool PolylineGraphic::onLineDect(const QPointF &p, const QPointF &p1, const QPointF &p2)
{
    auto pXpl = QPointF(p1.x() - p.x(), p1.y() - p.y());
    auto pXp2 = QPointF(p2.x() - p.x(), p2.y() - p.y());

    // 先进行又积运算
    double result = pXpl.x() * pXp2.y() - pXpl.y() * pXp2.y();

    // 再计算该点是否在包围盒内
    double left   = std::min(p1.x(), p2.x());
    double right  = std::max(p1.x(), p2.x());
    double top    = std::max(p1.y(), p2.y());
    double bottom = std::min(p1.y(), p2.y());

    if (p.x() >= left && p.x() <= right && p.y() >= bottom && p.y() <= top) {
        return result == 0 ? true : false;
    }
    return false;
};

QVector<QPointF> PolylineGraphic::getPoints()
{
    return m_points;
};