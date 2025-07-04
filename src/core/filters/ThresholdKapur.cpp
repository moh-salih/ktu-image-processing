// ThresholdKapur.cpp
#include "ThresholdKapur.h"
#include <cmath>
#include <stdexcept>

namespace core::filters {

    QImage ThresholdKapur::run(const QImage& input) const {
        if (input.isNull())
            throw std::runtime_error("Input image is null");

        QImage output = input.copy();
        const int size = input.width() * input.height();

        const uchar* in = input.constBits();
        uchar* out = output.bits();

        // Step 1: Histogram
        int hist[256] = {0};
        for (int i = 0; i < size; ++i) {
            hist[in[i]]++;
        }

        // Step 2: Normalize histogram to probabilities
        double prob[256];
        for (int i = 0; i < 256; ++i) {
            prob[i] = static_cast<double>(hist[i]) / size;
        }

        // Step 3: Find the threshold maximizing total entropy
        double maxEntropy = -1.0;
        int threshold = 0;

        for (int t = 1; t < 255; ++t) {
            double p1 = 0.0;
            for (int i = 0; i <= t; ++i)
                p1 += prob[i];

            if (p1 == 0.0 || p1 == 1.0) continue;

            double H1 = 0.0;
            for (int i = 0; i <= t; ++i)
                if (prob[i] > 0.0)
                    H1 -= (prob[i] / p1) * std::log(prob[i] / p1);

            double H2 = 0.0;
            for (int i = t + 1; i < 256; ++i)
                if (prob[i] > 0.0)
                    H2 -= (prob[i] / (1.0 - p1)) * std::log(prob[i] / (1.0 - p1));

            double totalEntropy = H1 + H2;
            if (totalEntropy > maxEntropy) {
                maxEntropy = totalEntropy;
                threshold = t;
            }
        }

        // Step 4: Apply threshold
        for (int i = 0; i < size; ++i) {
            out[i] = (in[i] >= threshold) ? 255 : 0;
        }

        return output;
    }

    void ThresholdKapur::buildUI(QWidget* panel, const std::function<void()>& /*refresh*/){
        auto* layout = qobject_cast<QVBoxLayout*>(panel->layout());
        if (!layout) return;

        auto* outer       = new QWidget(panel);
        auto* outerLayout = new QVBoxLayout(outer);
        outerLayout->setContentsMargins(8, 8, 8, 8);
        outerLayout->setSpacing(6);

        auto* group = new QGroupBox(QObject::tr("Kapur Thresholding"), outer);
        auto* vbox  = new QVBoxLayout(group);
        vbox->setContentsMargins(12, 8, 12, 8);

        auto* label = new QLabel(
            QObject::tr("Automatically finds the optimal threshold using\n"
            "maximum entropy of class distributions (Kapur method).\n"
            "No parameters are required."),
            group);
        label->setWordWrap(true);

        vbox->addWidget(label);
        group->setLayout(vbox);

        outerLayout->addWidget(group);
        layout->addWidget(outer);
        layout->addStretch();
    }

} 
