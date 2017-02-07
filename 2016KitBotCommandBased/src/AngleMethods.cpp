/*
 * AngleMethods.cpp
 *
 *  Created on: Jan 19, 2017
 *      Author: Driver
 */
#include <cmath>

double mod(double a, int n){
	return a - floor(a/n) * n;
//	return a % n;
}

double angle_diff(int a, int b){
	return mod((a-b) + 180, 360) - 180;
}

double formatAngle(double a)
{
	return mod(a + 180, 360) - 180;
}

