#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;
int dimension, capacity;
int *demand, *cost, *trucks_load;
int **adjacency, **routes;

int calcRouteCost(int *route, int length)
{
    int sum = 0;
    int lastPosition = 0;
    int currentPosition = route[0];

    for (int i = 1; i <= length; i++)
    {
        sum += adjacency[lastPosition][currentPosition];
        lastPosition = currentPosition;
        currentPosition = route[i];
    }

    sum += adjacency[lastPosition][0];

    return sum;
}

void internalSwap(int i, int j, int *route, int *newRoute)
{
    int aux = newRoute[i];
    newRoute[i] = newRoute[j];
    newRoute[j] = aux;
}

// MAYBE NOT
// void insertion(int i, int j, int *route, int *newRoute, int length)
// {
//     int k;

//     for(k = 0; k < j; k++) {
//         newRoute[k] = route[i + 1];
//     }

//     newRoute[k] = route[i];

//     for(k = j+1; k < length; k++) {
//         newRoute[k] = route[k];
//     }
// }

void invertion2opt(int i, int j, int *route)
{
    for (int k = i, l = j; k < l; k++, l--)
    {
        int aux = route[k];
        route[k] = route[l];
        route[l] = aux;
    }
}

void intraSwap(int i, int j, int *route1, int *route2)
{
    int aux = route1[i];
    route1[i] = route2[j];
    route2[j] = aux;
}

void copyRoute(int *src, int *dest, int length)
{
    for (int i = 0; i < length; i++)
    {
        dest[i] = src[i];
    }
}

void printRoute(int *route, int length)
{
    for (int i = 0; i < length; i++)
    {
        cout << route[i] << " ";
    }
    cout << "\n";
}

