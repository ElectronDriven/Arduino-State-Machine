#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>

// push buttons pins configuration
#define PIN_BP1 2
#define PIN_BP2 3
#define PIN_BP3 4
#define PIN_BUZZER 12

#define PB_INCREASE_TIME 4
#define PB_DECREASE_TIME 2
#define PB_ABORT         6
#define PB_START_PAUSE   1

// lcd pins configuration
#define LCD_RS 5
#define LCD_RW 6
#define LCD_EN 7
//#define LCD_D1 0
//#define LCD_D2 0
//#define LCD_D3 0
#define LCD_D4 8
#define LCD_D5 9
#define LCD_D6 10
#define LCD_D7 11



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
    events super;
    uint8_t subsecond;
}tick_events;

typedef enum{
    EVENT_HANDLED,
    EVENT_IGNORED,
    EVENT_TRANSITION
}events_status;

// prototypes
void application_init(application_structures *main_object);
events_status application_state_machine(application_structures * const main_object, events const * const event);

#endif