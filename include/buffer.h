//
// Created by Adelino, Raul and Yuri on 02/11/18.
//

#ifndef _BUFFER_H_
#define _BUFFER_H_ 

#include <iostream>
#include <queue>
#include "systemc.h"


SC_MODULE (Buffer)
{

public:
	
	std::queue<sc_uint <32> > data_flit;
	sc_in<sc_uint <32> > data;
	sc_out<sc_uint <32> > data_out;

	void add();
	sc_uint <32> consome();
	bool isEmpty();


	SC_CTOR(Buffer) {
        SC_METHOD(add);
        	sensitive << data;
	}

	
};
#endif
