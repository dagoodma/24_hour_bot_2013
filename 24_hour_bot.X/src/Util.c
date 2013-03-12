/*
 * File:   Util.c
 * Author: jurjohns
 *
 * Created on March 10, 2012, 4:52 PM
 */
#include "Util.h"

/**
 * Function: min
 * @param a, integer a
 * @param b, integer b
 * @return Lowest value, either a or b
 * @remark
 */
unsigned int min(int a, int b) {
    if (a < b)
        return a;
    else
        return b;
}

/**
 * Function: max
 * @param a, integer a
 * @param b, integer b
 * @return Highest value, either a or b
 * @remark
 */
unsigned int max(int a, int b) {
    if (a > b)
        return a;
    else
        return b;
}

/**
* Function: wait
* @return Nothing
* @remark Waits a small period of time
*/
void wait() {
    unsigned int wait = 0;
    for (wait = 0; wait <= 1000000; wait++)
        asm("nop");
}