#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

using namespace std;

// Generate a random number between a and b
float RandomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

// Generate N-1 rands between 0 and U
void genRand(vector<double> &randVector, int N, double U){
    for(int i = 0; i < N-1; i++){
        double randNo = RandomFloat(0, U);
        randVector.push_back(randNo);
    }
    randVector.push_back(0);
    randVector.push_back(U);
}


vector<double> merge(vector<double> leftList, vector<double> rightList) {

    // Merge sort based on wikipedia pseudocode
    // http://en.wikipedia.org/wiki/Merge_sort           

    vector<double> sortedVector;

    while (leftList.size() > 0 or rightList.size() > 0){
        if (leftList.size() > 0 and rightList.size() > 0){
            if (leftList.at(0) <= rightList.at(0)){ 
                sortedVector.insert(sortedVector.end(), leftList.at(0));
                leftList.erase(leftList.begin());
            }
            else{
                sortedVector.insert(sortedVector.end(), rightList.at(0));
                rightList.erase(rightList.begin());
            }
        }
        else if (leftList.size() > 0){
            sortedVector.insert(sortedVector.end(), leftList.at(0));
            leftList.erase(leftList.begin());
        }
        else if (rightList.size() > 0){
            sortedVector.insert(sortedVector.end(), rightList.at(0));
            rightList.erase(rightList.begin());
        }
    }

    return sortedVector;              
}

vector<double> mergeSort(vector<double> unsortedList) {

    // Merge sort based on wikipedia pseudocode
    // http://en.wikipedia.org/wiki/Merge_sort

    if (unsortedList.size() <= 1)
        return unsortedList;
    // if unsortedList size is 1, means it is sorted and return 

    // else sort               
    // begin by splitting the list into 2 sublists

    vector<double> leftList, rightList;
    int middleValue = (unsortedList.size() / 2);

    for(int i = 0; i < middleValue; i++){
        leftList.insert(leftList.end(), unsortedList.at(i));

    }

    for(int i = middleValue; i< unsortedList.size(); i++){
        rightList.insert(rightList.end(), unsortedList.at(i));

    }

    // keep calling mergeSort() to further split each sublist until size is 1 for sublist
    leftList = mergeSort(leftList);
    rightList = mergeSort(rightList);

    // merge the sublists and return the resulting merged sublist
    return merge(leftList, rightList);
}

void subRand(vector<double> &randVector, vector<double> &uVector){
    for( int i = 0; i < randVector.size() - 1; i++){
        uVector.insert(uVector.end(), (randVector.at(i+1) - randVector.at(i)));
    }
}

void genPVector(vector<int> &pVector, int N){
    for(int i = 0; i < N; i++){
        double randNo = RandomFloat(1, 6);
        randNo = pow(10.0, randNo);
        pVector.push_back( (int) randNo);
    }

}

int main(int argc, char *argv[]){
    srand(time(NULL)); // Initialise the random seed.

    double U = strtod(argv[1], NULL);// u of the taskset
    int N = strtod(argv[2], NULL);// no. of the tasks
    int no = strtod(argv[3], NULL);// no. of the tasksets

    for (int j = 0; j < no; j++){
        vector<double> randVector;
        vector<double> uVector;
        vector<int> pVector;

        // Generate uVector
        genRand(randVector, N, U);

        vector<double> sortedRandVector = mergeSort(randVector);
        subRand(sortedRandVector, uVector);

        // Generate pVector
        genPVector(pVector, N);

        cout << "<begin task set>\n";
        for (int i = 0; i < N; i++){
            int period = pVector.at(i);
            double uValue = uVector.at(i);
            double executionTime = period * uValue;

            cout << executionTime << " " << period << " " << period << "\n";
        }
        cout << "<end task set>\n";
    }
    return EXIT_SUCCESS;
} 
