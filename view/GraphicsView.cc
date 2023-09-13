#include "GraphicsView.h"

#include "../control/DragAction.h"
#include "../control/GraphicsControl.h"
#include "../control/PolylineDrag.h"
#include "GraphicsArchive.h"
#include "PropertyTable.h"

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
            _frame->zoomIn(6);
        else
            _frame->zoomOut(6);
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

        addState(eMovingView);
        this->_ptRightMouseDown = event->pos();
    }
    else if (event->button() == Qt::LeftButton)
    {
        if (hasState(eDragInit))
        {
            setDragMode(QGraphicsView::NoDrag);
            setInteractive(false);

            removeState(eDragInit);
        }

        if (_drag)
        {
            setInteractive(false); // 拖拽过程中禁止移动item
            _drag->onLeftClick(mapToScene(event->pos()));
        }
    }
    else if (event->button() == Qt::RightButton)
    {
        if (!_drag)
        {
            _resetViewMatrix->setCheckable(hasState(eChangeViewMatrix));
            _menu->exec(QCursor::pos());
        }
    }

    QGraphicsView::mousePressEvent(event);
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    setStyleSheet("QGraphicsView::rubberBand {background-color::transparent;}");
    // QApplication::restoreOverrideCursor();
    if (hasState(eMovingView))
    {
        // m_handCursor.setShape(Qt::ClosedHandCursor);
        // QApplication::setOverrideCursor(m_handCursor);
        QPoint  movPos = event->pos();
        QPointF dis    = mapToScene(movPos) - mapToScene(_ptRightMouseDown);
        QGraphicsView::translate(dis.x(), dis.y());
    }

#ifdef _DEBUG
    auto ptTest  = event->pos();
    auto ptScene = this->mapToScene(ptTest);
    // std::cout << "MousePoint(" << ptScene.x() << ", " << ptScene.y() << ")" << std::endl;
#endif

    if (_drag)
        _drag->onMove(mapToScene(event->pos()));

    QGraphicsView::mouseMoveEvent(event);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    setStyleSheet("QGraphicsView::rubberBand {background-color::transparent;}");
    // QApplication::restoreOverrideCursor();
    setDragMode(QGraphicsView::RubberBandDrag);
    setInteractive(true);

    removeState(eMovingView);

    if (_drag && event->button() == Qt::RightButton)
    {
        if (_drag->isEndDrag(mapToScene(event->pos())))
            _drag = nullptr;
    }

    QGraphicsView::mouseReleaseEvent(event);
}

void GraphicsView::keyPressEvent(QKeyEvent *event)
{
    sindy::viewKeyDown(event, this);
    QGraphicsView::keyPressEvent(event);
}

GraphicsView ::~GraphicsView()
{
    delete _menu;
}

