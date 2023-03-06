#include<main.h>

// prototypes of helper functions
static void display_time(uint32_t time);
static void display_message(String string);
static void display_clear(void);
static void do_beep(void);

// prototypes of state handler functions
static events_status stats_handler_idle(application_structures * const main_object, events const * const event);
static events_status stats_handler_timeset(application_structures * const main_object, events const * const event);
static events_status stats_handler_countdown(application_structures * const main_object, events const * const event);
static events_status stats_handler_pause(application_structures * const main_object, events const * const event);
static events_status stats_handler_stat(application_structures * const main_object, events const * const event);


void application_init(application_structures *main_object){
    main_object->active_state = IDLE;
    main_object->progress_time = 0;
}

events_status application_state_machine(application_structures * const main_object, events const * const event){
    switch (main_object->active_state)
    {
        case IDLE:{
            return stats_handler_idle(main_object, event);
        }

        case TIME_SET:{
            return stats_handler_timeset(main_object, event);
        }

        case COUNTDOWN:{
            return stats_handler_countdown(main_object, event);
        }
        
        case PAUSE:{
            return stats_handler_pause(main_object, event);
        }

        case STAT:{
            return stats_handler_stat(main_object, event);
        }

    }

    return EVENT_IGNORED;
}

static events_status stats_handler_idle(application_structures * const main_object, events const * const event){
    switch (event->signal){
        case ENTRY:{
            main_object->current_time = 0;
            main_object->elapsed_time = 0;
            display_time(0);
            display_message("SET TIME");
            return EVENT_HANDLED;
        }
        case EXIT:{
            display_clear();
            return EVENT_HANDLED;

        }
        case INCREASE_TIME:{
            main_object->current_time += 60;
            // new state
            main_object->active_state = TIME_SET;
            return EVENT_TRANSITION;
        }

        case START_PAUSE:{
            main_object->active_state = STAT;
            return EVENT_TRANSITION;
        }

        case TIME_TICK:{
            if (((tick_events*)(event))->subsecond == 5){
                do_beep();
                return EVENT_HANDLED;
            }
            return EVENT_IGNORED;
        }
    } // end of switch_case

    return EVENT_IGNORED;
}

static events_status stats_handler_timeset(application_structures * const main_object, events const * const event){
    switch (event->signal){
        case ENTRY:{
            display_time(main_object->current_time);
            return EVENT_HANDLED;

        }
        case EXIT:{
            display_clear();
            return EVENT_HANDLED;

        }
        case INCREASE_TIME:{
            main_object->current_time +=60;
            display_time(main_object->current_time);

            return EVENT_HANDLED;
        }
        case DECREASE_TIME:{
            if(main_object->current_time >= 60){
                main_object->current_time -=60;
                display_time(main_object->current_time);
                return EVENT_HANDLED;
            }
            return EVENT_IGNORED;
        }
        case START_PAUSE:{
            if(main_object->current_time >= 60){
                main_object->active_state = COUNTDOWN;
                return EVENT_TRANSITION;
            }
            return EVENT_IGNORED;

        }
        case ABORT:{
            main_object->active_state = IDLE;
            return EVENT_TRANSITION;

        }
        case TIME_TICK:{
            return EVENT_IGNORED;
        }
    } // end of switch_case
    
    return EVENT_IGNORED;

}

static events_status stats_handler_countdown(application_structures * const main_object, events const * const event){
    switch (event->signal){
        case ENTRY:{
            return EVENT_IGNORED;
        }
        case EXIT:{
            main_object->progress_time += main_object->elapsed_time;
            main_object->elapsed_time = 0;
            return EVENT_HANDLED;

        }
        case INCREASE_TIME:{
            return EVENT_IGNORED;
        }
        case DECREASE_TIME:{
            return EVENT_IGNORED;
        }
        case START_PAUSE:{
            main_object->active_state = PAUSE;
            return EVENT_TRANSITION;
        }
        case ABORT:{
            return EVENT_IGNORED;
        }
        case TIME_TICK:{
            if(((tick_events*)(event))->subsecond == 10){
                main_object->current_time -= 1;
                main_object->elapsed_time += 1;
                display_time(main_object->current_time);
                if(main_object->current_time == 0){
                    main_object->active_state = IDLE;
                    return EVENT_TRANSITION;
                }
                return EVENT_HANDLED;

            }
            return EVENT_IGNORED;
            
        }
    } // end of switch_case
    
    return EVENT_IGNORED;
}
static events_status stats_handler_pause(application_structures * const main_object, events const * const event){
    switch (event->signal){
        case ENTRY:{
            display_message("PAUSED");
            return EVENT_HANDLED;
        }
        case EXIT:{
            display_clear();
            return EVENT_HANDLED;

        }
        case INCREASE_TIME:{
            main_object->current_time += 60;
            main_object->active_state = TIME_SET;
            return EVENT_TRANSITION;
        }
        case DECREASE_TIME:{
            if(main_object->current_time >= 60){
                main_object->current_time -= 60;
                main_object->active_state = TIME_SET;
                return EVENT_TRANSITION;
            }
            return EVENT_IGNORED;
        }
        case START_PAUSE:{
            main_object->active_state = COUNTDOWN;
            return EVENT_TRANSITION;
        }
        case ABORT:{
            main_object->active_state = IDLE;
            return EVENT_TRANSITION;
        }
        case TIME_TICK:{
            return EVENT_IGNORED;
        }
    } // end of switch_case
    
    return EVENT_IGNORED;

}

static events_status stats_handler_stat(application_structures * const main_object, events const * const event){
    switch (event->signal){
        case ENTRY:{
            display_time(main_object->progress_time);
            display_message("PRODUCTIVE TIME: ");
            return EVENT_HANDLED;

        }
        case EXIT:{
            display_clear();
            return EVENT_HANDLED;

        }
        case INCREASE_TIME:{
            return EVENT_IGNORED;
        }
        case DECREASE_TIME:{
            return EVENT_IGNORED;
        }
        case START_PAUSE:{
            return EVENT_IGNORED;
        }
        case ABORT:{
            return EVENT_IGNORED;
        }
        case TIME_TICK:{
            if(((tick_events*)(event))->subsecond == 10){
                main_object->active_state = IDLE;
                return EVENT_TRANSITION;
            }
            return EVENT_IGNORED;
            
        }
    } // end of switch_case
    
    return EVENT_IGNORED;
}



// helper functions
static void display_time(uint32_t time){}
static void display_message(String string){}
static void display_clear(void){}
static void do_beep(void){}
