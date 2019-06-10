/**
 * @Author Dan Engbert (end1@umbc.edu)
 * This file contains a program that simulates scheduling drivers to deliver
 * orders from restaurants to customers.
 * The restaurant scheduler, each driver and the main program are all separate threads.
 *
 * Answers to part6:
 * 1. Restaurant thread is the producer, because it's pushing work to other threads for them to do.
 *   Driver threads are the consumers because they are doing the work (delivering orders).
 *   The data item being produced is the order for the driver to deliver.
 *
 * 2. Possible race condition:
 *   A driver could complete an order in between when the main thread acquires the lock
 *   and when it prints the number of orders completed by that driver.
 *   Since I am using a mutex, the driver will have completed the order, but will be unable to
 *   update its count of delivererd orders (until the main thread is done printing and releases
 *   the mutex) so the number of orders delivered will be off by 1 when printed.
 *
 * Help received:
 * man pages: fscanf, malloc, memset, pthread_mutex_destroy
 * https://stackoverflow.com/a/19613784
 * http://www.cplusplus.com/reference/cstdlib/strtol/
 * https://timmurphy.org/2010/05/04/pthreads-in-c-a-minimal-working-example/
 */

#define _DEFAULT_SOURCE // prevent warning about usleep
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>

// data structures:
struct Order {            // linked list of orders to be completed
    int rLoc;             // index of restaurant in restaurants (for pick up)
    int cLoc;             // index of customer in customer (for drop off)
    int id;               // order ID
    struct Order* next;
};
struct Driver {           // represents a driver thread
    pthread_t pth;
    int id;               // unique id representing this driver
    int loc[2];           // latest location [x,y] 
    int status;           // 0: idle, 1: delivering
    int stop;             // 0: continue working, 1: end thread after current order
    int delivered;        // total orders delivered
    struct Order *orders; // linked list of orders to process
};
struct Params {            // info needed by Restaurant thread
    int D;                 // number of drivers
    FILE *fp;              // file to read orders from
};

// global data:
//   customers and restaurants will not be modified by anyone
//   after threads are created by main() so a mutex is not needed.
int customers[10][2];          // [x,y] location of each customer
int restaurants[10][2];        // [x,y] location of each restaurant
int debug = 0;   // set to 1 to print extra info for debugging

// mutex for shared data access of drivers and rStop
pthread_mutex_t data_mutex;
struct Driver *drivers = NULL; // shared data (dynamically allocated array)
int rStop = 0;                 // shared data (1 tells Restaurant thread to end)

// function prototypes:
void freeOrders(struct Order* orders);
void driverTravel(struct Driver *driver, int indx, int orderID, int route);
void *driverThread(void *arg);
void *restaurantThread(void *arg);
struct Order *scheduling(struct Order *orders, int D, int print, int* stop);
void printStatus(struct Driver *drivers, int D);

/**
 * free the orders in a linked list of orders
 */
void freeOrders(struct Order* orders) {
    struct Order* cur = orders;
    while (orders != NULL) {
        cur = orders;
        orders = orders->next;
        free(cur);
    }
}

/**
 * function that moves a driver between locations
 * driver:    pointer to Driver that wants to travel
 * orderID:     unique ID of order
 * indx:      index of desired location
 *              (in customers or restaurants, depending on value of route)
 * int to[2]: destination location
 * route:     0 if this travel is to the restaurant
 *            1 if this travel is to the customer (from the restaurant)
 */
void driverTravel(struct Driver *driver, int indx, int orderID, int route) {
    pthread_mutex_lock(&data_mutex);
    int *to; // drivers destination [x,y]
    if (route == 0)
        to = restaurants[indx];
    else
        to = customers[indx];

    double travelDist = abs(driver->loc[0] - to[0]) + abs(driver->loc[1] - to[1]);
    int sleepTime = 1000 * 250 * travelDist;
    if (route == 0 && debug == 1) {
        printf("    * Driver %d: starting order %d, heading to restaurant %d (travel time = %d ms)\n",
                driver->id, orderID, indx, (int) (sleepTime / 1000));
    }
    pthread_mutex_unlock(&data_mutex);
    usleep(sleepTime);
    // update driver's location
    pthread_mutex_lock(&data_mutex);
    driver->loc[0] = to[0];
    driver->loc[1] = to[1];
    if (route == 0 && debug == 1) {
        printf("    * Driver %d: picked up order %d headed to customer %d (travel time = %d ms)\n",
                driver->id, orderID, indx, (int) (sleepTime / 1000));
    }
    else if (route == 1) {
        driver->delivered += 1;
        // if no orders assigned, set status to idle
        // otherwise, leave it as busy
        printf("Driver%d: finished order %d\n", driver->id, orderID);
        if (driver->orders == NULL) {
            driver->status = 0;
        }
    }
    pthread_mutex_unlock(&data_mutex);
}

