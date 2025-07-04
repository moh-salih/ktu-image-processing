#include "Prewitt.h"
#include <algorithm>   // std::clamp
#include <cmath>       // std::sqrt
#include "core/QMat.h"

namespace core::filters{

    QImage Prewitt::run(const QImage& inImg) const {
        /* --- 1. guarantee 8-bit grayscale input ------------------------------ */
        QImage src = (inImg.format() == QImage::Format_Grayscale8)? inImg : inImg.convertToFormat(QImage::Format_Grayscale8);
                    
        QImage dst(src.size(), QImage::Format_Grayscale8);
        dst.fill(0);

        QMat in(src), out(dst);
        
        static constexpr int GX[3][3] = {
            { -1, 0, 1 },
            { -1, 0, 1 },
            { -1, 0, 1 }
        };

        static constexpr int GY[3][3] = {
            { -1, -1, -1 },
            {  0,  0,  0 },
            {  1,  1,  1 }
        };
        
        for (int y = 1; y < in.rows() - 1; ++y){
            for (int x = 1; x < in.cols() - 1; ++x){
                int gx = 0, gy = 0;
                
                for (int j = -1; j <= 1; ++j)
                for (int i = -1; i <= 1; ++i) {
                    int v = in.getPixel(y + j, x + i).r;
                        gx += GX[j + 1][i + 1] * v;
                        gy += GY[j + 1][i + 1] * v;
                    }
                    
                    int mag = m_useHypot
                    ? static_cast<int>(std::sqrt(gx * gx + gy * gy))
                        : std::abs(gx) + std::abs(gy);   // faster L₁ approx

                        mag = std::clamp(mag, 0, 255);
                        out.setPixel(y, x, { static_cast<uchar>(mag), 0, 0, 255 });
                        }
        }
        return dst;
    }

    void Prewitt::buildUI(QWidget* panel, const std::function<void()>& refresh){
        auto* layout = qobject_cast<QVBoxLayout*>(panel->layout());
        if (!layout) return;

        // padded wrapper
        auto* outer       = new QWidget(panel);
        auto* outerLayout = new QVBoxLayout(outer);
        outerLayout->setContentsMargins(8, 8, 8, 8);
        outerLayout->setSpacing(6);

        // group box
        auto* group = new QGroupBox(QObject::tr("Prewitt Settings"), outer);
        auto* vbox  = new QVBoxLayout(group);
        vbox->setContentsMargins(12, 8, 12, 8);

        // checkbox
        auto* check = new QCheckBox(QObject::tr("Use Euclidean magnitude (sqrt) ✧"), group);
        check->setChecked(m_useHypot);

        vbox->addWidget(check);
        group->setLayout(vbox);
        outerLayout->addWidget(group);

        layout->addWidget(outer);
        layout->addStretch();

        // apply on toggle
        QObject::connect(check, &QCheckBox::toggled, check,
            [this, refresh](bool on) {
                m_useHypot = on;
                refresh();
            });
    }

}