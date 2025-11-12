//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR,
    STATE_COUNT
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET,
    EVENT_COUNT
} Event;

typedef void (*StateHandler)(void);

void handle_idle(void);
void handle_ready(void);
void handle_processing(void);
void handle_complete(void);
void handle_error(void);

StateHandler state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

int data_buffer[10];
size_t data_count = 0;
int processing_result = 0;

void handle_idle(void) {
    printf("State: IDLE\n");
    printf("Options: 0-Start, 5-Exit\n");
}

void handle_ready(void) {
    printf("State: READY\n");
    printf("Data count: %zu\n", data_count);
    printf("Options: 1-Add Data, 2-Process, 5-Exit\n");
}

void handle_processing(void) {
    printf("State: PROCESSING\n");
    printf("Processing %zu data points...\n", data_count);
    
    int sum = 0;
    for (size_t i = 0; i < data_count; i++) {
        sum += data_buffer[i];
    }
    
    if (data_count > 0) {
        processing_result = sum / (int)data_count;
        printf("Average: %d\n", processing_result);
    }
    
    printf("Options: 3-Success, 4-Failure, 5-Exit\n");
}

void handle_complete(void) {
    printf("State: COMPLETE\n");
    printf("Result: %d\n", processing_result);
    printf("Options: 5-Reset, 6-Exit\n");
}

void handle_error(void) {
    printf("State: ERROR\n");
    printf("Options: 5-Reset, 6-Exit\n");
}

Event get_user_input(State current_state) {
    int input;
    printf("Enter choice: ");
    
    if (scanf("%d", &input) != 1) {
        while (getchar() != '\n');
        return EVENT_COUNT;
    }
    
    switch (current_state) {
        case STATE_IDLE:
            if (input == 0) return EVENT_START;
            if (input == 5) return EVENT_FAILURE;
            break;
        case STATE_READY:
            if (input == 1) return EVENT_DATA;
            if (input == 2) return EVENT_PROCESS;
            if (input == 5) return EVENT_FAILURE;
            break;
        case STATE_PROCESSING:
            if (input == 3) return EVENT_SUCCESS;
            if (input == 4) return EVENT_FAILURE;
            if (input == 5) return EVENT_FAILURE;
            break;
        case STATE_COMPLETE:
        case STATE_ERROR:
            if (input == 5) return EVENT_RESET;
            if (input == 6) return EVENT_FAILURE;
            break;
        default:
            break;
    }
    
    return EVENT_COUNT;
}

State transition_table[STATE_COUNT][EVENT_COUNT] = {
    [STATE_IDLE] = {
        [EVENT_START] = STATE_READY,
        [EVENT_FAILURE] = STATE_ERROR
    },
    [STATE_READY] = {
        [EVENT_DATA] = STATE_READY,
        [EVENT_PROCESS] = STATE_PROCESSING,
        [EVENT_FAILURE] = STATE_ERROR
    },
    [STATE_PROCESSING] = {
        [EVENT_SUCCESS] = STATE_COMPLETE,
        [EVENT_FAILURE] = STATE_ERROR
    },
    [STATE_COMPLETE] = {
        [EVENT_RESET] = STATE_IDLE,
        [EVENT_FAILURE] = STATE_ERROR
    },
    [STATE_ERROR] = {
        [EVENT_RESET] = STATE_IDLE,
        [EVENT_FAILURE] = STATE_ERROR
    }
};

void reset_system(void) {
    data_count = 0;
    processing_result = 0;
    memset(data_buffer, 0, sizeof(data_buffer));
}

int main(void) {
    State current_state = STATE_IDLE;
    int running = 1;
    
    printf("State Machine Demo - Data Processor\n");
    printf("===================================\n");
    
    while (running) {
        state_handlers[current_state]();
        
        Event event = get_user_input(current_state);
        
        if (event == EVENT_COUNT) {
            printf("Invalid input!\n");
            continue;
        }
        
        if (event == EVENT_FAILURE && current_state != STATE_IDLE) {
            running = 0;
            continue;
        }
        
        if (event == EVENT_DATA && current_state == STATE_READY) {
            if (data_count < 10) {
                printf("Enter integer data: ");
                int value;
                if (scanf("%d", &value) == 1) {
                    data_buffer[data_count++] = value;
                    printf("Data added. Total: %zu\n", data_count);
                } else {
                    while (getchar() != '\n');
                    printf("Invalid data!\n");
                }
            } else {
                printf("Buffer full!\