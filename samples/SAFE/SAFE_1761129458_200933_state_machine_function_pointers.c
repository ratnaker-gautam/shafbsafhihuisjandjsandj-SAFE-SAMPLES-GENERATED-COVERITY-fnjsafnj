//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_COUNT
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET,
    EVENT_COUNT
} Event;

typedef State (*StateHandler)(Event);

State handle_idle(Event event);
State handle_reading(Event event);
State handle_processing(Event event);
State handle_writing(Event event);
State handle_error(Event event);

StateHandler state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_reading,
    handle_processing,
    handle_writing,
    handle_error
};

char* state_names[STATE_COUNT] = {
    "IDLE",
    "READING",
    "PROCESSING",
    "WRITING",
    "ERROR"
};

char* event_names[EVENT_COUNT] = {
    "START",
    "DATA_READY",
    "PROCESS_COMPLETE",
    "WRITE_COMPLETE",
    "ERROR",
    "RESET"
};

State current_state = STATE_IDLE;
int data_buffer[10];
unsigned int data_count = 0;

State handle_idle(Event event) {
    if (event == EVENT_START) {
        printf("Starting data processing pipeline\n");
        return STATE_READING;
    }
    return STATE_IDLE;
}

State handle_reading(Event event) {
    if (event == EVENT_DATA_READY) {
        if (data_count < 10) {
            data_buffer[data_count] = data_count * 10;
            data_count++;
            printf("Read data item: %d\n", data_buffer[data_count - 1]);
        }
        if (data_count >= 5) {
            return STATE_PROCESSING;
        }
        return STATE_READING;
    } else if (event == EVENT_ERROR) {
        return STATE_ERROR;
    }
    return STATE_READING;
}

State handle_processing(Event event) {
    if (event == EVENT_PROCESS_COMPLETE) {
        printf("Processing complete, processed %u items\n", data_count);
        for (unsigned int i = 0; i < data_count; i++) {
            data_buffer[i] *= 2;
        }
        return STATE_WRITING;
    } else if (event == EVENT_ERROR) {
        return STATE_ERROR;
    }
    return STATE_PROCESSING;
}

State handle_writing(Event event) {
    if (event == EVENT_WRITE_COMPLETE) {
        printf("Writing results:\n");
        for (unsigned int i = 0; i < data_count; i++) {
            printf("  Item %u: %d\n", i, data_buffer[i]);
        }
        data_count = 0;
        return STATE_IDLE;
    } else if (event == EVENT_ERROR) {
        return STATE_ERROR;
    }
    return STATE_WRITING;
}

State handle_error(Event event) {
    if (event == EVENT_RESET) {
        printf("Resetting from error state\n");
        data_count = 0;
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

Event get_next_event() {
    static int step = 0;
    Event events[] = {
        EVENT_START,
        EVENT_DATA_READY,
        EVENT_DATA_READY,
        EVENT_DATA_READY,
        EVENT_DATA_READY,
        EVENT_DATA_READY,
        EVENT_PROCESS_COMPLETE,
        EVENT_WRITE_COMPLETE
    };
    
    if (step < 8) {
        return events[step++];
    }
    return EVENT_RESET;
}

int main() {
    printf("State Machine Demo Started\n");
    
    int cycles = 0;
    while (cycles < 15) {
        printf("Current state: %s\n", state_names[current_state]);
        
        Event event = get_next_event();
        printf("Processing event: %s\n", event_names[event]);
        
        if (current_state >= 0 && current_state < STATE_COUNT && 
            event >= 0 && event < EVENT_COUNT) {
            State new_state = state_handlers[current_state](event);
            if (new_state >= 0 && new_state < STATE_COUNT) {
                current_state = new_state;
            } else {
                current_state = STATE_ERROR;
            }
        } else {
            current_state = STATE_ERROR;
        }
        
        cycles++;
        if (current_state == STATE_IDLE && cycles > 10) {
            break;
        }
    }
    
    printf("State Machine Demo Completed\n");
    return 0;
}