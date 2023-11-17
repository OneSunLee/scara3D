#include <qlayoutitem.h>

#include "aboutpage.h"

AboutPage::AboutPage(QWidget* parent) : 
    PageWidget(parent)
{
    _contentWidget->setMouseTracking(true);

    // 构造标题布局
    _titleLayout = new QVBoxLayout(_contentWidget);
    _titleLayout->setContentsMargins(28, 46, 28, 28);
    _titleLayout->setSpacing(18);
    _titleLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    _contentWidget->setLayout(_titleLayout);

    // 构造标题
    _titleLabel = new QLabel("ABOUT", _contentWidget);
    _titleLabel->setFont(_titleFont);
    _titleLayout->addWidget(_titleLabel);
    _titleLabel->show();

    // 构造主界面布局
    _mainWidget = new QWidget(_contentWidget);
    _mainWidget->setObjectName("mainWidget");
    _mainWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    _mainWidget->setStyleSheet("QWidget#mainWidget { background-color: #efefef; border-radius: 8px; }");
    _mainLayout = new QVBoxLayout(_mainWidget);
    _mainLayout->setAlignment(Qt::AlignTop);
    _mainLayout->setContentsMargins(0, 8, 0, 8);
    _mainLayout->setSpacing(8);
    _mainWidget->setLayout(_mainLayout);
    _titleLayout->addWidget(_mainWidget);
    _mainWidget->show();
    
    // 内部的各个label信息
    _authorLabel = new LineEditWidget(_mainWidget);
    _authorLabel->mainLayout()->insertWidget(0, new QLabel("Authored by"));
    _authorLabel->mainLayout()->insertSpacing(1, 8);
    _authorLabel->setBackgroundColor(QColor(0, 0, 0, 0));
    _authorLabel->setHoverColor(QColor(0, 0, 0, 10));
    _authorLabel->setPressedColor(QColor(0, 0, 0, 20));
    _authorLabel->setText("MA Jie");
    _authorLabel->setEnabled(false);
    _mainLayout->addWidget(_authorLabel);
    _authorLabel->show();

    _versionLabel = new LineEditWidget(_mainWidget);
    _versionLabel->mainLayout()->insertWidget(0, new QLabel("Version"));
    _versionLabel->mainLayout()->insertSpacing(1, 8);
    _versionLabel->setBackgroundColor(QColor(0, 0, 0, 0));
    _versionLabel->setHoverColor(QColor(0, 0, 0, 10));
    _versionLabel->setPressedColor(QColor(0, 0, 0, 20));
    _versionLabel->setText("v1.0.0 release");
    _versionLabel->setEnabled(false);
    _mainLayout->addWidget(_versionLabel);
    _versionLabel->show();

    _dateLabel = new LineEditWidget(_mainWidget);
    _dateLabel->mainLayout()->insertWidget(0, new QLabel("Last Update"));
    _dateLabel->mainLayout()->insertSpacing(1, 8);
    _dateLabel->setBackgroundColor(QColor(0, 0, 0, 0));
    _dateLabel->setHoverColor(QColor(0, 0, 0, 10));
    _dateLabel->setPressedColor(QColor(0, 0, 0, 20));
    _dateLabel->setText("2023-11-14");
    _dateLabel->setEnabled(false);
    _mainLayout->addWidget(_dateLabel);
    _dateLabel->show();

    //行间断
    QWidget* _line = new QWidget(_mainWidget);
    _line->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    _line->setFixedHeight(1);
    _line->setStyleSheet("background-color: #c2c2c2;");
    _mainLayout->addWidget(_line);
    _line->show();
    
    _skyTerDev = new LineEditWidget(_mainWidget);
    _skyTerDev->mainLayout()->insertWidget(0, new QLabel("Sky & Terrain developer"));
    _skyTerDev->mainLayout()->insertSpacing(1, 8);
    _skyTerDev->setBackgroundColor(QColor(0, 0, 0, 0));
    _skyTerDev->setHoverColor(QColor(0, 0, 0, 10));
    _skyTerDev->setPressedColor(QColor(0, 0, 0, 20));
    _skyTerDev->setText("MA Jie @ mm34nn");
    _skyTerDev->setEnabled(false);
    _mainLayout->addWidget(_skyTerDev);
    _skyTerDev->show();
    
    _lightingDev = new LineEditWidget(_mainWidget);
    _lightingDev->mainLayout()->insertWidget(0, new QLabel("Lighting developer"));
    _lightingDev->mainLayout()->insertSpacing(1, 8);
    _lightingDev->setBackgroundColor(QColor(0, 0, 0, 0));
    _lightingDev->setHoverColor(QColor(0, 0, 0, 10));
    _lightingDev->setPressedColor(QColor(0, 0, 0, 20));
    _lightingDev->setText("MA Jie @ mm34nn");
    _lightingDev->setEnabled(false);
    _mainLayout->addWidget(_lightingDev);
    _lightingDev->show();
    
    _coreDev = new LineEditWidget(_mainWidget);
    _coreDev->mainLayout()->insertWidget(0, new QLabel("Core developer"));
    _coreDev->mainLayout()->insertSpacing(1, 8);
    _coreDev->setBackgroundColor(QColor(0, 0, 0, 0));
    _coreDev->setHoverColor(QColor(0, 0, 0, 10));
    _coreDev->setPressedColor(QColor(0, 0, 0, 20));
    _coreDev->setText("MA Jie @ mm34nn");
    _coreDev->setEnabled(false);
    _mainLayout->addWidget(_coreDev);
    _coreDev->show();
    
    _uiDesigner = new LineEditWidget(_mainWidget);
    _uiDesigner->mainLayout()->insertWidget(0, new QLabel("UI designed by"));
    _uiDesigner->mainLayout()->insertSpacing(1, 8);
    _uiDesigner->setBackgroundColor(QColor(0, 0, 0, 0));
    _uiDesigner->setHoverColor(QColor(0, 0, 0, 10));
    _uiDesigner->setPressedColor(QColor(0, 0, 0, 20));
    _uiDesigner->setText("MA Jie @ mm34nn");
    _uiDesigner->setEnabled(false);
    _mainLayout->addWidget(_uiDesigner);
    _uiDesigner->show();
    
    //行间断
    _line = new QWidget(_mainWidget);
    _line->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    _line->setFixedHeight(1);
    _line->setStyleSheet("background-color: #c2c2c2;");
    _mainLayout->addWidget(_line);
    _line->show();
    
    _license = new LineEditWidget(_mainWidget);
    _license->mainLayout()->insertWidget(0, new QLabel("License"));
    _license->mainLayout()->insertSpacing(1, 8);
    _license->setBackgroundColor(QColor(0, 0, 0, 0));
    _license->setHoverColor(QColor(0, 0, 0, 10));
    _license->setPressedColor(QColor(0, 0, 0, 20));
    _license->setText("GPLv3 (According to Qt6 Documents)");
    _license->setEnabled(false);
    _mainLayout->addWidget(_license);
    _license->show();
    
    _qtVersion = new LineEditWidget(_mainWidget);
    _qtVersion->mainLayout()->insertWidget(0, new QLabel("Qt Version"));
    _qtVersion->mainLayout()->insertSpacing(1, 8);
    _qtVersion->setBackgroundColor(QColor(0, 0, 0, 0));
    _qtVersion->setHoverColor(QColor(0, 0, 0, 10));
    _qtVersion->setPressedColor(QColor(0, 0, 0, 20));
    _qtVersion->setText("Qt 6.4.0");
    _qtVersion->setEnabled(false);
    _mainLayout->addWidget(_qtVersion);
    _qtVersion->show();
    
    _usedLibraries = new LineEditWidget(_mainWidget);
    _usedLibraries->mainLayout()->insertWidget(0, new QLabel("Special thanks to"));
    _usedLibraries->mainLayout()->insertSpacing(1, 8);
    _usedLibraries->setBackgroundColor(QColor(0, 0, 0, 0));
    _usedLibraries->setHoverColor(QColor(0, 0, 0, 10));
    _usedLibraries->setPressedColor(QColor(0, 0, 0, 20));
    _usedLibraries->setText("Assimp, GLM, stb_image for providing marvelous opensource libraries");
    _usedLibraries->setEnabled(false);
    _mainLayout->addWidget(_usedLibraries);
    _usedLibraries->show();
    
    _githubLink = new LineEditWidget(_mainWidget);
    _githubLink->mainLayout()->insertWidget(0, new QLabel("Opensource Repo"));
    _githubLink->mainLayout()->insertSpacing(1, 8);
    _githubLink->setBackgroundColor(QColor(0, 0, 0, 0));
    _githubLink->setHoverColor(QColor(0, 0, 0, 10));
    _githubLink->setPressedColor(QColor(0, 0, 0, 20));
    _githubLink->setText("github/OneSunLee I do not share the codes yet, please wait.");
    _githubLink->setEnabled(false);
    _mainLayout->addWidget(_githubLink);
    _githubLink->show();
}

