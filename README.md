# GraphicsViewEx
## 简介
这是一个简易的绘图项目，基于QT的QGraphicsView开发，会陆续开发一些图形相关的功能

## 功能
### 基础
- 移动画布：按住鼠标中键并移动
- 缩放画布：拖动右侧滑块
- 旋转画布：拖动底部滑块
- 删除：选中图形后按Delete键

### 绘制
- 绘制多段线：P键开始，鼠标拾取顶点，右键结束、C键闭合、Esc取消绘制
- 绘制包围盒：选中图形后按B键

### 数据
支持ctrl+s保存文档

  
## 构建
- 本项目使用CMake构建，需要[安装QT](https://download.qt.io/archive/qt/5.14/5.14.2/)
- Windows系统，请在环境变量中提供QT路径，如"D:/Qt/Qt5.14.2/5.14.2/msvc2017_64/bin"

## 设计
### 代码结构
| 目录 | 说明 |
| :- | :- |
control | 图形控制层，对图形的操作
core | 整个模型的核心代码
data | 图形数据的序列化、反序列化等
graphics | 自定义实体层
utility | 工具函数

### 类设计
- 支持添加自定义图形：请从BasicGraphic派生子类，并实现相关虚函数
- 文档序列化采用第三方库kiwi，你只需要提供包含各个字段的模式文件，它会自动生成各语言的编解码代码
  - 字段描述文件(xxx.kiwi)
    - 你的图形继承体系下的所有父类、子类的成员变量应该放在同一个struct内，这是和结构化数据流最大的不同
    - message中的成员由flag管理，可以只保存修改的字段；struct中的字段必须全部提供，适用于固定不变的数据，比如二维点结构，不能缺了x或y
    - message中的数字只是一个递增的序号（kiwic编译时，这里经常报错）
    - 不同版本的字段变更，对于会写入磁盘或远程服务器的数据，要向前兼容只能新增字段，不能删除字段（这只会浪费一个指针的内存）
  - [利用kiwic生成C++的序列化、反序列化代码](https://github.com/evanw/kiwi/blob/master/examples/cpp.md)
    - 安装kiwic时，要以管理员身份启动命令提示符CMD：npm install -g kiwi-schema
    - cd到schema.kiwi文件所在位置（data目录）：kiwic --schema schema.kiwi --cpp schema.h
    - 将已生成的schema.h文件中的实现部分放到schema.cc中，搜"#ifdef"即可定位到
      - 收缩宏包围的代码段，剪切到源文件中，注意要在相同命名空间下(sindyk)，并包含schema.h
  - 序列化采用非侵入式设计，主要基于访问者模式，核心在于子类实现了visit函数


## 其它
- 如有其它技术问题，请联系QQ群：571208653
- 如果觉得不错，请点击Star让更多人看到
