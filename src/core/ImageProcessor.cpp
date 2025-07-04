// File: ImageProcessor.cpp
#include "ImageProcessor.h"
#include "filters/Median.h"
#include "filters/Mean.h"  
#include "filters/Centroid.h"
#include "filters/Sobel.h"
#include "filters/Prewitt.h"
#include "filters/Gaussian.h"
#include "filters/Sharpen.h"
#include "filters/HistogramEqualization.h" 
#include "filters/ThresholdManual.h"  
#include "filters/ThresholdOtsu.h"  
#include "filters/ThresholdKapur.h"  
#include "filters/Erosion.h" 
#include "filters/Dilation.h"
#include "filters/Skeletonization.h"
#include "filters/Shift.h"
#include "filters/Mirror.h"
#include "filters/ContrastStretch.h"
#include "filters/Rotate.h"  



using namespace core;

namespace core {

    ImageProcessor::ImageProcessor() {
        registerFilter("Median", std::make_unique<core::filters::Median>());
        registerFilter("Mean",   std::make_unique<core::filters::Mean>());   
        registerFilter("Centroid", std::make_unique<core::filters::Centroid>());
        registerFilter("Sobel", std::make_unique<core::filters::Sobel>());
        registerFilter("Prewitt", std::make_unique<core::filters::Prewitt>());
        registerFilter("Gaussian", std::make_unique<core::filters::Gaussian>());
        registerFilter("Sharpen", std::make_unique<filters::Sharpen>());
        registerFilter("HistogramEqualization", std::make_unique<filters::HistogramEqualization>()); 
        registerFilter("ThresholdManual", std::make_unique<filters::ThresholdManual>());
        registerFilter("ThresholdOtsu", std::make_unique<filters::ThresholdOtsu>());
        registerFilter("ThresholdKapur", std::make_unique<filters::ThresholdKapur>());
        registerFilter("Erosion", std::make_unique<filters::Erosion>());
        registerFilter("Dilation", std::make_unique<filters::Dilation>());
        registerFilter("Skeletonization", std::make_unique<filters::Skeletonization>());
        registerFilter("Shift", std::make_unique<filters::Shift>());
        registerFilter("ContrastStretch", std::make_unique<filters::ContrastStretch>());
        registerFilter("Mirror", std::make_unique<filters::Mirror>());
        registerFilter("Rotate", std::make_unique<filters::Rotate>());

    }

} // namespace core
