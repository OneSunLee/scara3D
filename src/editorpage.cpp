#include <qfontdatabase.h>

#include "editorpage.h"

EditorPage::EditorPage(QWidget* parent) :
    PageWidget(parent)
{
    _contentWidget->setMouseTracking(true);

    //创建 标题
    _titleLayout = new QVBoxLayout(_contentWidget);
    _titleLayout->setContentsMargins(28, 46, 28, 28);
    _titleLayout->setSpacing(18);
    _titleLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    _contentWidget->setLayout(_titleLayout);
    
    // 创建文本
    _titleLabel = new QLabel("EDITOR", _contentWidget);
    _titleLabel->setFont(_titleFont);
    _titleLayout->addWidget(_titleLabel);
    _titleLabel->show();
    
    // 主布局
    _mainWidget = new QWidget(_contentWidget);
    _mainWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _mainLayout = new QHBoxLayout(_mainWidget);
    _mainLayout->setContentsMargins(0, 0, 0, 0);
    _mainLayout->setSpacing(16);
    _mainWidget->setLayout(_mainLayout);
    _titleLayout->addWidget(_mainWidget);
    _mainWidget->show();

    //创建modelslector
    _modelSelector = new ModelSelector(_mainWidget);
    _mainLayout->addWidget(_modelSelector);
    _modelSelector->show();

    // 创建editing layout
    _editingLayout = new QVBoxLayout(_mainWidget);
    _editingLayout->setContentsMargins(0, 0, 0, 0);
    _editingLayout->setSpacing(16);
    _mainLayout->addLayout(_editingLayout);
        
    // 创建scene viewer
    _sceneViewerContainer = new RoundedCornerWidget(_mainWidget);
    _sceneViewerContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _sceneViewerContainerLayout = new QVBoxLayout(_sceneViewerContainer->mainWidget());
    _sceneViewerContainerLayout->setContentsMargins(0, 0, 0, 0);
    _sceneViewerContainerLayout->setSpacing(0);
    _sceneViewerContainer->mainWidget()->setLayout(_sceneViewerContainerLayout);
    _sceneViewer = new SceneViewer(_sceneViewerContainer->mainWidget());
    _sceneViewerContainerLayout->addWidget(_sceneViewer);
    _sceneViewer->show();
    _editingLayout->addWidget(_sceneViewerContainer);
    _sceneViewerContainer->show();

    // 创建model setter
    _modelSetter = new ModelSetter(_mainWidget);
    _modelSetter->setMaximumHeight(150);
    _editingLayout->addWidget(_modelSetter);
    _modelSetter->show();
    _modelSetter->setObjectName("ModelSetter");
    _modelSetter->setStyleSheet("#ModelSetter { background-color: #f0f0f0; border-radius: 10px; }");

    // 信号槽
    connect(_modelSelector, &ModelSelector::onObjectSelected, _sceneViewer, &SceneViewer::addObject);
    connect(_sceneViewer, &SceneViewer::onSelect, _modelSetter, &ModelSetter::update);
    connect(_sceneViewer, &SceneViewer::onUpdate, _modelSetter, &ModelSetter::update);
    connect(_modelSetter, &ModelSetter::onAdjustStart, _sceneViewer, &SceneViewer::setDragFlag);
    connect(_modelSetter, &ModelSetter::onAdjustEnd, _sceneViewer, &SceneViewer::clearDragFlag);
    connect(_modelSetter, &ModelSetter::onAdjust, this, [=]() {
        _sceneViewer->update();
    });
    connect(_modelSetter, &ModelSetter::onDeleteObject, _sceneViewer, &SceneViewer::deleteObject);
}

EditorPage::~EditorPage() {}

void EditorPage::updateSetting(QPair<QString, QString> setting) {
    if (_sceneViewer != nullptr) {
        _sceneViewer->updateSetting(setting);
    }
}

PushButton* EditorPage::getPageIconButton(QWidget* context) {
    // 变量检查
    if (_iconButton != nullptr) {
        return _iconButton;
    }
    
    // 创建icon
    _iconButton = new PushButton(nullptr, context);
    _iconButton->setMargin(20, 18, 16, 18);
    _iconButtonLabel = new QLabel(_iconButton);
    _iconButtonLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    _iconButtonLabel->setText("\uf304");    // set icon to "pen" icon
    _iconButtonLabel->setAlignment(Qt::AlignLeft);
    _iconButton->setChildWidget(_iconButtonLabel);
    
    // 返回
    return _iconButton;
}

PushButton* EditorPage::getPageTextButton(QWidget* context) {
    // 变量检查
    if (_textButton != nullptr) {
        return _textButton;
    }

    // 创建按钮
    _textButton = new PushButton(nullptr, context);
    _textButton->setMargin(20, 18, 16, 18);
    _textButtonWidget = new QWidget(_textButton);
    _textButtonLayout = new QHBoxLayout(_textButtonWidget);
    _textButtonLayout->setContentsMargins(0, 0, 0, 0);
    _textButtonLayout->setSpacing(12);
    _textButtonWidget->setLayout(_textButtonLayout);

    // 创建按钮
    _textButtonIcon = new QLabel(_textButtonWidget);
    _textButtonIcon->setText("\uf304");    // set icon to "pen" icon
    _textButtonIcon->setAlignment(Qt::AlignLeft);
    
    _textButtonLabel = new QLabel(_textButtonWidget);
    _textButtonLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    _textButtonLabel->setText("Editor");
    _textButtonLabel->setAlignment(Qt::AlignLeft);
    
    //添加到按钮文本布局
    _textButtonLayout->addWidget(_textButtonIcon);
    _textButtonLayout->addWidget(_textButtonLabel);
    _textButtonIcon->show();
    _textButtonLabel->show();

    // 挂对象树
    _textButton->setChildWidget(_textButtonWidget);
    _textButtonWidget->show();

    // 返回
    return _textButton;
}
