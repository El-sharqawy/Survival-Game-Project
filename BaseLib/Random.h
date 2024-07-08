//
// Created by osama on 07/07/24.
//

#ifndef RANDOM_H
#define RANDOM_H

extern void				srandom(unsigned long seed);
extern unsigned long	nrandom();
extern float			frandom(float flLow, float flHigh);
extern long				random_range(long from, long to);
extern float			DegreeToRadian(float degrees);

#endif //RANDOM_H