GraphicsView::GraphicsView(GraphicsFrame *v) : QGraphicsView(), _frame(v)
{
    // 黑色背景
    setBackgroundBrush(QColor{0, 0, 0});

    // 菜单
    _menu = new QMenu(this);

    QAction *action = new QAction("open document.", this);
    _menu->addAction(action);
    QObject::connect(action, &QAction::triggered, this, &sindy::onOpenDocument);

    action = new QAction("save document.(ctrl+s)", this);
    _menu->addAction(action);
    QObject::connect(action, &QAction::triggered, this, &sindy::onSaveDocument);

    QAction *separator = new QAction(this);
    separator->setSeparator(true);
    _menu->addAction(separator);

#ifdef USING_BOOST_BOOLEAN
    action = new QAction("boolean-intersection", this);
    _menu->addAction(action);
    QObject::connect(action, &QAction::triggered, this, &sindy::booleanIntersection);

    action = new QAction("boolean-union", this);
    _menu->addAction(action);
    QObject::connect(action, &QAction::triggered, this, &sindy::booleanUnion);

    action = new QAction("boolean-difference", this);
    _menu->addAction(action);
    QObject::connect(action, &QAction::triggered, this, &sindy::booleanDifference);

    separator = new QAction(this);
    separator->setSeparator(true);
    _menu->addAction(separator);
#endif // USING_BOOST_BOOLEAN

    action = new QAction("create selected box.(b)", this);
    _menu->addAction(action);
    QObject::connect(action, &QAction::triggered, this, &sindy::createSelectedBox);

    action = new QAction("create polyline.(p)", this);
    _menu->addAction(action);
    QObject::connect(action, &QAction::triggered, this, &sindy::onCreatePolyline);

    action = new QAction("delete selected items.(Delete)", this);
    _menu->addAction(action);
    QObject::connect(action, &QAction::triggered, this, &sindy::deleteSelectedItems);

    separator = new QAction(this);
    separator->setSeparator(true);
    _menu->addAction(separator);

    _resetViewMatrix = new QAction("reset view matrix.", this);
    _resetViewMatrix->setCheckable(false);
    _menu->addAction(_resetViewMatrix);
    QObject::connect(_resetViewMatrix, &QAction::triggered, this, [&]() { _frame->resetView(); });

    action = new QAction("import from json.", this);
    _menu->addAction(action);
    QObject::connect(action, &QAction::triggered, this, &sindy::onImportFromJson);
}

GraphicsFrame::GraphicsFrame(GraphicsScene *scene, const QString &name, QWidget *parent) : QFrame(parent)
{
    setFrameStyle(Sunken | StyledPanel);
    _pGraphicsView = new GraphicsView(this);
    _pGraphicsView->setScene(scene);

    GeArchive().setCurView(_pGraphicsView);
    GeArchive().setScene(scene);

    _pGraphicsView->setRenderHint(QPainter::Antialiasing, false);
    _pGraphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    _pGraphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    _pGraphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    _pGraphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    // _pGraphicsView->setTransform(QTransform::fromScale(1, -1), true);

    _pZoomSlider = new QSlider;
    _pZoomSlider->setMinimum(0);
    _pZoomSlider->setMaximum(500);
    _pZoomSlider->setValue(250);
    _pZoomSlider->setTickPosition(QSlider::TicksRight);

    // Zoom slider layout
    QVBoxLayout *zoomSliderLayout = new QVBoxLayout;
    zoomSliderLayout->addWidget(_pZoomSlider);

    _pRotateSlider = new QSlider;
    _pRotateSlider->setOrientation(Qt::Horizontal);
    _pRotateSlider->setMinimum(-360);
    _pRotateSlider->setMaximum(360);
    _pRotateSlider->setValue(0);
    _pRotateSlider->setTickPosition(QSlider::TicksBelow);

    // Rotate slider layout
    QHBoxLayout *rotateSliderLayout = new QHBoxLayout;
    rotateSliderLayout->addWidget(_pRotateSlider);

    // 属性设置
    auto checkBox1 = new QCheckBox;
    checkBox1->setText("property");
    auto checkBox2 = new QCheckBox;
    checkBox2->setText("boolean");

    auto leftH = new QHBoxLayout;
    leftH->addWidget(checkBox1);
    leftH->addWidget(checkBox2);
    // 表格
    _property = new PropertyTable();
    scene->setPropertyTable(_property);
    _property->setShowGrid(false);

    auto leftV = new QVBoxLayout;
    leftV->addLayout(leftH);
    leftV->addWidget(_property);

    QGridLayout *topLayout = new QGridLayout;
    topLayout->addWidget(_pGraphicsView, 0, 1);
    topLayout->addLayout(leftV, 0, 0);
    topLayout->addLayout(zoomSliderLayout, 0, 2);
    topLayout->addLayout(rotateSliderLayout, 1, 1);
    topLayout->setColumnStretch(0, 15);
    topLayout->setColumnStretch(1, 100);
    topLayout->setColumnStretch(2, 1);
    // topLayout->setColumnMinimumWidth(2, 10);
    setLayout(topLayout);

    connect(_pZoomSlider, &QAbstractSlider::valueChanged, this, &GraphicsFrame::setupMatrix);
    connect(_pRotateSlider, &QAbstractSlider::valueChanged, this, &GraphicsFrame::setupMatrix);
    connect(_pGraphicsView->verticalScrollBar(), &QAbstractSlider::valueChanged, this,
            &GraphicsFrame::setResetButtonEnabled);
    connect(_pGraphicsView->horizontalScrollBar(), &QAbstractSlider::valueChanged, this,
            &GraphicsFrame::setResetButtonEnabled);

    QObject::connect(checkBox1, &QCheckBox::stateChanged, [&](int arg1) {
        if (arg1 == Qt::Checked)
        {
            _pGraphicsView->addState(GraphicsView::eShowProperty);
        }
        else
        {
            _pGraphicsView->removeState(GraphicsView::eShowProperty);
        }
    });

    QObject::connect(checkBox2, &QCheckBox::stateChanged, [&](int arg1) {
        if (arg1 == Qt::Checked)
        {
            _pGraphicsView->addState(GraphicsView::eShowBooleanMenu);
        }
        else
        {
            _pGraphicsView->removeState(GraphicsView::eShowBooleanMenu);
        }
    });

    setupMatrix();

    _pGraphicsView->setRenderHint(QPainter::Antialiasing, true);
}

