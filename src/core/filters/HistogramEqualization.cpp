#include "HistogramEqualization.h"
#include "core/QMat.h"
#include <array>
#include <cmath>

namespace core::filters{

    QImage HistogramEqualization::run(const QImage& input) const {
        QMat in(input);
        QImage output = input.copy();
        QMat out(output);

        const int width = in.cols();
        const int height = in.rows();
        const int total = width * height;

        // Compute histogram
        std::array<int, 256> hist = {0};
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                auto p = in.getPixel(y, x);
                hist[p.r]++; // grayscale image, so r == g == b
            }
        }

        // Compute cumulative distribution function (CDF)
        std::array<int, 256> cdf;
        cdf[0] = hist[0];
        for (int i = 1; i < 256; ++i)
            cdf[i] = cdf[i - 1] + hist[i];

        // Find minimum non-zero CDF value
        int cdfMin = 0;
        for (int i = 0; i < 256; ++i) {
            if (cdf[i] != 0) {
                cdfMin = cdf[i];
                break;
            }
        }

        // Create LUT (Lookup Table)
        std::array<uchar, 256> lut;
        int denom = total - cdfMin;
        if (denom <= 0) denom = 1;
        for (int i = 0; i < 256; ++i) {
            lut[i] = static_cast<uchar>(std::round((cdf[i] - cdfMin) * 255.0 / denom));
        }

        // Apply transformation
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                auto p = in.getPixel(y, x);
                uchar newVal = lut[p.r];
                out.setPixel(y, x, {newVal, newVal, newVal});
            }
        }

        return output;
    }

    void HistogramEqualization::buildUI(QWidget* panel, const std::function<void()>& /*refresh*/){
        auto* layout = qobject_cast<QVBoxLayout*>(panel->layout());
        if (!layout) return;

        // ── padded wrapper ───────────────────────────────────────────────
        auto* outer       = new QWidget(panel);
        auto* outerLayout = new QVBoxLayout(outer);
        outerLayout->setContentsMargins(8, 8, 8, 8);
        outerLayout->setSpacing(6);

        // ── group box ────────────────────────────────────────────────────
        auto* group = new QGroupBox(QObject::tr("Histogram Equalization"), outer);
        auto* vbox  = new QVBoxLayout(group);
        vbox->setContentsMargins(12, 8, 12, 8);

        // description
        auto* info = new QLabel(
            QObject::tr("Automatically redistributes pixel intensities to\n"
            "use the full 0–255 dynamic range.\n"
            "No parameters are required."),
            group);
        info->setWordWrap(true);

        vbox->addWidget(info);
        group->setLayout(vbox);

        outerLayout->addWidget(group);
        layout->addWidget(outer);
        layout->addStretch();
    }

}