#include "DragAction.h"

#include "../GraphicUtility.h"
#include "qgraphicsscene.h"

extern QGraphicsScene* g_pScene;

DragAction::DragAction(const QPointF& firstClick)
{
    m_ploy = new PolylineGraphic(firstClick);
};

PolylineGraphic* DragAction::getPolyline()
{
    return m_ploy;
};

void DragAction::onLeftClick(const QPointF& curClick)
{
    m_ploy->addPoint(curClick);
};

void DragAction::onMove(const QPointF& curMove)
{
    m_ploy->setLastPt(curMove);
};

bool DragAction::keyAction(int keyNum, int& curNum)
{
    if (curNum == -1) {
        if (keyNum == Qt::Key_P) {
            curNum = Qt::Key_P;
        }
        else if (keyNum == Qt::Key_L) {
            curNum = Qt::Key_L;
        }
        else if (keyNum == Qt::Key_Y) {
            curNum = Qt::Key_Y;
        }
        else if (keyNum == Qt::Key_S) {
            // curNum = Qt::Key_S;
            // QList<QGraphicsItem*> itemsList = items();
            // if (itemsList.size() != 0) {
            //    for (auto& item : itemsList) {
            //        auto boundingBox = item->boundingRect();
            //        g_pScene->addItem(new PolylineGraphic(boundingBox.left(), boundingBox.bottom(), boundingBox.right(), boundingBox.top()));
            //    }
            //}
        }
        return false;
    }
    else {                               // 正在绘制图形
        if (keyNum == Qt::Key_Escape) {  // 取消绘制
            curNum = -1;
            g_pScene->removeItem(m_ploy);
        }
        else if (keyNum == Qt::Key_C && curNum == Qt::Key_P) {  // 闭合多段线
            m_ploy->closePolyLine(m_ploy->getPoints()[0]);
            curNum = -1;
        }
        else {
            return false;
        }
    }

    return true;
};