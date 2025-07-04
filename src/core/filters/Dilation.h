// Dilation.h
#pragma once
#include "core/filters/Filter.h"

namespace core::filters {

    class Dilation : public Filter {
    public:
        QImage run(const QImage& input) const override;
        void buildUI(QWidget* panel, const std::function<void()>& refresh) override;
    private:
        int m_radius = 1;  // kernel radius
    };

} 
