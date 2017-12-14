#include "main.h"
// declaring tables
struct table Table[MAX_TABLES];

int debug_mode = 0;
pthread_t cookHandle[100];
int n;
int main()
{
    /// Questioning debug mode
    printf("\nEnable debug mode? (1 - yes, 0 - no) [May affect performance]\n");
    scanf("%i",&debug_mode);

    /// Intro
    printf("\nWELCOME TO OUR RESTAURANT\n~Le Magnifique Croissante~\n");
    printf("Our waiters are looking for orders...");

    /// Questioning max order count
    printf("\nHow many orders shall we take today?\n");
    scanf("%i",&n);

    /// Database definitions
    def_table(Table);
    def_food();  ///defining the Foods
    def_cooks();    ///defining the Chefs
    def_utensils();     ///def the Utensils
    srand(time(NULL));   //Random seed

    /// Useful vars
    bool infinite_loop = true;
	bool can_debug = true; // a separator, for debugging not to fill up the screen
	int i,r,t;
	pthread_t orderHandle[MAX_TABLES];
    pthread_mutex_init(&mutex_order, NULL);

    /// The infinite loop of work
        while(infinite_loop){
                if(order_count < n){

                /// GENERATOR -- PRODUCER
                //[ASSUMING THE RESTAURANT IS IMPOSSIBLY POPULAR AND NEW CLIENTS COME EVERY 2-4min]
                sleep(random(2,4)); // Waiter takes an order in that time for simplicity


                // Each time, cleaning TempOrder
                for(i = 0; i<10; i++){
                    TempOrder.items[i] = 0;
                    TempOrder.progress[i] = 3; //setting progress as unspecified (0-notDone,1-inProcess,2-Done,3-notSpecified)
                }

                // To then populate it
                TempOrder.Items = random(1,5); //setting the number of items per order
                    for(i = 0; i<TempOrder.Items; i++){
                    TempOrder.items[i] = random(1,8); //setting the items in order among food id's: [1..8]
                    TempOrder.PrepTimes[i] = Food[TempOrder.items[i]].preptime; //registering in array the respective food prep times
                    TempOrder.progress[i] = 0; // setting each food item as notDone
                    }

                order_count += 1;
                ///Setting order data///
                TempOrder.max_wait = 1.3 * maximum(TempOrder.PrepTimes,TempOrder.Items);
                TempOrder.StartTime = std::chrono::high_resolution_clock::now();
                TempOrder.priority = set_priority(TempOrder);
                TempOrder.id = order_count;

                ///Notify about the new order///
                print_new_order(TempOrder.id,TempOrder.max_wait,TempOrder.priority);

                ///Pushing the order in a queue based on priority
                switch(TempOrder.priority){
                case 1:
                OrderList_1.push(TempOrder);
                break;
                case 2:
                OrderList_2.push(TempOrder);
                break;
                case 3:
                OrderList_3.push(TempOrder);
                break;
                case 4:
                OrderList_4.push(TempOrder);
                break;
                case 5:
                OrderList_5.push(TempOrder);
                break;
                default: // default order is last
                OrderList_1.push(TempOrder);
                break;}

}
                ///ANALYZER -- CONSUMER
                int take = 10;          // how many orders can be taken when 1 is added
                order TempOrderN[10];
                for(r=0; r<take; r++){
                if(BUSY_TABLES < MAX_TABLES){
                    t = find_empty_table(Table);
                    Sleep(10); //printf spacer
                    /// Taking care of the queued orders, based on priority
                    if(!OrderList_5.empty()){ // 5 first, if it's empty check 4, and so on
                        OrderList_5.front().table = t; // assign order's table to the found free table

                        print_accept_order(OrderList_5.front());

                        /// making the table busy
                        Table[t-1].is_free = false; //Tables are 0-9 therefore => [t-1]
                        BUSY_TABLES = BUSY_TABLES + 1;

                        /// storing the queued order in a temporary order to be accessed by the thread safely
                        TempOrderN[t-1] = OrderList_5.front();
                        OrderList_5.pop(); // order has been taken care of

                        TempOrderN[t-1].EndTime=std::chrono::high_resolution_clock::now();

                        /// storing the time the client was waiting
                        TempOrderN[t-1].timeInQueue = std::chrono::duration_cast<std::chrono::milliseconds>(TempOrderN[t-1].EndTime - TempOrderN[t-1].StartTime);

                        /// starting the order preparations
                        pthread_create(&orderHandle[t], NULL, thread_take_order, (void*)&TempOrderN[t-1]);

                        can_debug = true; // let debug show each time an order was taken, otherwise let it wait
                        break;
                        }

                        // same as above
                        if(!OrderList_4.empty()){
                        OrderList_4.front().table = t; // set order.table linked to current order
                        print_accept_order(OrderList_4.front());Table[t-1].is_free = false;
                        BUSY_TABLES = BUSY_TABLES + 1;
                        TempOrderN[t-1]= OrderList_4.front();
                        OrderList_4.pop(); // order has been taken care of
                        TempOrderN[t-1].EndTime=std::chrono::high_resolution_clock::now();
                        TempOrderN[t-1].timeInQueue = std::chrono::duration_cast<std::chrono::milliseconds>(TempOrderN[t-1].EndTime - TempOrderN[t-1].StartTime);
                        pthread_create(&orderHandle[t], NULL, thread_take_order, (void*)&TempOrderN[t-1]);
                        can_debug = true;
                        break;
                        } // aha
                        if(!OrderList_3.empty()){
                        OrderList_3.front().table = t; // set order.table linked to current order
                        print_accept_order(OrderList_3.front());
                        Table[t-1].is_free = false;
                        BUSY_TABLES = BUSY_TABLES + 1;
                        TempOrderN[t-1]= OrderList_3.front();
                        OrderList_3.pop(); // order has been taken care of
                        TempOrderN[t-1].EndTime=std::chrono::high_resolution_clock::now();
                        TempOrderN[t-1].timeInQueue = std::chrono::duration_cast<std::chrono::milliseconds>(TempOrderN[t-1].EndTime - TempOrderN[t-1].StartTime);
                        pthread_create(&orderHandle[t], NULL, thread_take_order, (void*)&TempOrderN[t-1]);
                        can_debug = true;
                        break;
                        } //i'm serious
                        if(!OrderList_2.empty()){
                        OrderList_2.front().table = t; // set order.table linked to current order
                        print_accept_order(OrderList_2.front());
                        Table[t-1].is_free = false; //[0-9] => -1
                        BUSY_TABLES = BUSY_TABLES + 1;
                        TempOrderN[t-1]= OrderList_2.front();
                        OrderList_2.pop(); // order has been taken care of
                        TempOrderN[t-1].EndTime=std::chrono::high_resolution_clock::now();
                        TempOrderN[t-1].timeInQueue = std::chrono::duration_cast<std::chrono::milliseconds>(TempOrderN[t-1].EndTime - TempOrderN[t-1].StartTime);
                        pthread_create(&orderHandle[t], NULL, thread_take_order, (void*)&TempOrderN[t-1]);
                        can_debug = true;
                        break;
                        } //yep
                        if(!OrderList_1.empty()){
                        OrderList_1.front().table = t; // set order.table linked to current order
                        print_accept_order(OrderList_1.front());
                        Table[t-1].is_free = false;
                        BUSY_TABLES = BUSY_TABLES + 1;
                        TempOrderN[t-1]= OrderList_1.front();
                        OrderList_1.pop(); // order has been taken care of
                        TempOrderN[t-1].EndTime=std::chrono::high_resolution_clock::now();
                        TempOrderN[t-1].timeInQueue = std::chrono::duration_cast<std::chrono::milliseconds>(TempOrderN[t-1].EndTime - TempOrderN[t-1].StartTime);
                        pthread_create(&orderHandle[t], NULL, thread_take_order, (void*)&TempOrderN[t-1]);
                        can_debug = true;
                        break;
                        }

                    }
                }
                if(debug_mode && can_debug){
                ///DEBUGGING QUEUE - used to see if the prioritized orders are taken first;
                ///                                    (BUSY_TABLES == MAX_TABLES required)
                pthread_mutex_lock (&mutex_order);
                printf("\n---\nBUSY_TABLES: %i",BUSY_TABLES);
                printf("\n5P-Queue:");
                print_queue(OrderList_5);
                printf("4P-Queue:");
                print_queue(OrderList_4);
                printf("3P-Queue:");
                print_queue(OrderList_3);
                printf("2P-Queue:");
                print_queue(OrderList_2);
                printf("1P-Queue:");
                print_queue(OrderList_1);

                printf("\n Cooking Apparatus:");
                printf("\n Free Stoves: %i", Utensil[1].num);
                printf("\n Free Ovens: %i\n", Utensil[2].num);

                printf("\n Cooks:");
                for(int cooq = 0; cooq<NUM_OF_COOKS; cooq++ )
                {printf("\n %s[rank-%i]: energy = %i", Chef[cooq].name,Chef[cooq].rank,Chef[cooq].prof);}
                printf("\n---\n");
                pthread_mutex_unlock (&mutex_order);
                can_debug = false;
                }


        /// close the restaurant when the orders are no more, and the tables are not busy
        if(BUSY_TABLES==0 && order_count>=n){close_restaurant(); infinite_loop = false;}
        }

        puts("\n= = = = = = = = =");
        puts("Thank you for today!\n");
        pthread_exit(NULL);
        pthread_mutex_destroy(&mutex_order);
        return 1;
}
/// P R I N T I N G ///
void print_accept_order(order o)
{
    pthread_mutex_lock (&mutex_order);
    printf("\n[ORDER %i] : ACCEPTED\nOn Table %i ",o.id,o.table);

    int i;
    printf("\nItems to cook: ");
    for(i=0; i<10; i++){
    //skip foods that don't exist in menu
        if(o.items[i]!=0){
            printf("%i ",o.items[i]);
        }
    }
    printf("\nTimes: ");
    for(i=0; i<10; i++){
            if(o.items[i]!=0){printf("%.0f ",Food[o.items[i]].preptime);}
    }
    printf("\nComplexities: ");
    for(i=0; i<10; i++){
            if(o.items[i]!=0){printf("%i ",Food[o.items[i]].complexity);}
    }
    printf("\nUtils: ");
    for(i=0; i<10; i++){
            if(o.items[i]!=NULL){printf("%s ",Food[o.items[i]].tool);}
    }

    pthread_mutex_unlock (&mutex_order);
}
void print_new_order(int id, float wait, int p)
{
    pthread_mutex_lock (&mutex_order);
    printf("\n\n[NEW] ORDER Nr#%i\nmax_wait:%.2f, priority:%i \n",id,wait,p);
    pthread_mutex_unlock (&mutex_order);
}
void print_completed_order(order* obj, float time_to_cook)
{

    printf("\n\n[ORDER Nr#%i] : COMPLETED\nOn Table %i\n",obj->id,obj->table);
    printf("totalWait: %.2f, timeCooking: %.2f, maxWait: %.2f \n",obj->TakenTime,time_to_cook,obj->max_wait);

}

