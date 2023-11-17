#pragma once

#include <qwidget.h>
#include <qboxlayout.h>
#include <qlabel.h>

#include "slider.h"

class ModelAttributeSlide : public QWidget
{
    Q_OBJECT

public:
    ModelAttributeSlide(const QString& label, float min, float max, int step, QWidget* parent = 0);
    ~ModelAttributeSlide();

private:
    QHBoxLayout* _stretchLayout = nullptr;
    QLabel* _label = nullptr;
    QLabel* _val = nullptr;
    Slider* _slider = nullptr;

public:
    // Get
    float val() const { return _slider->val(); }
    float lev() const { return _slider->lev(); }
    
    // Set
    void setLabel(const QString& label);
    void setMin(float min);
    void setMax(float max);
    void setStep(float max);
    void setValue(float val);   // 实际值
    void setTransformation(std::function<float(float)> transform, std::function<float(float)> inverse);

    void setEnabled(bool enable = true);
    
signals:
    void onChanged(float val);
    void onChangeStart();
    void onChangeEnd();
};
