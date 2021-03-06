/*
 * DLBSmotor.h
 *
 *  Created on: 5 ��� 2017 �.
 *      Author: ddemidov
 */

#ifndef DLBSMOTOR_H_
#define DLBSMOTOR_H_

#if !defined(USE_DLBS_MOTOR)
typedef int __dummy_type;
#else

#include "mbed.h"
#include "LV8907_BLDC.h"
#include <debug.h>

#define BLDC_MAX_RPM	13400
#define BLDC_MIN_RPM	4400
#define BLDC_INTR_SET	1
#define BLDC_INTR_RESET	0

class DLBSmotor {
	static LV8907_BLDC bldc;				//BLDC class object
	static void bldcHndlr();
public:
	DLBSmotor();
	virtual ~DLBSmotor();
	int init();
	int start(int RPM);
	int stop();
};

#endif

#endif /* DLBSMOTOR_H_ */
