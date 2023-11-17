#pragma once

#include <qwidget.h>
#include <qgraphicseffect.h>

#include "pushbutton.h"

class PageWidget : public QWidget
{
    
    Q_OBJECT
        
public:
    PageWidget(QWidget* parent = 0);
    ~PageWidget();

protected:
    // 页面布局和content（其他东西的容器控件）
    QVBoxLayout* _stretchLayout = nullptr;
    QWidget* _contentWidget = nullptr;
    
    // 边条按钮
    PushButton* _iconButton = nullptr;
    PushButton* _textButton = nullptr;

    // 不透明效果
    QGraphicsOpacityEffect* _pageOpacityEffect = nullptr;

    // 存储页的绘制位置
    QPoint _originPagePosition = QPoint(0, 0);
    
public:
    virtual PushButton* getPageIconButton(QWidget* context) = 0;  // 提供按钮给一个icon
    virtual PushButton* getPageTextButton(QWidget* context) = 0;  // 提供按钮给一个icon和文本
    
public:
    void onStage();     // 提供一个进入舞台的动画当页面被选择时
    void offStage();    // 提供一个离开舞台的动画当页面被取消选择时
};