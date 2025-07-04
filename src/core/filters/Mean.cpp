// Mean.cpp
#include "Mean.h"
#include <array>     // std::array
#include <algorithm> // std::clamp

namespace core::filters {

    QImage Mean::run(const QImage& src) const{
        // 1) make a writable copy for the destination
        QImage dst = src.copy();
        
        // 2) wrap images with QMat helpers
        QMat in(src), out(dst);
        
        const int r = m_size / 2;               // half-window radius
        const int windowArea = m_size * m_size;      // number of samples in the kernel

        for (int y = r; y < in.rows() - r; ++y)
        {
            for (int x = r; x < in.cols() - r; ++x)
            {
                std::array<int,4> acc = {0,0,0,0};   // channel accumulators
                
                // collect all pixels in the k×k neighbourhood
                in.forEachKernel(y, x, m_size, [&](const QMat::Pixel& p)
                {
                    acc[0] += p.r;
                    acc[1] += p.g;
                    acc[2] += p.b;
                    acc[3] += p.a;
                });

                // compute arithmetic mean per channel
                QMat::Pixel mean;
                mean.r = static_cast<uchar>(acc[0] / windowArea);
                mean.g = static_cast<uchar>(acc[1] / windowArea);
                mean.b = static_cast<uchar>(acc[2] / windowArea);
                mean.a = static_cast<uchar>(acc[3] / windowArea);
                
                out.setPixel(y, x, mean);
            }
        }
        return dst;
    }

    void Mean::buildUI(QWidget* panel, const std::function<void()>& refresh){
        auto* layout = qobject_cast<QVBoxLayout*>(panel->layout());
        if (!layout) return;

        // Outer padded wrapper
        auto* outer = new QWidget(panel);
        auto* outerLayout = new QVBoxLayout(outer);
        outerLayout->setContentsMargins(8, 8, 8, 8);
        outerLayout->setSpacing(6);

        // Group box for visual consistency
        auto* group = new QGroupBox(QObject::tr("Mean Filter"), outer);
        auto* vbox  = new QVBoxLayout(group);
        vbox->setContentsMargins(12, 8, 12, 8);

        // Spin box for kernel size
        auto* spin = new QSpinBox(group);
        spin->setRange(3, 15);         // Kernel size: 3 to 15
        spin->setSingleStep(2);        // Only odd values
        spin->setValue(m_size);
        spin->setSuffix(" px");

        vbox->addWidget(new QLabel(QObject::tr("Kernel Size (odd):"), group));
        vbox->addWidget(spin);
        group->setLayout(vbox);

        outerLayout->addWidget(group);
        layout->addWidget(outer);
        layout->addStretch();

        // Live update when value changes
        QObject::connect(spin, QOverload<int>::of(&QSpinBox::valueChanged), spin,
            [this, refresh](int val) {
                m_size = (val % 2 == 0) ? val + 1 : val; // Ensure it's odd
                refresh();
            });
    }

}