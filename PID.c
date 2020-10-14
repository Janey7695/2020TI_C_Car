/*
 * PID.c
 *
 *  Created on: 2020Äê10ÔÂ13ÈÕ
 *      Author: Lenovo
 */

#include <PID.h>
#include <config.h>
int PID(int8 Targ,int8 Real,float Kp)
{
    int8 erro;
    if(Targ>Real)
    {
        erro=Targ-Real;
        return Kp*erro;
    }
    if(Targ<Real)
    {
        erro=Real-Targ;
        return Kp*erro;
    }
    return 0;

}
