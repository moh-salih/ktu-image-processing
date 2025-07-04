#include "Sharpen.h"
#include <algorithm>   // std::clamp

namespace core::filters {

    QImage Sharpen::run(const QImage& inImg) const{
        // ensure single-channel for simplicity
        QImage src = (inImg.format() == QImage::Format_Grayscale8)
                    ? inImg
                    : inImg.convertToFormat(QImage::Format_Grayscale8);

        QImage dst(src.size(), QImage::Format_Grayscale8);
        dst.fill(0);

        QMat in(src), out(dst);

        /* dynamic sharpening kernel:
        [  0  -1   0 ]
        [ -1   C  -1 ]   where C = m_strength (typical 4–10)
        [  0  -1   0 ] */
        const int C = m_strength;

        for (int y = 1; y < in.rows() - 1; ++y)
            for (int x = 1; x < in.cols() - 1; ++x)
            {
                int acc =
                    0  * in.getPixel(y-1,x-1).r + -1 * in.getPixel(y-1,x).r + 0  * in.getPixel(y-1,x+1).r +
                -1 * in.getPixel(y  ,x-1).r +  C * in.getPixel(y  ,x).r + -1 * in.getPixel(y  ,x+1).r +
                    0  * in.getPixel(y+1,x-1).r + -1 * in.getPixel(y+1,x).r + 0  * in.getPixel(y+1,x+1).r;

                uchar v = static_cast<uchar>(std::clamp(acc, 0, 255));
                out.setPixel(y, x, { v, v, v, 255 });
            }

        return dst;
    }

    void Sharpen::buildUI(QWidget* panel, const std::function<void()>& refresh){
        auto* layout = qobject_cast<QVBoxLayout*>(panel->layout());
        if (!layout) return;

        /* padded wrapper */
        auto* outer       = new QWidget(panel);
        auto* outerLayout = new QVBoxLayout(outer);
        outerLayout->setContentsMargins(8,8,8,8);
        outerLayout->setSpacing(6);

        auto* group = new QGroupBox(QObject::tr("Sharpen Settings"), outer);
        auto* vbox  = new QVBoxLayout(group);
        vbox->setContentsMargins(12,8,12,8);

        auto* label  = new QLabel(QString(QObject::tr("Strength") + ": %1").arg(m_strength), group);
        label->setAlignment(Qt::AlignLeft);

        auto* slider = new QSlider(Qt::Horizontal, group);
        slider->setRange(1, 10);         // 1 → light, 10 → very strong
        slider->setValue(m_strength);

        vbox->addWidget(label);
        vbox->addWidget(slider);
        group->setLayout(vbox);

        outerLayout->addWidget(group);
        layout->addWidget(outer);
        layout->addStretch();

        QObject::connect(slider, &QSlider::valueChanged, slider,
            [this, label, refresh](int val) {
                m_strength = val;
                label->setText(QString(QObject::tr("Strength") + ": %1").arg(val));
                refresh();
            });
    }

} 
