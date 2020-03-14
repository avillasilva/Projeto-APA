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
int *demand, *cost;
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

void swap(int i, int j, int *route, int *newRoute, int length)
{
    for (int k = 0; k < length; k++)
    {
        newRoute[k] = route[k];
    }

    int aux = newRoute[i];
    newRoute[i] = route[j];
    route[j] = aux;
}

void copyRoute(int *src, int *dest, int length)
{
    for (int i = 0; i < length; i++)
    {
        dest[i] = src[i];
    }
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
        cost = new int[dimension]; // Stores the cost of each truck
        int remaining_clients = dimension - 1;
        bool visited_clients[dimension];
        int num_trucks = 0;
        int current_pos = 0;  // The current position of the current truck
        int count = 0;        // The index to the next empty position of the trucks routes
        int current_capacity; // The current capacity of the current truck
        int numClientsPerTruck[dimension];

        for (int i = 0; i < dimension; i++)
        {
            for (int j = 0; j < dimension; j++)
            {
                routes[i][j] = -1;
            }

            visited_clients[i] = false;
            cost[i] = 0;
        }

        while (remaining_clients > 0)
        {
            current_pos = 0;
            current_capacity = capacity;
            num_trucks++;
            count = 0;
            int pos = 0;

            while (current_capacity > 0 && remaining_clients > 0)
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

                if (demand[pos] <= current_capacity)
                {
                    routes[num_trucks][count] = pos;
                    cost[num_trucks] += min;
                    count++;
                    current_capacity -= demand[pos];
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
             << "Naive CVRP:"
             << "\n";
        cout << "\n\n"
             << "Trucks Required: " << num_trucks << "\n\n";

        for (int i = 1; i <= num_trucks; i++)
        {
            cout << "Total cost of the truck " << i << " : " << cost[i] << "\n";
            cout << "number of clients: " << numClientsPerTruck[i] << "\n";
            cout << "Route of the truck " << i << " : 0 -> ";
            for (int j = 0; j < dimension; j++)
            {
                if (routes[i][j] > -1)
                {
                    cout << routes[i][j] << " -> ";
                }
            }
            cout << "0\n\n";
        }

        int newRoute[numClientsPerTruck[1]];
        int x = rand() % numClientsPerTruck[1];
        int y = rand() % numClientsPerTruck[1];
        swap(x, y, routes[1], newRoute, numClientsPerTruck[1]);

        cout << "x: " << x << " y: " << y << "\n";


        cout << "Rota de origem:\n";
        for (int i = 0; i < numClientsPerTruck[1]; i++) {
            cout << routes[1][i] << " ";
        }

        cout << "\nRota de destino:\n";
        for (int i = 0; i < numClientsPerTruck[1]; i++) {
            cout << newRoute[i] << " ";
        }
        cout << "\n";

        cout << "new cost: " << calcRouteCost(newRoute, numClientsPerTruck[1]) << "\n";

        // for (int i = 1; i <= num_trucks; i++)
        // {
        //     if (numClientsPerTruck[i] > 1)
        //     {
        //         int newCost;
        //         int newRoute[numClientsPerTruck[i]];
        //         int limit = 5;

        //         while (limit >= 0)
        //         {
        //             int x = rand() % numClientsPerTruck[i];
        //             int y = rand() % numClientsPerTruck[i];

        //             swap(x, y, routes[i], newRoute, numClientsPerTruck[i]);

        //             cout << "Rota o\n";

        //             for (int k = 0; k < numClientsPerTruck[i]; k++)
        //             {
        //                 cout << routes[i][k] << " ";
        //             }

        //             cout << "\nRota 1\n";
        //             for (int k = 0; k < numClientsPerTruck[i]; k++)
        //             {
        //                 cout << routes[i][k] << " ";
        //             }

        //             cout << "\n";
        //             newCost = calcRouteCost(newRoute, numClientsPerTruck[i]);

        //             if (newCost < cost[i])
        //             {
        //                 copyRoute(newRoute, routes[i], numClientsPerTruck[i]);
        //                 cost[i] = newCost;
        //             }

        //             limit--;
        //         }
        //     }
        // }

        for (int i = 0; i < num_trucks; i++)
        {
            cout << "novo custo: " << cost[i] << "\n";
        }
    }

    else
    {
        cout << "Unable to open file! Please, check the instance name.\n";
    }

   return 0;
}