/// T H R E A D I N G ///
void *thread_take_order(void* o){ //order handle
//temp var for original order
order *obj = ((order *)o);
//get the cook time and store it
float time_to_cook = cook_order(obj->items, obj->PrepTimes, obj->Items, obj->progress, obj->table);
//set the taken time: sum of client wait and cook time
obj->TakenTime = time_to_cook + ((int(obj->timeInQueue.count()))*timeCoeff/1000); // divBy1000 because milliseconds precision
//printing is safe with mutexes
pthread_mutex_lock (&mutex_order);
print_completed_order(obj,time_to_cook);
finish_order((float)obj->TakenTime,(float)obj->max_wait, obj->table);  //rating and cleaning table
pthread_mutex_unlock (&mutex_order);

}
void *thread_cook(void* Util){ //thread to cook one food
//temp storage of the required vars in a utility object
util *U = ((util *)Util);
int i = U->i;
int k = U->k;
int toolid = U->toolid;

//wait the food to get cooked
sleep((int)(Food[i].preptime));
U->p = 2; //assign that food is done
Chef[k].prof += 1; // regain the chef energy (proficiency)
if (toolid != 0){Utensil[toolid].num += 1;} // and the tool availability, if the tool is not "none", (a.k.a. 0)

}

/// C O O K I N G ///
float cook_order(int items[10],float PrepTimes[], int Items, int progress[10], int t){

    int i;
    util Util[Items][NUM_OF_COOKS];
    int k;
    float time_to_cook  = 0;

    // starting now to count time spent cooking
    chrono::high_resolution_clock::time_point StartCook = chrono::high_resolution_clock::now();

    ///loop that iterates through items of the order and different cooks,
    ///checking if the complexity fits the rank, proficiency is good, utensil is available and food is not taken care of yet(progress=0)
    LookForCooks:
    for(i = 0; i<Items; i++){
        for(k = NUM_OF_COOKS-1; k >= 0; k--){ //starting up with interns first
            if  ((Food[items[i]].complexity <= Chef[k].rank) && (Chef[k].prof > 0) && (Utensil[Food[items[i]].toolID].num > 0) && (progress[i] == 0) ){

                // Decrease proficiency (aka ENERGY POINTS CONCEPT) because the cook works on something
                Chef[k].prof -= 1;
                // And Decrease the amount of available tools, if it's a real tool, excluding "none"
                if (Food[items[i]].toolID != 0){Utensil[Food[items[i]].toolID].num -= 1;}
                ///assigning an utility to store the data for thread processing
                //i - the ordered food id
                //k - the cook id
                //p - the progress of the food
                //tool - the tool (id) required for the food preparation

                Util[i][k].i = items[i];
                Util[i][k].k = k;
                progress[i] = 1; // assign that food is being taken care of
                Util[i][k].p = progress[i];
                Util[i][k].toolid = Food[items[i]].toolID;



                ///start a thread for cooking one food
                pthread_create(&cookHandle[i], NULL, thread_cook, (void*)&Util[i][k]);
                }
        //set the progress of food to Done when it is, according to the cook thread
        if (Util[i][k].p == 2){progress[i] = 2;}
        }
    }
    Sleep(250); //a way around not to overload CPU, spacing the loop frequency to 0.25/s
    /// keep waiting and assigning cooks until all the foods in the order will be ready
    for (i = 0; i<Items; i++){ if(progress[i] != 2) goto LookForCooks;}

    // stopping to count time spent on cooking
    chrono::high_resolution_clock::time_point EndCook = chrono::high_resolution_clock::now();
    /// cook time calculation
    milliseconds cookInterval = chrono::duration_cast<chrono::milliseconds>(EndCook - StartCook);
    time_to_cook = float((int)(cookInterval.count()*timeCoeff-3620))/1000; //3620 millisec offset for code execution. important at time scaling

    return time_to_cook;
}

