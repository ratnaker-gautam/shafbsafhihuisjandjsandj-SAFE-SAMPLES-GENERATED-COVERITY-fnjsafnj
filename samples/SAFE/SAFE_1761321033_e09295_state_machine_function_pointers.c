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
size_t data_count = 0;

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
            printf("Read data item: %d\n", data_count * 10);
        }
        
        if (data_count >= 3) {
            printf("Data reading complete\n");
            return STATE_PROCESSING;
        }
        return STATE_READING;
    }
    if (event == EVENT_ERROR) {
        return STATE_ERROR;
    }
    return STATE_READING;
}

State handle_processing(Event event) {
    if (event == EVENT_PROCESS_COMPLETE) {
        printf("Data processing complete\n");
        int sum = 0;
        for (size_t i = 0; i < data_count && i < 10; i++) {
            sum += data_buffer[i];
        }
        printf("Processed data sum: %d\n", sum);
        return STATE_WRITING;
    }
    if (event == EVENT_ERROR) {
        return STATE_ERROR;
    }
    return STATE_PROCESSING;
}

State handle_writing(Event event) {
    if (event == EVENT_WRITE_COMPLETE) {
        printf("Data writing complete\n");
        data_count = 0;
        return STATE_IDLE;
    }
    if (event == EVENT_ERROR) {
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
        EVENT_PROCESS_COMPLETE,
        EVENT_WRITE_COMPLETE
    };
    
    if (step < 6) {
        return events[step++];
    }
    return EVENT_RESET;
}

int main() {
    printf("State Machine Demo - Data Processing Pipeline\n");
    printf("=============================================\n");
    
    int iterations = 0;
    const int max_iterations = 10;
    
    while (iterations < max_iterations) {
        printf("\nCurrent state: %s\n", state_names[current_state]);
        
        Event event = get_next_event();
        printf("Processing event: %s\n", event_names[event]);
        
        State new_state = state_handlers[current_state](event);
        
        if (new_state != current_state) {
            printf("State transition: %s -> %s\n", 
                   state_names[current_state], state_names[new_state]);
            current_state = new_state;
        }
        
        iterations++;
        
        if (current_state == STATE_ERROR) {
            printf("Error state detected, sending reset event\n");
            event = EVENT_RESET;
            new_state = state_handlers[current_state](event);
            if (new_state != current_state) {
                printf("State transition: %s -> %s\n", 
                       state_names[current_state], state_names[new_state]);
                current_state = new_state;
            }
        }
    }
    
    printf("\nDemo completed after %d iterations\n", iterations);
    return 0;
}