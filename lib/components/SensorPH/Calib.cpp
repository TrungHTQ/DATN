#include "Calib.h"

namespace PHCalib {
    
    bool isValidADC(int adcValue) {
        return (adcValue >= ADC_MIN_VALID && adcValue <= ADC_MAX_VALID);
    }
    
    bool isValidPH(float pH) {
        return (pH >= PH_MIN_VALID && pH <= PH_MAX_VALID);
    }
    
    float adcToPH(int adcValue) {
        // Optional validation warning
        if (!isValidADC(adcValue)) {
            Serial.printf("[Calib] Warning: ADC %d outside expected range (%d-%d)\n", 
                         adcValue, ADC_MIN_VALID, ADC_MAX_VALID);
        }
        
        float pH = SLOPE * adcValue + OFFSET;
        
        if (pH <= PH_MIN_VALID) pH = PH_MIN_VALID;
        if (pH >= PH_MAX_VALID) pH = PH_MAX_VALID;
        
        Serial.printf("[Calib] ADC %d -> pH %.2f\n", adcValue, pH);
        
        return pH;
    }

    
} 
