// File: ImageProcessor.h
#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include <stdexcept>
#include <QImage>
#include "core/filters/Filter.h"

namespace core {

class ImageProcessor {
public:

    using Ptr = std::unique_ptr<filters::Filter>;
    void registerFilter(const std::string& name, Ptr f) {
        table[name] = std::move(f);
    }

    static ImageProcessor& instance()          // handy global accessor
    {
        static ImageProcessor s;               // C++-11 thread-safe
        return s;
    }

    QImage apply(const std::string& name, const QImage& img) const {
        auto it = table.find(name);
        if (it == table.end())
            throw std::runtime_error("Filter not found: " + name);
        return it->second->run(img);
    }

    template<typename T>
    T& find(const std::string& name) {
        auto it = table.find(name);
        if (it == table.end())
            throw std::runtime_error("Filter not found: " + name);

        T* ptr = dynamic_cast<T*>(it->second.get());
        if (!ptr)
            throw std::runtime_error("Filter type mismatch for: " + name);
        return *ptr;
    }



private:
    std::unordered_map<std::string, Ptr> table;
    ImageProcessor();                               // NEW – auto-register built-ins
};

} // namespace core
