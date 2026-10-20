/*
 * routes.c - the pre-planned move list for each path.
 *
 * The route begins with the robot facing down the path (after ORIENT).
 * Each leg follows the wall / ledge until the front cliff sensor sees the
 * drop, backs up, then turns. The route ends with the robot in the centre
 * of the storage unit, ready for DEPOSIT_SEARCH.
 *
 * Turns use TURN(): +ve = left. Flip ROTATE_SIGN in nav_config.h if the
 * robot turns the wrong way everywhere.
 */
#include "routes.h"
#include "nav_config.h"

// Path A: wall on the left, drop on the right
static const Move route_A[] = {
    { MOVE_FOLLOW, WALL_LEFT },                      // to the first ledge
    { MOVE_ROTATE, TURN(TURN_CORNER_A_DEG) },        // left 90
    { MOVE_FOLLOW, WALL_LEFT },                      // to the corner
    { MOVE_ROTATE, TURN(TURN_TO_STORAGE_A_DEG) },    // left 135
    { MOVE_DRIVE,  STORAGE_DRIVE_MM },               // into the storage unit
    { MOVE_END,    0 }
};

// Path B: drop on the left, wall on the right
static const Move route_B[] = {
    { MOVE_FOLLOW, WALL_RIGHT },
    { MOVE_ROTATE, TURN(TURN_CORNER_B_DEG) },        // right 90
    { MOVE_FOLLOW, WALL_RIGHT },
    { MOVE_ROTATE, TURN(TURN_TO_STORAGE_B_DEG) },    // right 135
    { MOVE_DRIVE,  STORAGE_DRIVE_MM },
    { MOVE_END,    0 }
};

const Move *Route_For_Path(Path p)
{
    return (p == PATH_A) ? route_A : route_B;
}
