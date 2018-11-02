//
// Created by Adelino and Raul on 02/11/18.
//

#ifndef _FLIT_H_
#define _FLIT_H_ 
#include <iostream>
#include <systemc.h>

class Flit {
	
public:
	sc_uint <32> data;
	sc_uint <32> trailer;
};

#endif