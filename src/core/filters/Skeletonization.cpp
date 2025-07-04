// ---------------  filters/Skeletonization.cpp  ------------------------
#include "Skeletonization.h"
#include "core/QMat.h"
#include <vector>

namespace core::filters {

    static inline bool isWhite(uchar v) { return v == 255; }

    QImage Skeletonization::run(const QImage& inImg) const{
        // --- guarantee binary 8-bit input ---------------------------------
        QImage src = (inImg.format() == QImage::Format_Grayscale8)
                ? inImg
                : inImg.convertToFormat(QImage::Format_Grayscale8);

        QImage dst = src.copy();                 // we thin directly in-place
        QMat   mat(dst);

        const int rows = mat.rows();
        const int cols = mat.cols();

        std::vector<QPoint> toDelete;

        bool changed;
        do {
            changed = false;

            /* ================= FIRST sub-iteration ======================= */
            toDelete.clear();
            for (int y = 1; y < rows - 1; ++y) {
                for (int x = 1; x < cols - 1; ++x) {
                    if (!isWhite(mat.getPixel(y,x).r)) continue;

                    // p2 … p9 clockwise (p1 is centre, not used)
                    const bool p2 = isWhite(mat.getPixel(y-1,x  ).r);
                    const bool p3 = isWhite(mat.getPixel(y-1,x+1).r);
                    const bool p4 = isWhite(mat.getPixel(y  ,x+1).r);
                    const bool p5 = isWhite(mat.getPixel(y+1,x+1).r);
                    const bool p6 = isWhite(mat.getPixel(y+1,x  ).r);
                    const bool p7 = isWhite(mat.getPixel(y+1,x-1).r);
                    const bool p8 = isWhite(mat.getPixel(y  ,x-1).r);
                    const bool p9 = isWhite(mat.getPixel(y-1,x-1).r);

                    const int  B  = p2+p3+p4+p5+p6+p7+p8+p9;              // 2 ≤ B ≤ 6
                    const int  A  = (!p2 && p3) + (!p3 && p4) + (!p4 && p5) +
                                    (!p5 && p6) + (!p6 && p7) + (!p7 && p8) +
                                    (!p8 && p9) + (!p9 && p2);           // exactly 1

                    if (A==1 && 2<=B && B<=6 &&
                        (!p2 || !p4 || !p6) &&
                        (!p4 || !p6 || !p8))
                        toDelete.emplace_back(x,y);
                }
            }
            for (const QPoint& pt : toDelete)
                mat.setPixel(pt.y(), pt.x(), {0,0,0,255});
            changed |= !toDelete.empty();

            /* ================= SECOND sub-iteration ====================== */
            toDelete.clear();
            for (int y = 1; y < rows - 1; ++y) {
                for (int x = 1; x < cols - 1; ++x) {
                    if (!isWhite(mat.getPixel(y,x).r)) continue;

                    const bool p2 = isWhite(mat.getPixel(y-1,x  ).r);
                    const bool p3 = isWhite(mat.getPixel(y-1,x+1).r);
                    const bool p4 = isWhite(mat.getPixel(y  ,x+1).r);
                    const bool p5 = isWhite(mat.getPixel(y+1,x+1).r);
                    const bool p6 = isWhite(mat.getPixel(y+1,x  ).r);
                    const bool p7 = isWhite(mat.getPixel(y+1,x-1).r);
                    const bool p8 = isWhite(mat.getPixel(y  ,x-1).r);
                    const bool p9 = isWhite(mat.getPixel(y-1,x-1).r);

                    const int  B  = p2+p3+p4+p5+p6+p7+p8+p9;
                    const int  A  = (!p2 && p3) + (!p3 && p4) + (!p4 && p5) +
                                    (!p5 && p6) + (!p6 && p7) + (!p7 && p8) +
                                    (!p8 && p9) + (!p9 && p2);

                    if (A==1 && 2<=B && B<=6 &&
                        (!p2 || !p4 || !p8) &&
                        (!p2 || !p6 || !p8))
                        toDelete.emplace_back(x,y);
                }
            }
            for (const QPoint& pt : toDelete)
                mat.setPixel(pt.y(), pt.x(), {0,0,0,255});
            changed |= !toDelete.empty();

        } while (changed);

        return dst;
    }

    void Skeletonization::buildUI(QWidget* panel, const std::function<void()>& /*refresh*/){
        auto* layout = qobject_cast<QVBoxLayout*>(panel->layout());
        if (!layout) return;

        // Padded wrapper
        auto* outer       = new QWidget(panel);
        auto* outerLayout = new QVBoxLayout(outer);
        outerLayout->setContentsMargins(8, 8, 8, 8);
        outerLayout->setSpacing(6);

        // Group box
        auto* group = new QGroupBox(QObject::tr("Skeletonization"), outer);
        auto* vbox  = new QVBoxLayout(group);
        vbox->setContentsMargins(12, 8, 12, 8);

        // Info label
        auto* label = new QLabel(
            QObject::tr("Thins binary shapes to their 1-pixel-wide skeletons\n"
            "using the Zhang–Suen iterative thinning algorithm.\n"
            "No parameters are required."),
            group);
        label->setWordWrap(true);

        vbox->addWidget(label);
        group->setLayout(vbox);

        outerLayout->addWidget(group);
        layout->addWidget(outer);
        layout->addStretch();
    }

} 
