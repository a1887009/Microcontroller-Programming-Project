/*
 * motion.c - precise turns and drives using the P-controller wrappers.
 *
 * Pattern for every move: setpoint once -> controller every loop ->
 * stop when the complete flag is set.
 *
 * Two things to know about the wrappers:
 *  - The complete flag is copied in Kobuki_Read(). Setpoints here are always
 *    set AFTER that loop's read, so the next loop's flag is already fresh.
 *  - Once complete, the controller stops sending commands, and the Kobuki
 *    keeps its last speed - so we send STOP explicitly.
 */
#include "motion.h"
#include "nav_config.h"

// Orient is used for a single rotation
void Orient_Start(const Kobuki_Typedef *k, int16_t turn_deg)
{
    Kobuki_Rotate_Angle_Setpoint(k->angle, Wrap_Deg(turn_deg));
}

Task_Status Orient_Run(const Kobuki_Typedef *k)
{
    if (k->rotation_complete) {
        Kobuki_Drive(STOP);
        return TASK_DONE;
    }
    Kobuki_Rotate_Angle(k->angle);
    return TASK_RUNNING;
}


// Moves are used to follow a list of instructions

// FOLLOW has its own small state machine
typedef enum { FOLLOW_DRIVE, FOLLOW_CORRECT, FOLLOW_BACKUP } Follow_Phase;

static struct {
    const Move   *list;
    uint8_t       index;
    Follow_Phase  phase;        // FOLLOW only
    uint8_t       cliff_count;  // FOLLOW only: front cliff loops in a row
    uint16_t      loops;        // FOLLOW only: for the timeout
} moves;

// Define the targets
static void Moves_Begin_Current(const Kobuki_Typedef *k)
{
    const Move *m = &moves.list[moves.index];
    if (m->type == MOVE_ROTATE) {
        Kobuki_Rotate_Angle_Setpoint(k->angle, m->value);
    } else if (m->type == MOVE_DRIVE) {
        Kobuki_Drive_Distance_Setpoint(k->distance, m->value);
    } else if (m->type == MOVE_FOLLOW) {
        moves.phase       = FOLLOW_DRIVE;
        moves.cliff_count = 0;
        moves.loops       = 0;
    }
}

// Current move finished: stop and set up the next one
static Task_Status Moves_Next(const Kobuki_Typedef *k)
{
    Kobuki_Drive(STOP);
    moves.index++;
    Moves_Begin_Current(k);
    return TASK_RUNNING;
}

// Which way (if any) to turn away from a side bumper / side cliff.
// Returns degrees, +ve = left, 0 = nothing touched.
static int16_t Side_Correction(const Kobuki_Typedef *k, int16_t wall_side)
{
    if (k->cliff  & SENSOR_RIGHT)  return  SIDE_CORRECT_DEG;   // ledge on the right -> turn left
    if (k->cliff  & SENSOR_LEFT)   return -SIDE_CORRECT_DEG;   // ledge on the left  -> turn right
    if (k->bumper & SENSOR_LEFT)   return -SIDE_CORRECT_DEG;   // wall on the left   -> turn right
    if (k->bumper & SENSOR_RIGHT)  return  SIDE_CORRECT_DEG;   // wall on the right  -> turn left
    if (k->bumper & SENSOR_CENTRE) {                           // head-on: turn away from the wall side
        return (wall_side == WALL_LEFT) ? -FRONT_CORRECT_DEG : FRONT_CORRECT_DEG;
    }
    return 0;
}

// Drive along a wall / ledge until the front cliff sees the drop
static Task_Status Follow_Run(const Kobuki_Typedef *k, int16_t wall_side)
{
    switch (moves.phase) {
        case FOLLOW_DRIVE:
            if (++moves.loops > LEG_TIMEOUT_LOOPS) {   // never found the ledge
                Kobuki_Drive(STOP);
                return TASK_FAILED;
            }

            // End of leg: front cliff. Stop at once, confirm, then back up.
            if (k->cliff & SENSOR_CENTRE) {
                Kobuki_Drive(STOP);
                if (++moves.cliff_count >= CLIFF_CONFIRM_LOOPS) {
                    Kobuki_Drive_Distance_Setpoint(k->distance, -LEG_BACKUP_MM);
                    moves.phase = FOLLOW_BACKUP;
                }
                return TASK_RUNNING;
            }
            moves.cliff_count = 0;

            // Side contact: turn a little away from it, then carry on
            {
                int16_t turn = Side_Correction(k, wall_side);
                if (turn != 0) {
                    Kobuki_Drive(STOP);
                    Kobuki_Rotate_Angle_Setpoint(k->angle, TURN(turn));
                    moves.phase = FOLLOW_CORRECT;
                    return TASK_RUNNING;
                }
            }

            Kobuki_Drive(LEG_SPEED);
            return TASK_RUNNING;

        case FOLLOW_CORRECT:
            if (k->rotation_complete) {
                moves.phase = FOLLOW_DRIVE;
                Kobuki_Drive(LEG_SPEED);        // drive straight on
                return TASK_RUNNING;
            }
            Kobuki_Rotate_Angle(k->angle);
            return TASK_RUNNING;

        case FOLLOW_BACKUP:
            if (k->distance_complete) {
                return Moves_Next(k);           // leg done
            }
            Kobuki_Drive_Distance(k->distance);
            return TASK_RUNNING;
    }
    return TASK_FAILED;
}

// Point the local moves structs to the lists of moves
void Moves_Start(const Kobuki_Typedef *k, const Move *list)
{
    moves.list  = list;
    moves.index = 0;
    Moves_Begin_Current(k);
}

Task_Status Moves_Run(const Kobuki_Typedef *k)
{
    const Move *m = &moves.list[moves.index];

    switch (m->type) {
        case MOVE_END:
            Kobuki_Drive(STOP);
            return TASK_DONE;

        case MOVE_FOLLOW:
            return Follow_Run(k, m->value);

        case MOVE_ROTATE:
            if (k->rotation_complete) return Moves_Next(k);
            Kobuki_Rotate_Angle(k->angle);
            return TASK_RUNNING;

        case MOVE_DRIVE:
            if (k->distance_complete) return Moves_Next(k);
            Kobuki_Drive_Distance(k->distance);
            return TASK_RUNNING;
    }
    return TASK_FAILED;
}
