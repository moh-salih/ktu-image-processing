#pragma once
#include "core/filters/Filter.h"
#include "core/QMat.h"

#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <QGroupBox>

namespace core::filters {

    class Sharpen : public Filter{
    public:
        explicit Sharpen(int strength = 5) : m_strength(strength) {}
        QImage run(const QImage& inImg) const override;
        void buildUI(QWidget* panel, const std::function<void()>& refresh) override;
    private:
        int m_strength = 5;
    };

} 
