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

State handle_idle(Event evt) {
    switch (evt) {
        case EVENT_START:
            printf("Transition: IDLE -> READING\n");
            return STATE_READING;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

State handle_reading(Event evt) {
    switch (evt) {
        case EVENT_DATA_READY:
            printf("Transition: READING -> PROCESSING\n");
            return STATE_PROCESSING;
        case EVENT_ERROR:
            printf("Transition: READING -> ERROR\n");
            return STATE_ERROR;
        case EVENT_RESET:
            printf("Transition: READING -> IDLE\n");
            return STATE_IDLE;
        default:
            return STATE_READING;
    }
}

State handle_processing(Event evt) {
    switch (evt) {
        case EVENT_PROCESS_DONE:
            printf("Transition: PROCESSING -> WRITING\n");
            return STATE_WRITING;
        case EVENT_ERROR:
            printf("Transition: PROCESSING -> ERROR\n");
            return STATE_ERROR;
        case EVENT_RESET:
            printf("Transition: PROCESSING -> IDLE\n");
            return STATE_IDLE;
        default:
            return STATE_PROCESSING;
    }
}

State handle_writing(Event evt) {
    switch (evt) {
        case EVENT_WRITE_DONE:
            printf("Transition: WRITING -> IDLE\n");
            return STATE_IDLE;
        case EVENT_ERROR:
            printf("Transition: WRITING -> ERROR\n");
            return STATE_ERROR;
        case EVENT_RESET:
            printf("Transition: WRITING -> IDLE\n");
            return STATE_IDLE;
        default:
            return STATE_WRITING;
    }
}

State handle_error(Event evt) {
    switch (evt) {
        case EVENT_RESET:
            printf("Transition: ERROR -> IDLE\n");
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

StateHandler state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_reading,
    handle_processing,
    handle_writing,
    handle_error
};

const char* state_names[STATE_COUNT] = {
    "IDLE",
    "READING",
    "PROCESSING",
    "WRITING",
    "ERROR"
};

const char* event_names[EVENT_COUNT] = {
    "START",
    "DATA_READY",
    "PROCESS_DONE",
    "WRITE_DONE",
    "ERROR",
    "RESET"
};

int get_user_event(void) {
    char input[32];
    printf("\nCurrent events:\n");
    for (int i = 0; i < EVENT_COUNT; i++) {
        printf("%d: %s\n", i, event_names[i]);
    }
    printf("Enter event number (0-%d) or -1 to quit: ", EVENT_COUNT - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return -1;
    }
    
    char* endptr;
    long choice = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') {
        return -2;
    }
    
    if (choice < -1 || choice >= EVENT_COUNT) {
        return -2;
    }
    
    return (int)choice;
}

int main(void) {
    State current_state = STATE_IDLE;
    int running = 1;
    
    printf("State Machine Demo\n");
    printf("Initial state: %s\n", state_names[current_state]);
    
    while (running) {
        int event_num = get_user_event();
        
        if (event_num == -1) {
            running = 0;
            continue;
        }
        
        if (event_num == -2) {
            printf("Invalid input. Please try again.\n");
            continue;
        }
        
        Event evt = (Event)event_num;
        State new_state = state_handlers[current_state](evt);
        
        if (new_state >= STATE_COUNT) {
            printf("Invalid state transition!\n");
            new_state = STATE_ERROR;
        }
        
        if (new_state != current_state) {
            printf("State changed: %s -> %s\n", 
                   state_names[current_state], state_names[new_state]);
            current_state = new_state;
        } else {
            printf("Remaining in state: %s\n", state_names[current_state]);
        }
    }
    
    printf("State machine terminated.\n");
    return 0;
}