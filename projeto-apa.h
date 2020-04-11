#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <random>
#include <ctime>

using namespace std;

#define MAX_INT 2147483647
#define RAND_NUM 5

int dimension, capacity;
int *demand, *cost, *trucks_load;
int **adjacency, **routes;


/**
 * [Method]: calcRouteCost
 * [Usage]: Calculates the total distance traveled by the truck in a given route
 * 
 * @param  route    Route of a truck
 * @param  length   Number of clients served in the route
 */
int calcRouteCost(int *route, int length);


/**
 * [Method]: internalSwap
 * [Usage]: Swap the position of two clients given a route
 * 
 * @param  i        Index of the first client
 * @param  j        Index of the second client
 * @param  route    Route of a truck
 */
void internalSwap(int i, int j, int *route);


/**
 * [Method]: invertion2opt
 * [Usage]: Reverse the position of the clients in an interval of a route
 * 
 * @param  i        Index of the first client
 * @param  j        Index of the second client
 * @param  route    Route of a truck
 */
void invertion2opt(int i, int j, int *route);


/**
 * [Method]: externalSwap
 * [Usage]: Swap the position of two clients given two diferent routes
 * 
 * @param  i        Index of the client in the first route
 * @param  j        Index of the client in the second route
 * @param  route1   Route of a truck
 * @param  route2   Route of a truck
 */
void externalSwap(int i, int j, int *route1, int *route2);


/**
 * [Method]: copyRoute
 * [Usage]: Copy the route of a truck to another array
 * 
 * @param  src      Source route
 * @param  dest     Destination route
 * @param  length   Number of clients served in the source route
 */
void copyRoute(int *src, int *dest, int length);


/**
 * [Method]: printRoute
 * [Usage]: Print the clients served in the route
 * 
 * @param  route    Route of a truck
 * @param  length   Number of clients served in the route
 */
void printRoute(int *route, int length);


/**
 * [Method]: routeDemand
 * [Usage]: Calculates the total demand of the clients served a truck in a given route
 * 
 * @param  route    Route of a truck
 * @param  length   Number of clients served in the route
 */
int routeDemand(int *route, int length);


/**
 * [Method]: applyInternalSwap
 * [Usage]: Apply the internal swap in the given routes
 * 
 * @param  num_trucks           Number of trucks used
 * @param  routes               Array of routes
 * @param  numClientsPerTruck   Array containing the number of clients per truck
 * @param  cost                 Route of a truck
 */
int applyInternalSwap(int num_trucks, int *routes[], int *numClientsPerTruck, int *cost);


/**
 * [Method]: applyInvertion2opt
 * [Usage]: Apply the invertion in the given routes
 * 
 * @param  num_trucks           Number of trucks used
 * @param  routes               Array of routes
 * @param  numClientsPerTruck   Array containing the number of clients per truck
 * @param  cost                 Route of a truck
 */
int applyInvertion2opt(int num_trucks, int *routes[], int *numClientsPerTruck, int *cost);


/**
 * [Method]: applyInvertion2opt
 * [Usage]: Apply the invertion in the given routes
 * 
 * @param  num_trucks           Number of trucks used
 * @param  routes               Array of routes
 * @param  numClientsPerTruck   Array containing the number of clients per truck
 * @param  cost                 Route of a truck
 * @param  trucks_load          Array containing the current load of the trucks used
 */
int applyExternalSwap(int num_trucks, int *routes[], int *numClientsPerTruck, int *cost, int *trucks_load);
