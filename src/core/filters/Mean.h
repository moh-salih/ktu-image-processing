#pragma once
#include "core/filters/Filter.h"
#include <QSpinBox>
#include <QLabel>
#include <QVBoxLayout>

namespace core::filters {
    class Mean : public Filter {
    public:
        explicit Mean(int size = 3) : m_size(size) {}
        QImage run(const QImage& input) const override;
        
        void buildUI(QWidget* panel, const std::function<void()>& refresh) override;
    private:
        int m_size;
    };
} 