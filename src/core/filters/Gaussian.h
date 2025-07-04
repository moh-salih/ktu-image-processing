#pragma once
#include "core/filters/Filter.h"

namespace core::filters {

    class Gaussian : public Filter{
    public:
        Gaussian(int size = 3): m_size(size){}
        QImage run(const QImage& src) const override;
        void buildUI(QWidget* panel, const std::function<void()>& refresh) override;
    private:
        int m_size;
    };

}
