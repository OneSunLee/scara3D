#pragma once

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qevent.h>

#include "src/framelesswindow.h"
#include "src/sidebar.h"
//#include "src/editorpage.h"
#include "src/aboutpage.h"
//#include "src/settingpage.h"


#include <QMainWindow>

class MainWindow : public FramelessWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    // 主UI布局
    QHBoxLayout* _mainLayout = nullptr;

    // Side bar和pages
    SideBar* _sideBar = nullptr;
//    EditorPage* _editorPage = nullptr;
    AboutPage* _aboutPage = nullptr;
//    SettingPage* _settingPage = nullptr;

    // pages的预留空间，用于自适应窗体
    QWidget* _placeHolderWidget = nullptr;

private:
    void resizePages(QResizeEvent* event);

private:
    virtual void showEvent(QShowEvent* event) override;
    virtual bool eventFilter(QObject* object, QEvent* event) override;

};
#endif // MAINWINDOW_H
