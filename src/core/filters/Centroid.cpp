//  ─── Centroid.cpp ─────────────────────────────────────────────────────
#include "Centroid.h"
#include "core/QMat.h"

#include <QtGlobal>     // qClamp / std::clamp
#include <algorithm>    // std::clamp
#include <stdexcept>
#include <QObject>

namespace core::filters {

    QImage Centroid::run(const QImage& src) const{
        if (src.format() != QImage::Format_Grayscale8)
            throw std::runtime_error("Centroid needs an 8-bit B/W image");

        /* ---- 1. pass: accumulate foreground coordinates ------------------ */
        const int    w      = src.width();
        const int    h      = src.height();
        const int    stride = src.bytesPerLine();
        const uchar* bits   = src.constBits();

        long long sumX = 0, sumY = 0, cnt = 0;
        for (int y = 0; y < h; ++y) {
            const uchar* row = bits + y * stride;
            for (int x = 0; x < w; ++x) {
                if (row[x] == 255) {        // foreground pixel
                    sumX += x;  sumY += y;  ++cnt;
                }
            }
        }

        if (cnt == 0)            // nothing white – return untouched copy
            return src;

        /* ---- 2. centroid & drawing --------------------------------------- */
        const QPointF c(double(sumX) / cnt, double(sumY) / cnt);
        m_last = c;              // cache for clients that query it later

        QImage dst = src.convertToFormat(QImage::Format_RGB32);
        QMat   out(dst);

        const int cx = std::clamp<int>(int(c.x()), 0, w - 1);
        const int cy = std::clamp<int>(int(c.y()), 0, h - 1);

        const QMat::Pixel red {255, 0, 0, 255};

        for (int i = -2; i <= 2; ++i) {
            if (unsigned(cy + i) < unsigned(h))
                out.setPixel(cy + i, cx, red);       // vertical stroke
            if (unsigned(cx + i) < unsigned(w))
                out.setPixel(cy, cx + i, red);       // horizontal stroke
        }
        return dst;
    }


    void Centroid::buildUI(QWidget* panel, const std::function<void()>& /*refresh*/){
        auto* layout = qobject_cast<QVBoxLayout*>(panel->layout());
        if (!layout) return;

        auto* outer = new QWidget(panel);
        auto* outerLayout = new QVBoxLayout(outer);
        outerLayout->setContentsMargins(8, 8, 8, 8);
        outerLayout->setSpacing(6);

        auto* group = new QGroupBox(QObject::tr("Centroid Info"), outer);
        auto* vbox  = new QVBoxLayout(group);
        vbox->setContentsMargins(12, 8, 12, 8);

        auto* label = new QLabel(group);

        if (m_last.has_value()) {
            const auto& c = *m_last;
            label->setText(QString(QObject::tr("Centroid at: ")+ "(x=%1, y=%2)").arg(c.x(), 0, 'f', 2).arg(c.y(), 0, 'f', 2));
        } else {
            label->setText(QObject::tr("No foreground pixels found."));
        }

        vbox->addWidget(label);
        group->setLayout(vbox);

        outerLayout->addWidget(group);
        layout->addWidget(outer);
        layout->addStretch();
    }


} 
