// core/filters/Erosion.cpp
#include "Erosion.h"
#include "core/QMat.h"

namespace core::filters {

    QImage Erosion::run(const QImage& src) const{
        QImage dst = src.copy();
        QMat in(src), out(dst);

        for (int y = 1; y < in.rows() - 1; ++y)
            for (int x = 1; x < in.cols() - 1; ++x)
            {
                bool white = true;
                in.forEachKernel(y, x, 3, [&](const QMat::Pixel& p){
                    white &= (p.r == 255);
                });
                uchar v = static_cast<uchar>(white ? 255 : 0);
                out.setPixel(y, x, {v, v, v, 255});
            }
        return dst;
    }

   void Erosion::buildUI(QWidget* panel, const std::function<void()>& refresh){
        auto* layout = qobject_cast<QVBoxLayout*>(panel->layout());
        if (!layout) return;

        // Outer container to add margin around the group box
        auto* outer = new QWidget(panel);
        auto* outerLayout = new QVBoxLayout(outer);
        outerLayout->setContentsMargins(8, 8, 8, 8);
        outerLayout->setSpacing(6);

        auto* group = new QGroupBox(QObject::tr("Erosion Settings"), outer);
        auto* form  = new QFormLayout(group);
        form->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        form->setFormAlignment(Qt::AlignTop);
        form->setContentsMargins(12, 8, 12, 8);

        auto* spin = new QSpinBox(group);
        spin->setRange(1, 5);  // 3x3 to 11x11
        spin->setValue(m_radius);
        spin->setSuffix(" px");

        form->addRow(QObject::tr("Kernel Radius:"), spin);
        group->setLayout(form);
        outerLayout->addWidget(group);

        layout->addWidget(outer);
        layout->addStretch();

        QObject::connect(spin, QOverload<int>::of(&QSpinBox::valueChanged), spin,
            [this, refresh](int val) {
                m_radius = val;
                refresh();
            });
    }

} 