/// E N D I N G   O R D E R///
void finish_order(float order_time, float max_wait,int t){
    Table[t-1].is_free = true; //assigning the used table as free

    ///calculating the restaurant rating
    reviews += 1;                               //adding to the review count
    current_points = rate(order_time,max_wait); //rate based on taken time and max wait
    total_points += current_points;
    restaurant_rating = total_points/(float)reviews;

    printf("Review: %i\nRestaurant Rating: %.2f\n",current_points,restaurant_rating);

    Sleep(10); //printf spacer
    BUSY_TABLES -= 1; //cleaning the used table

}

/// U T I L S ///
int set_priority(class order o){ // sets priority to the order based on the amount of items inside it. YES WE LIKE LITTLE ORDERS BETTER
    int t = o.Items;
    if (t == 1) return 5;
    if (t == 2) return 4;
    if (t == 3) return 3;
    if (t == 4) return 2;
    if (t > 4) return 1;
    else return 0;
}
int rate(float order_time,float max_wait){ // rate the customer service based on time taken completing the order and max wait time
    if (order_time< max_wait)           return 5;
    else if (order_time<max_wait*1.1) 	return 4;
    else if (order_time<max_wait*1.2) 	return 3;
    else if (order_time<max_wait*1.3) 	return 2;
    else if (order_time<max_wait*1.4) 	return 1;
    else return 0;
}

