#ifndef DISP_H
#define DISP_H


class Disp{
public:
	Disp(int input_pin);
	void extendDisp();
	void retractDisp();
private:
	bool extended;
	int pin;

};


#endif