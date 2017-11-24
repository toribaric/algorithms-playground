#ifndef SHORTEST_DEFS_H
#define SHORTEST_DEFS_H

#include <vector>
#include <tuple>

#define BOARD_WIDTH 1000
#define BOARD_HEIGHT 1000
#define UNIT_DISTANCE 2.23607
#define INNER_RADIUS UNIT_DISTANCE * 2
#define INNER_FIELD_LENGTH 8
#define NUM_VERTICES 8
#define VERTICES_DIMENSION 2

const int VERTICES[NUM_VERTICES][VERTICES_DIMENSION] = {
    {1, -2}, {2, -1}, {1, 2}, {2, 1}, {-1, -2}, {-2, -1}, {-1, 2}, {-2, 1}
};

typedef struct point {
    int x;
    int y;
    std::vector<struct point> path;
    double distance;
} Point;

typedef std::tuple<std::vector<Point>, double> Accumulator;

#endif
