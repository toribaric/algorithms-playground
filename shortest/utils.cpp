#include <iostream>
#include <cmath>
#include "utils.h"
using namespace std;

double getDistance(Point start, Point end) {
    return sqrt(pow(end.x - start.x, 2) + pow(end.y - start.y, 2));
}

function<bool(Point)> isPointMatch (Point dest) {
    return [=](Point p) -> bool {
        return dest.x == p.x && dest.y == p.y;
    };
}

bool wasPrevious(Point *previous, Point current) {
    while (previous != NULL) {
        if (previous->x == current.x && previous->y == current.y) {
            return true;
        }

        previous = previous->prev;
    }

    return false;
}

string generatePath(Point point) {
    string path = "";
    Point *previous = &point;
    while (previous != NULL) {
        string pathNode = "(" + to_string(previous->x) + "," + to_string(previous->y) + ")";
        reverse(pathNode.begin(), pathNode.end());
        path += pathNode;
        previous = previous->prev;
    }

    reverse(path.begin(), path.end());
    return point.path + path;
}

void printResult (Accumulator acc, Point start, Point end) {
    cout << "starting with: " << start.x << ", " << start.y << "\n";
    vector<Point> paths = get<0>(acc);
    for (vector<Point>::iterator i = paths.begin(); i != paths.end(); ++i) {
        cout << "--- DESTINATION --- : (" << end.x << "," << end.y << ")\n";
        cout << "* PATH: " << i->path;     
        cout << "\n* DISTANCE: " << i->distance << "\n";
        cout << "-------------------\n";
    }

    cout << "shortest distance: " << get<0>(acc).back().distance << "\n";
}

template<typename T>
bool contains (vector<T> path, T element, function<bool(T)>(*isMatch)(T));

template<typename T, typename F>
T withExecTime (F func);
