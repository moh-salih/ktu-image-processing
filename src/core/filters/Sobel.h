#pragma once
#include "Filter.h"

namespace core::filters {

    class Sobel : public Filter{
    public:
        explicit Sobel(bool useHypot = false) : m_useHypot(useHypot) {}
        QImage run(const QImage& src) const override;
        void buildUI(QWidget* panel, const std::function<void()>& refresh) override;
    private:
        bool m_useHypot;   
    };

} 
