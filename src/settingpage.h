#pragma once

#include <qwidget.h>
#include <qlabel.h>
#include <qboxlayout.h>
#include <qpair.h>

#include "pagewidget.h"
#include "lineeditwidget.h"
#include "modelattrslide.h"

class SettingPage : public PageWidget
{
    Q_OBJECT

public:
    SettingPage(QWidget* parent = 0);
    ~SettingPage();

private:
    // 按钮图标
    QLabel* _iconButtonLabel = nullptr;
    QWidget* _textButtonWidget = nullptr;
    QHBoxLayout* _textButtonLayout = nullptr;
    QLabel* _textButtonIcon = nullptr;
    QLabel* _textButtonLabel = nullptr;

    // UI 
    QVBoxLayout* _titleLayout = nullptr;
    const QFont _titleFont = QFont("DengXian", 26, QFont::ExtraLight);
    QLabel* _titleLabel = nullptr;

    QWidget* _mainWidget = nullptr;
    QVBoxLayout* _mainLayout = nullptr;

    // 贴到检查盒子表面
    PushButton* _stickSurfaceBtn = nullptr;
    QLabel* _stickSurfaceBtnLabel = nullptr;

    // 地形和天空盒子选择器
    QWidget* _skyTerSelContainer = nullptr;
    QHBoxLayout* _skyTerSelLayout = nullptr;
    PushButton* _skySelector = nullptr;
    QLabel* _skySelectorLabel = nullptr;
    PushButton* _terrainSelector = nullptr;
    QLabel* _terrainSelectorLabel = nullptr;
    
    // 方向光设置
    //QWidget* _dirLightSetter = nullptr;
    //QHBoxLayout* _dirLightSetterLayout = nullptr;
    PushButton* _dirLightSwitch = nullptr;
    QLabel* _dirLightSwitchLabel = nullptr;
    //QVBoxLayout* _dirLightColorLayout = nullptr;
    ModelAttributeSlide* _dirLightColorR = nullptr;
    ModelAttributeSlide* _dirLightColorG = nullptr;
    ModelAttributeSlide* _dirLightColorB = nullptr;
    //QVBoxLayout* _dirLightAttrLayout;
    ModelAttributeSlide* _dirLightIntensity = nullptr;
    ModelAttributeSlide* _dirLightTheta = nullptr;
    ModelAttributeSlide* _dirLightPhi = nullptr;

private:
    void selectSkyBox();
    void selectTerrain();

public:
    virtual PushButton* getPageIconButton(QWidget* context) override;
    virtual PushButton* getPageTextButton(QWidget* context) override;

signals:
    void onSettingsChanged(QPair<QString, QString> settings);
};