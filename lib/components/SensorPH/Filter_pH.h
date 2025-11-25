#ifndef FILTER_H
#define FILTER_H

#include <Arduino.h>

class SimpleFilter {
private:
    float readings[5];          // Fixed array cho 5 readings
    int currentIndex;           // Vị trí hiện tại
    int sampleCount;            // Số samples đã có

public:
    SimpleFilter();
    
    float addReading(float newValue);       // Thêm reading mới và return average
    float getAverage();                     // Get current average
    void reset();                           // Clear all readings
};

#endif