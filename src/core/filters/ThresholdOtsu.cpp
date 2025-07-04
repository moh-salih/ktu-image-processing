// ThresholdOtsu.cpp
#include "ThresholdOtsu.h"
#include <stdexcept>
#include <vector>

namespace core::filters {

    QImage ThresholdOtsu::run(const QImage& input) const {
        if (input.isNull())
            throw std::runtime_error("Input image is null");

        QImage output = input.copy();
        const int size = input.width() * input.height();

        const uchar* in = input.constBits();
        uchar* out = output.bits();

        // Step 1: Compute histogram
        int hist[256] = {0};
        for (int i = 0; i < size; ++i) {
            hist[in[i]]++;
        }

        // Step 2: Compute global sum
        int total = size;
        double sum = 0;
        for (int i = 0; i < 256; ++i)
            sum += i * hist[i];

        // Step 3: Otsu's method
        double sumB = 0;
        int wB = 0, wF = 0;
        double maxVar = 0;
        int threshold = 0;

        for (int t = 0; t < 256; ++t) {
            wB += hist[t];
            if (wB == 0) continue;

            wF = total - wB;
            if (wF == 0) break;

            sumB += t * hist[t];

            double mB = sumB / wB;
            double mF = (sum - sumB) / wF;

            double varBetween = wB * wF * (mB - mF) * (mB - mF);

            if (varBetween > maxVar) {
                maxVar = varBetween;
                threshold = t;
            }
        }

        // Step 4: Apply threshold
        for (int i = 0; i < size; ++i) {
            out[i] = (in[i] >= threshold) ? 255 : 0;
        }

        return output;
    }

    void ThresholdOtsu::buildUI(QWidget* panel, const std::function<void()>& /*refresh*/){
        auto* layout = qobject_cast<QVBoxLayout*>(panel->layout());
        if (!layout) return;

        auto* outer       = new QWidget(panel);
        auto* outerLayout = new QVBoxLayout(outer);
        outerLayout->setContentsMargins(8, 8, 8, 8);
        outerLayout->setSpacing(6);

        auto* group = new QGroupBox(QObject::tr("Otsu Thresholding"), outer);
        auto* vbox  = new QVBoxLayout(group);
        vbox->setContentsMargins(12, 8, 12, 8);

        auto* label = new QLabel(
            QObject::tr("Finds the threshold that minimizes intra-class variance\n"
            "between foreground and background.\n"
            "Fully automatic – no parameters required."),
            group);
        label->setWordWrap(true);

        vbox->addWidget(label);
        group->setLayout(vbox);

        outerLayout->addWidget(group);
        layout->addWidget(outer);
        layout->addStretch();
    }

} 
