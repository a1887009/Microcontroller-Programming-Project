/*
 * routes.c - the pre-planned move list for each path.
 *
 * The route begins with the robot oriented in the path direction.
 * The route has three types of movement; drive, rotate, and stop
 * The route ends with the robot in the centre of the storage unit 
 * ready to perform the DEPOSIT_SEARCH to orient to the direction 
 * of the deposit marker.
 *
 */
#include "routes.h"

static const Move route_A[] = {
    { MOVE_DRIVE,  1000 },
    { MOVE_ROTATE,   90 },
    { MOVE_DRIVE,  1000 },
    { MOVE_ROTATE,   45 },
    { MOVE_DRIVE,   400 },
    { MOVE_END,       0 }
};

static const Move route_B[] = {
    { MOVE_DRIVE,  1000 },
    { MOVE_ROTATE,  -90 },
    { MOVE_DRIVE,  1000 },
    { MOVE_ROTATE,  -45 },
    { MOVE_DRIVE,   400 },
    { MOVE_END,       0 }
};

const Move *Route_For_Path(Path p)
{
    return (p == PATH_A) ? route_A : route_B;
}
