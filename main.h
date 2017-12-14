#ifndef MAIN_H
#define MAIN_H
#include "foods.h"
#include "restaurant.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <queue>
#include <stack>
#include <algorithm>
#include <functional>
#include <vector>
#include <chrono>
#include <sys/time.h>
#include <iostream>
#include <windows.h>
#define MAX_TABLES 10
#define NUM_OF_COOKS 8 //Let's hire 8 cooks because..

#define NUM_OF_STOVES 8 // we get a lot of customers, okay?
#define NUM_OF_OVENS 8  // okay.

using namespace std;

int timeCoeff = 10; //default:1 max:10
//bigger is faster i.e. timeCoeff = 1/timeScale
#define sleep(z) Sleep((z*1000)/timeCoeff) //default:1000 (1sec)

///VARS
queue<order> OrderList_5;
queue<order> OrderList_4;
queue<order> OrderList_3;
queue<order> OrderList_2;
queue<order> OrderList_1;
order TempOrder;
cook Chef[NUM_OF_COOKS];
cooktool Utensil[3];
struct food Food[9];
int BUSY_TABLES = 0;
int order_count = 0;
int reviews = 0; // rate count
int total_points=0;  //for rating
int current_points=0;//for rating
float restaurant_rating = 0; //final rating
pthread_mutex_t mutex_order;



///FUNCS
void *thread_take_order(void*);
float cook_order(int[], float[], int, int[], int);
void finish_order(float order_time, float max_wait, int table);
int rate(float order_time,float max_wait);

int def_cooks( );
int def_food( );
int def_utensils();
int def_table(struct table[]);

int find_empty_table(struct table[]);

void test(int time);
void close_restaurant();
int random(int, int);
float maximum(float[],int);
int set_priority(order);

void print_queue(queue<order>);
void print_accept_order(order o);
void print_new_order(int id, float wait, int p);
void print_completed_order(order* o,float);
#endif
