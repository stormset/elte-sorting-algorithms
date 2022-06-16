#ifndef SORTING_H
#define SORTING_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono> /*Measuring time*/

class Sorting {
public:
    enum Supported {MERGE_SORT, QUICK_SORT, INSERTION_SORT, BUBBLE_SORT, MAX_SELECT};

    Sorting(bool enableStatistics = false, bool printResult = false) {
        srand(time(NULL));
        stats.doStatistics = enableStatistics;
        stats.printResult = printResult;
    }

    /*
     * SORTING ALGORITHMS
     */
    /*** MergeSort ***/
    template<typename T>
    void mergeSort(std::vector<T>& A){
        stats.startClock();
        ms(A, 0, A.size());
        stats.stopClock().printOutput(A).showStats(true, true).clear();
    }

    template<typename T>
    void ms(std::vector<T>& A, int u, int v){
        if (v - u > 1){
            int m = (u + v) / 2;
            ms(A, u, m);
            ms(A, m, v);
            merge(A, u, m, v);
        }
    }

    template<typename T>
    void merge(std::vector<T>& A, int u, int m, int v){
        int d = m - u;
        T *Z = new T[d];
        for (int i = 0; i < d; ++i) Z[i] = A[u + i];
        /**/stats.moves += d;/**/

        int k = u, j = 0, i = m; /* invariáns: k < i */
        while (j < d && i < v){
            /**/stats.comparison++; stats.moves++;/**/
            if (A[i] < Z[j])
                A[k++] = A[i++];
            else
                A[k++] = Z[j++];
        }

        while (j < d) {
            A[k++] = Z[j++];
            /**/stats.moves++;/**/
        }

        delete[] Z;
    }
    /*** MergeSort END ***/

    /*** QuickSort ***/
    template<typename T>
    void quickSort(std::vector<T>& A){
        stats.startClock();
        qs(A, 0, A.size() - 1);
        stats.stopClock().printOutput(A).showStats(true, true).clear();
    }

    template<typename T>
    void qs(std::vector<T>& A, int p, int r){
        if (r - p > 0){
            int q = partition(A, p, r);
            qs(A, p, q-1);
            qs(A, q+1, r);
        }
    }

    template<typename T>
    int partition(std::vector<T>& A, int p, int r){
        int i = randomBetween(p, r);
        T x = A[i]; A[i] = A[r];
        /**/stats.moves += 2;/**/
        i = p;
        while (i < r && (stats.comparison++, A[i] <= x)) i++;
        if (i < r){
            for (int j = i+1; j < r; ++j) {
                if ((stats.comparison++, A[j] < x)){
                    std::swap(A[j], A[i]);
                    /**/stats.moves += 3;/**/
                    i++;
                }
            }
            A[r] = A[i];
            A[i] = x;
            /**/stats.moves += 2;/**/
        } else{
            A[r]= x;
            /**/stats.moves++;/**/
        }
        return i;
    }
    /*** QuickSort END ***/

    /*** InsertionSort ***/
    template<typename T>
    void insertionSort(std::vector<T>& A){
        stats.startClock();
        for (int i = 1; i < (int) A.size(); ++i) {
            /**/stats.comparison++;/**/
            if (A[i-1] > A[i]){
                T x = A[i];
                A[i] = A[i-1];
                int j = i-2;
                while (j >= 0 && (stats.comparison++, A[j] > x)){
                    /**/stats.moves++;/**/
                    A[j+1] = A[j];
                    j--;
                }
                A[j+1] = x;
                /**/stats.moves += 3;/**/
            }
        }
        stats.stopClock().printOutput(A).showStats(true, true).clear();
    }
    /*** InsertionSort END ***/

    /*** BubbleSort ***/
    template<typename T>
    void bubbleSort(std::vector<T>& A){
        stats.startClock();
        int i = A.size()-1;
        while (i > 0) {
            int last = 0;
            for (int j = 0; j < i; ++j) {
                if ((stats.comparison++, A[j] > A[j+1])){
                    std::swap(A[j], A[j+1]);
                    /**/stats.moves += 3;/**/
                    last = j;
                }
            }
            i = last;
        }
        stats.stopClock().printOutput(A).showStats(true, true).clear();
    }
    /*** BubbleSort END ***/

    /*** MaxSelectSort ***/
    template<typename T>
    void maxSelectSort(std::vector<T>& A){
        stats.startClock();
        for (int i = A.size()-1; i > 0; --i) {
            T max = A[0]; int id = 0;
            /**/stats.moves++;/**/
            for (int j = 1; j <= i; ++j) {
                if ((stats.comparison++, A[j] > max)) {
                    max = A[j];
                    /**/stats.moves++;/**/
                    id = j;
                }
            }
            std::swap(A[i], A[id]);
            /**/stats.moves += 3;/**/
        }
        stats.stopClock().printOutput(A).showStats(true, true).clear();
    }
    /*** MaxSelectSort ***/

    /*
     * SORTING ALGORITHMS END
     */

    //Helper
    template<typename T>
    static void printVector(const std::vector<T>& vec){
        std::cout<<"[";
        for (const T& item : vec) {
            std::cout<<item;
            if (&item != &vec.back()) std::cout<<", ";
        }
        std::cout<<"]\n";
    }

private:
    struct Statistics {
        std::chrono::time_point<std::chrono::system_clock> start;
        std::chrono::time_point<std::chrono::system_clock> stop;
        long moves = 0;
        long comparison = 0;
        bool doStatistics = false;
        bool printResult = true;

        void startClock(){
            if (doStatistics)
                start = std::chrono::high_resolution_clock::now();
        }
        Statistics& stopClock(){
            if (doStatistics)
                stop = std::chrono::high_resolution_clock::now();
            return *this;
        }
        double getDuration() const{
            return std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
        }
        Statistics& showStats(bool showMoves = true, bool showComparison = true){
            if (doStatistics){
                double time = getDuration();
                auto prec = std::cout.precision();
                std::cout << "Time: " << time << " ms (~" << std::setprecision(2)<<time/10e5<<" sec)\n"<<std::setprecision(prec);
                if (showMoves)
                    std::cout << "Moves: " << moves << "\n";
                if (showComparison)
                    std::cout << "Comparisons: " << comparison << "\n";
            }
            return *this;
        }
        template<typename T>
        Statistics& printOutput(const std::vector<T>& vec){
            if (printResult){
                std::cout<<"Result: "; printVector(vec);
            }
            return *this;
        }
        void clear(){
            moves = comparison = 0;
        }
    };

    static int randomBetween(int from, int to){
        return rand() % (to - from + 1) + from;
    }

    Statistics stats{};
};

#endif //SORTING_H
