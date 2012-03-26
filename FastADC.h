class FastADC {
    
    public:
    	FastADC();
    	void StartADCRes(int);
        void StartADCSpeed(int);
        void StartADCMaxSpeed(int);
        void Stop();
        int Get();
        
    private:
	char Old_ADCSRB;
	char Old_ADCSRA;
	char Old_ADMUX;
//	char OnFastADC;
};

extern FastADC FreeRunningADC;