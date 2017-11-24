#ifndef SHORTEST_UTILS_H
#define SHORTEST_UTILS_H

#include <vector>
#include <functional>
#include <time.h>
#include "defs.h"

double getDistance(Point start, Point end);

std::function<bool(Point)> isPointMatch (Point dest);

void printResult (Accumulator acc, Point start, Point end);

template<typename T>
bool contains (std::vector<T> path, T element, std::function<bool(T)>(*isMatch)(T)) {
    typename std::vector<T>::iterator it = find_if(path.begin(), path.end(), isMatch(element));
    if (it != path.end()) {
        return true;
    }

    return false;
}

template<typename T, typename F>
T withExecTime (F func) {
    clock_t startTime = clock();
    T result = func();
    printf("Time taken: %.2fs\n\n", (double)(clock() - startTime) / CLOCKS_PER_SEC);  
    return result;  
}

#endif
