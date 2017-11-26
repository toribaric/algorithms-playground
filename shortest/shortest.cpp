#include <iostream>
#include <vector>
#include <tuple>
#include "defs.h"
#include "utils.h"
using namespace std;

template<typename F1, typename F2>
int invokeCorrectionStep (Point *corrected, bool doCorrect, F1 correctX, F2 correctY) {
    if (doCorrect) {
        corrected->x += correctX();
        corrected->y += correctY();
        return 1;
    }

    return 0;
}

vector<Point> getVertices(Point point) {
    vector<Point> vertices;
    for (const int (&v)[2] : VERTICES) {
        Point connected = {point.x + v[0], point.y + v[1], &point, point.path};
        connected.distance = getDistance(point, connected) + point.distance;
        if (!wasPrevious(&point, connected)) {
            vertices.push_back(connected);
        }
    }

    return vertices;
}

bool isPrematureDirectionChange (Point start, Point end, Point prev, Point current) {
    return getDistance(current, end) > INNER_RADIUS &&
        ((start.x < end.x && current.x < prev.x) ||
         (start.y < end.y && current.y < prev.y) ||
         (start.x > end.x && current.x > prev.x) ||
         (start.y > end.y && current.y > prev.y));
}

bool isOut (Point current) {
    return (abs(current.x) > BOARD_WIDTH || abs(current.y) > BOARD_HEIGHT) ||
        (current.x <= 0 || current.y <= 0);
}

/**
 * Find shortest knight's path between start and end fields.
 * 
 * Do recursive calls for each possible vertex of each point until a vertex
 * matches the destination. There are 8 possible vertices to which the
 * graph can continue from the current one, corresponding to 8 possible
 * L-shaped moves of the knight.
 * 
 * There are 5 conditions under which the recursion for the next vertex
 * won't be done:
 * - the distance between the next vertex and the destination would be 
 * greater than the distance of the last found path - this condition triggers
 * only after the first path has been found and it super-boosts the algorithm
 * convergence - until that happens, the other conditions are cleaning the stack
 * - distance between the next vertex and the destination would be greater than
 * the length of the inner field (standard chessboard length of 8 units)
 * - distance between the current vertex and the destination was within the
 * unit distance (the distance of one L-shape, or one knight move), and the
 * distance between the next vertex and the destination would be greater than
 * the unit distance
 * - the path chagned the direction prematurely - before it entered the "inner
 * radius" (length of 2 unit distances), inside which it's allowed to change
 * directions in order to eventually match the destination (as L-shaped movements
 * can go around the destination a few times before hitting it, but it's never
 * necessary to go outside of the 2-units distance to do it)
 * - next vertex would be outside of the defined field boundaries
 * 
 * @param start starting position.
 * @param current current position.
 * @param end destination.
 * @param acc accumulator holding the shortest paths and their distances.
 * @return acc accumulator holding the shortest paths and their distances.
 */ 
Accumulator findPath(Point start, Point current, Point end, Accumulator acc) {
    double minDst = get<1>(acc);
    if (isPointMatch(end)(current)) {        
        current.path = generatePath(current);
        get<0>(acc).push_back(current);
        get<1>(acc) = current.distance < minDst || minDst == 0 ? current.distance : minDst;
        return acc;
    }

    vector<Point> vertices = getVertices(current);
    double currentDst = getDistance(current, end);
    for (vector<Point>::iterator it = vertices.begin(); it != vertices.end(); ++it) {
        double nextDistane = getDistance(*it, end);
        bool isTooDistant = minDst > 0 && it->distance >= minDst;
        bool leftInnerField = nextDistane > INNER_FIELD_LENGTH;
        bool leftUnitDst = currentDst <= UNIT_DISTANCE && nextDistane > UNIT_DISTANCE;
        if (isTooDistant || leftInnerField || leftUnitDst ||
            isPrematureDirectionChange(start, end, current, *it) || isOut(*it)) {
            continue;
        }

        acc = findPath(start, *it, end, acc);
    }

    return acc;
}

/**
 * Correct starting point coords to within the inner field length.
 * 
 * Do the correction with long L-shaped movements for both axes as long
 * as the starting point isn't within the inner field (the standard
 * chessboard field of 8x8 fields).
 * 
 * @param start starting point.
 * @param end destination.
 * @return corrected starting point.
 */
Point getCorrectedStart (Point start, Point end) {
    bool isXBefore = start.x < end.x;
    bool isYBefore = start.y < end.y;
    Point corrected = {.x = start.x, .y = start.y};
    for (int i = 0, j = 0, k = 0; ;k++) {
        bool xCorrected = abs(end.x - corrected.x) < INNER_FIELD_LENGTH;
        bool yCorrected = abs(end.y - corrected.y) < INNER_FIELD_LENGTH;
        bool doCorrectX = !xCorrected && (k % 2 == 0 || yCorrected);
        bool doCorrectY = !yCorrected && (k % 2 != 0 || xCorrected);
        if (xCorrected && yCorrected) {
            break;
        }

        Point previous = {.x = corrected.x, .y = corrected.y};
        corrected.path += "(" + to_string(corrected.x) + "," + to_string(corrected.y) + ")";
        i += invokeCorrectionStep(&corrected, doCorrectX, [=]() { return isXBefore ? 2 : -2; },
                [=]() { return i % 2 == 0 ? 1 : -1; }); 
        j += invokeCorrectionStep(&corrected, doCorrectY, [=]() { return j % 2 == 0 ? 1 : -1; },
                [=]() { return isYBefore ? 2 : -2; }); 
        corrected.distance += getDistance(previous, corrected);
    }

    return corrected;
}

void findShortestKnightPath(const int (&startCoord)[2], const int (&endCoord)[2]) {
    Point start = {.x = startCoord[0], .y = startCoord[1]};
    Point end = {.x = endCoord[0], .y = endCoord[1]};
    Point correctedStart = getCorrectedStart(start, end);
    Accumulator acc = withExecTime<Accumulator>(
        [&s = correctedStart, &e = end]() {
            return findPath(s, s, e, make_tuple(vector<Point>(), 0.00));
        }
    );
    printResult(acc, correctedStart, end);    
}