/**
 * function for driver threads (D instances)
 *
 * a driver is assigned an order by another thread placing an order
 *   in driver->orders (linked list of orders assigned to this driver).
 *   can only process one order at a time.
 *
 * after delivering order: driver idles at customer's location until a new
 *   order is assigned.
 *
 * a driver is signaled to stop when another thread sets driver->stop = 1
 *   when signaled it will stop after finishing it's current delivery (if any)
 */
void *driverThread(void *arg) {
    // note this pointer is to shared data:
    struct Driver *driver = (struct Driver *) arg;

    struct Order* order = NULL;
    while (1) {
        // get lock
        pthread_mutex_lock(&data_mutex);
        // end thread if we are requested to stop
        if (driver->stop == 1) {
            pthread_mutex_unlock(&data_mutex);
            break;
        }
        // check if we have an order to process
        if (driver->orders == NULL) {
            pthread_mutex_unlock(&data_mutex);
            continue;
        }
        // pop order off linked list
        order = driver->orders;
        driver->orders = driver->orders->next;
        order->next = NULL;
        driver->status = 1; // set status to busy
        pthread_mutex_unlock(&data_mutex);

        driverTravel(driver, order->rLoc, order->id, 0); // travel to desired restaurant to pick up order
        driverTravel(driver, order->cLoc, order->id, 1); // then travel to customer
        free(order); // free memory
        order = NULL;
    }
    if (debug == 1) {
        printf("***driver%d ending!\n", driver->id);
    }
    return NULL;
}

/**
 * thread for reading orders and scheduling drivers to make deliveries
 *
 * When multiple orders arrive simultaneously, or if all drivers are busy,
 * prioritize which order to handle next.
 */
void *restaurantThread(void *arg) {
    struct Params *params = (struct Params *) arg;
    // orders not yet assigned (dynamically allocatd)
    struct Order *orders = NULL;
    int orderID = 0;
    int stop = 0; // scheduling() will set this value to 1 if we should stop this thread

    // read customer orders
    int rNum; // restaurant number
    int cNum; // customer number
    while (fscanf(params->fp, "%d %d\n", &rNum, &cNum) != EOF) {
        if (rNum < 0) {
            // if restaurant number is negative, sleep for 250 ms (and don't treat it as an order)
            usleep(1000 * 250);
            continue;
        }
        // for now just store all orders without assigning them
        struct Order *cur = orders; // last element in linked list
        if (orders == NULL) {
            orders = malloc(sizeof(struct Order));
            cur = orders;
        }
        else {
            while (cur->next != NULL) {
                cur = cur->next;
            }
            cur->next = malloc(sizeof(struct Order));
            cur = cur->next;
        }
        // set values of this order:
        cur->next = NULL;
        cur->id = orderID++;
        cur->rLoc = rNum;
        cur->cLoc = cNum;

        // do scheduling
        orders = scheduling(orders, params->D, 1, &stop);
        if (stop == 1)
            break;
    }
    // after reading all orders, continue scheduling until all have been assigned
    while (orders != NULL) {
        if (stop == 1)
            break;
        orders = scheduling(orders, params->D, 0, &stop);
    }
    // stop this thread when no orders are left to schedule, or the thread was signaled to stop
    if (debug == 1) {
        printf("***Restraunt thread ending!\n");
    }
    freeOrders(orders);  // free memory
    fclose(params->fp);  // close file
    return NULL;
}

/**
 * helper function for restrauntThread()
 * does one cycle of scheduling (assigning orders to drivers)
 * returns the new head of the linked list of unscheduled orders
 *
 * orders:  linked list of orders not yet scheduled
 * D:       number of drivers
 * stop:    pointer to an int
 *              (restrauntThread will check the value and stop if needed)
 *
 */
struct Order *scheduling(struct Order *orders, int D, int print, int* stop) {
    // print out list of unscheduled orders (for debugging)
    struct Order *cur = orders;
    if (print == 1 && debug == 1) {
        printf("\n---queue of orders to be scheduled:---\n");
        while (cur != NULL) {
            printf("\torder %d (%d -> %d)\n", cur->id, cur->rLoc, cur->cLoc);
            cur = cur->next;
        }
        printf("--------------------------------------\n");
    }