int main(int argc, char *argv[])
{
    string instance;
    time_t t;
    cout << "Please, input the instance name: ";
    getline(cin, instance);

    string trash;
    ifstream infile;
    infile.open("instancias_teste/" + instance);
    srand((unsigned)time(&t));

    if (infile.is_open())
    {

        infile >> trash; // NAME:
        infile >> trash; // Name in fact
        infile >> trash; // DIMENSION:
        infile >> dimension;
        infile >> trash; // CAPACITY:
        infile >> capacity;
        infile >> trash; //DEMAND_SECTION

        demand = new int[dimension];

        for (int i = 0; i < dimension; i++)
        {
            infile >> trash; //Index
            infile >> demand[i];
        }

        adjacency = new int *[dimension];
        for (int i = 0; i < dimension; i++)
        {
            adjacency[i] = new int[dimension];
        }

        infile >> trash; //EDGE_WEIGHT_SECTION

        for (int i = 0; i < dimension; i++)
        {
            for (int j = 0; j < dimension; j++)
            {
                infile >> adjacency[i][j];
            }
        }

        infile.close();

        routes = new int *[dimension]; // The routes of each truck
        for (int i = 0; i < dimension; i++)
        {
            routes[i] = new int[dimension];
        }
        trucks_load = new int[dimension];
        cost = new int[dimension]; // Stores the cost of each truck
        int remaining_clients = dimension - 1;
        bool visited_clients[dimension];
        int num_trucks = 0;
        int current_pos = 0; // The current position of the current truck
        int count = 0;       // The index to the next empty position of the trucks routes
        int numClientsPerTruck[dimension];

        for (int i = 0; i < dimension; i++)
        {
            for (int j = 0; j < dimension; j++)
            {
                routes[i][j] = -1;
            }

            visited_clients[i] = false;
            cost[i] = 0;
            trucks_load[i] = 0;
        }

        while (remaining_clients > 0)
        {
            num_trucks++;
            current_pos = 0;
            count = 0;
            int pos = 0;

            while (trucks_load[num_trucks] <= capacity && remaining_clients > 0)
            {
                // The minimun distance begins with a large to number to avoid errors
                int min = 999999999;

                // Finds the nearest neighbor that has not been visited
                for (int i = 1; i < dimension; i++)
                {
                    if (current_pos != i && adjacency[current_pos][i] < min && !visited_clients[i])
                    {
                        min = adjacency[current_pos][i];
                        pos = i;
                    }
                }

                if (demand[pos] <= capacity - trucks_load[num_trucks])
                {
                    routes[num_trucks][count] = pos;
                    cost[num_trucks] += min;
                    count++;
                    trucks_load[num_trucks] += demand[pos];
                    visited_clients[pos] = true;
                    remaining_clients--;
                    current_pos = pos;
                    numClientsPerTruck[num_trucks] = count;

                    // If the capacity of the current truck is not suficient,
                    // it will return to the warehouse
                }
                else
                {
                    cost[num_trucks] += adjacency[current_pos][0];
                    break;
                }
            }
        }

        cout << "\n\n"
             << "*****  Naive CVRP  *****"
             << "\n";
        cout << "\n\n"
             << "Trucks Required: " << num_trucks << "\n\n";

        int totalCost = 0;
        for (int i = 1; i <= num_trucks; i++)
        {
            cout << "Total cost of the truck " << i << " : " << cost[i] << "\n";
            cout << "Number of clients: " << numClientsPerTruck[i] << "\n";
            cout << "Route of the truck " << i << " : 0 -> ";
            totalCost += cost[i];
            for (int j = 0; j < dimension; j++)
            {
                if (routes[i][j] > -1)
                {
                    cout << routes[i][j] << " -> ";
                }
            }
            cout << "0\n\n";
        }
        cout << "Total Cost: " << totalCost << "\n\n";

        for (int i = 1; i <= num_trucks; i++)
        {
            int newCost;
            int newRoute[numClientsPerTruck[i]];

            for (int j = 0; j < numClientsPerTruck[i]; j++)
            {
                for (int k = j + 1; k < numClientsPerTruck[i]; k++)
                {
                    copyRoute(routes[i], newRoute, numClientsPerTruck[i]);
                    internalSwap(j, k, routes[i], newRoute);

                    newCost = calcRouteCost(newRoute, numClientsPerTruck[i]);

                    if (newCost < cost[i])
                    {
                        cout << "Change in the route " << i << "\n";
                        copyRoute(newRoute, routes[i], numClientsPerTruck[i]);
                        cost[i] = newCost;
                    }
                }
            }
        }

        cout << "\n\n***** After Internal-Swap ******:\n\n";

        totalCost = 0;
        for (int i = 1; i <= num_trucks; i++)
        {
            cout << "Total cost of the truck " << i << " : " << cost[i] << "\n";
            cout << "Number of clients: " << numClientsPerTruck[i] << "\n";
            cout << "Truck Load: " << trucks_load[i] << "\n";
            cout << "Route of the truck " << i << " : 0 -> ";
            totalCost += cost[i];
            for (int j = 0; j < dimension; j++)
            {
                if (routes[i][j] > -1)
                {
                    cout << routes[i][j] << " -> ";
                }
            }
            cout << "0\n\n";
        }
        cout << "Total Cost: " << totalCost << "\n\n";

        for (int i = 1; i <= num_trucks; i++)
        {
            int newCost;
            int newRoute[numClientsPerTruck[i]];

            for (int j = 0; j < numClientsPerTruck[i]; j++)
            {
                for (int k = j + 1; k < numClientsPerTruck[i]; k++)
                {
                    copyRoute(routes[i], newRoute, numClientsPerTruck[i]);
                    invertion2opt(j, k, newRoute);

                    newCost = calcRouteCost(newRoute, numClientsPerTruck[i]);

                    if (newCost < cost[i])
                    {
                        cout << "Change in the route " << i << "\n";
                        copyRoute(newRoute, routes[i], numClientsPerTruck[i]);
                        cost[i] = newCost;
                    }
                }
            }
        }

        cout << "\n\n***** After Invertion2opt ******:\n\n";

        totalCost = 0;
        for (int i = 1; i <= num_trucks; i++)
        {
            cout << "Total cost of the truck " << i << " : " << cost[i] << "\n";
            cout << "Number of clients: " << numClientsPerTruck[i] << "\n";
            cout << "Truck Load: " << trucks_load[i] << "\n";
            cout << "Route of the truck " << i << " : 0 -> ";
            totalCost += cost[i];
            for (int j = 0; j < dimension; j++)
            {
                if (routes[i][j] > -1)
                {
                    cout << routes[i][j] << " -> ";
                }
            }
            cout << "0\n\n";
        }
        cout << "Total Cost: " << totalCost << "\n\n";

        for (int i = 1; i <= num_trucks; i++)
        {
            for (int j = i + 1; j <= num_trucks; j++)
            {
                int newCost1, newCost2;
                int newRoute1[numClientsPerTruck[i]];
                int newRoute2[numClientsPerTruck[j]];

                for (int k = 0; k < numClientsPerTruck[i]; k++)
                {
                    for (int l = 0; l < numClientsPerTruck[j]; l++)
                    {
                        copyRoute(routes[i], newRoute1, numClientsPerTruck[i]);
                        copyRoute(routes[j], newRoute2, numClientsPerTruck[j]);
                        intraSwap(k, l, newRoute1, newRoute2);

                        newCost1 = calcRouteCost(newRoute1, numClientsPerTruck[i]);
                        newCost2 = calcRouteCost(newRoute2, numClientsPerTruck[j]);

                        if (newCost1 + newCost2 < cost[i] + cost[j])
                        {
                            cout << "Change in the routes " << i << " " << j << "\n";
                            copyRoute(newRoute1, routes[i], numClientsPerTruck[i]);
                            copyRoute(newRoute2, routes[j], numClientsPerTruck[j]);
                            cost[i] = newCost1;
                            cost[j] = newCost2;
                        }
                    }
                }
            }
        }

        cout << "\n\n***** After INTRA-SWAP ******:\n\n";

        totalCost = 0;
        for (int i = 1; i <= num_trucks; i++)
        {
            cout << "Total cost of the truck " << i << " : " << cost[i] << "\n";
            cout << "Number of clients: " << numClientsPerTruck[i] << "\n";
            cout << "Truck Load: " << trucks_load[i] << "\n";
            cout << "Route of the truck " << i << " : 0 -> ";
            totalCost += cost[i];
            for (int j = 0; j < dimension; j++)
            {
                if (routes[i][j] > -1)
                {
                    cout << routes[i][j] << " -> ";
                }
            }
            cout << "0\n\n";
        }
        cout << "Total Cost: " << totalCost << "\n\n";
    }

    else
    {
        cout << "Unable to open file! Please, check the instance name.\n";
    }

    return 0;
}
