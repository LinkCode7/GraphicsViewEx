#ifndef GRAPHICS_MAKER_H
#define GRAPHICS_MAKER_H

class IGeGraphic;

/*
 * @brief 处理对象创建时的业务
 */
class GraphicMaker
{
public:
    void receive(IGeGraphic *pNew);

protected:
    virtual void subCreate(IGeGraphic *pNew) = 0;

private:
    void addToDocument(IGeGraphic *pNew);
};

class BoxMaker : public GraphicMaker
{
public:
    void subCreate(IGeGraphic *pNew) override {}
};

class AimMaker : public GraphicMaker
{
public:
    void subCreate(IGeGraphic *pNew) override {}
};

class PolylineMaker : public GraphicMaker
{
public:
    void subCreate(IGeGraphic *pNew) override {}
};

class PolygonMaker : public GraphicMaker
{
public:
    void subCreate(IGeGraphic *pNew) override {}
};

class GeSquarePointsMaker : public GraphicMaker
{
public:
    void subCreate(IGeGraphic *pNew) override {}
};

class GePolylineIndexMaker : public GraphicMaker
{
public:
    void subCreate(IGeGraphic *pNew) override {}
};

class GeSegmentMaker : public GraphicMaker
{
public:
    void subCreate(IGeGraphic *pNew) override {}
};

#endif // !GRAPHICS_MAKER_H
