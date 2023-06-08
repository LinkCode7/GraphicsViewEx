#ifndef PROPERTY_TABLE_H
#define PROPERTY_TABLE_H
#include "QTableWidget"

class GraphicsView;

class PropertyTable : public QTableWidget
{
public:
    PropertyTable();

    void doSelectionChanged();

private slots:
    void cellClicked(int row, int column);

private:
    void initProperty();

    QString cellText(int row, int column);

    QColor getSelectedBodyColor() const;
    void   setSelectedBodyColor(QColor const& color);
};

#endif // !PROPERTY_TABLE_H
