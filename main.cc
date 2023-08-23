#include "main.h"

#include <qmatrix4x4.h>

#include <QApplication>
#include <QHBoxLayout>
#include <QSplitter>

#include "./view/GraphicsArchive.h"
#include "./view/GraphicsScene.h"
#include "./view/GraphicsView.h"
#include "utility/utility.h"

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
    auto pScene = new GraphicsScene(this);

    GraphicsFrame *frame = new GraphicsFrame(pScene, "view");

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(frame);
    setLayout(layout);

    setWindowTitle(tr("GraphicsView"));

#define SCENE_LENGTH 100000
    // 绘制坐标轴，把scene撑大
    QPen pen(Qt::gray, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    pScene->addLine(-SCENE_LENGTH, 0, SCENE_LENGTH, 0, pen);
    pScene->addLine(0, SCENE_LENGTH, 0, -SCENE_LENGTH, pen);
#undef SCENE_LENGTH

    // sindy::addChipToScene();
    sindy::addTestEntity();
}
