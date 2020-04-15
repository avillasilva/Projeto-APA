#include "projeto-apa.h"

int main(int argc, char *argv[])
{
    string trash;
    ifstream infile;
    srand(time(NULL));
    infile.open(argv[1]);
    cout.precision(10);

    clock_t start, end;
    double cpu_time_used;

    if (infile.is_open())
    {

        infile >> trash; // string NAME:
        infile >> trash; // Name in fact
        infile >> trash; // string DIMENSION:
        infile >> dimension;
        infile >> trash; // string CAPACITY:
        infile >> capacity;
        infile >> trash; // string DEMAND_SECTION

        demand = new int[dimension];

        for (int i = 0; i < dimension; i++)
        {
            infile >> trash; // string Index
            infile >> demand[i];
        }

        adjacency = new int *[dimension];
        for (int i = 0; i < dimension; i++)
        {
            adjacency[i] = new int[dimension];
        }

        infile >> trash; // string EDGE_WEIGHT_SECTION

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

        for (int i = 0; i < dimension; i++) //Initialize variables
        {
            for (int j = 0; j < dimension; j++)
            {
                routes[i][j] = -1;
            }
            visited_clients[i] = false;
            cost[i] = 0;
            trucks_load[i] = 0;
        }

        start = clock();
        while (remaining_clients > 0)
        {
            num_trucks++;
            current_pos = 0;
            count = 0;
            while (trucks_load[num_trucks] <= capacity && remaining_clients > 0)
            {
                // The minimun distance begins with a large to number to avoid errors
                int min, pos, current_min = 0;
                // Finds the nearest neighbor that has not been visited
                int mins[RAND_NUM], positions[RAND_NUM];
                for (int i = 0; i < RAND_NUM; i++)
                {
                    min = 99999999;
                    for (int j = 1; j < dimension; j++)
                    {
                        if (current_pos != j && adjacency[current_pos][j] < min && 
                        adjacency[current_pos][j] > current_min && !visited_clients[j])
                        {
                            min = adjacency[current_pos][j];
                            pos = j;
                        }
                    }
        
                    current_min = min;
                    mins[i] = min;
                    positions[i] = pos;
                }
                
                int trand = rand() % RAND_NUM;
                // Put the client in the route of the truck and update the variables
                if(mins[trand] != 99999999){
                    if (demand[positions[trand]] <= capacity - trucks_load[num_trucks])
                    {
                        routes[num_trucks][count] = positions[trand];
                        cost[num_trucks] += mins[trand];
                        count++;
                        trucks_load[num_trucks] += demand[positions[trand]];
                        visited_clients[positions[trand]] = true;
                        remaining_clients--;
                        current_pos = positions[trand];
                        numClientsPerTruck[num_trucks] = count;
                    }
                    // If the capacity of the current truck is not suficient, it will return to the warehouse
                    else
                    {
                        cost[num_trucks] += adjacency[current_pos][0];
                        break;
                    }
                }
                
            }
        }
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        cout << "Greedy time: "<< fixed << cpu_time_used << "\n";

        int totalCost = 0;
        int i = 0;
        int result;

        cout << "\n\n********** Greedy Algorithm ***********:\n\n";
        cout << "Trucks Required: " << num_trucks << "\n\n";
        totalCost = 0;
        for (int i = 1; i <= num_trucks; i++)
        {
            cout << "Total cost of the truck " << i << " : " << cost[i] << "\n";
            cout << "Number of clients: " << numClientsPerTruck[i] << "\n";
            cout << "Truck Load: " << trucks_load[i] << "\n";
            cout << "Route of the truck " << i << " :";
            totalCost += cost[i];
            printRoute(routes[i], numClientsPerTruck[i]);
            cout << "-----------------------------------------------------------------------------------------------------"
                 << "\n";
        }
        cout << "Total Cost: " << totalCost << "\n\n\n";

        // VND
        start = clock();
        while (i < 3)
        {
            switch (i)
            {
            case 0:
                result = applyInternalSwap(num_trucks, routes, numClientsPerTruck, cost);
                break;

            case 1:
                result = applyExternalSwap(num_trucks, routes, numClientsPerTruck, cost, trucks_load);
                break;

            case 2:
                result = applyInvertion2opt(num_trucks, routes, numClientsPerTruck, cost);
                break;
            }

            result ? i = 0 : i++;
        }
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        cout << "VND time: " << cpu_time_used << "\n";

        cout << "\n\n********** After VND ***********:\n\n";
        cout << "Trucks Required: " << num_trucks << "\n\n";
        totalCost = 0;
        for (int i = 1; i <= num_trucks; i++)
        {
            cout << "Total cost of the truck " << i << " : " << cost[i] << "\n";
            cout << "Number of clients: " << numClientsPerTruck[i] << "\n";
            cout << "Truck Load: " << trucks_load[i] << "\n";
            cout << "Route of the truck " << i << " :";
            totalCost += cost[i];
            printRoute(routes[i], numClientsPerTruck[i]);
            cout << "-----------------------------------------------------------------------------------------------------"
                 << "\n";
        }
        cout << "Total Cost: " << totalCost << "\n\n\n";
    }

    else
    {
        cout << "Unable to open file! Please, check the instance name.\n";
    }
    cout << "============================================================================================================================================" << "\n";
    return 0;
}

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

