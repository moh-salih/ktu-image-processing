#include "ContrastStretch.h"
#include "core/QMat.h"

namespace core::filters {

    QImage ContrastStretch::run(const QImage& input) const {
        if (input.isNull()) return input;

        QImage dst = input.copy();
        QMat src(input);
        QMat out(dst);

        uchar minVal = 255, maxVal = 0;
        
        // Option 1: Using coordinates version
        src.forEachPixel([&](int y, int x, const QMat::Pixel& p) {
            uchar v = p.r;
            minVal = std::min(minVal, v);
            maxVal = std::max(maxVal, v);
        });
        
        // Option 2: Alternative using manual loops
        /*
        for (int y = 0; y < src.rows(); ++y) {
            for (int x = 0; x < src.cols(); ++x) {
                auto p = src.getPixel(y, x);
                minVal = std::min(minVal, p.r);
                maxVal = std::max(maxVal, p.r);
            }
        }
        */
        
        int range = maxVal - minVal;
        if (range == 0) return input;
        
        src.forEachPixel([&](int y, int x, const QMat::Pixel& p) {
            uchar v = static_cast<uchar>((p.r - minVal) * 255 / range);
            out.setPixel(y, x, {v, v, v, p.a});
        });
        
        return dst;
    }

    void ContrastStretch::buildUI(QWidget* panel, const std::function<void()>& refresh){
        auto* layout = qobject_cast<QVBoxLayout*>(panel->layout());
        if (!layout) return;

        // Outer wrapper for padding
        auto* outer = new QWidget(panel);
        auto* outerLayout = new QVBoxLayout(outer);
        outerLayout->setContentsMargins(8, 8, 8, 8);
        outerLayout->setSpacing(6);

        // Group box
        auto* group = new QGroupBox(QObject::tr("Contrast Stretching"), outer);
        auto* vbox  = new QVBoxLayout(group);
        vbox->setContentsMargins(12, 8, 12, 8);

        // Info label
        auto* info = new QLabel(QObject::tr("Automatically stretches contrast\nbased on min/max intensity."), group);
        info->setWordWrap(true);

        vbox->addWidget(info);
        group->setLayout(vbox);

        outerLayout->addWidget(group);
        layout->addWidget(outer);
        layout->addStretch();
    }

} 