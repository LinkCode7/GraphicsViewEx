#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QGraphicsScene;
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private:
    void populateScene();
};

#endif  // MAIN_WINDOW_H
