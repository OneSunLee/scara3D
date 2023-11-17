#include <qpropertyanimation.h>
#include <qparallelanimationgroup.h>

#include "lineeditwidget.h"

LineEditWidget::LineEditWidget(QWidget* parent) :
    QWidget(parent)
{
    // 生成颜色
    generateColor(_defaultColorScheme);

    // 初始化ui
    initializeUI();

    // 编辑textedit框的信号槽
    connect(_editor, &QLineEdit::returnPressed, this, &LineEditWidget::endEdit);
    connect(_editor, &QLineEdit::editingFinished, this, &LineEditWidget::endEdit);
    connect(_editor, &QLineEdit::textChanged, this, &LineEditWidget::onTextChanged);
}

LineEditWidget::~LineEditWidget() {
}

void LineEditWidget::initializeUI() {
    // 创建和设置主布局
    _mainLayout = new QHBoxLayout(this);
    _mainLayout->setContentsMargins(12, 0, 12, 0);
    _mainLayout->setSpacing(8);
    setLayout(_mainLayout);

    // 创建编辑器的容器
    _editorWidget = new QWidget(this);
    _mainLayout->addWidget(_editorWidget);
    _editorWidget->show();

    // 创建编辑器的布局去拉伸编辑器窗体
    _editorWidgetLayout = new QHBoxLayout(_editorWidget);
    _editorWidgetLayout->setContentsMargins(0, 10, 0, 10);
    _editorWidgetLayout->setSpacing(0);
    _editorWidget->setLayout(_editorWidgetLayout);

    // 创建事件过滤器以进行自适应indicator
    _editorWidget->installEventFilter(this);

    // 创建文本编辑框
    _editor = new QLineEdit(_editorWidget);
    _editor->setText("");
    _editor->setFont(_defaultFont);
    _editor->setAlignment(Qt::AlignRight);
    _editor->setReadOnly(true);
    _editor->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    _editor->setStyleSheet("QLineEdit{color:#2c2c2c;background-color:#00ffffff;border-style:none;}");
    //_editor->setFixedHeight(_editor->fontMetrics().lineSpacing());  // Restrict one line
    _editor->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
    _editorWidgetLayout->addWidget(_editor);
    _editor->show();

    // 创建 indicator 边框体
    _indicator = new QWidget(_editorWidget);
    _indicator->setObjectName("indicator");
    _indicator->setStyleSheet("QWidget#indicator{background-color:" + _indicatorColor.name(QColor::HexArgb) + ";"
        "border-radius:" + QString::number((float)_indicatorWidth / 2) + "px;}");
    _indicator->resize(_indicatorWidth, _indicatorWidth);
    _indicator->move(_editorWidget->width() - _indicatorWidth, _editorWidget->height() - _indicatorWidth - _indicatorSpacing);
    _indicatorEffect = new QGraphicsOpacityEffect(_indicator);
    _indicatorEffect->setOpacity(0);
    _indicator->setGraphicsEffect(_indicatorEffect);
    _indicator->show();

    // 创建背景色窗体
    _backgroundWidget = new QWidget(this);
    _backgroundWidget->resize(size());
    _backgroundWidget->setObjectName("backgroundWidget");
    _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _backgroundColor.name(QColor::HexArgb) + ";"
        "border-radius:" + QString::number(_cornerRadius) + "px;}");
    _backgroundWidget->lower();
    _backgroundWidget->show();

    // 设置尺寸调整的策略
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

void LineEditWidget::generateColor(QColor colorScheme) {
    _backgroundColor = colorScheme.lighter(120);
    _backgroundColor.setAlpha(5);
    _hoverColor = colorScheme.lighter(120);
    _hoverColor.setAlpha(40);
    _pressedColor = colorScheme.lighter(120);
    _pressedColor.setAlpha(50);
    _indicatorColor = colorScheme;
}

void LineEditWidget::startEdit() {
    if (_editing) {
        return;
    }

    // 设置编辑符号位
    _editing = true;
    
    // 启用QLineedit
    _editor->setReadOnly(false);
    _editor->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    _editor->setFocus();
    _editor->setCursorPosition(_editor->text().length());

    // 移动光标，更新光标符号
    QCursor::setPos(QCursor::pos() + QPoint(1, 0));
    QCursor::setPos(QCursor::pos() + QPoint(-1, 0));

    // 增加发光和淡出动画给indicator
    QParallelAnimationGroup* startEditAnimation = new QParallelAnimationGroup(this);
    QPropertyAnimation* growAnimation = new QPropertyAnimation(_indicator, "geometry");
    QPropertyAnimation* fadeInAnimation = new QPropertyAnimation(_indicatorEffect, "opacity");
    growAnimation->setDuration(200);
    growAnimation->setEasingCurve(QEasingCurve::OutQuad);
    fadeInAnimation->setDuration(200);
    fadeInAnimation->setEasingCurve(QEasingCurve::OutQuad);
    growAnimation->setStartValue(_indicator->geometry());
    growAnimation->setEndValue(QRect(
        0,
        _editorWidget->height() - _indicatorWidth - _indicatorSpacing,
        _editorWidget->width(),
        _indicatorWidth
    ));
    fadeInAnimation->setStartValue(_indicatorEffect->opacity());
    fadeInAnimation->setEndValue(0.999);
    startEditAnimation->addAnimation(growAnimation);
    startEditAnimation->addAnimation(fadeInAnimation);
    startEditAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    
    // 编辑信号
    emit onStartEditing(_editor->text());
}

