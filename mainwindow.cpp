#include "mainwindow.h"
#include "src/logger.h"

#define MAX_MOUSE_MOVEMENT 300

MainWindow::MainWindow(QWidget *parent)
    : FramelessWindow(20,parent)
{

    // 主布局
    _mainLayout = new QHBoxLayout(_windowWidget);
    _mainLayout->setContentsMargins(0, 0, 0, 0);
    _mainLayout->setSpacing(0);
    _windowWidget->setLayout(_mainLayout);

    // 创建placeHolder控件，中央界面预留一片空间
    _placeHolderWidget = new QWidget(_windowWidget);
    _placeHolderWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _placeHolderWidget->setMouseTracking(true);

    // 自适应窗口大小，使用事件过滤器
    _placeHolderWidget->installEventFilter(this);

    // 创建SideBar，用于page切换
    _sideBar = new SideBar(_windowWidget);
    connect(_sideBar, &SideBar::onPageChanged, this, [=](PageWidget* previousPage, PageWidget* currentPage) {
        // 输入检查
        if (previousPage == currentPage) {
            return;
        }
        if (previousPage != nullptr) {
            previousPage->offStage();
        }
        if (currentPage != nullptr) {
            currentPage->onStage();
        }
    });
    _sideBar->setMouseTracking(true);

    // 添加placeholder和siderbar到我们的界面上
    _mainLayout->addWidget(_sideBar);
    _mainLayout->addWidget(_placeHolderWidget);

//    // Editor界面添加到SideBar
//    _editorPage = new EditorPage(_placeHolderWidget);
//    _editorPage->setMouseTracking(true);
//    _sideBar->addPage(_editorPage);

//    //  Setting界面添加到SideBar
//    _settingPage = new SettingPage(_placeHolderWidget);
//    _settingPage->setMouseTracking(true);
//    _sideBar->addPage(_settingPage);

    // AboutPage 界面添加到SideBar
    _aboutPage = new AboutPage(_placeHolderWidget);
    _aboutPage->setMouseTracking(true);
    _sideBar->addPage(_aboutPage);

//    connect(_settingPage, &SettingPage::onSettingsChanged, _editorPage, &EditorPage::updateSetting);
}

MainWindow::~MainWindow()
{
}

void MainWindow::resizePages(QResizeEvent* event) {
    // 输入检查
    if (event == nullptr) {
        return;
    }

    // 得到窗体的大小
    QSize size = event->size();

//    // pages的自适应 
//    _editorPage->resize(size);
//    _settingPage->resize(size);
    _aboutPage->resize(size);
}

void MainWindow::showEvent(QShowEvent* event) {
    // 显示
    FramelessWindow::showEvent(event);

//    //pages 自适应到预留空间的大小
//    _editorPage->resize(_placeHolderWidget->size());
//    _settingPage->resize(_placeHolderWidget->size());
    _aboutPage->resize(_placeHolderWidget->size());
}

bool MainWindow::eventFilter(QObject* object, QEvent* event) {
    // 输入检查
    if (object == nullptr || event == nullptr) {
        return false;
    }

    //判断事件来源是 _placeHolderWidget
    if (object == _placeHolderWidget) {
        // Check if the event is a resize event
        if (event->type() == QEvent::Resize) {
            // Resize all the pages
            resizePages(static_cast<QResizeEvent*>(event));
        }
    }

    // 事件筛选器
    return FramelessWindow::eventFilter(object, event);
}


