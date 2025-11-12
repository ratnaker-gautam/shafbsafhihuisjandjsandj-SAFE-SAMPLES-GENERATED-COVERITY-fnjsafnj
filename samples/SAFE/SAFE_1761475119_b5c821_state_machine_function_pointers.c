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
int data_count = 0;

State handle_idle(Event event) {
    if (event == EVENT_START) {
        printf("State: IDLE -> READING\n");
        return STATE_READING;
    }
    return STATE_IDLE;
}

State handle_reading(Event event) {
    if (event == EVENT_DATA_READY) {
        if (data_count < 10) {
            data_buffer[data_count] = data_count * 10;
            data_count++;
            printf("Read data: %d\n", data_buffer[data_count-1]);
        }
        if (data_count >= 5) {
            printf("State: READING -> PROCESSING\n");
            return STATE_PROCESSING;
        }
        return STATE_READING;
    } else if (event == EVENT_ERROR) {
        printf("State: READING -> ERROR\n");
        return STATE_ERROR;
    }
    return STATE_READING;
}

State handle_processing(Event event) {
    if (event == EVENT_PROCESS_COMPLETE) {
        printf("Processing complete. Data processed: ");
        for (int i = 0; i < data_count; i++) {
            printf("%d ", data_buffer[i]);
        }
        printf("\n");
        printf("State: PROCESSING -> WRITING\n");
        return STATE_WRITING;
    } else if (event == EVENT_ERROR) {
        printf("State: PROCESSING -> ERROR\n");
        return STATE_ERROR;
    }
    return STATE_PROCESSING;
}

State handle_writing(Event event) {
    if (event == EVENT_WRITE_COMPLETE) {
        printf("Data written successfully.\n");
        printf("State: WRITING -> IDLE\n");
        data_count = 0;
        return STATE_IDLE;
    } else if (event == EVENT_ERROR) {
        printf("State: WRITING -> ERROR\n");
        return STATE_ERROR;
    }
    return STATE_WRITING;
}

State handle_error(Event event) {
    if (event == EVENT_RESET) {
        printf("State: ERROR -> IDLE\n");
        data_count = 0;
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

void process_event(Event event) {
    if (event < 0 || event >= EVENT_COUNT) {
        return;
    }
    State new_state = state_handlers[current_state](event);
    if (new_state >= 0 && new_state < STATE_COUNT) {
        current_state = new_state;
    }
}

int main() {
    printf("State Machine Demo Started\n");
    
    process_event(EVENT_START);
    
    for (int i = 0; i < 6; i++) {
        process_event(EVENT_DATA_READY);
    }
    
    process_event(EVENT_PROCESS_COMPLETE);
    
    process_event(EVENT_WRITE_COMPLETE);
    
    process_event(EVENT_START);
    process_event(EVENT_ERROR);
    process_event(EVENT_RESET);
    
    printf("State Machine Demo Completed\n");
    return 0;
}