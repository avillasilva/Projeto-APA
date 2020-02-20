#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

int main(int argc, char *argv[]) {

    string instance;
    cout << "Please, input the instance name: ";
    getline(cin,instance);

    int i,j;
    int dimension;
    int capacity;
    string trash;
    ifstream infile;
    infile.open("instancias_teste/" + instance);

    if (infile.is_open()) {
        
        infile >> trash; // NAME:
        infile >> trash; // Name in fact
        infile >> trash; // DIMENSION:
        infile >> dimension;
        infile >> trash; // CAPACITY:
        infile >> capacity;
        infile >> trash; //DEMAND_SECTION
        
        int demand[dimension];
        
        for (i = 0; i < dimension; i++) {
            infile >> trash;  //Index
            infile >> demand[i];
        }

        int ews[dimension][dimension];  
        
        infile >> trash;  //EDGE_WEIGHT_SECTION

        for(i = 0; i < dimension; i++) {
            for(j = 0; j < dimension; j++) {
                infile >> ews[i][j];
            }
        }

        infile.close();

        int routes[dimension][dimension];          // The routes of each truck
        int cost[dimension];                       // Stores the cost of each truck 
        int remaining_clients = dimension - 1;
        bool visited_clients[dimension];           
        int num_trucks = 0;                        
        int current_pos = 0;                       // The current position of the current truck
        int count = 0;                             // The index to the next empty position of the trucks routes
        int current_capacity;                      // The current capacity of the current truck

        for(int i = 0; i < dimension; i++) {
            for (int j = 0; j < dimension; j++) {
                routes[i][j] = -1;
            }

            visited_clients[i] = false;
            cost[i] = 0;
        } 

        while (remaining_clients > 0) {
            current_pos = 0;
            current_capacity = capacity;
            num_trucks++;
            count = 0;
            int pos = 0;

            while (current_capacity > 0 && remaining_clients > 0) {
                // The minimun distance begins with a large to number to avoid errors
                int min = 999999999;

                // Finds the nearest neighbor that has not been visited
                for (int i = 1; i < dimension; i++) {
                    if (current_pos != i && ews[current_pos][i] < min && !visited_clients[i]) {
                        min = ews[current_pos][i];
                        pos = i;
                    }
                }


                if (demand[pos] <= current_capacity) {
                    routes[num_trucks][count] = pos;
                    cost[num_trucks] += min;
                    count++;
                    current_capacity -= demand[pos];
                    visited_clients[pos] = true;
                    remaining_clients--;
                    current_pos = pos;
                
                // If the capacity of the current truck is not suficient,
                // it will return to the warehouse
                } else {
                    cost[num_trucks] += ews[current_pos][0];
                    break;
                }
            }
        }

        cout << "\n\n" << "Trucks Required: " << num_trucks << "\n\n";

        for(int i = 1; i <= num_trucks; i++) {
            cout << "Total cost of the truck " << i << " : " << cost[i] << "\n";
            cout << "Route of the truck " << i << " : 0 -> ";
            for (int j = 0; j < dimension; j++) {
                if(routes[i][j] > -1) {
                    cout << routes[i][j] << " -> ";
                }
            }
            cout << "0\n\n";
        }
    }

    else {
        cout << "Unable to open file! Please, check the instance name.\n";
    }

    return 0;
}
