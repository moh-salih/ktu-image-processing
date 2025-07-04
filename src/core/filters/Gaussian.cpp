#include "Gaussian.h"
#include "core/QMat.h"
#include <array>

namespace core::filters{

    QImage Gaussian::run(const QImage& inImg) const{
        QImage src = (inImg.format() == QImage::Format_Grayscale8) ? inImg : inImg.convertToFormat(QImage::Format_Grayscale8);

        QImage dst(src.size(), QImage::Format_Grayscale8);
        dst.fill(0);
        
        QMat in(src), out(dst);

        static constexpr int KERNEL[3][3] = {
            {1, 2, 1},
            {2, 4, 2},
            {1, 2, 1}
        };
        static constexpr int SUM = 16;
        
        for (int y = 1; y < in.rows() - 1; ++y)
        {
            for (int x = 1; x < in.cols() - 1; ++x)
            {
                int acc = 0;

                for (int j = -1; j <= 1; ++j)
                {
                    for (int i = -1; i <= 1; ++i)
                    {
                        int pixel = in.getPixel(y + j, x + i).r;
                        acc += KERNEL[j + 1][i + 1] * pixel;
                    }
                }
                
                uchar blurred = static_cast<uchar>(acc / SUM);
                out.setPixel(y, x, {blurred, 0, 0, 255});
            }
        }

        return dst;
    }

    void Gaussian::buildUI(QWidget* panel, const std::function<void()>& refresh){
        auto* layout = qobject_cast<QVBoxLayout*>(panel->layout());
        if (!layout) return;

        // Outer wrapper
        auto* outer = new QWidget(panel);
        auto* outerLayout = new QVBoxLayout(outer);
        outerLayout->setContentsMargins(8, 8, 8, 8);
        outerLayout->setSpacing(6);

        // Group box
        auto* group = new QGroupBox(QObject::tr("Gaussian Blur"), outer);
        auto* vbox  = new QVBoxLayout(group);
        vbox->setContentsMargins(12, 8, 12, 8);

        // Kernel size spinner
        auto* spin = new QSpinBox(group);
        spin->setRange(3, 11);        // 3x3 up to 11x11
        spin->setSingleStep(2);       // Only odd sizes
        spin->setValue(m_size);
        spin->setSuffix(" px");

        vbox->addWidget(new QLabel(QObject::tr("Kernel Size (odd):"), group));
        vbox->addWidget(spin);
        group->setLayout(vbox);

        outerLayout->addWidget(group);
        layout->addWidget(outer);
        layout->addStretch();

        // Live update
        QObject::connect(spin, QOverload<int>::of(&QSpinBox::valueChanged), spin,
            [this, refresh](int val) {
                m_size = (val % 2 == 0) ? val + 1 : val;
                refresh();
            });
    }

}