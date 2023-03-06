#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>

typedef enum{
    // signals of application
    // external activity signal
    INCREASE_TIME,
    DECREASE_TIME,
    START_PAUSE,
    ABORT,
    TIME_TICK,
    // internal activity signal
    ENTRY,
    EXIT
}application_signals;

typedef enum{
    // state of application
    IDLE,
    TIME_SET,
    COUNTDOWN,
    PAUSE,
    STAT
}application_states;

typedef struct{
    // structures of application 
    uint8_t current_time;
    uint8_t elapsed_time;
    uint8_t progress_time;
    application_states active_state;
}application_structures;

typedef struct{
    // user generated events
    uint8_t signal;
}events;

typedef struct{
    // user generated events
    events super;
}user_events;

typedef struct{
    // time tick events
    events signal;
    uint8_t subsecond;
}tick_events;

typedef enum{
    EVENT_HANDLED,
    EVENT_IGNORED,
    EVENT_TRANSITION
}events_status;

// prototypes
void application_init(application_structures *main_object);

#endif