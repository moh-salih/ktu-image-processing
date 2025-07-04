#pragma once
#include "core/filters/Filter.h"

namespace core::filters {

    class Skeletonization : public Filter {
        Q_DECLARE_TR_FUNCTIONS(Skeletonization)
    public:
        QImage run(const QImage& input) const override;
        void buildUI(QWidget* panel, const std::function<void()>& /*refresh*/) override;
    };

} 