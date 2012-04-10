class FastADC {
    
    public:
    	FastADC();
    	void StartADC(int,int);
        void Stop();
        int Get();
        
    private:
	char Old_ADCSRB;
	char Old_ADCSRA;
	char Old_ADMUX;
};

extern FastADC FreeRunningADC;