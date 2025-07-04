// ── filters/Centroid.h ──────────────────────────────────────────────────────────
#pragma once
#include "core/filters/Filter.h"
#include <optional>

namespace core::filters {

    class Centroid : public Filter{
    public:
        QImage run(const QImage& src) const override;
        void buildUI(QWidget* panel, const std::function<void()>& /*refresh*/) override;
    private:
        mutable std::optional<QPointF> m_last;
    };

} 
