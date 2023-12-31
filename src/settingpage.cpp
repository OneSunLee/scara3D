#include <qfiledialog.h>
#include <qmessagebox.h>
#include "../include/GLM/glm.hpp"

#include "settingpage.h"
#include "logger.h"

SettingPage::SettingPage(QWidget* parent) :
    PageWidget(parent)
{
    _contentWidget->setMouseTracking(true);

    // 创建标题布局
    _titleLayout = new QVBoxLayout(_contentWidget);
    _titleLayout->setContentsMargins(28, 46, 28, 28);
    _titleLayout->setSpacing(18);
    _titleLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    _contentWidget->setLayout(_titleLayout);

    // 创建标题
    _titleLabel = new QLabel("SETTINGS", _contentWidget);
    _titleLabel->setFont(_titleFont);
    _titleLayout->addWidget(_titleLabel);
    _titleLabel->show();

    // 创建主布局
    _mainWidget = new QWidget(_contentWidget);
    _mainWidget->setObjectName("mainWidget");
    _mainWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    _mainWidget->setStyleSheet("QWidget#mainWidget { background-color: #f5f5f5; border-radius: 8px; }");
    _mainLayout = new QVBoxLayout(_mainWidget);
    _mainLayout->setAlignment(Qt::AlignTop);
    _mainLayout->setContentsMargins(12, 12, 12, 12);
    _mainLayout->setSpacing(8);
    _mainWidget->setLayout(_mainLayout);
    _titleLayout->addWidget(_mainWidget);
    _mainWidget->show();

    // 创建stick表面按钮，用于让新创建的model自动靠在表面上
    _stickSurfaceBtn = new PushButton(nullptr, _mainWidget);
    _stickSurfaceBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _stickSurfaceBtn->setMaximumHeight(48);
    _stickSurfaceBtn->setBackgroundColor(QColor(220, 220, 220));
    _stickSurfaceBtnLabel = new QLabel("Stick to Surface", _stickSurfaceBtn);
    _stickSurfaceBtnLabel->setAlignment(Qt::AlignCenter);
    _stickSurfaceBtnLabel->show();
    _stickSurfaceBtn->setChildWidget(_stickSurfaceBtnLabel);
    _mainLayout->addWidget(_stickSurfaceBtn);
    _stickSurfaceBtn->show();
    connect(_stickSurfaceBtn, &PushButton::onClick, this, [=]() {
        if (_stickSurfaceBtn->isSelected()) {
            _stickSurfaceBtn->deselect();
            emit onSettingsChanged(QPair<QString, QString>("stickSurface", "false"));
        }
        else {
            _stickSurfaceBtn->select();
            emit onSettingsChanged(QPair<QString, QString>("stickSurface", "true"));
        }
    });
    
    // 创建天空盒子选择器
    _skyTerSelContainer = new QWidget(_mainWidget);
    _skyTerSelContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _skyTerSelContainer->setMaximumHeight(48);
    _skyTerSelLayout = new QHBoxLayout(_skyTerSelContainer);
    _skyTerSelLayout->setContentsMargins(0, 0, 0, 0);
    _skyTerSelLayout->setSpacing(6);
    _skyTerSelContainer->setLayout(_skyTerSelLayout);
    _mainLayout->addWidget(_skyTerSelContainer);
    _skyTerSelContainer->show();
    
    _skySelector = new PushButton(nullptr, _skyTerSelContainer);
    _skySelector->setBackgroundColor(QColor(220, 220, 220));
    _skySelector->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _skySelectorLabel = new QLabel("Import Skybox", _skySelector);
    _skySelectorLabel->setAlignment(Qt::AlignCenter);
    _skySelectorLabel->show();
    _skySelector->setChildWidget(_skySelectorLabel);
    _skyTerSelLayout->addWidget(_skySelector);
    _skySelector->show();
    connect(_skySelector, &PushButton::onClick, this, &SettingPage::selectSkyBox);
    
    _terrainSelector = new PushButton(nullptr, _skyTerSelContainer);
    _terrainSelector->setBackgroundColor(QColor(220, 220, 220));
    _terrainSelector->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _terrainSelectorLabel = new QLabel("Import Terrain", _terrainSelector);
    _terrainSelectorLabel->setAlignment(Qt::AlignCenter);
    _terrainSelectorLabel->show();
    _terrainSelector->setChildWidget(_terrainSelectorLabel);
    _skyTerSelLayout->addWidget(_terrainSelector);
    _terrainSelector->show();
    connect(_terrainSelector, &PushButton::onClick, this, &SettingPage::selectTerrain);
    
    // 创建平行光设置器
    _dirLightSwitch = new PushButton(nullptr, _mainWidget);
    _dirLightSwitch->setBackgroundColor(QColor(220, 220, 220));
    _dirLightSwitch->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _dirLightSwitch->setMaximumHeight(48);
    _dirLightSwitchLabel = new QLabel("Directional Light", _dirLightSwitch);
    _dirLightSwitchLabel->setAlignment(Qt::AlignCenter);
    _dirLightSwitchLabel->show();
    _dirLightSwitch->setChildWidget(_dirLightSwitchLabel);
    _mainLayout->addWidget(_dirLightSwitch);
    _dirLightSwitch->show();
    connect(_dirLightSwitch, &PushButton::onClick, this, [=]() {
        if (_dirLightSwitch->isSelected()) {
            _dirLightSwitch->deselect();
            _dirLightColorR->setEnabled(false);
            _dirLightColorG->setEnabled(false);
            _dirLightColorB->setEnabled(false);
            _dirLightIntensity->setEnabled(false);
            _dirLightTheta->setEnabled(false);
            _dirLightPhi->setEnabled(false);
            emit onSettingsChanged(QPair<QString, QString>("dirLight", "false"));
        }
        else {
            _dirLightSwitch->select();
            _dirLightColorR->setEnabled(true);
            _dirLightColorG->setEnabled(true);
            _dirLightColorB->setEnabled(true);
            _dirLightIntensity->setEnabled(true);
            _dirLightTheta->setEnabled(true);
            _dirLightPhi->setEnabled(true);
            emit onSettingsChanged(QPair<QString, QString>("dirLight", "true"));
        }
    });
    
    _dirLightColorR = new ModelAttributeSlide("Red", 0, 255, 255, _mainWidget);
    _dirLightColorR->setValue(255);
    _dirLightColorR->setEnabled(false);
    _mainLayout->addWidget(_dirLightColorR);
    _dirLightColorR->show();
    connect(_dirLightColorR, &ModelAttributeSlide::onChanged, this, [=](int value) {
        emit onSettingsChanged(QPair<QString, QString>("dirLightColorR", QString::number(value / 255.0f)));
    });

    _dirLightColorG = new ModelAttributeSlide("Green", 0, 255, 255, _mainWidget);
    _dirLightColorG->setValue(255);
    _dirLightColorG->setEnabled(false);
    _mainLayout->addWidget(_dirLightColorG);
    _dirLightColorG->show();
    connect(_dirLightColorG, &ModelAttributeSlide::onChanged, this, [=](int value) {
        emit onSettingsChanged(QPair<QString, QString>("dirLightColorG", QString::number(value / 255.0f)));
    });
    
    _dirLightColorB = new ModelAttributeSlide("Blue", 0, 255, 255, _mainWidget);
    _dirLightColorB->setValue(255);
    _dirLightColorB->setEnabled(false);
    _mainLayout->addWidget(_dirLightColorB);
    _dirLightColorB->show();
    connect(_dirLightColorB, &ModelAttributeSlide::onChanged, this, [=](int value) {
        emit onSettingsChanged(QPair<QString, QString>("dirLightColorB", QString::number(value / 255.0f)));
    });
    
    _dirLightIntensity = new ModelAttributeSlide("Intensity", 0, 100, 100, _mainWidget);
    _dirLightIntensity->setValue(50);
    _dirLightIntensity->setEnabled(false);
    _mainLayout->addWidget(_dirLightIntensity);
    _dirLightIntensity->show();
    connect(_dirLightIntensity, &ModelAttributeSlide::onChanged, this, [=](int value) {
        emit onSettingsChanged(QPair<QString, QString>("dirLightIntensity", QString::number(value)));
    });
    
    _dirLightTheta = new ModelAttributeSlide("Rotate\u03B8", 0, 360, 3600, _mainWidget);
    _dirLightTheta->setValue(270);
    _dirLightTheta->setEnabled(false);
    _mainLayout->addWidget(_dirLightTheta);
    _dirLightTheta->show();
    connect(_dirLightTheta, &ModelAttributeSlide::onChanged, this, [=]() {
        // 计算方向矢量，用theta和phi
        float theta = _dirLightTheta->val();    // theta 
        float phi = _dirLightPhi->val();        // phi
        float x = sin(glm::radians(phi)) * cos(glm::radians(theta));
        float y = sin(glm::radians(phi)) * sin(glm::radians(theta));
        float z = cos(glm::radians(phi));
        glm::vec3 dir = glm::normalize(glm::vec3(x, y, z));
        emit onSettingsChanged(QPair<QString, QString>("dirLightDir", QString::number(dir.x) + "," + QString::number(dir.y) + "," + QString::number(dir.z)));
    });
    
    _dirLightPhi = new ModelAttributeSlide("Rotate\u03C6", 0, 360, 3600, _mainWidget);
    _dirLightPhi->setValue(90);
    _dirLightPhi->setEnabled(false);
    _mainLayout->addWidget(_dirLightPhi);
    _dirLightPhi->show();
    connect(_dirLightPhi, &ModelAttributeSlide::onChanged, this, [=]() {
        // 计算平行光矢量 theta 和 phi
        float theta = _dirLightTheta->val();    // theta 
        float phi = _dirLightPhi->val();        // phi 
        float x = sin(glm::radians(phi)) * cos(glm::radians(theta));
        float y = sin(glm::radians(phi)) * sin(glm::radians(theta));
        float z = cos(glm::radians(phi));
        glm::vec3 dir = glm::normalize(glm::vec3(x, y, z));
        emit onSettingsChanged(QPair<QString, QString>("dirLightDir", QString::number(dir.x) + "," + QString::number(dir.y) + "," + QString::number(dir.z)));
    });
}

