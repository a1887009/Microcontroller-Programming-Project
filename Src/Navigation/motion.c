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

static struct {
    const Move *list;
    uint8_t     index;
} moves;

// Define the targets
static void Moves_Begin_Current(const Kobuki_Typedef *k)
{
    const Move *m = &moves.list[moves.index];
    if (m->type == MOVE_ROTATE) {
        Kobuki_Rotate_Angle_Setpoint(k->angle, m->value);
    } else if (m->type == MOVE_DRIVE) {
        Kobuki_Drive_Distance_Setpoint(k->distance, m->value);
    }
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

    if (m->type == MOVE_END) { // End condition
        Kobuki_Drive(STOP);
        return TASK_DONE;
    }

    uint8_t done = (m->type == MOVE_ROTATE) ? k->rotation_complete
                                            : k->distance_complete; // If its not a rotation its a drive (2 types only)
    if (done) {
        Kobuki_Drive(STOP);
        moves.index++;
        Moves_Begin_Current(k); // Setup next move
        return TASK_RUNNING;
    }

    if (m->type == MOVE_ROTATE) Kobuki_Rotate_Angle(k->angle);
    else                        Kobuki_Drive_Distance(k->distance); // If its not a rotation its a drive (2 types only)
    return TASK_RUNNING;
}
