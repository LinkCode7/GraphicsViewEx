#include "main.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QSplitter>

#include "./core/GraphicsView.h"
#include "utility/utility.h"

QGraphicsScene *g_pScene = nullptr;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    MainWindow window;
    window.showMaximized();

    return app.exec();
}

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    g_pScene = new QGraphicsScene(this);

    GraphicsFrame *frame = new GraphicsFrame("view");
    frame->view()->setScene(g_pScene);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(frame);
    setLayout(layout);

    setWindowTitle(tr("GraphicsView"));

#define SCENE_LENGTH 100000
    // 绘制坐标轴，把scene撑大
    QPen pen(Qt::gray, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    g_pScene->addLine(-SCENE_LENGTH, 0, SCENE_LENGTH, 0, pen);
    g_pScene->addLine(0, SCENE_LENGTH, 0, -SCENE_LENGTH, pen);
#undef SCENE_LENGTH

    // sindy::addChipToScene();
    sindy::addTestEntity();
}
