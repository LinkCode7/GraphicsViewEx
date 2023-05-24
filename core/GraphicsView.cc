#include "GraphicsView.h"

#include "../control/DragAction.h"
#include "../control/GraphicsControl.h"
#include "../control/PolylineDrag.h"

#if defined(QT_PRINTSUPPORT_LIB)
    #include <QtPrintSupport/qtprintsupportglobal.h>
    #if QT_CONFIG(printdialog)
        #include <QPrintDialog>
        #include <QPrinter>
    #endif
#endif

#ifndef QT_NO_OPENGL
    #include <QtOpenGL>
#else
    #include <QtWidgets>
#endif

#include <QtMath>
#include <iostream>

#if QT_CONFIG(wheelevent)
void GraphicsView::wheelEvent(QWheelEvent *e)
{
    if (e->modifiers() & Qt::ControlModifier)
    {
        if (e->angleDelta().y() > 0)
            m_view->zoomIn(6);
        else
            m_view->zoomOut(6);
        e->accept();
    }
    else
    {
        QGraphicsView::wheelEvent(e);
    }
}
#endif

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    setStyleSheet("QGraphicsView::rubberBand {background-color::transparent;}");
    if (event->button() == Qt::MidButton)
    {
        // m_handCursor.setShape(Qt::OpenHandCursor);
        // QApplication::setOverrideCursor(m_handCursor);
        setDragMode(QGraphicsView::ScrollHandDrag);
        setInteractive(false);
        this->m_bMoveView        = true;
        this->m_ptRightMouseDown = event->pos();
    }
    else if (event->button() == Qt::LeftButton)
    {
        if (hasState(eDragInit))
        {
            setDragMode(QGraphicsView::NoDrag);
            setInteractive(false);

            removeState(eDragInit);
        }

        if (m_drag)
        {
            setInteractive(false);  // 拖拽过程中禁止移动item
            m_drag->onLeftClick(mapToScene(event->pos()));
        }
    }

    QGraphicsView::mousePressEvent(event);
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    setStyleSheet("QGraphicsView::rubberBand {background-color::transparent;}");
    // QApplication::restoreOverrideCursor();
    if (m_bMoveView)
    {
        // m_handCursor.setShape(Qt::ClosedHandCursor);
        // QApplication::setOverrideCursor(m_handCursor);
        QPoint  movPos = event->pos();
        QPointF dis    = mapToScene(movPos) - mapToScene(m_ptRightMouseDown);
        QGraphicsView::translate(dis.x(), dis.y());
    }

#ifdef _DEBUG
    auto ptTest  = event->pos();
    auto ptScene = this->mapToScene(ptTest);
    std::cout << "MousePoint(" << ptScene.x() << ", " << ptScene.y() << ")" << std::endl;
#endif

    if (m_drag)
        m_drag->onMove(mapToScene(event->pos()));

    QGraphicsView::mouseMoveEvent(event);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    setStyleSheet("QGraphicsView::rubberBand {background-color::transparent;}");
    // QApplication::restoreOverrideCursor();
    setDragMode(QGraphicsView::RubberBandDrag);
    setInteractive(true);
    m_bMoveView = false;

    if (m_drag && event->button() == Qt::RightButton)
    {
        if (m_drag->isEndDrag(mapToScene(event->pos())))
            m_drag = nullptr;
    }

    QGraphicsView::mouseReleaseEvent(event);
}

void GraphicsView::keyPressEvent(QKeyEvent *event)
{
    sindy::viewKeyDown(static_cast<Qt::Key>(event->key()), this);
    QGraphicsView::keyPressEvent(event);
}