void LineEditWidget::endEdit() {
    if (!_editing) {
        return;
    }

    // 设置编辑位
    _editing = false;
    
    // 禁用编辑框
    _editor->setReadOnly(true);
    _editor->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    _editor->setSelection(0, 0);

    // 增加indicator的缩小和淡出动画
    QParallelAnimationGroup* endEditAnimation = new QParallelAnimationGroup(this);
    QPropertyAnimation* shrinkAnimation = new QPropertyAnimation(_indicator, "geometry");
    QPropertyAnimation* fadeOutAnimation = new QPropertyAnimation(_indicatorEffect, "opacity");
    shrinkAnimation->setDuration(200);
    shrinkAnimation->setEasingCurve(QEasingCurve::OutQuad);
    fadeOutAnimation->setDuration(200);
    fadeOutAnimation->setEasingCurve(QEasingCurve::OutQuad);
    shrinkAnimation->setStartValue(_indicator->geometry());
    shrinkAnimation->setEndValue(QRect(
        _editorWidget->width() - _indicatorWidth,
        _editorWidget->height() - _indicatorWidth - _indicatorSpacing,
        _indicatorWidth,
        _indicatorWidth
    ));
    fadeOutAnimation->setStartValue(_indicatorEffect->opacity());
    fadeOutAnimation->setEndValue(0);
    endEditAnimation->addAnimation(shrinkAnimation);
    endEditAnimation->addAnimation(fadeOutAnimation);
    endEditAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    
    // 发出信号
    emit onEndEditing(_editor->text());
}

void LineEditWidget::showEvent(QShowEvent* event) {
    // 显示窗体
    QWidget::showEvent(event);
    
    // 初始化状态
    if (_initialized) {
        return;
    }
    
    // 尺寸依赖窗口
    _backgroundWidget->resize(size());
    if (_editing) {
        _indicator->move(0, _editorWidget->height() - _indicatorWidth - _indicatorSpacing);
        _indicator->resize(_editorWidget->width(), _indicatorWidth);
    }
    else {
        _indicator->move(_editorWidget->width() - _indicatorWidth, _editorWidget->height() - _indicatorWidth - _indicatorSpacing);
        _indicator->resize(_indicatorWidth, _indicatorWidth);
    }
    
    // 设置标志位
    _initialized = true;
}

void LineEditWidget::enterEvent(QEnterEvent* event) {
    // 检查变量
    if (_enabled) {
        setCursor(Qt::PointingHandCursor);
    }
    
    // 改变背景颜色
    _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _hoverColor.name(QColor::HexArgb) + ";"
        "border-radius:" + QString::number(_cornerRadius) + "px;}");

    // 设置标志位
    _hovered = true;
}

void LineEditWidget::leaveEvent(QEvent* event) {
    setCursor(Qt::ArrowCursor);

    //改变背景色
    _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _backgroundColor.name(QColor::HexArgb) + ";"
        "border-radius:" + QString::number(_cornerRadius) + "px;}");

    // 设置标志位
    _hovered = false;
    _pressed = false;
}

void LineEditWidget::mousePressEvent(QMouseEvent* event) {
    // 改变背景色
    _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _pressedColor.name(QColor::HexArgb) + ";"
        "border-radius:" + QString::number(_cornerRadius) + "px;}");

    // 设置标志位
    _pressed = true;
}

void LineEditWidget::mouseReleaseEvent(QMouseEvent* event) {
    // 改变背景颜色
    _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _hoverColor.name(QColor::HexArgb) + ";"
        "border-radius:" + QString::number(_cornerRadius) + "px;}");

    // 触发点击
    if (_pressed) {
        if (_enabled) {
            if (_editing) {
                endEdit();
            }
            else {
                startEdit();
            }
        }
        else {
            if (_editing) {
                endEdit();
            }
        }
    }

    // 设置压下去的标志位
    _pressed = false;
}

void LineEditWidget::focusInEvent(QFocusEvent* event) {
    // 调用焦点进入函数
    QWidget::focusInEvent(event);

    // 检查状态
    if (!_enabled) {
        return;
    }

    // 检查背景色
    _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _hoverColor.name(QColor::HexArgb) + ";"
        "border-radius:" + QString::number(_cornerRadius) + "px;}");
    
    // 编辑
    if (!_editing) {
        startEdit();
    }
}

