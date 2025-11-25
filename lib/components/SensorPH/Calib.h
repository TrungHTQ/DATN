#ifndef CALIB_H
#define CALIB_H

#include <Arduino.h>

namespace PHCalib {
    // ===== CALIBRATION CONSTANTS =====
    const float SLOPE = -0.00374f;      // pH per ADC unit (NEGATIVE slope!)
    const float OFFSET = 18.6f;         // pH offset
    
    // ===== VALIDATION RANGES =====
    const int ADC_MIN_VALID = 1000;   
    const int ADC_MAX_VALID = 3500;   
    const float PH_MIN_VALID = 0.0f;     
    const float PH_MAX_VALID = 14.0f;    
    
    // ===== FUNCTION DECLARATIONS =====
    float adcToPH(int adcValue); 
    bool isValidADC(int adcValue);    
    bool isValidPH(float pH);           
}

#endif