#include <iostream>
#include <fstream>
#include <vector>
#include "argument_parser.h"
#include "sorting.h"

#define TYPE int

using namespace std;

/*
 * Egyes tanult rendezési algoritmusok kipróbálása.
 */

template<typename T>
bool readFromFile(vector<T>& vec, const vector<char*>& filenames);
template<typename T>
void doSorting(const vector<T> &vec, Sorting& sorting, Sorting::Supported algorithm);

/*
 * Megjegyzés: merge sort - tömb esetén ~700-800 ms-sel gyorsabb (10000db int)
 */
int main(int argc, char **argv) {
    // Parse parameters
    ParameterProcessor pp(argc, argv);
    try {
        pp.addSupported('f', "Use file as input source.", true, "< filename >")
                .addSupported('h', "Do not print the result of sorting.")
                .addSupported('s', "Enable simple statistics.")
                .addSupported('m', "Use merge sort.")
                .addSupported('q', "Use quick sort.")
                .addSupported('i', "Use insertion sort.")
                .addSupported('b', "Use bubble sort.")
                .addSupported('x', "Use maximum selection sort.")
                .parse();
    }catch (ParameterProcessor::ParseException exception){
        exit(0);
    }

    // Prepare input array
    vector<TYPE> input;
    if (pp.isPresent('f')) {
        if (!readFromFile(input, pp.getAdditionalParams()))
            exit(1);
    } else {
        cout<<"Enter values separated by comma and then input EOF (CTRL+D on Linux / CTRL+Z on Windows)\n";
        TYPE val;
        while (cin>>val) {
            input.push_back(val);
            if (cin.peek() == ',')
                cin.ignore();
        }
    }
    //Sorting::printVector(input);

    //Sorting
    Sorting sorting(pp.isPresent('s'), !pp.isPresent('h')); // Holds the statistics for the invoked algorithm

    bool selected = false;
    if (pp.isPresent('m')) {
        selected = true;
        cout << "\nMerge sort:\n";
        doSorting(input, sorting, Sorting::MERGE_SORT);
    }
    if (pp.isPresent('q')) {
        selected = true;
        cout << "\nQuick sort:\n";
        doSorting(input, sorting, Sorting::QUICK_SORT);
    }
    if (pp.isPresent('i')) {
        selected = true;
        cout << "\nInsertion sort:\n";
        doSorting(input, sorting, Sorting::INSERTION_SORT);
    }
    if (pp.isPresent('b')) {
        selected = true;
        cout << "\nBubble sort:\n";
        doSorting(input, sorting, Sorting::BUBBLE_SORT);
    }
    if (pp.isPresent('x')) {
        selected = true;
        cout << "\nMaximum selection sort:\n";
        doSorting(input, sorting, Sorting::MAX_SELECT);
    }
    if (!selected){
        cout<<"No algorithm was selected. Using default. Use --help to see options.\n";
        cout << "\nMerge sort:\n";
        doSorting(input, sorting, Sorting::MERGE_SORT);
    }

    return 0;
}

//Will copy the content of the vector into an other vector, so we don't sort the original vector, and can test
//other sorting algorithms
template<typename T>
void doSorting(const vector<T> &vec, Sorting& sorting, Sorting::Supported algorithm){
    std::vector<T> copy(vec);
    switch (algorithm) {
        case Sorting::MERGE_SORT:
            sorting.mergeSort(copy);
            break;
        case Sorting::QUICK_SORT:
            sorting.quickSort(copy);
            break;
        case Sorting::INSERTION_SORT:
            sorting.insertionSort(copy);
            break;
        case Sorting::BUBBLE_SORT:
            sorting.bubbleSort(copy);
            break;
        case Sorting::MAX_SELECT:
            sorting.maxSelectSort(copy);
            break;
    }
}

template<typename T>
bool readFromFile(vector<T> &vec, const vector<char *> &filenames) {
    ifstream is;
    //Loop until we don't find a file
    for (char* filename : filenames){
        is.open(filename);
        if (is.fail()) {cerr<<"Can't open file: "<<filename<<"\n"; continue;}

        T elem;
        while (is >> elem) vec.push_back(elem);

        is.close();
        return true;
    }
    return false;
}