SettingPage::~SettingPage() {}

PushButton* SettingPage::getPageIconButton(QWidget* context) {
    // 检查变量
    if (_iconButton != nullptr) {
        return _iconButton;
    }

    // 生成新图标按钮
    _iconButton = new PushButton(nullptr, context);
    _iconButton->setMargin(20, 18, 16, 18);
    _iconButtonLabel = new QLabel(_iconButton);
    _iconButtonLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    QFont iconButtonFont("Font Awesome 6 Free Solid", 12);
    iconButtonFont.setStyleStrategy(QFont::PreferAntialias);
    _iconButtonLabel->setFont(iconButtonFont);
    _iconButtonLabel->setText("\uf013");
    _iconButtonLabel->setAlignment(Qt::AlignLeft);
    _iconButton->setChildWidget(_iconButtonLabel);

    // 返回新的图标
    return _iconButton;
}

PushButton* SettingPage::getPageTextButton(QWidget* context) {
    // 检查变量
    if (_textButton != nullptr) {
        return _textButton;
    }

    // 创建新的文本按钮
    _textButton = new PushButton(nullptr, context);
    _textButton->setMargin(20, 18, 16, 18);
    _textButtonWidget = new QWidget(_textButton);
    _textButtonLayout = new QHBoxLayout(_textButtonWidget);
    _textButtonLayout->setContentsMargins(0, 0, 0, 0);
    _textButtonLayout->setSpacing(12);
    _textButtonWidget->setLayout(_textButtonLayout);

    // 创建新的文本内容
    _textButtonIcon = new QLabel(_textButtonWidget);
    QFont textButtonFont("Font Awesome 6 Free Solid", 12);
    textButtonFont.setStyleStrategy(QFont::PreferQuality);
    _textButtonIcon->setFont(textButtonFont);
    _textButtonIcon->setText("\uf013");
    _textButtonIcon->setAlignment(Qt::AlignLeft);

    _textButtonLabel = new QLabel(_textButtonWidget);
    _textButtonLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    _textButtonLabel->setText("Settings");
    _textButtonLabel->setAlignment(Qt::AlignLeft);

    // 添加文本按钮内容到布局上
    _textButtonLayout->addWidget(_textButtonIcon);
    _textButtonLayout->addWidget(_textButtonLabel);
    _textButtonIcon->show();
    _textButtonLabel->show();

    // 挂到对象树上
    _textButton->setChildWidget(_textButtonWidget);
    _textButtonWidget->show();

    // 返回
    return _textButton;
}

