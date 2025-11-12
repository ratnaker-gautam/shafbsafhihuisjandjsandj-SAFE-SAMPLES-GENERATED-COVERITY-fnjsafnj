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

static State current_state = STATE_IDLE;
static int data_value = 0;
static int process_count = 0;

void handle_idle(void) {
    printf("State: IDLE - Waiting for start command\n");
}

void handle_ready(void) {
    printf("State: READY - Data loaded: %d\n", data_value);
}

void handle_processing(void) {
    printf("State: PROCESSING - Working on data: %d\n", data_value);
    process_count++;
}

void handle_complete(void) {
    printf("State: COMPLETE - Processed %d times\n", process_count);
}

void handle_error(void) {
    printf("State: ERROR - Something went wrong\n");
}

StateHandler state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

int state_transitions[STATE_COUNT][EVENT_COUNT] = {
    {STATE_READY,    STATE_IDLE, STATE_IDLE,      STATE_IDLE,    STATE_ERROR, STATE_IDLE},
    {STATE_READY,    STATE_READY, STATE_PROCESSING, STATE_READY,  STATE_ERROR, STATE_IDLE},
    {STATE_PROCESSING, STATE_PROCESSING, STATE_PROCESSING, STATE_COMPLETE, STATE_ERROR, STATE_IDLE},
    {STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_IDLE},
    {STATE_ERROR,    STATE_ERROR, STATE_ERROR,    STATE_ERROR,    STATE_ERROR, STATE_IDLE}
};

void process_event(Event event, int data) {
    if (event < 0 || event >= EVENT_COUNT) {
        printf("Invalid event\n");
        return;
    }
    
    State next_state = state_transitions[current_state][event];
    
    if (next_state < 0 || next_state >= STATE_COUNT) {
        printf("Invalid state transition\n");
        return;
    }
    
    switch (event) {
        case EVENT_START:
            if (current_state == STATE_IDLE) {
                data_value = 0;
                process_count = 0;
            }
            break;
        case EVENT_DATA:
            if (data >= 0 && data <= 100) {
                data_value = data;
            } else {
                printf("Invalid data value\n");
                return;
            }
            break;
        case EVENT_PROCESS:
            if (current_state == STATE_READY && data_value > 0) {
                data_value *= 2;
                if (data_value > 1000) {
                    data_value = 1000;
                }
            }
            break;
        case EVENT_SUCCESS:
            break;
        case EVENT_FAILURE:
            break;
        case EVENT_RESET:
            data_value = 0;
            process_count = 0;
            break;
        default:
            break;
    }
    
    current_state = next_state;
    state_handlers[current_state]();
}

void print_menu(void) {
    printf("\nAvailable events:\n");
    printf("0 - START\n");
    printf("1 - DATA (requires value 0-100)\n");
    printf("2 - PROCESS\n");
    printf("3 - SUCCESS\n");
    printf("4 - FAILURE\n");
    printf("5 - RESET\n");
    printf("6 - EXIT\n");
    printf("Choose event: ");
}

int main(void) {
    int choice;
    int value;
    
    printf("State Machine Demo Started\n");
    state_handlers[current_state]();
    
    while (1) {
        print_menu();
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (choice == 6) {
            printf("Exiting state machine demo\n");
            break;
        }
        
        if (choice < 0 || choice >= EVENT_COUNT) {
            printf("Invalid event choice\n");
            continue;
        }
        
        if (choice == 1) {
            printf("Enter data value (0-100): ");
            if (scanf("%d", &value) != 1) {
                printf("Invalid data input\n");
                while (getchar() != '\n');
                continue;
            }
            process_event((Event)choice, value);
        } else {
            process_event((Event)choice, 0);
        }
    }
    
    return 0;
}