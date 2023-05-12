# GraphicsView
## 简介
这是一个简易的绘图项目，基于QT的QGraphicsView开发，同时参考了QtCreator中的Chips案例，适合有C++基础但对QT不太熟悉的开发人员自学。

## 说明
- 支持添加自定义图形
- 支持画布的缩放、平移、旋转，这是通过QGraphicsView实现的，这大大简化了开发代码，要做到这些，请留意以下代码：
```cpp
// BasicGraphic.cc
BasicGraphic::BasicGraphic() : m_color{0, 255, 0}
{
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
}
```
- 默认的画布拖拽是按住鼠标左键，以下代码实现了按住鼠标右键拖动画布：
```cpp
// GraphicsView.cc
void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        // m_handCursor.setShape(Qt::OpenHandCursor);
        // QApplication::setOverrideCursor(m_handCursor);
        setDragMode(QGraphicsView::ScrollHandDrag);
        setInteractive(false);
        this->m_bMoveView        = true;
        this->m_ptRightMouseDown = event->pos();
    }

    QGraphicsView::mousePressEvent(event);
}
void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    // QApplication::restoreOverrideCursor();
    if (m_bMoveView) {
        // m_handCursor.setShape(Qt::ClosedHandCursor);
        // QApplication::setOverrideCursor(m_handCursor);
        QPoint  movPos = event->pos();
        QPointF dis    = mapToScene(movPos) - mapToScene(m_ptRightMouseDown);
        QGraphicsView::translate(dis.x(), dis.y());
    }
    QGraphicsView::mouseMoveEvent(event);

#ifdef _DEBUG
    auto ptTest  = event->pos();
    auto ptScene = this->mapToScene(ptTest);
    std::cout << "MousePoint(" << ptScene.x() << ", " << ptScene.y() << ")" << std::endl;
#endif
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    // QApplication::restoreOverrideCursor();
    setDragMode(QGraphicsView::RubberBandDrag);
    setInteractive(true);
    this->m_bMoveView = false;
    QGraphicsView::mouseReleaseEvent(event);
}
```

- 鼠标左键按住图形可以拖动单个图形，这会改变图形的中心点QGraphicsItem::pos()，推测绘制时会把pos作为偏移量设到矩阵中，所以内部图形中，和定位有关的自定义数据应该加上这个偏移量。

## 其它
- 你可以在main.cc中调用sindy::addTestEntity()，这是QtCreator中的Chips案例的简化版，是个有趣的东西。
