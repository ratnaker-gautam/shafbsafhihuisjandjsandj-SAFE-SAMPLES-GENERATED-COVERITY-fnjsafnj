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
} state_t;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET,
    EVENT_COUNT
} event_t;

typedef state_t (*state_handler_t)(event_t);

static state_t current_state = STATE_IDLE;

state_t handle_idle(event_t event) {
    switch (event) {
        case EVENT_START:
            printf("Transition: IDLE -> READY\n");
            return STATE_READY;
        default:
            printf("Invalid event for IDLE state\n");
            return STATE_IDLE;
    }
}

state_t handle_ready(event_t event) {
    switch (event) {
        case EVENT_DATA:
            printf("Transition: READY -> PROCESSING\n");
            return STATE_PROCESSING;
        case EVENT_RESET:
            printf("Transition: READY -> IDLE\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for READY state\n");
            return STATE_READY;
    }
}

state_t handle_processing(event_t event) {
    switch (event) {
        case EVENT_SUCCESS:
            printf("Transition: PROCESSING -> COMPLETE\n");
            return STATE_COMPLETE;
        case EVENT_FAILURE:
            printf("Transition: PROCESSING -> ERROR\n");
            return STATE_ERROR;
        default:
            printf("Invalid event for PROCESSING state\n");
            return STATE_PROCESSING;
    }
}

state_t handle_complete(event_t event) {
    switch (event) {
        case EVENT_RESET:
            printf("Transition: COMPLETE -> IDLE\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for COMPLETE state\n");
            return STATE_COMPLETE;
    }
}

state_t handle_error(event_t event) {
    switch (event) {
        case EVENT_RESET:
            printf("Transition: ERROR -> IDLE\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for ERROR state\n");
            return STATE_ERROR;
    }
}

static state_handler_t state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

void process_event(event_t event) {
    if (event < 0 || event >= EVENT_COUNT) {
        printf("Invalid event\n");
        return;
    }
    
    state_t new_state = state_handlers[current_state](event);
    
    if (new_state < 0 || new_state >= STATE_COUNT) {
        printf("Invalid state transition\n");
        return;
    }
    
    current_state = new_state;
}

void print_menu(void) {
    printf("\nCurrent state: ");
    switch (current_state) {
        case STATE_IDLE: printf("IDLE"); break;
        case STATE_READY: printf("READY"); break;
        case STATE_PROCESSING: printf("PROCESSING"); break;
        case STATE_COMPLETE: printf("COMPLETE"); break;
        case STATE_ERROR: printf("ERROR"); break;
        default: printf("UNKNOWN"); break;
    }
    printf("\n");
    
    printf("Available events:\n");
    printf("0 - START\n");
    printf("1 - DATA\n");
    printf("2 - PROCESS\n");
    printf("3 - SUCCESS\n");
    printf("4 - FAILURE\n");
    printf("5 - RESET\n");
    printf("6 - EXIT\n");
    printf("Choose event: ");
}

int main(void) {
    int choice;
    
    printf("State Machine Demo\n");
    printf("Using function pointers for state transitions\n");
    
    while (1) {
        print_menu();
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (choice == 6) {
            break;
        }
        
        if (choice < 0 || choice >= EVENT_COUNT) {
            printf("Invalid event choice\n");
            continue;
        }
        
        process_event((event_t)choice);
    }
    
    printf("State machine terminated\n");
    return 0;
}