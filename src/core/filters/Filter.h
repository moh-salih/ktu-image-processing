#pragma once
#include <QImage>
#include "core/QMat.h"

#include <QtCore>
#include <QtWidgets>

namespace core::filters {

    class Filter {
    public:
        virtual ~Filter() = default;
        virtual QImage run(const QImage& src) const = 0;
        virtual void buildUI(QWidget* container, const std::function<void()>& refresh) { Q_UNUSED(container); }
    };  

} 
