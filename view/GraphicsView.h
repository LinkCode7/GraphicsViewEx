#ifndef GRAPHICS_VIEW_H
#define GRAPHICS_VIEW_H

#include <QFrame>
#include <QGraphicsView>

QT_BEGIN_NAMESPACE
class QLabel;
class QSlider;
class QToolButton;
QT_END_NAMESPACE

class GraphicsFrame;
class DragAction;
class PropertyTable;

class GraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    GraphicsView(GraphicsFrame *v) : QGraphicsView(), _frame(v) { setBackgroundBrush(QColor{239, 239, 239}); }

    enum ViewState
    {
        eUnknownState = 0,
        eDragInit     = 1,
        eDragging     = eDragInit << 1,
    };
    void addState(ViewState state) { _state |= state; }
    void removeState(ViewState state) { _state &= (~state); }
    bool hasState(ViewState state) const { return _state & state; }

    std::shared_ptr<DragAction> drag() const { return _drag; }
    void                        setDrag(std::shared_ptr<DragAction> drag) { _drag = drag; }

    void setCommand(QString const &str) { _strCommand = str; }

    QColor getBgColor() const { return backgroundBrush().color(); }
    void   setBgColor(QColor const &color) { setBackgroundBrush(color); }

protected:
#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent *) override;
#endif

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    GraphicsFrame *_frame;
    bool           _bMoveView        = false;
    QPoint         _ptRightMouseDown = {};

    int                         _state = 0;
    QString                     _strCommand;
    std::shared_ptr<DragAction> _drag;
};

class QTableWidgetItem;
class GraphicsScene;

class GraphicsFrame : public QFrame
{
    Q_OBJECT
public:
    explicit GraphicsFrame(GraphicsScene *scene, const QString &name, QWidget *parent = nullptr);

    QGraphicsView *view() const;

public slots:
    void zoomIn(int level = 1);
    void zoomOut(int level = 1);

private slots:
    void resetView();
    void setResetButtonEnabled();
    void setupMatrix();
    void togglePointerMode();
    void toggleOpenGL();
    void toggleAntialiasing();
    void print();
    void rotateLeft();
    void rotateRight();

private:
    GraphicsView *_pGraphicsView;

    QToolButton *  _pResetBtn;
    QSlider *      _pZoomSlider;
    QSlider *      _pRotateSlider;
    PropertyTable *_property;
};

#endif // GRAPHICS_VIEW_H
