/*
 * PID.c
 *
 *  Created on: 2020Äê10ÔÂ13ÈÕ
 *      Author: Lenovo
 */

#include <PID.h>
#include <config.h>
int Pway(int8 Targ,int8 Real)
{
    int8 delta;
    if(Targ>Real)
    {
        delta=Targ-Real;
        return delta/2+1;
    }
    if(Targ<Real)
    {
        delta=Real-Targ;
        return delta/2+1;
    }
    return 0;

}
