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
        printf("Starting data processing...\n");
        return STATE_READING;
    }
    return STATE_IDLE;
}

State handle_reading(Event event) {
    if (event == EVENT_DATA_READY) {
        if (data_count < 10) {
            data_buffer[data_count] = data_count * 10;
            data_count++;
            printf("Read data: %d\n", data_buffer[data_count - 1]);
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
        printf("Processing complete. Data: ");
        for (size_t i = 0; i < data_count; i++) {
            printf("%d ", data_buffer[i] * 2);
        }
        printf("\n");
        return STATE_WRITING;
    } else if (event == EVENT_ERROR) {
        return STATE_ERROR;
    }
    return STATE_PROCESSING;
}

State handle_writing(Event event) {
    if (event == EVENT_WRITE_COMPLETE) {
        printf("Data written successfully.\n");
        data_count = 0;
        return STATE_IDLE;
    } else if (event == EVENT_ERROR) {
        return STATE_ERROR;
    }
    return STATE_WRITING;
}

State handle_error(Event event) {
    if (event == EVENT_RESET) {
        printf("Resetting state machine...\n");
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
    if (new_state < 0 || new_state >= STATE_COUNT) {
        return;
    }
    if (new_state != current_state) {
        printf("State transition: %s -> %s\n", state_names[current_state], state_names[new_state]);
        current_state = new_state;
    }
}

int main() {
    printf("State Machine Demo\n");
    printf("Commands: s=START, d=DATA_READY, p=PROCESS_COMPLETE, w=WRITE_COMPLETE, e=ERROR, r=RESET, q=QUIT\n");
    
    char input[10];
    while (1) {
        printf("Enter command: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if (input[0] == 'q') {
            break;
        }
        
        Event event;
        switch (input[0]) {
            case 's': event = EVENT_START; break;
            case 'd': event = EVENT_DATA_READY; break;
            case 'p': event = EVENT_PROCESS_COMPLETE; break;
            case 'w': event = EVENT_WRITE_COMPLETE; break;
            case 'e': event = EVENT_ERROR; break;
            case 'r': event = EVENT_RESET; break;
            default:
                printf("Invalid command\n");
                continue;
        }
        
        process_event(event);
    }
    
    return 0;
}