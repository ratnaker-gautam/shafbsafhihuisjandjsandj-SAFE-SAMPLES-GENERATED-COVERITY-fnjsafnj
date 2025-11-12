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
    EVENT_PROCESS_DONE,
    EVENT_WRITE_DONE,
    EVENT_ERROR,
    EVENT_RESET,
    EVENT_COUNT
} Event;

typedef State (*StateHandler)(Event);

State handle_idle(Event evt);
State handle_reading(Event evt);
State handle_processing(Event evt);
State handle_writing(Event evt);
State handle_error(Event evt);

StateHandler state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_reading,
    handle_processing,
    handle_writing,
    handle_error
};

const char* state_names[STATE_COUNT] = {
    "IDLE", "READING", "PROCESSING", "WRITING", "ERROR"
};

const char* event_names[EVENT_COUNT] = {
    "START", "DATA_READY", "PROCESS_DONE", "WRITE_DONE", "ERROR", "RESET"
};

State current_state = STATE_IDLE;
int data_buffer[10];
int data_count = 0;

State handle_idle(Event evt) {
    if (evt == EVENT_START) {
        printf("Transition: IDLE -> READING\n");
        return STATE_READING;
    }
    return STATE_IDLE;
}

State handle_reading(Event evt) {
    if (evt == EVENT_DATA_READY) {
        if (data_count < 10) {
            data_buffer[data_count] = data_count * 10;
            data_count++;
            printf("Read data: %d\n", data_buffer[data_count-1]);
        }
        if (data_count >= 3) {
            printf("Transition: READING -> PROCESSING\n");
            return STATE_PROCESSING;
        }
        return STATE_READING;
    } else if (evt == EVENT_ERROR) {
        printf("Transition: READING -> ERROR\n");
        return STATE_ERROR;
    }
    return STATE_READING;
}

State handle_processing(Event evt) {
    if (evt == EVENT_PROCESS_DONE) {
        int sum = 0;
        for (int i = 0; i < data_count; i++) {
            if (sum > 1000 - data_buffer[i]) {
                printf("Transition: PROCESSING -> ERROR\n");
                return STATE_ERROR;
            }
            sum += data_buffer[i];
        }
        printf("Processing complete. Sum: %d\n", sum);
        printf("Transition: PROCESSING -> WRITING\n");
        return STATE_WRITING;
    } else if (evt == EVENT_ERROR) {
        printf("Transition: PROCESSING -> ERROR\n");
        return STATE_ERROR;
    }
    return STATE_PROCESSING;
}

State handle_writing(Event evt) {
    if (evt == EVENT_WRITE_DONE) {
        printf("Data written successfully.\n");
        printf("Transition: WRITING -> IDLE\n");
        data_count = 0;
        return STATE_IDLE;
    } else if (evt == EVENT_ERROR) {
        printf("Transition: WRITING -> ERROR\n");
        return STATE_ERROR;
    }
    return STATE_WRITING;
}

State handle_error(Event evt) {
    if (evt == EVENT_RESET) {
        printf("System reset.\n");
        printf("Transition: ERROR -> IDLE\n");
        data_count = 0;
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

void process_event(Event evt) {
    if (evt < 0 || evt >= EVENT_COUNT) {
        return;
    }
    State new_state = state_handlers[current_state](evt);
    if (new_state >= 0 && new_state < STATE_COUNT) {
        current_state = new_state;
    }
}

int main() {
    printf("State Machine Demo Started\n");
    
    process_event(EVENT_START);
    process_event(EVENT_DATA_READY);
    process_event(EVENT_DATA_READY);
    process_event(EVENT_DATA_READY);
    process_event(EVENT_PROCESS_DONE);
    process_event(EVENT_WRITE_DONE);
    
    printf("\nSecond cycle with error:\n");
    process_event(EVENT_START);
    process_event(EVENT_DATA_READY);
    process_event(EVENT_ERROR);
    process_event(EVENT_RESET);
    
    printf("\nThird cycle with overflow check:\n");
    process_event(EVENT_START);
    for (int i = 0; i < 12; i++) {
        process_event(EVENT_DATA_READY);
    }
    process_event(EVENT_PROCESS_DONE);
    process_event(EVENT_WRITE_DONE);
    
    printf("State Machine Demo Completed\n");
    return 0;
}