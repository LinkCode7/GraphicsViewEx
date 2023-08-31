#include "PropertyTable.h"

#include <QColorDialog>

#include "../graphics/IGeGraphic.h"
#include "GraphicsArchive.h"
#include "GraphicsView.h"
#include "qgraphicsscene.h"

PropertyTable::PropertyTable()
{
    setRowCount(1);
    setColumnCount(2);

    initProperty();

    // setEditTriggers(QAbstractItemView::NoEditTriggers); // 设置单元格不可编辑
    connect(this, &QTableWidget::cellClicked, this, &PropertyTable::cellClicked);
}

void PropertyTable::doSelectionChanged()
{
    auto pItem = this->item(0, 1);
    if (!pItem)
        return;

    auto count = rowCount();

    const auto* list = GeArchive().doc()->selectedGraphics();
    auto        size = list->size();

    std::vector<std::pair<std::string, std::string>> fields;

    IGeGraphic* pFirst = size == 0 ? nullptr : list->front();
    if (pFirst)
    {
        pFirst->list(fields);
        auto fieldSize = fields.size() + 1;
        // if (count < fieldSize)
        setRowCount(fieldSize);
    }

    auto old = getSelectedBodyColor(pFirst);
    pItem->setBackgroundColor(old);

    int index = 1;
    for (auto const& keyValue : fields)
    {
        pItem = this->item(index, 0);
        if (!pItem)
        {
            setItem(index, 0, new QTableWidgetItem);
            pItem = this->item(index, 0);
        }
        pItem->setText(keyValue.first.c_str());

        pItem = this->item(index, 1);
        if (!pItem)
        {
            setItem(index, 1, new QTableWidgetItem);
            pItem = this->item(index, 1);
        }
        pItem->setText(keyValue.second.c_str());
        ++index;
    }

    this->viewport()->update();
}

void PropertyTable::cellClicked(int row, int column)
{
    if (row != 0)
        return;

    auto pItem = this->item(row, 0);
    if (!pItem)
        return;

    const auto* list = GeArchive().doc()->selectedGraphics();
    auto        size = list->size();
    auto        old  = getSelectedBodyColor(size == 0 ? nullptr : list->front());

    QColor color = QColorDialog::getColor(old, this, "Please select a color");

    if (old != color)
    {
        setSelectedBodyColor(color);
        this->viewport()->update();
    }
}

void PropertyTable::initProperty()
{
    auto item = new QTableWidgetItem;
    item->setText("COLOR");
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

QColor PropertyTable::getSelectedBodyColor(IGeGraphic* pFirst) const
{
    return pFirst ? pFirst->getGeColor() : GeArchive().curView()->backgroundBrush().color();
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