// File: core/filters/Mirror.h
#pragma once
#include "core/filters/Filter.h"
#include "core/QMat.h"

#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>

namespace core::filters {

    class Mirror : public Filter{
    public:
        enum class Direction { Horizontal, Vertical };
        explicit Mirror(Direction dir = Direction::Horizontal) : m_dir(dir) {}
        
        QImage run(const QImage& img) const override;
        void buildUI(QWidget* panel, const std::function<void()>& refresh) override;
    private:
        Direction m_dir;
    };

} 