    // ***scheduling algorithm:***
    // assign any backlogged orders to available drivers (if possible)
    // (starting with oldest order)
    // NOTE: this is the most activity any thread does while holding the lock
    pthread_mutex_lock(&data_mutex);
    // check if the Restraunt thread has been signaled to stop
    if (rStop == 1) {
        pthread_mutex_unlock(&data_mutex);
        *stop = 1; // indicate that we should stop this thread
        return orders;
    }
    for (int d=0; d<D; d++) {
        if (drivers[d].status == 0 && orders != NULL) {
            //** Restaurant: Assigning order 0 (3 -> 6) to driver 4
            // (if idle then driver.orders is NULL)
            if (debug == 1)
                printf("    ** Restraunt: Assigning order %d (%d -> %d) to driver %d\n",
                        orders->id, orders->rLoc, orders->cLoc, drivers[d].id);
            drivers[d].orders = orders;
            orders = orders->next;
            drivers[d].orders->next = NULL;
            // set driver to busy so we don't assign it another order before it starts this one
            drivers[d].status = 1;
        }
    }
    pthread_mutex_unlock(&data_mutex);
    return orders;
}

/**
 * prints the following:
 *    Status of each driver, either idling or actively delivering.
 *    Number of orders delivered by each driver.
 *    Sum of all orders successfully delivered by all drivers.
 */
void printStatus(struct Driver *drivers, int D) {
    int total = 0; // total orders completed
    pthread_mutex_lock(&data_mutex);
    for (int d=0; d<D; d++) {
        printf("  Driver%d: %s, completed %d orders\n",
                d, (drivers[d].status == 0) ? "idle": "busy", drivers[d].delivered);
        total += drivers[d].delivered;
    }
    printf("  Total completed deliveries: %d\n", total);
    pthread_mutex_unlock(&data_mutex);
}

int main(int argc, char *argv[]) {
    struct Params params; // store info needed by Restaurant thread
    if (argc != 3) {
        printf("2 args required.\n");
        exit(1);
    }
    params.fp = fopen(argv[1], "r");
    if (params.fp == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    // parse file:
    int num1, num2;
    int T = strtol(argv[2], NULL, 10); // time to simulate for
    // parse number of drivers:
    if (fscanf(params.fp, "%d\n", &params.D) == EOF) {
        printf("Error parsing file.\n");
        exit(1);
    }
    // create array of drivers (shared data)
    drivers = malloc(params.D * sizeof(struct Driver));
    if (drivers == NULL) {
        printf("Error allocating memory.\n");
        exit(1);
    }
    // each driver starts at (0, 0)
    memset(drivers, 0, params.D * sizeof(struct Driver));
    int count = 0; // count of lines read (not counting the line read above)
    while (fscanf(params.fp, "%d %d\n", &num1, &num2) != EOF) {
        if (count < 10) {
             // coordinates of: restaurant #count
             restaurants[count][0] = num1;
             restaurants[count][1] = num2;
        }
        else if (10 <= count && count < 20) {
             // coordinates of: customer #(count-10)
             customers[count-10][0] = num1;
             customers[count-10][1] = num2;
        }
        count++;
        if (count == 20)
            break;
    }
    if (count != 20) {
        printf("Error parsing file: at least 21 lines were expected.\n");
        exit(1);
    }

    // print data parsed
    for (int i=0; i<10; i++) {
        printf("Restaurant %d is at %d, %d\n", i, restaurants[i][0], restaurants[i][1]);
    }
    for (int i=0; i<10; i++) {
        printf("Customer %d is at %d, %d\n", i, customers[i][0], customers[i][1]);
    }

    // spawn restaurant and driver threads:
    pthread_mutex_init(&data_mutex, NULL);      // initialize mutex
    for (int d=0; d<params.D; d++) {
        drivers[d].id = d;
        drivers[d].orders = NULL;
        pthread_create(&drivers[d].pth, NULL, driverThread, &drivers[d]);
    }
    pthread_t res_pth; // for Restaurant thread
    pthread_create(&res_pth, NULL, restaurantThread, &params);

    // Once per second display status info:
    int time = 0;
    while (time < T) {
        usleep(1000 * 1000); // wait one second
        time++;
        printf("After %d seconds:\n", time);
        printStatus(drivers, params.D);
    }
    // after T seconds end the simulation (signal Restaurant and each driver to terminate)
    // (drivers will finish their current order)
    if (debug == 1)
        printf("\n\nTime expired: signaling all threads to stop.\n");
    pthread_mutex_lock(&data_mutex);
    for (int d=0; d<params.D; d++) {
        drivers[d].stop = 1;
    }
    rStop = 1;
    pthread_mutex_unlock(&data_mutex);

    // reap all the threads
    for (int d=0; d<params.D; d++) {
        pthread_join(drivers[d].pth, NULL);
    }
    pthread_join(res_pth, NULL);
    // free memory
    for (int d=0; d<params.D; d++) {
        freeOrders(drivers[d].orders);
        drivers[d].orders = NULL;
    }
    free(drivers);
    pthread_mutex_destroy(&data_mutex);
    if (debug == 1)
        printf("\nmain thread done!\n");
    return 0;
}