QGraphicsView *GraphicsFrame::view() const
{
    return static_cast<QGraphicsView *>(_pGraphicsView);
}

void GraphicsFrame::resetView()
{
    _pZoomSlider->setValue(250);
    _pRotateSlider->setValue(0);
    setupMatrix();
    _pGraphicsView->ensureVisible(QRectF(0, 0, 0, 0));

    _pGraphicsView->removeState(GraphicsView::eChangeViewMatrix);
}

void GraphicsFrame::setResetButtonEnabled()
{
    _pGraphicsView->addState(GraphicsView::eChangeViewMatrix);
}

void GraphicsFrame::setupMatrix()
{
    qreal scale = qPow(qreal(2), (_pZoomSlider->value() - 250) / qreal(50));

    QMatrix matrix;
    matrix.scale(scale, scale);
    matrix.rotate(_pRotateSlider->value());

    _pGraphicsView->setMatrix(matrix);
    setResetButtonEnabled();
}

void GraphicsFrame::togglePointerMode()
{
    bool isSelectMode = true;
    _pGraphicsView->setDragMode(isSelectMode ? QGraphicsView::RubberBandDrag : QGraphicsView::ScrollHandDrag);
    _pGraphicsView->setInteractive(isSelectMode);
}

void GraphicsFrame::toggleOpenGL()
{
#ifndef QT_NO_OPENGL
    bool isOpenGL = false;
    _pGraphicsView->setViewport(isOpenGL ? new QGLWidget(QGLFormat(QGL::SampleBuffers)) : new QWidget);
#endif
}

void GraphicsFrame::toggleAntialiasing()
{
    bool isAntialias = false;
    _pGraphicsView->setRenderHint(QPainter::Antialiasing, isAntialias);
}

void GraphicsFrame::print()
{
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printdialog)
    QPrinter     printer;
    QPrintDialog dialog(&printer, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        QPainter painter(&printer);
        _pGraphicsView->render(&painter);
    }
#endif
}

void GraphicsFrame::zoomIn(int level)
{
    _pZoomSlider->setValue(_pZoomSlider->value() + level);
}

void GraphicsFrame::zoomOut(int level)
{
    _pZoomSlider->setValue(_pZoomSlider->value() - level);
}

void GraphicsFrame::rotateLeft()
{
    _pRotateSlider->setValue(_pRotateSlider->value() - 10);
}

void GraphicsFrame::rotateRight()
{
    _pRotateSlider->setValue(_pRotateSlider->value() + 10);
}