AboutPage::~AboutPage() {}

PushButton* AboutPage::getPageIconButton(QWidget* context) {
    // 检查变量
    if (_iconButton != nullptr) {
        return _iconButton;
    }

    // 生成新图标
    _iconButton = new PushButton(nullptr, context);
    _iconButton->setMargin(20, 18, 16, 18);
    _iconButtonLabel = new QLabel(_iconButton);
    _iconButtonLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    _iconButtonLabel->setText("\uf059");
    _iconButtonLabel->setAlignment(Qt::AlignLeft);
    _iconButton->setChildWidget(_iconButtonLabel);

    // 返回我们的button
    return _iconButton;
}

PushButton* AboutPage::getPageTextButton(QWidget* context) {
    // 检查变量
    if (_textButton != nullptr) {
        return _textButton;
    }

    // 创建新按钮
    _textButton = new PushButton(nullptr, context);
    _textButton->setMargin(20, 18, 16, 18);
    _textButtonWidget = new QWidget(_textButton);
    _textButtonLayout = new QHBoxLayout(_textButtonWidget);
    _textButtonLayout->setContentsMargins(0, 0, 0, 0);
    _textButtonLayout->setSpacing(12);
    _textButtonWidget->setLayout(_textButtonLayout);

    // 创建新按钮
    _textButtonIcon = new QLabel(_textButtonWidget);
    _textButtonIcon->setText("\uf059");    // set icon to "pen" icon
    _textButtonIcon->setAlignment(Qt::AlignLeft);

    _textButtonLabel = new QLabel(_textButtonWidget);
    _textButtonLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    _textButtonLabel->setText("About");
    _textButtonLabel->setAlignment(Qt::AlignLeft);

    // 添加按钮到界面上
    _textButtonLayout->addWidget(_textButtonIcon);
    _textButtonLayout->addWidget(_textButtonLabel);
    _textButtonIcon->show();
    _textButtonLabel->show();

    // 把我们new的按钮挂在Qt对象树上
    _textButton->setChildWidget(_textButtonWidget);
    _textButtonWidget->show();

    // 返回按钮
    return _textButton;
}
