# 机械手仿真协同软件（进行中）

## 编译环境

Qt版本:	Qt6.4.2  
编译器:	clang_arm_v8a  
安卓版本:	Android13   
请自行配置Android SDK NDK 和 JDK 等，与Android版本匹配，每个人电脑、手机型号不同，不好统一。  

## 文件夹

android 一些app的显示配置和图标  
include 用到的库  
src 源码，具体分组见Cmake的sub group  
assets 暂时没放进android的资源  

## 文件包括

Aboutpage	关于标签页UI，布置  
editorpage	我们的主标签页UI，布置 
settingpage	设置标签页UI，布置  
  
framelesswindow	我们要创建无框窗口，继承自该父类  
lineeditwidget	一个显示文本的控件，类似label UI  
pagewidget	标签页的UI，功能  
scrolllistwidget	滚动列表UI空间，功能，用来放thumbnail  
sidebar		左边条的UI控件‘  
slider		滑动条UI控件  
pushbutton	按钮控件  
  
modelselectable	UI，含删除键等，布置  
ModelSelector	模型选择UI，属于editor，功能  
ModelSetter	模型设置UI，含删除键等，功能  
modelthumbnailwidget左边栏单个物品的模型UI,功能  
modelattrslide	模型的滑动条设置UI，布置  
roundedcornerwidget 包裹模型显示UI的容器，UI功能  
SceneViewer	模型显示UI，属于editor，QOpenGLWidget  
  
boundary 		定义了一个方框，世界大小。  
camera		相机的操作  
  
ebo		顶点索引信息opengl  
vao		顶点指针数组opengl  
vbo		顶点缓冲信息opengl  
utils		宏opengl  
vertex		顶点类opengl  
  
HitRecord		拾取操作光线判断：一条光线Ray射到面上的位置。  
ray		一条光线  
mesh		跟拾取有关，返回HitRecord，一个待渲染的表  
model		模型的网格数组vector（一个网格包含顶点、索引、纹理三个vector）和纹理vector
		里面有一些我们目前的TODO和hitrecord有关，还有加载模型、处理节点、处理mesh
		遍历网格进行渲染、遍历网格光能否射到、t用来复制一个新模型  
shader		着色器，Opengl  
renderable	渲染  
skybox		天空盒子  
terrain		地面  
texture		纹理  
illuminer		光源的基类
  
logger		debug日志  
  
## obj资源（Creo导出）
  
base_part 底座  
first_arm 大臂  
second_arm 小臂  
mtl为同时导出的材质纹理。  
test为电机，想测试一下creo的色彩能否正常显示。  
  
## TODO列表

按优先级排列  
1 android端的移植，一半  
1.1 去assimp库导入,PROE的模型文件纹理是MTL格式，非贴图，把MTL纹理给上去  
1.2 assets的处理，对GLSL着色器的解决  
1.3 打包字体的问题  
2sceneviewer用键盘前后左右运动  
3直接加载三个零件，然后绑定两个配合零件的轴线。  
算法上想法：  
a.独立于模型做个轴线类，轴线:vector<int>相关零件索引；vec3 fun(coords){计算出零件的坐标}。  
零件动先算所有轴线，从轴线再算零件。  
b.或对model的类元素增加一个轴线坐标类  
a是轴线含零件，b是零件含轴线  
再考虑怎么实现。  
4光标拖动小臂绕着小臂轴转，拖动大臂，小臂和大臂同时转。  
5修正注释  
  
## 部分已解决的小问题

屏幕翻转，完成  
asset打包进apk，完成  
  
# 助记笔记

LearningOpenglDetails.txt
CMakeCheatsheet.txt
GlslFragmentShader.txt
LearningCG.txt
NOTE_obj_mtl_import.txt
LearningOpenglDetails.txt
  



