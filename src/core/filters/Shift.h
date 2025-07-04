#pragma once
#include "core/filters/Filter.h"
#include "core/QMat.h"

#include <QSpinBox>
#include <QGridLayout>
#include <QLabel>

namespace core::filters {

    class Shift : public Filter {
    public:
        Shift(int dx = 10, int dy = 10): m_dx(dx), m_dy(dy) {}

        QImage run(const QImage& img) const override;
        void buildUI(QWidget* panel, const std::function<void()>& refresh) override;
    private:
        int m_dx, m_dy;
    };

} 