void LineEditWidget::focusOutEvent(QFocusEvent* event) {
    // 焦点离开事件
    QWidget::focusOutEvent(event);

    // 改变背景颜色
    _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _backgroundColor.name(QColor::HexArgb) + ";"
        "border-radius:" + QString::number(_cornerRadius) + "px;}");

    // 结束编辑
    if (_editing) {
        endEdit();
    }
}

void LineEditWidget::resizeEvent(QResizeEvent* event) {
    // 检查初始状态
    if (!_initialized) {
        return;
    }

    // 自适应背景框
    _backgroundWidget->resize(size());
}

bool LineEditWidget::eventFilter(QObject* object, QEvent* event) {
    // 自适应indicator边框
    if (object == _editorWidget) {
        if (event->type() == QEvent::Resize) {
            if (_editing) {
                _indicator->move(0, _editorWidget->height() - _indicatorWidth - _indicatorSpacing);
                _indicator->resize(_editorWidget->width(), _indicatorWidth);
            }
            else {
                _indicator->move(_editorWidget->width() - _indicatorWidth, _editorWidget->height() - _indicatorWidth - _indicatorSpacing);
                _indicator->resize(_indicatorWidth, _indicatorWidth);
            }
        }
    }

    // 事件过滤器
    return QWidget::eventFilter(object, event);
}

void LineEditWidget::setText(const QString& text) {
    // 设置文本
    _editor->setText(text);
}

void LineEditWidget::setPlaceholderText(const QString& text) {
    // 设置预设字符
    _editor->setPlaceholderText(text);
}

void LineEditWidget::setValidator(const QValidator* validator) {
    // 设置验证器
    _editor->setValidator(validator);
}

void LineEditWidget::setEnabled(bool enabled) {
    // 修改状态
    _enabled = enabled;

    // 检查状态
    if (_editing && !_enabled) {
        endEdit();
    }
}

void LineEditWidget::setMargins(QMargins margins) {
    // 设置边条
    _mainLayout->setContentsMargins(margins);
}

void LineEditWidget::setMargins(int left, int top, int right, int bottom) {
    // 设置边条
    _mainLayout->setContentsMargins(left, top, right, bottom);
}

void LineEditWidget::setBackgroundColor(QColor color) {
    // 设置背景色
    _backgroundColor = color;

    // 检查状态
    if (!_hovered && !_pressed) {
        _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _backgroundColor.name(QColor::HexArgb) + ";"
            "border-radius:" + QString::number(_cornerRadius) + "px;}");
    }
}

void LineEditWidget::setHoverColor(QColor color) {
    // 设置鼠标移上去的颜色
    _hoverColor = color;

    // 检查状态
    if (_hovered && !_pressed) {
        _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _hoverColor.name(QColor::HexArgb) + ";"
            "border-radius:" + QString::number(_cornerRadius) + "px;}");
    }
}

void LineEditWidget::setPressedColor(QColor color) {
    // 点下去的颜色
    _pressedColor = color;

    // 检查状态
    if (_pressed) {
        _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _pressedColor.name(QColor::HexArgb) + ";"
            "border-radius:" + QString::number(_cornerRadius) + "px;}");
    }
}

void LineEditWidget::setIndicatorColor(QColor color) {
    // 设置indicator边框颜色
    _indicatorColor = color;
    
    _indicator->setStyleSheet("QWidget#indicator{background-color:" + _indicatorColor.name(QColor::HexArgb) + ";"
        "border-radius:" + QString::number(_indicatorWidth) + "px;}");
}

void LineEditWidget::setColorScheme(QColor primaryColor) {
    // 生成颜色
    generateColor(primaryColor);

    // 检查当前状态
    if (!_hovered && !_pressed) {
        _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _backgroundColor.name(QColor::HexArgb) + ";"
            "border-radius:" + QString::number(_cornerRadius) + "px;}");
    }
    else if (_hovered && !_pressed) {
        _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _hoverColor.name(QColor::HexArgb) + ";"
            "border-radius:" + QString::number(_cornerRadius) + "px;}");
    }
    else if (_pressed) {
        _backgroundWidget->setStyleSheet("QWidget#backgroundWidget{background-color:" + _pressedColor.name(QColor::HexArgb) + ";"
            "border-radius:" + QString::number(_cornerRadius) + "px;}");
    }

    _indicator->setStyleSheet("QWidget#indicator{background-color:" + _indicatorColor.name(QColor::HexArgb) + ";"
        "border-radius:" + QString::number(_indicatorWidth) + "px;}");
}

QString LineEditWidget::text() const {
    // 返回文本
    return _editor->text();
}

QHBoxLayout* LineEditWidget::mainLayout() const {
    // 返回主布局
    return _mainLayout;
}