GraphicsFrame::GraphicsFrame(const QString &name, QWidget *parent) : QFrame(parent)
{
    setFrameStyle(Sunken | StyledPanel);
    m_pGraphicsView = new GraphicsView(this);

    m_pGraphicsView->setRenderHint(QPainter::Antialiasing, false);
    m_pGraphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    m_pGraphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    m_pGraphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    m_pGraphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    // m_pGraphicsView->setTransform(QTransform::fromScale(1, -1), true);

    m_pZoomSlider = new QSlider;
    m_pZoomSlider->setMinimum(0);
    m_pZoomSlider->setMaximum(500);
    m_pZoomSlider->setValue(250);
    m_pZoomSlider->setTickPosition(QSlider::TicksRight);

    // Zoom slider layout
    QVBoxLayout *zoomSliderLayout = new QVBoxLayout;
    zoomSliderLayout->addWidget(m_pZoomSlider);

    m_pRotateSlider = new QSlider;
    m_pRotateSlider->setOrientation(Qt::Horizontal);
    m_pRotateSlider->setMinimum(-360);
    m_pRotateSlider->setMaximum(360);
    m_pRotateSlider->setValue(0);
    m_pRotateSlider->setTickPosition(QSlider::TicksBelow);

    // Rotate slider layout
    QHBoxLayout *rotateSliderLayout = new QHBoxLayout;
    rotateSliderLayout->addWidget(m_pRotateSlider);

    m_pResetBtn = new QToolButton;
    m_pResetBtn->setText(tr("reset"));
    m_pResetBtn->setEnabled(false);

    QGridLayout *topLayout = new QGridLayout;
    topLayout->addWidget(m_pGraphicsView, 0, 0);
    topLayout->addLayout(zoomSliderLayout, 0, 1);
    topLayout->addLayout(rotateSliderLayout, 1, 0);
    topLayout->addWidget(m_pResetBtn, 1, 1);
    setLayout(topLayout);

    connect(m_pResetBtn, &QAbstractButton::clicked, this, &GraphicsFrame::resetView);
    connect(m_pZoomSlider, &QAbstractSlider::valueChanged, this, &GraphicsFrame::setupMatrix);
    connect(m_pRotateSlider, &QAbstractSlider::valueChanged, this, &GraphicsFrame::setupMatrix);
    connect(m_pGraphicsView->verticalScrollBar(), &QAbstractSlider::valueChanged, this, &GraphicsFrame::setResetButtonEnabled);
    connect(m_pGraphicsView->horizontalScrollBar(), &QAbstractSlider::valueChanged, this, &GraphicsFrame::setResetButtonEnabled);

    setupMatrix();

    m_pGraphicsView->setRenderHint(QPainter::Antialiasing, true);
}

QGraphicsView *GraphicsFrame::view() const
{
    return static_cast<QGraphicsView *>(m_pGraphicsView);
}

void GraphicsFrame::resetView()
{
    m_pZoomSlider->setValue(250);
    m_pRotateSlider->setValue(0);
    setupMatrix();
    m_pGraphicsView->ensureVisible(QRectF(0, 0, 0, 0));

    m_pResetBtn->setEnabled(false);
}

void GraphicsFrame::setResetButtonEnabled()
{
    m_pResetBtn->setEnabled(true);
}

void GraphicsFrame::setupMatrix()
{
    qreal scale = qPow(qreal(2), (m_pZoomSlider->value() - 250) / qreal(50));

    QMatrix matrix;
    matrix.scale(scale, scale);
    matrix.rotate(m_pRotateSlider->value());

    m_pGraphicsView->setMatrix(matrix);
    setResetButtonEnabled();
}

void GraphicsFrame::togglePointerMode()
{
    bool isSelectMode = true;
    m_pGraphicsView->setDragMode(isSelectMode ? QGraphicsView::RubberBandDrag : QGraphicsView::ScrollHandDrag);
    m_pGraphicsView->setInteractive(isSelectMode);
}

void GraphicsFrame::toggleOpenGL()
{
#ifndef QT_NO_OPENGL
    bool isOpenGL = false;
    m_pGraphicsView->setViewport(isOpenGL ? new QGLWidget(QGLFormat(QGL::SampleBuffers)) : new QWidget);
#endif
}

void GraphicsFrame::toggleAntialiasing()
{
    bool isAntialias = false;
    m_pGraphicsView->setRenderHint(QPainter::Antialiasing, isAntialias);
}

void GraphicsFrame::print()
{
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printdialog)
    QPrinter     printer;
    QPrintDialog dialog(&printer, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        QPainter painter(&printer);
        m_pGraphicsView->render(&painter);
    }
#endif
}

void GraphicsFrame::zoomIn(int level)
{
    m_pZoomSlider->setValue(m_pZoomSlider->value() + level);
}

void GraphicsFrame::zoomOut(int level)
{
    m_pZoomSlider->setValue(m_pZoomSlider->value() - level);
}

void GraphicsFrame::rotateLeft()
{
    m_pRotateSlider->setValue(m_pRotateSlider->value() - 10);
}

void GraphicsFrame::rotateRight()
{
    m_pRotateSlider->setValue(m_pRotateSlider->value() + 10);
}
