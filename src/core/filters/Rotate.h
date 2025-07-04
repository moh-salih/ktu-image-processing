#pragma once
#include "core/filters/Filter.h"
#include "core/QMat.h"

#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>

#include <QtMath>

namespace core::filters {

    class Rotate : public Filter {
    public:
        Rotate(int angleDeg = 90): m_angle(angleDeg) {}

        QImage run(const QImage& img) const override;
        void buildUI(QWidget* panel, const std::function<void()>& refresh) override;

    private:
        int m_angle;
    };

} 
