#ifndef GRAPHICS_MAKER_H
#define GRAPHICS_MAKER_H

class BasicGraphic;

/*
 * @brief 处理对象创建时的业务
 */
class GraphicMaker
{
public:
    void receive(BasicGraphic *pNew);

protected:
    virtual void subCreate(BasicGraphic *pNew) = 0;

private:
    void addToDocument(BasicGraphic *pNew);
};

class BoxMaker : public GraphicMaker
{
public:
    void subCreate(BasicGraphic *pNew) override {}
};

class PointMaker : public GraphicMaker
{
public:
    void subCreate(BasicGraphic *pNew) override {}
};

class PolylineMaker : public GraphicMaker
{
public:
    void subCreate(BasicGraphic *pNew) override {}
};

#endif // !GRAPHICS_MAKER_H
