// Median.cpp
#include "Median.h"
#include <vector>
#include <algorithm>

namespace core::filters{

    QImage Median::run(const QImage& src) const {
        QImage dst = src.copy();
        QMat in(src), out(dst);
        
        const int r = m_size / 2;
        for (int y = r; y < in.rows() - r; ++y){
            for (int x = r; x < in.cols() - r; ++x) {
                std::vector<uchar> win;
                in.forEachKernel(y, x, m_size, [&](const QMat::Pixel& p){
                    win.push_back(p.r);          // assuming grayscale
                });
                std::nth_element(win.begin(), win.begin() + win.size() / 2, win.end()); out.setPixel(y, x, {win[win.size()/2],0,0,255});
            }
        }
            
        return dst;
    }

    void Median::buildUI(QWidget* panel, const std::function<void()>& refresh){
        auto* layout = qobject_cast<QVBoxLayout*>(panel->layout());
        if (!layout) return;

        // outer wrapper with margin
        auto* outer       = new QWidget(panel);
        auto* outerLayout = new QVBoxLayout(outer);
        outerLayout->setContentsMargins(8, 8, 8, 8);
        outerLayout->setSpacing(6);

        // group-box container
        auto* group = new QGroupBox(QObject::tr("Median Filter"), outer);
        auto* vbox  = new QVBoxLayout(group);
        vbox->setContentsMargins(12, 8, 12, 8);

        // spin-box for odd kernel size
        auto* spin = new QSpinBox(group);
        spin->setRange(3, 11);         // 3×3 … 11×11
        spin->setSingleStep(2);        // only odd values
        spin->setValue(m_size);
        spin->setSuffix(" px");

        vbox->addWidget(new QLabel(QObject::tr("Kernel Size (odd):"), group));
        vbox->addWidget(spin);
        group->setLayout(vbox);

        outerLayout->addWidget(group);
        layout->addWidget(outer);
        layout->addStretch();

        // live update
        QObject::connect(spin, QOverload<int>::of(&QSpinBox::valueChanged), spin,
            [this, refresh](int val) {
                m_size = (val % 2 == 0) ? val + 1 : val;  // ensure odd
                refresh();
            });
    }

}