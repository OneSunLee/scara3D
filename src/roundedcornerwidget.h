#pragma once

#include <qwidget.h>
#include <qevent.h>
#include <qboxlayout.h>

class RoundedCornerWidget : public QWidget {
    
    Q_OBJECT

public:
    RoundedCornerWidget(QWidget* parent = 0);
    ~RoundedCornerWidget();

protected:
    // 设置
    int _cornerRadius = 15;
    QColor _borderColor = QColor(255, 255, 255);

    // UI元素
    QWidget* _mainWidget = nullptr;
    QVBoxLayout* _mainLayout = nullptr;
    QWidget* _borderWidget = nullptr;
    
    // 圆角掩码初始化
    bool _initialized = false;
    void initializeWidgetUI();

    // UI 事件
    virtual void showEvent(QShowEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;

public:
    // API
    void setCornerRadius(int radius);
    void setBorderColor(QColor color);
    QWidget* mainWidget() const;
};
