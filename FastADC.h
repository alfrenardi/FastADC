#ifndef FastADC_h
#define FastADC_h

#if ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

class FastADC {
    
    public:
        void StartADCRes();
        void StartADCSpeed();
        void Stop();
        int Get();
    private:
	char Old_ADCSRB;
	char Old_ADCSRA;
	char Old_ADMUX;
	char OnFastADC;
};
#endif