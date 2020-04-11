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

int main()
{
    srand(time(NULL));
    
    for(int i = 0; i < 5; i++) {
        cout << rand() % 5 << "\n";
    }
    return 0;
}