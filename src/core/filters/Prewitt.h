#pragma once
#include "core/filters/Filter.h"

namespace core::filters {

    class Prewitt : public Filter{
    public:
        explicit Prewitt(bool useHypot = false) : m_useHypot(useHypot) {}
        QImage run(const QImage& src) const override;
        void buildUI(QWidget* panel, const std::function<void()>& refresh) override;
    private:
        bool m_useHypot;   
    };

} 