void SettingPage::selectSkyBox() {
    // 选择一个目录，含 back.jpg, bottom.jpg, front.jpg, left.jpg, right.jpg, top.jpg
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select Skybox Directory"), QDir::currentPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (dir.isEmpty()) {
        Logger::warning("Tempt to open invalid skybox folder");
        emit onSettingsChanged(QPair<QString, QString>("skybox", ""));
        return;
    }
    
    // 检查目录文件是否齐全
    QDir skyboxDir(dir);
    QStringList skyboxFiles = skyboxDir.entryList(QStringList() << "*.jpg", QDir::Files);
    if (
        skyboxFiles.indexOf("back.jpg") == -1   || 
        skyboxFiles.indexOf("bottom.jpg") == -1 || 
        skyboxFiles.indexOf("front.jpg") == -1  || 
        skyboxFiles.indexOf("left.jpg") == -1   || 
        skyboxFiles.indexOf("right.jpg") == -1  || 
        skyboxFiles.indexOf("top.jpg") == -1
       ) {
        QMessageBox::warning(this, "Error", "The selected directory does not contain all the required files.");
        Logger::warning("Tempt to open invalid skybox folder");
        emit onSettingsChanged(QPair<QString, QString>("skybox", ""));
        return;
    }
    
    emit onSettingsChanged(QPair<QString, QString>("skybox", dir));
}

void SettingPage::selectTerrain() {
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select Terrain Directory"), QDir::currentPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (dir.isEmpty()) {
        Logger::warning("Tempt to open invalid terrain folder");
        emit onSettingsChanged(QPair<QString, QString>("terrain", ""));
        return;
    }
    
    QDir terrainDir(dir);
    //  *.jpg 和 *.png
    QStringList terrainFiles = terrainDir.entryList(QStringList() << "*.jpg" << "*.png", QDir::Files);
    if (
        terrainFiles.indexOf("heightmap.png") == -1 ||
        terrainFiles.indexOf("texture.jpg") == -1
        ) {
        QMessageBox::warning(this, "Error", "The selected directory does not contain all the required files.");
        Logger::warning("Tempt to open invalid terrain folder");
        emit onSettingsChanged(QPair<QString, QString>("terrain", ""));
        return;
    }

    emit onSettingsChanged(QPair<QString, QString>("terrain", dir));
}
