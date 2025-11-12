//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_COUNT
} state_t;

typedef enum {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_DONE,
    EVENT_WRITE_DONE,
    EVENT_ERROR,
    EVENT_RESET,
    EVENT_COUNT
} event_t;

typedef state_t (*state_handler_t)(event_t);

static state_t handle_idle(event_t evt);
static state_t handle_reading(event_t evt);
static state_t handle_processing(event_t evt);
static state_t handle_writing(event_t evt);
static state_t handle_error(event_t evt);

static state_handler_t state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_reading,
    handle_processing,
    handle_writing,
    handle_error
};

static const char* state_names[STATE_COUNT] = {
    "IDLE",
    "READING",
    "PROCESSING",
    "WRITING",
    "ERROR"
};

static const char* event_names[EVENT_COUNT] = {
    "START",
    "DATA_READY",
    "PROCESS_DONE",
    "WRITE_DONE",
    "ERROR",
    "RESET"
};

static state_t current_state = STATE_IDLE;
static int data_buffer[16];
static size_t data_count = 0;

static state_t handle_idle(event_t evt) {
    if (evt == EVENT_START) {
        printf("Starting data processing pipeline\n");
        return STATE_READING;
    }
    return STATE_IDLE;
}

static state_t handle_reading(event_t evt) {
    if (evt == EVENT_DATA_READY) {
        if (data_count < sizeof(data_buffer)/sizeof(data_buffer[0])) {
            data_buffer[data_count] = data_count * 10;
            data_count++;
            printf("Read data item: %d\n", data_buffer[data_count-1]);
            if (data_count >= 5) {
                return STATE_PROCESSING;
            }
        }
        return STATE_READING;
    } else if (evt == EVENT_ERROR) {
        return STATE_ERROR;
    }
    return STATE_READING;
}

static state_t handle_processing(event_t evt) {
    if (evt == EVENT_PROCESS_DONE) {
        printf("Processing completed on %zu items\n", data_count);
        for (size_t i = 0; i < data_count; i++) {
            data_buffer[i] *= 2;
        }
        return STATE_WRITING;
    } else if (evt == EVENT_ERROR) {
        return STATE_ERROR;
    }
    return STATE_PROCESSING;
}

static state_t handle_writing(event_t evt) {
    if (evt == EVENT_WRITE_DONE) {
        printf("Writing completed. Results:\n");
        for (size_t i = 0; i < data_count; i++) {
            printf("  Item %zu: %d\n", i, data_buffer[i]);
        }
        data_count = 0;
        return STATE_IDLE;
    } else if (evt == EVENT_ERROR) {
        return STATE_ERROR;
    }
    return STATE_WRITING;
}

static state_t handle_error(event_t evt) {
    if (evt == EVENT_RESET) {
        printf("Resetting from error state\n");
        data_count = 0;
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

static event_t get_next_event(void) {
    static int step = 0;
    event_t events[] = {
        EVENT_START,
        EVENT_DATA_READY,
        EVENT_DATA_READY,
        EVENT_DATA_READY,
        EVENT_DATA_READY,
        EVENT_DATA_READY,
        EVENT_PROCESS_DONE,
        EVENT_WRITE_DONE
    };
    
    if (step < (int)(sizeof(events)/sizeof(events[0]))) {
        return events[step++];
    }
    return EVENT_RESET;
}

int main(void) {
    printf("State Machine Demo Started\n");
    
    int iterations = 0;
    while (iterations < 20) {
        event_t evt = get_next_event();
        
        if (evt >= EVENT_COUNT) {
            printf("Invalid event generated\n");
            break;
        }
        
        printf("Current state: %s, Event: %s\n", 
               state_names[current_state], event_names[evt]);
        
        state_t new_state = state_handlers[current_state](evt);
        
        if (new_state >= STATE_COUNT) {
            printf("Invalid state transition\n");
            break;
        }
        
        current_state = new_state;
        
        if (current_state == STATE_IDLE && evt == EVENT_RESET) {
            break;
        }
        
        iterations++;
    }
    
    printf("State Machine Demo Completed\n");
    return 0;
}