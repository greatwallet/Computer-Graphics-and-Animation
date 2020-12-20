# HW1
## 作业标题 
多边形的几何操作

## 作业内容：

* 实现任意多边形（凹、凸、带内环）的输入及显示，可自定义多边形（边界、内部）的颜色；
* 实现任意多边形的平移、旋转、缩放；
* 实现两个任意多边形的裁剪，并支持多步裁剪；
* 只能使用画点、画线的函数，如SetPixel、MoveTo、LineTo等。

## TODO
- [X] Input Polygon wo holes
- [X] Simple Transformation like translation, rotation and scale
- [ ] change the input file to json format 
- [ ] leverage json format file to define holes in polygon
- [ ] Leverage `Sutherland-Hodgeman Algorithm` to perform cutting
- [ ] give a interactive console window or GUI
