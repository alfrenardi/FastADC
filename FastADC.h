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
	void GetArray();
    private:
	char Old_ADCSRB;
	char Old_ADCSRA;
	char Old_ADMUX;
    	*int val_list;
	int val_rd;
	int val_wr;
	char OnFastADC;
};

#endif
