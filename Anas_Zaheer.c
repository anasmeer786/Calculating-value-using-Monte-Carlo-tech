#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <pthread.h>

void PointCounter(void);
int globle_var = 0; //Globle variable
int totalCount = 0; // total accurance given by user
int counter = 0;
pthread_mutex_t mutex; // mutex for syncronization

/* Generates a double precision random number */
double random_double()
{
    return random() / ((double)RAND_MAX + 1);
}

/* Check for points inside circle */
void PointCounter(void)
{
    int i, count = 0; /* # of points in the 1st quadrant of unit circle */
    double x = 0.0, y = 0.0;
    int hit_count = 0.0; // total number of hit_count
    for (i = 0; i < totalCount ; i++) // loop will exicut until count user given
    {
        /* generate random numbers between -1.0 and +1.0 (exclusive) */
        x = random_double() * 2.0 - 1.0; // generate random number
        y = random_double() * 2.0 - 1.0;
        if (sqrt(x*x + y*y) < 1.0 ) // squar root is less then 1 then only increment hit_count
            ++hit_count;
    }
    globle_var = hit_count; // Put hit_count in globle_var for further process
}

void * Child(void *p) //Chile Thread
{
    pthread_mutex_lock(&mutex); // lock resource
    PointCounter(); // generate number and store count in globle_var
    pthread_mutex_unlock(&mutex); //unlock resource
    return NULL;
}

void * Parent(void *p)
{
    double pi;
    pthread_mutex_lock(&mutex); // lock resource
    pi = (double)globle_var / totalCount * 4; // calculate pi
    printf("Estimate of pi is %g \n", pi); // disply result
    pthread_mutex_unlock(&mutex); //unlock resource
    return NULL;
}

int main(int argc, char* argv[])
{
    pthread_t par, chi; // two threads

    /* Initialize mutex and condition variable objects */
    pthread_mutex_init(&mutex, NULL); // mutex init

    printf("Enter the number of iterations used to estimate pi: ");
    scanf("%d", &totalCount); // input number of times accurance of random numbers

    pthread_create(&par, NULL, Child, NULL); // create child thread
    pthread_create(&chi, NULL, Parent, NULL); // create parent thread

    pthread_join(chi, NULL); // wait until child conplition
    pthread_join(par, NULL); // wait until complition of parent theread

    pthread_mutex_destroy(&mutex); // destroy mutex
    return 0;
}

