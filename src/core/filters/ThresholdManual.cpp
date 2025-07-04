// ThresholdManual.cpp
#include "ThresholdManual.h"
#include <QtGlobal>
#include <stdexcept>

#include <QMainWindow>
#include <QSlider>
#include <QWidget>
#include <QLabel>

namespace core::filters {

    QImage ThresholdManual::run(const QImage& img) const {
        if (img.format() != QImage::Format_Grayscale8)
            throw std::runtime_error("ThresholdManual requires Format_Grayscale8");

        QImage dst = img.copy();
        QMat in(img);
        QMat out(dst);

        out.forEachPixel([&](int, int, QMat::Pixel& p) {
            p.r = p.g = p.b = (p.r > m_threshold) ? 255 : 0;
        });

        return dst;
    }

    void ThresholdManual::buildUI(QWidget* panel, const std::function<void()>& refresh){
        auto* layout = qobject_cast<QVBoxLayout*>(panel->layout());
        if (!layout) return;

        // 1) Outer wrapper for 8px margins and 6px spacing
        auto* outer       = new QWidget(panel);
        auto* outerLayout = new QVBoxLayout(outer);
        outerLayout->setContentsMargins(8, 8, 8, 8);
        outerLayout->setSpacing(6);

        // 2) Group box with its 12/8/12/8 inner margins
        auto* group = new QGroupBox(QObject::tr("Manual Threshold"), outer);
        auto* vbox  = new QVBoxLayout(group);
        vbox->setContentsMargins(12, 8, 12, 8);
        vbox->setSpacing(6);

        // 3) Your controls
        auto* label  = new QLabel(
            QString(QObject::tr("Threshold") + ": %1").arg(m_threshold, 3),
            group
        );
        auto* slider = new QSlider(Qt::Horizontal, group);
        slider->setRange(0, 255);
        slider->setValue(m_threshold);

        vbox->addWidget(label);
        vbox->addWidget(slider);
        group->setLayout(vbox);

        // 4) Insert into outer wrapper
        outerLayout->addWidget(group);
        outer->setLayout(outerLayout);

        // 5) Finally add wrapper to the panel layout
        layout->addWidget(outer);
        layout->addStretch();

        // 6) Connect for live updates
        QObject::connect(slider, &QSlider::valueChanged, slider,
            [this, label, refresh](int val) {
                m_threshold = val;
                label->setText(
                    QString(QObject::tr("Threshold") + ": %1").arg(val, 3)
                );
                refresh();
            }
        );
    }


} 
