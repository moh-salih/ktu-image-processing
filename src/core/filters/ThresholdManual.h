#pragma once
#include "core/filters/Filter.h"
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <QGroupBox>

namespace core::filters {

    class ThresholdManual : public Filter {
    public:
        ThresholdManual() = default;
        QImage run(const QImage& img) const override;
        void buildUI(QWidget* panel, const std::function<void()>& refresh) override;
    private:
        int m_threshold = 127;
    };

} 
