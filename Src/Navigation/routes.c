#include "routes.h"
#include "nav_config.h"

// Path A: wall on the left, drop on the right
static const Move route_A[] = {
    { MOVE_FOLLOW, WALL_LEFT },                      // to the first ledge
    { MOVE_ROTATE, TURN_CORNER_A_DEG },              // left 90
    { MOVE_FOLLOW, WALL_LEFT },                      // to the corner
    { MOVE_ROTATE, TURN_TO_STORAGE_A_DEG },          // left 135
    { MOVE_DRIVE,  STORAGE_DRIVE_MM },               // into the storage unit
    { MOVE_END,    0 }
};

// Path B: drop on the left, wall on the right
static const Move route_B[] = {
    { MOVE_FOLLOW, WALL_RIGHT },
    { MOVE_ROTATE, TURN_CORNER_B_DEG },        // right 90
    { MOVE_FOLLOW, WALL_RIGHT },
    { MOVE_ROTATE, TURN_TO_STORAGE_B_DEG },    // right 135
    { MOVE_DRIVE,  STORAGE_DRIVE_MM },
    { MOVE_END,    0 }
};

const Move *Route_For_Path(Path p)
{
    return (p == PATH_A) ? route_A : route_B;
}
