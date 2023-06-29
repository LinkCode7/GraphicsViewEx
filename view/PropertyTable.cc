#include "PropertyTable.h"

#include <QColorDialog>

#include "../graphics/IGeGraphic.h"
#include "GraphicsArchive.h"
#include "GraphicsView.h"
#include "qgraphicsscene.h"

PropertyTable::PropertyTable()
{
    setRowCount(5);
    setColumnCount(2);

    initProperty();

    setEditTriggers(QAbstractItemView::NoEditTriggers); // 设置单元格不可编辑
    connect(this, &QTableWidget::cellClicked, this, &PropertyTable::cellClicked);
}

void PropertyTable::cellClicked(int row, int column)
{
    auto pItem = this->item(row, 0);
    if (!pItem)
        return;

    auto   old   = getSelectedBodyColor();
    QColor color = QColorDialog::getColor(old, this, "Please select a color");

    if (old != color)
    {
        setSelectedBodyColor(color);
        this->viewport()->update();
    }
}

void PropertyTable::doSelectionChanged()
{
    auto pItem = this->item(0, 1);
    if (!pItem)
        return;

    auto old = getSelectedBodyColor();
    pItem->setBackgroundColor(old);
    this->viewport()->update();
}

void PropertyTable::initProperty()
{
    auto item = new QTableWidgetItem;
    item->setText("ContourColor");
    setItem(0, 0, item);

    auto count = rowCount();
    for (auto row = 0; row < count; ++row)
    {
        setItem(row, 1, new QTableWidgetItem);
    }
}

QString PropertyTable::cellText(int row, int column)
{
    auto pItem = this->item(row, column);
    if (!pItem)
        return {};
    return pItem->text();
}

QColor PropertyTable::getSelectedBodyColor() const
{
    const auto* list = GeArchive().doc()->selectedGraphics();
    for (auto const& item : *list)
    {
        return item->getGeColor();
    }

    return GeArchive().curView()->backgroundBrush().color();
}

void PropertyTable::setSelectedBodyColor(QColor const& color)
{
    const auto* list = GeArchive().doc()->selectedGraphics();
    if (list->size() == 0)
    {
        GeArchive().curView()->setBackgroundBrush(color);
        return;
    }

    for (auto const& item : *list)
    {
        item->setGeColor(color);
    }
}