#include "Filter_pH.h"

SimpleFilter::SimpleFilter() {
    currentIndex = 0;
    sampleCount = 0;
    
    for (int i = 0; i < 5; i++) {
        readings[i] = 0.0;
    }
}

float SimpleFilter::addReading(float newValue) {

    readings[currentIndex] = newValue;
    
    currentIndex = (currentIndex + 1) % 5;
    
    if (sampleCount < 5) {
        sampleCount++;
    }
    return getAverage();
}

float SimpleFilter::getAverage() {
    if (sampleCount == 0) return 0.0;
    
    float sum = 0.0;
    for (int i = 0; i < sampleCount; i++) {
        sum += readings[i];
    }
    
    return sum / sampleCount;
}

void SimpleFilter::reset() {
    currentIndex = 0;
    sampleCount = 0;
    
    for (int i = 0; i < 5; i++) {
        readings[i] = 0.0;
    }
}