#ifndef RESTAURANT_H
#define RESTAURANT_H
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <chrono>
using namespace std::chrono;

typedef struct table
{
    bool is_free;// = true;

}table;

class order{
public:
   int     id;
   int     table;    // 0-9 table id
   int     items[10];// the items in order
   int     priority; // low 1 - 5 high              //[system priority of taking order]
   float   max_wait; // 1.3 * max(Food[].prep_time) //[max wait time for client]
   int     Items;    // number of items per order
   float   PrepTimes[10]; //list of times to prepare the items
   float   TakenTime;
   int    progress[10];// 0 -nothing 1-progress 2-done
   high_resolution_clock::time_point StartTime;
   high_resolution_clock::time_point EndTime;
   milliseconds timeInQueue;
};

typedef struct cooktool{
   int   num;       // amount of tools
   char  name[10];  // stove / oven
}cooktool ;

typedef struct cook{
   int   rank; //1-3 complexity
   int   prof; //1-4 dishes at once
   char  name[25];
   char  catch_phrase[100];
}cook ;

class util{
public:
    int i,k,toolid,p=3;

};

#endif