void internalSwap(int i, int j, int *route)
{
    int aux = route[i];
    route[i] = route[j];
    route[j] = aux;
}

void invertion2opt(int i, int j, int *route)
{
    for (int k = i, l = j; k < l; k++, l--)
    {
        int aux = route[k];
        route[k] = route[l];
        route[l] = aux;
    }
}

void externalSwap(int i, int j, int *route1, int *route2)
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
    cout << ": 0 -> ";
    for (int i = 0; i < length; i++)
    {
        cout << route[i] << " -> ";
    }
    cout << "0"
         << "\n";
}

int routeDemand(int *route, int length)
{
    int sum = 0;
    for (int i = 0; i < length; i++)
    {
        sum += demand[route[i]];
    }

    return sum;
}

int applyInternalSwap(int num_trucks, int *routes[], int *numClientsPerTruck, int *cost)
{
    int check = 0;

    for (int i = 1; i <= num_trucks; i++)
    {
        int newCost;
        int newRoute[numClientsPerTruck[i]];

        for (int j = 0; j < numClientsPerTruck[i]; j++)
        {
            for (int k = 0; k < numClientsPerTruck[i]; k++)
            {
                copyRoute(routes[i], newRoute, numClientsPerTruck[i]);
                internalSwap(j, k, newRoute);

                newCost = calcRouteCost(newRoute, numClientsPerTruck[i]);

                if (newCost < cost[i])
                {
                    copyRoute(newRoute, routes[i], numClientsPerTruck[i]);
                    cost[i] = newCost;
                    check = 1;
                }
            }
        }
    }

    return check;
}

int applyInvertion2opt(int num_trucks, int *routes[], int *numClientsPerTruck, int *cost)
{
    int check = 0;

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
                    copyRoute(newRoute, routes[i], numClientsPerTruck[i]);
                    cost[i] = newCost;
                    check = 1;
                }
            }
        }
    }

    return check;
}

int applyExternalSwap(int num_trucks, int *routes[], int *numClientsPerTruck, int *cost, int *trucks_load)
{
    int check = 0;

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
                    externalSwap(k, l, newRoute1, newRoute2);

                    if (routeDemand(newRoute1, numClientsPerTruck[i]) <= capacity && routeDemand(newRoute2, numClientsPerTruck[j]) <= capacity)
                    {

                        newCost1 = calcRouteCost(newRoute1, numClientsPerTruck[i]);
                        newCost2 = calcRouteCost(newRoute2, numClientsPerTruck[j]);

                        if (newCost1 + newCost2 < cost[i] + cost[j])
                        {
                            copyRoute(newRoute1, routes[i], numClientsPerTruck[i]);
                            copyRoute(newRoute2, routes[j], numClientsPerTruck[j]);
                            cost[i] = newCost1;
                            cost[j] = newCost2;
                            trucks_load[i] = routeDemand(newRoute1, numClientsPerTruck[i]);
                            trucks_load[j] = routeDemand(newRoute2, numClientsPerTruck[j]);
                            check = 1;
                        }
                    }
                }
            }
        }
    }

    return check;
}
