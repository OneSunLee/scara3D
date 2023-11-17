#include <qpainterpath.h>
#include <qregularexpression.h>

#include "roundedcornerwidget.h"

RoundedCornerWidget::RoundedCornerWidget(QWidget* parent) : QWidget(parent) {
    // 创建主布局和主窗体
    _mainLayout = new QVBoxLayout(this);
    _mainLayout->setContentsMargins(0, 1, 1, 1);
    _mainLayout->setSpacing(0);
    _mainWidget = new QWidget(this);
    _mainWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _mainLayout->addWidget(_mainWidget);
    _mainWidget->show();
    setLayout(_mainLayout);
}

RoundedCornerWidget::~RoundedCornerWidget() {
}

void RoundedCornerWidget::showEvent(QShowEvent* event) {
    // 显示
    QWidget::showEvent(event);

    if (_initialized) {
        return;
    }

    // 初始化UI
    initializeWidgetUI();

    // 初始化的FLAG
    _initialized = true;
}

void RoundedCornerWidget::initializeWidgetUI() {
    // 创建border并显示在给定位置
    _borderWidget = new QWidget(this);
    _borderWidget->setObjectName("borderWidget");
    QString borderWidgetStyleSheet =
        "QWidget#borderWidget{background-color:#00FFFFFF;border:2px solid " + _borderColor.name(QColor::HexArgb) + ";"
        "border-radius:" + QString::number(_cornerRadius) + "px;}";
    _borderWidget->setStyleSheet(borderWidgetStyleSheet);
    _borderWidget->setAttribute(Qt::WA_TransparentForMouseEvents);
    _borderWidget->move(0, 0);
    _borderWidget->resize(width(), height());
    _borderWidget->show();

    // 创建圆角的掩码
    QPainterPath path;
    path.addRoundedRect(_mainWidget->rect(), _cornerRadius, _cornerRadius);
    QRegion region(path.toFillPolygon().toPolygon());
    _mainWidget->setMask(region);
}

void RoundedCornerWidget::resizeEvent(QResizeEvent* event) {
    // 自适应边界
    if (_borderWidget != nullptr) {
        _borderWidget->move(0, 0);
        _borderWidget->resize(width(), height());
    }
    
    // Resize window mask
    if (_mainWidget != nullptr) {
        QPainterPath path;
        path.addRoundedRect(_mainWidget->rect(), _cornerRadius, _cornerRadius);
        QRegion region(path.toFillPolygon().toPolygon());
        _mainWidget->setMask(region);
    }
}

void RoundedCornerWidget::setCornerRadius(int radius) {
    // 圆角半径
    _cornerRadius = radius;

    // 修改圆角半径
    if (_borderWidget != nullptr) {
        QString styleSheet = _borderWidget->styleSheet();
        styleSheet.replace(QRegularExpression("border-radius:\\d+px;"),//正则\d+数字
            QString("border-radius:%1px;").arg(_cornerRadius));
        _borderWidget->setStyleSheet(styleSheet);
    }
    
    // 重置主窗体的掩码
    QPainterPath path;
    path.addRoundedRect(_mainWidget->rect(), _cornerRadius, _cornerRadius);
    QRegion region(path.toFillPolygon().toPolygon());
    _mainWidget->setMask(region);
}

void RoundedCornerWidget::setBorderColor(QColor color) {
    // 设置颜色
    _borderColor = color;

    // 修改边界的当前样式页
    if (_borderWidget != nullptr) {
        QString styleSheet = _borderWidget->styleSheet();
        styleSheet.replace(QRegularExpression("border:2px solid #\\w+;"),
            QString("border:2px solid %1;").arg(_borderColor.name(QColor::HexArgb)));
        _borderWidget->setStyleSheet(styleSheet);
    }
}

QWidget* RoundedCornerWidget::mainWidget() const {
    return _mainWidget;
}
