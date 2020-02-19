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

        int routes[dimension][dimension];
        int remaining_clients = dimension - 1;
        bool visited_clients[dimension];
        int num_trucks = 0;
        int current_pos = 0;
        int count = 0;

        for(int i = 0; i < dimension; i++) {
            for (int j = 0; j < dimension; j++) {
                routes[i][j] = -1;
            }

            visited_clients[i] = false;
        } 

        while (remaining_clients > 0) {
            current_pos = 0;
            int current_capacity = capacity;
            num_trucks++;
            count = 0;
            int pos = 0;

            while (current_capacity > 0 && remaining_clients > 0) {
                int min = 999999999;

                // Encontra o cliente mais próximo na linha que não foi visitado.
                for (int i = 0; i < dimension; i++) {
                    if (current_pos != i && ews[current_pos][i] < min && !visited_clients[i]) {
                        min = ews[current_pos][i];
                        pos = i;
                    }
                }

                cout << "pos: " << pos << " min: " << min << " trucks: " << num_trucks << " current_capacity: " << current_capacity << "\n";

                if (demand[pos] <= current_capacity) {
                    if(!visited_clients[pos]) {
                        routes[num_trucks][count] = pos;
                        count++;
                        current_capacity -= demand[pos];
                        visited_clients[pos] = true;
                        remaining_clients--;
                        current_pos = pos;
                    }
                } else {
                    break;
                }
            }
        }

        for(int i = 1; i < num_trucks; i++) {
            cout << "Rota do caminhao " << i << ": ";
            for (int j = 0; j < dimension; j++) {
                if(routes[i][j] > -1) {
                    cout << routes[i][j] << " ";
                }
            }
            cout << "\n";
        }
    }

    else {
        cout << "Unable to open file! Please, check the instance name.\n";
    }

    return 0;
}