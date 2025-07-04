// Shift.cpp
#include "Shift.h"
#include "core/QMat.h"

namespace core::filters {

    QImage Shift::run(const QImage& img) const {
        QImage dst(img.size(), img.format());
        dst.fill(Qt::white);                   

        QMat in(img);
        QMat out(dst);

        out.forEachPixel([&](int y,int x,QMat::Pixel& p){
            int srcX = x - m_dx;
            int srcY = y - m_dy;
            if (in.inBounds(srcY,srcX))
                p = in.at(srcY,srcX);
            else
                p = {0,0,0};
        });
        return dst;
    }

    void Shift::buildUI(QWidget* panel, const std::function<void()>& refresh){
        auto* layout = qobject_cast<QVBoxLayout*>(panel->layout());
        if (!layout) return;

        // Outer wrapper with padding
        auto* outer = new QWidget(panel);
        auto* outerLayout = new QVBoxLayout(outer);
        outerLayout->setContentsMargins(8, 8, 8, 8);
        outerLayout->setSpacing(6);

        // Group box
        auto* group = new QGroupBox(QObject::tr("Shift Settings"), outer);
        auto* grid  = new QGridLayout(group);
        grid->setContentsMargins(12, 8, 12, 8);
        grid->setHorizontalSpacing(10);
        grid->setVerticalSpacing(8);

        // Controls
        auto* dxSpin = new QSpinBox(group);
        dxSpin->setRange(-1000, 1000);
        dxSpin->setValue(m_dx);
        dxSpin->setSingleStep(10);
        

        auto* dySpin = new QSpinBox(group);
        dySpin->setRange(-1000, 1000);
        dySpin->setValue(m_dy);
        dySpin->setSingleStep(10);

        // Labels + layout
        grid->addWidget(new QLabel(QObject::tr("X Shift:")), 0, 0);
        grid->addWidget(dxSpin,                0, 1);
        grid->addWidget(new QLabel(QObject::tr("Y Shift:")), 1, 0);
        grid->addWidget(dySpin,                1, 1);

        group->setLayout(grid);
        outerLayout->addWidget(group);
        layout->addWidget(outer);
        layout->addStretch();

        // Refresh on change
        auto onChange = [this, dxSpin, dySpin, refresh] {
            m_dx = dxSpin->value();
            m_dy = dySpin->value();
            refresh();
        };

        QObject::connect(dxSpin, QOverload<int>::of(&QSpinBox::valueChanged), dxSpin, onChange);
        QObject::connect(dySpin, QOverload<int>::of(&QSpinBox::valueChanged), dySpin, onChange);
    }

} 