/// TABLE STUFF
int def_table(struct table t[]){ //setting all tables to be free (utility used earlier, because the project was written in C initially...)
    int i;
    for(i=0; i<MAX_TABLES; i++)
    {
    t[i].is_free = true;
    }
}
int find_empty_table(struct table t[]){ //scan tables and return the first free one
    int i;
    for(i=0; i<MAX_TABLES; i++)
    {
    if(t[i].is_free)return i+1;
    }
    return 0; //error case that will never happen
}

/// DEFINING DATABASES
int def_cooks( ) { // This is the list of cooks

    //We have two Pro Chefs to back up the business

   Chef[0].rank = 3;// 1-3 complexity
   Chef[0].prof = 4; //1-4 dishes at once
   strcpy( Chef[0].name, "Albert Monroe");
   strcpy( Chef[0].catch_phrase, "My cookings are going to blend with your dreams!");

   Chef[1].rank = 3;// 1-3 complexity
   Chef[1].prof = 3; //1-4 dishes at once
   strcpy( Chef[1].name, "James Robbins");
   strcpy( Chef[1].catch_phrase, "Beware, you might never want to eat home again!");

   //We have few more skill-gambled intern chefs to help the business with what they can

   Chef[2].rank = random(1,2);// 1-3 complexity
   Chef[2].prof = random(1,3); //1-4 dishes at once
   strcpy( Chef[2].name, "Bobby The Intern");
   strcpy( Chef[2].catch_phrase, "You know I'm new here!");

   Chef[3].rank = random(1,2);// 1-3 complexity
   Chef[3].prof = random(1,3); //1-4 dishes at once
   strcpy( Chef[3].name, "Janny The Intern");
   strcpy( Chef[3].catch_phrase, "Hi, I hope you'll hire me!");

   Chef[4].rank = random(1,2);// 1-3 complexity
   Chef[4].prof = random(1,3); //1-4 dishes at once
   strcpy( Chef[4].name, "Nicky The Intern");
   strcpy( Chef[4].catch_phrase, "Cool, I like this place, I'm staying!");

   Chef[5].rank = random(1,2);// 1-3 complexity
   Chef[5].prof = random(1,3); //1-4 dishes at once
   strcpy( Chef[5].name, "Larry The Intern");
   strcpy( Chef[5].catch_phrase, "You will like my home cooking skills!");

   Chef[6].rank = random(1,2);// 1-3 complexity
   Chef[6].prof = random(1,3); //1-4 dishes at once
   strcpy( Chef[6].name, "Harry The Intern");
   strcpy( Chef[6].catch_phrase, "Hi there, me - the best!");

   Chef[7].rank = random(1,2);// 1-3 complexity
   Chef[7].prof = random(1,3); //1-4 dishes at once
   strcpy( Chef[7].name, "Patty The Intern");
   strcpy( Chef[7].catch_phrase, "How cute, all this kitchen is mine?");

return 1;
}
int def_food( ) { // This is the restaurant menu

   strcpy( Food[0].name, "Water");
   Food[0].preptime = 0;
   strcpy( Food[0].tool, "none");
   Food[0].complexity = 0;
   Food[0].toolID = 0;

   strcpy( Food[1].name, "Pizza");
   Food[1].preptime = 20;
   strcpy( Food[1].tool, "oven");
   Food[1].complexity = 2;
   Food[1].toolID = 2;

   strcpy( Food[2].name, "Salad");
   Food[2].preptime = 10;
   strcpy( Food[2].tool, "none");
   Food[2].complexity = 1;
   Food[2].toolID = 0;

   strcpy( Food[3].name, "Zeama");
   Food[3].preptime = 7;
   strcpy( Food[3].tool, "stove");
   Food[3].complexity = 1;
   Food[3].toolID = 1;

   strcpy( Food[4].name, "Scallop Sashimi with Meyer Lemon Confit");
   Food[4].preptime = 32;
   strcpy( Food[4].tool, "none");
   Food[4].complexity = 3;
   Food[4].toolID = 0;

   strcpy( Food[5].name, "Island Duck with Mulberry Mustard");
   Food[5].preptime = 35;
   strcpy( Food[5].tool, "oven");
   Food[5].complexity = 3;
   Food[5].toolID = 2;

   strcpy( Food[6].name, "Waffles");
   Food[6].preptime = 10;
   strcpy( Food[6].tool, "stove");
   Food[6].complexity = 1;
   Food[6].toolID = 1;

   strcpy( Food[7].name, "Aubergine");
   Food[7].preptime = 20;
   strcpy( Food[7].tool, "none");
   Food[7].complexity = 2;
   Food[7].toolID = 0;

   strcpy( Food[8].name, "Lasagna");
   Food[8].preptime = 30;
   strcpy( Food[8].tool, "oven");
   Food[8].complexity = 2;
   Food[8].toolID = 2;

return 1;
}
int def_utensils(){ // This is the tools the restaurant has
Utensil[0].num = 99; // this number won't change however, but just in case, we have a lot of none's :)
strcpy( Utensil[0].name, "none"); //bare hands, huh
Utensil[1].num = NUM_OF_STOVES;
strcpy( Utensil[1].name, "stove");
Utensil[2].num = NUM_OF_OVENS;
strcpy( Utensil[2].name, "oven");
return 1;
}
/// Q U E U E   U T I L S ///
void print_queue(queue<order> o){ //Utility to print the queue

    while(!o.empty()) {
        cout << o.front().priority << " ";
        o.pop();
    }
    cout << '\n';
}

//bool operator<(const order& a, const order& b) { // obsolete: comparing orders by priority
//          return a.priority < b.priority;
//}

/// G E N E R A L   U T I L S ///
int random(int min, int max){   // return a random value from given range
   return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}
float maximum(float a[], int n){ // find max value in an array
  float max;
  int i;
  max = a[0];
  for (i = 1; i < n; i++)
  {
    if (a[i] > max)
    {
       max = a[i];
    }
  }
  return max;
}
void test(int time){ //just a test of time difference
    auto StartTime = chrono::steady_clock::now();
    sleep(time);
    auto EndTime = chrono::steady_clock::now();
    float offset = 0.015; // noticed this random-ish value gets added to the final one
    float Delta = std::chrono::duration_cast<std::chrono::duration<float> >(EndTime - StartTime).count()-offset;
    printf("\n\n%f passed\n\n",Delta);
}
void close_restaurant(){
    ///just wait a bit more until everyone's done cooking
    // (I know i could just use pthread join instead, but)
    while(BUSY_TABLES!=0){
        sleep(1);
    };
}
