/* File: primes.c */
/* Author: Britton Wolfe */
/* Date: July 16, 2009 */
/* This program outputs all the primes in the range given */
/* by the command line arguments */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main(int argc, const char** argv){

    int lowerBound, upperBound;

    if(argc != 3){
        fprintf(stderr, "USAGE: %s lowerBound upperBound\n", argv[0]);
        return -1;
    }
  
    lowerBound = atoi(argv[1]);
    upperBound = atoi(argv[2]);
  
    if(lowerBound < 1 || upperBound < 1){
        fprintf(stderr, "ERROR: both the lowerBound (%d) and the upperBound (%d)"
	          " must be positive.\n", lowerBound, upperBound);
        return -2;
    }
  
    {
        /* TODO: fill in the code that outputs the prime numbers */
        for (int n = lowerBound; n <= upperBound; n++) {
    if (n < 2) continue; 

    int isPrime = 1; // n is prime initially
    int limit = sqrt(n);
    for (int i = 2; i <= limit; i++) {
        if (n % i == 0) { // if n is divisible by i, it's not prime
            isPrime = 0;
            break;
        }
    }
    if (isPrime) {
        printf("%d\n", n);
    }
}

    }

    return 0;
}
