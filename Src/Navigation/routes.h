#ifndef ROUTES_H
#define ROUTES_H

#include "motion.h"
#include "nav_common.h"

// Pass the route depending on the path detected
const Move *Route_For_Path(Path p);

#endif // ROUTES_H
