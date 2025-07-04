#include "Rotate.h"

namespace core::filters{
    
    QImage Rotate::run(const QImage& img) const{
        QImage dst(img.size(), img.format());
        dst.fill(Qt::black);

        QMat in(img);
        QMat out(dst);

        const double rad  = qDegreesToRadians(static_cast<double>(m_angle));
        const double cA   = std::cos(rad);
        const double sA   = std::sin(rad);
        const int    cx   = in.width()/2;
        const int    cy   = in.height()/2;

        out.forEachPixel([&](int y,int x,QMat::Pixel& p){
            int x0 = x-cx,  y0 = y-cy;
            int srcX = static_cast<int>( x0*cA + y0*sA + cx );
            int srcY = static_cast<int>(-x0*sA + y0*cA + cy );
            p = in.inBounds(srcY,srcX) ? in.at(srcY,srcX) : QMat::Pixel{0,0,0};
        });
        return dst;
    }

    void Rotate::buildUI(QWidget* panel, const std::function<void()>& refresh){
        auto* layout = qobject_cast<QVBoxLayout*>(panel->layout());
        if (!layout) return;

        // Outer wrapper for consistent 8px margin + 6px spacing
        auto* outer       = new QWidget(panel);
        auto* outerLayout = new QVBoxLayout(outer);
        outerLayout->setContentsMargins(8, 8, 8, 8);
        outerLayout->setSpacing(6);

        // Group box with its own 12/8/12/8 margins + 6px spacing
        auto* group = new QGroupBox(QObject::tr("Rotation Settings"), outer);
        auto* inner = new QVBoxLayout(group);
        inner->setContentsMargins(12, 8, 12, 8);
        inner->setSpacing(6);

        // -- Your actual controls go here --
        auto* label = new QLabel(
            QString(QObject::tr("Angle") + ": %1°").arg(m_angle),
            group
        );
        auto* slider = new QSlider(Qt::Horizontal, group);
        slider->setRange(-180, 180);
        slider->setSingleStep(1);
        slider->setValue(m_angle);

        inner->addWidget(label);
        inner->addWidget(slider);
        group->setLayout(inner);

        // Put the group into the outer wrapper…
        outerLayout->addWidget(group);
        outer->setLayout(outerLayout);

        // …and the wrapper into the panel
        layout->addWidget(outer);
        layout->addStretch();

        // Live updates
        QObject::connect(slider, &QSlider::valueChanged, panel,
            [this, label, refresh](int val) {
                m_angle = val;
                label->setText(
                    QString(QObject::tr("Angle") + ": %1°").arg(val)
                );
                refresh();
            }
        );
    }
}