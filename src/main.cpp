#include <main.h>

// main application object
application_structures progress_timer;

void application_event_dispatcher(application_structures * const main_object, events const * const event);
static uint8_t pb_value_process(uint8_t pb_value);
static void display_init();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("ELECTRON DRIVEN");

  display_init();

  pinMode(PIN_BP1, INPUT);
  pinMode(PIN_BP2, INPUT);
  pinMode(PIN_BP3, INPUT);


  application_init(&progress_timer);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("Hello MEHDI"); delay(1000);
  uint8_t pb1, pb2, pb3, pb_value;
  user_events ue;
  static uint32_t current_time = millis();
  static tick_events te;

  // read the buttons status
  pb1 = digitalRead(PIN_BP1);
  pb2 = digitalRead(PIN_BP2);
  pb3 = digitalRead(PIN_BP3);
  pb_value = (pb1<<2)|(pb2<<1)|pb3;

  // software button debouncing
  pb_value = pb_value_process(pb_value);

  // make an event!
  if(pb_value){
    if(pb_value == PB_INCREASE_TIME){
      ue.super.signal = INCREASE_TIME;
    }
    else if(pb_value == PB_DECREASE_TIME){
      ue.super.signal = DECREASE_TIME;
    }
    else if(pb_value == PB_ABORT){
      ue.super.signal = ABORT;
    }
    else if(pb_value == PB_START_PAUSE){
      ue.super.signal = START_PAUSE;
    }
  }

  // send it to event dispatcher
  application_event_dispatcher(&progress_timer, &ue.super);

  // dispatch the time tick event for every 100ms
  if(millis() - current_time >= 100){
    current_time = millis();
    te.super.signal = TIME_TICK;

    te.subsecond += 1;
    if(te.subsecond >10) te.subsecond = 1;

    application_event_dispatcher(&progress_timer, &te.super);

  }  

}

void application_event_dispatcher(application_structures * const main_object, events const * const event){
  events_status status;
  application_states source, target;

  source = main_object->active_state;
  status = application_state_machine(main_object, event);

  if(status == EVENT_TRANSITION){
    target = main_object->active_state;
    // entry/exit events
    events ee;
    // run the exit action for the source state
    ee.signal = EXIT;
    main_object->active_state = source;
    application_state_machine(main_object, &ee);
    // run the entry action for the target
    ee.signal = ENTRY;
    main_object->active_state = target;
    application_state_machine(main_object, &ee);

  }
}

// de-bouncing
static uint8_t pb_value_process(uint8_t pb_value){
  static buttons_states button_state = NOTPRESSED;
  static uint32_t current_time = millis();
  
  switch (button_state)
  {
  case NOTPRESSED:{
     if(button_state){
      button_state = BOUNCE;
      current_time = millis();
     }
     break;
  }

  case BOUNCE:{
    if(millis() - current_time >= 50){
      if(pb_value){
        button_state = PRESSED;
        return pb_value;
      }
      else
        button_state = NOTPRESSED;

    }
    break;
  }

  case PRESSED:{
    if(!pb_value){
      button_state = BOUNCE;
      current_time = millis();
    }
    break;
  }  

}

  return 0;
}

static void display_init(){
  
}