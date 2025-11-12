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
} state_t;

typedef enum {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET,
    EVENT_COUNT
} event_t;

typedef state_t (*state_handler_t)(event_t);

static int data_buffer[32];
static size_t data_count = 0;

static state_t handle_idle(event_t event) {
    if (event == EVENT_START) {
        printf("Starting data acquisition\n");
        return STATE_READING;
    }
    return STATE_IDLE;
}

static state_t handle_reading(event_t event) {
    if (event == EVENT_DATA_READY) {
        if (data_count < 32) {
            data_buffer[data_count] = data_count * 10;
            data_count++;
            printf("Read data point %zu\n", data_count);
            if (data_count >= 10) {
                return STATE_PROCESSING;
            }
            return STATE_READING;
        } else {
            return STATE_ERROR;
        }
    } else if (event == EVENT_ERROR) {
        return STATE_ERROR;
    }
    return STATE_READING;
}

static state_t handle_processing(event_t event) {
    if (event == EVENT_PROCESS_COMPLETE) {
        int sum = 0;
        for (size_t i = 0; i < data_count; i++) {
            sum += data_buffer[i];
        }
        printf("Processing complete. Sum: %d\n", sum);
        return STATE_WRITING;
    } else if (event == EVENT_ERROR) {
        return STATE_ERROR;
    }
    return STATE_PROCESSING;
}

static state_t handle_writing(event_t event) {
    if (event == EVENT_WRITE_COMPLETE) {
        printf("Data written successfully\n");
        return STATE_IDLE;
    } else if (event == EVENT_ERROR) {
        return STATE_ERROR;
    }
    return STATE_WRITING;
}

static state_t handle_error(event_t event) {
    if (event == EVENT_RESET) {
        data_count = 0;
        printf("System reset\n");
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

int main(void) {
    state_handler_t handlers[STATE_COUNT] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_error
    };
    
    state_t current_state = STATE_IDLE;
    int event_input;
    int running = 1;
    
    printf("State Machine Controller\n");
    printf("Events: 0=START, 1=DATA_READY, 2=PROCESS_COMPLETE, 3=WRITE_COMPLETE, 4=ERROR, 5=RESET, 6=QUIT\n");
    
    while (running) {
        printf("Current state: %d | Enter event: ", current_state);
        if (scanf("%d", &event_input) != 1) {
            while (getchar() != '\n');
            printf("Invalid input\n");
            continue;
        }
        
        if (event_input < 0 || event_input >= EVENT_COUNT) {
            if (event_input == 6) {
                running = 0;
                continue;
            }
            printf("Invalid event\n");
            continue;
        }
        
        event_t event = (event_t)event_input;
        state_t new_state = handlers[current_state](event);
        
        if (new_state < 0 || new_state >= STATE_COUNT) {
            printf("Invalid state transition\n");
            continue;
        }
        
        current_state = new_state;
    }
    
    printf("Exiting state machine\n");
    return 0;
}