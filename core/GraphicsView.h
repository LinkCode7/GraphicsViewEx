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

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(GraphicsFrame *v) : QGraphicsView(), m_view(v) {}

protected:
#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent *) override;
#endif

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    enum ViewState
    {
        eUnknownState = 0,
        eDragInit     = 1,
        eDragging     = eDragInit << 1,
    };
    void addState(ViewState state) { m_state |= state; }
    void removeState(ViewState state) { m_state &= (~state); }
    bool hasState(ViewState state) const { return m_state & state; }

private:
    GraphicsFrame *m_view;
    bool           m_bMoveView        = false;
    QPoint         m_ptRightMouseDown = {};

    int                         m_state = 0;
    QString                     m_strCommand;
    std::shared_ptr<DragAction> m_drag;
};

class GraphicsFrame : public QFrame
{
    Q_OBJECT
public:
    explicit GraphicsFrame(const QString &name, QWidget *parent = nullptr);

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
    GraphicsView *m_pGraphicsView;

    QToolButton *m_pResetBtn;
    QSlider *    m_pZoomSlider;
    QSlider *    m_pRotateSlider;
};

#endif  // GRAPHICS_VIEW_H
