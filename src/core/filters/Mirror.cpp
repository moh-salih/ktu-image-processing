// Mirror.cpp
#include "Mirror.h"
#include "core/QMat.h"

namespace core::filters {

    QImage Mirror::run(const QImage& img) const {
        /* 1. convert to a QMat-compatible format if necessary ------------
         *    – keep colour if the original is colour;
         *    – otherwise degrade to 8-bit grayscale                       */
        QImage src = img;
        switch (src.format())
        {
            case QImage::Format_Grayscale8:
            case QImage::Format_RGB888:
            case QImage::Format_RGBA8888:
            case QImage::Format_RGB32:
            case QImage::Format_ARGB32:
                break; // 👍 already fine
            case QImage::Format_Indexed8:
            case QImage::Format_Grayscale16:
                src = src.convertToFormat(QImage::Format_Grayscale8);
                break;
            default:
                src = src.convertToFormat(QImage::Format_ARGB32);
        }

        /* 2. set-up QMat wrappers -------------------------------------- */
        QMat in(src);
        QImage dst(src.size(), src.format());
        QMat  out(dst);

        /* 3. pixel-wise mirroring --------------------------------------- */
        out.forEachPixel([&](int y, int x, QMat::Pixel& p) {
            const int srcX = (m_dir == Direction::Horizontal)
                           ? (in.width()  - 1 - x) : x;
            const int srcY = (m_dir == Direction::Vertical)
                           ? (in.height() - 1 - y) : y;
            p = in.getPixel(srcY, srcX);
        });

        return dst;
    }

    void Mirror::buildUI(QWidget* panel, const std::function<void()>& refresh) {
        auto* layout = qobject_cast<QVBoxLayout*>(panel->layout());
        if (!layout) return;

        // Outer wrapper with margins
        auto* outer = new QWidget(panel);
        auto* outerLayout = new QVBoxLayout(outer);
        outerLayout->setContentsMargins(8, 8, 8, 8);
        outerLayout->setSpacing(6);

        // Group box container
        auto* group = new QGroupBox(QObject::tr("Mirror Settings"), outer);
        auto* form  = new QFormLayout(group);
        form->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        form->setFormAlignment(Qt::AlignTop);
        form->setContentsMargins(12, 8, 12, 8);

        // Control
        auto* combo = new QComboBox(group);
        combo->addItems({ QObject::tr("Horizontal"), QObject::tr("Vertical") });
        combo->setCurrentIndex(static_cast<int>(m_dir));

        form->addRow(QObject::tr("Direction:"), combo);
        group->setLayout(form);
        outerLayout->addWidget(group);

        layout->addWidget(outer);
        layout->addStretch();

        QObject::connect(combo, QOverload<int>::of(&QComboBox::currentIndexChanged), combo,
            [this, refresh](int idx) {
                m_dir = static_cast<Direction>(idx);
                refresh();
            });
    }

} 
