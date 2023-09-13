#include "IGeGraphic.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "../utility/SindyMath.h"

IGeGraphic::IGeGraphic(ObjectType type) : _color{0, 255, 0}, _type(type)
{
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);

    // 暂时不控制
    _runTimeFlag.full(true);
    _runTimeFlag.remove(RuntimeFlag::eDocumentParsing);

    //// y轴颠倒
    // QTransform matrix;
    // matrix.scale(1, -1);
    // setTransform(matrix, true);
}

QRectF IGeGraphic::boundingRect() const
{
    return {};
}

QPainterPath IGeGraphic::shape() const
{
    return QGraphicsItem::shape();
}

void IGeGraphic::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}

void IGeGraphic::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

void IGeGraphic::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
}

void IGeGraphic::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}

QColor IGeGraphic::getDrawColor(QStyle::State state)
{
    auto color = state & QStyle::State_Selected ? _color.darker(230) : _color;
    // if (state & QStyle::State_MouseOver)
    //    color = color.lighter(125);
    return color;
}

double IGeGraphic::getDrawWidth(QStyle::State state)
{
    return state & QStyle::State_Selected ? 2 : 1;
}

void IGeGraphic::list(std::vector<std::pair<std::string, std::string>> &fields) const
{
    fields.emplace_back(std::make_pair("type", std::to_string(int(_type))));
    fields.emplace_back(std::make_pair("color", std::to_string(getArgb32())));
    fields.emplace_back(std::make_pair("id", std::to_string(_id)));
    fields.emplace_back(std::make_pair("name", _name));
    fields.emplace_back(std::make_pair("saveFlag", std::to_string(_saveFlag)));

    auto        mat = this->sceneMatrix();
    std::string str = sindy::simplifyFloat(mat.dx(), 3) + ", " + sindy::simplifyFloat(mat.dy(), 3);
    fields.emplace_back(std::make_pair("move", str));
}
