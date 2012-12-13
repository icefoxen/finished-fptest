/* Soem quick and stupid stuff to test the relative speeds of fixed and 
   floating point math, and play with fixed point in general.

   Compile with:
   gcc -O3 -lm -Wall -std=c99 -o fptest fptest.c
*/

#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

// We are going to use fixed-point numbers with 48 bits above the decimal point
// and 16 bits below it.
typedef int64_t fix;
#define FIXSIZE 16
#define FIXMASK 0xFFFF

inline fix fadd(fix x, fix y) {
   return x + y;
}

inline fix fsub(fix x, fix y) {
   return x - y;
}

inline fix fmul(fix x, fix y) {
   return (x * y) >> FIXSIZE;
}

// This has some SERIOUS precision problems, actually.
inline fix fdiv(fix x, fix y) {
   return (x / y) << FIXSIZE;
}

inline fix int2fix(int64_t x) {
   return x << FIXSIZE;
}

inline fix double2fix(double x) {
   double abovePoint = trunc(x);
   double belowPoint = x - abovePoint;
   int64_t above = ((int64_t) abovePoint) << FIXSIZE;
   int64_t below = (int64_t) (belowPoint * (1 << FIXSIZE)) & FIXMASK;
   return above | below;
}

inline int64_t fix2int(fix x) {
   return x >> FIXSIZE;
}

inline double fix2double(fix x) {
   double abovePoint = x >> FIXSIZE;
   double belowPoint = ((double) (x & FIXMASK)) / (1 << FIXSIZE);
   return abovePoint + belowPoint;
}

void printTestMath() {
   fix a = double2fix(1.42);
   double b = fix2double(a);
   printf("Started with 1.42, converted to %ld, then back to %f\n", a, b);
   fix c = fmul(double2fix(93.324), a);
   fix d = fdiv(double2fix(93.324), a);
   printf("Multiplied and divided by 93.324, got %f and %f\n", fix2double(c), fix2double(d));
   fix e = fdiv(double2fix(93.324), double2fix(2.2));
   printf("93.324 / 2.2 = %f\n", fix2double(e));
}

#define COUNT 100000000

double testFloatMath() {
   printf("Testing floating point math (float):\n");
   float x = 0;
   {
      clock_t start = clock();
      for(int i = 0; i < COUNT; i++) {
	 x = fadd(x, 23.23543);
      }
      clock_t end = clock();
      clock_t diff = end - start;
      printf("Did %d adds in %f seconds; %f per second\n", 
	     COUNT, (double) diff/CLOCKS_PER_SEC, COUNT/((double) diff/CLOCKS_PER_SEC));
   }
   {
      clock_t start = clock();
      for(int i = 0; i < COUNT; i++) {
	 x = fsub(x, 12.21);
      }
      clock_t end = clock();
      clock_t diff = end - start;
      printf("Did %d subtracts in %f seconds; %f per second\n", 
	     COUNT, (double) diff/CLOCKS_PER_SEC, COUNT/((double) diff/CLOCKS_PER_SEC));
   }
   {
      clock_t start = clock();
      for(int i = 0; i < COUNT; i++) {
	 x = fmul(x,23.23543);
      }
      clock_t end = clock();
      clock_t diff = end - start;
      printf("Did %d multiplies in %f seconds; %f per second\n", 
	     COUNT, (double) diff/CLOCKS_PER_SEC, COUNT/((double) diff/CLOCKS_PER_SEC));
   }
   {
      clock_t start = clock();
      for(int i = 0; i < COUNT; i++) {
	 x = fdiv(x, 23.23543);
      }
      clock_t end = clock();
      clock_t diff = end - start;
      printf("Did %d divides in %f seconds; %f per second\n", 
	     COUNT, (double) diff/CLOCKS_PER_SEC, COUNT/((double)diff/CLOCKS_PER_SEC));
   }

   double x2 = 0;
   printf("Testing floating point math (double)\n");
   {
      clock_t start = clock();
      for(int i = 0; i < COUNT; i++) {
	 x2 += 23.23543;
      }
      clock_t end = clock();
      clock_t diff = end - start;
      printf("Did %d adds in %f seconds; %f per second\n", 
	     COUNT, (double) diff/CLOCKS_PER_SEC, COUNT/((double) diff/CLOCKS_PER_SEC));
   }
   {
      clock_t start = clock();
      for(int i = 0; i < COUNT; i++) {
	 x2 -= 12.432;
      }
      clock_t end = clock();
      clock_t diff = end - start;
      printf("Did %d subtracts in %f seconds; %f per second\n", 
	     COUNT, (double) diff/CLOCKS_PER_SEC, COUNT/((double) diff/CLOCKS_PER_SEC));
   }
   {
      clock_t start = clock();
      for(int i = 0; i < COUNT; i++) {
	 x2 *= 23.23543;
      }
      clock_t end = clock();
      clock_t diff = end - start;
      printf("Did %d multiplies in %f seconds; %f per second\n", 
	     COUNT, (double) diff/CLOCKS_PER_SEC, COUNT/((double) diff/CLOCKS_PER_SEC));
   }
   {
      clock_t start = clock();
      for(int i = 0; i < COUNT; i++) {
	 x2 /= 23.23543;
      }
      clock_t end = clock();
      clock_t diff = end - start;
      printf("Did %d divides in %f seconds; %f per second\n", 
	     COUNT, (double) diff/CLOCKS_PER_SEC, COUNT/((double) diff/CLOCKS_PER_SEC));
   }

   return x + x2;
}

fix testFixMath() {
   printf("Testing fixed point math\n");
   fix x = 0;
   {
      clock_t start = clock();
      for(int i = 0; i < COUNT; i++) {
	 x += 23.23543;
      }
      clock_t end = clock();
      clock_t diff = end - start;
      printf("Did %d adds in %f seconds; %f per second\n", 
	     COUNT, (double) diff/CLOCKS_PER_SEC, COUNT/((double) diff/CLOCKS_PER_SEC));
   }
   {
      clock_t start = clock();
      for(int i = 0; i < COUNT; i++) {
	 x -= 12.23;
      }
      clock_t end = clock();
      clock_t diff = end - start;
      printf("Did %d subtracts in %f seconds; %f per second\n", 
	     COUNT, (double) diff/CLOCKS_PER_SEC, COUNT/((double) diff/CLOCKS_PER_SEC));
   }
   {
      clock_t start = clock();
      for(int i = 0; i < COUNT; i++) {
	 x *= 23.23543;
      }
      clock_t end = clock();
      clock_t diff = end - start;
      printf("Did %d multiplies in %f seconds; %f per second\n", 
	     COUNT, (double) diff/CLOCKS_PER_SEC, COUNT/((double) diff/CLOCKS_PER_SEC));
   }
   {
      clock_t start = clock();
      for(int i = 0; i < COUNT; i++) {
	 x /= 23.23543;
      }
      clock_t end = clock();
      clock_t diff = end - start;
      printf("Did %d divides in %f seconds; %f per second\n", 
	     COUNT, (double) diff/CLOCKS_PER_SEC, COUNT/((double) diff/CLOCKS_PER_SEC));
   }
   return x;
}

int main(int argc, char** argv) {
   testFloatMath();
   testFixMath();
   return 0;   
}
