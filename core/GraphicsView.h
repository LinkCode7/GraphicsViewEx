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

private:
    GraphicsFrame *m_view;
    bool           m_bMoveView        = false;
    QPoint         m_ptRightMouseDown = {};
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
