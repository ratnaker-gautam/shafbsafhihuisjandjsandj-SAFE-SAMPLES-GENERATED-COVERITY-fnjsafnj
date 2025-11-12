//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: state_machine
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

typedef struct {
    State next_state;
    int requires_data;
} Transition;

Transition state_table[STATE_COUNT][EVENT_COUNT] = {
    {{STATE_IDLE, 0}, {STATE_READY, 1}, {STATE_IDLE, 0}, {STATE_IDLE, 0}, {STATE_IDLE, 0}, {STATE_IDLE, 0}},
    {{STATE_IDLE, 0}, {STATE_READY, 1}, {STATE_PROCESSING, 0}, {STATE_READY, 0}, {STATE_ERROR, 0}, {STATE_IDLE, 0}},
    {{STATE_PROCESSING, 0}, {STATE_PROCESSING, 0}, {STATE_PROCESSING, 0}, {STATE_COMPLETE, 0}, {STATE_ERROR, 0}, {STATE_IDLE, 0}},
    {{STATE_COMPLETE, 0}, {STATE_COMPLETE, 0}, {STATE_COMPLETE, 0}, {STATE_COMPLETE, 0}, {STATE_COMPLETE, 0}, {STATE_IDLE, 0}},
    {{STATE_ERROR, 0}, {STATE_ERROR, 0}, {STATE_ERROR, 0}, {STATE_ERROR, 0}, {STATE_ERROR, 0}, {STATE_IDLE, 0}}
};

int process_event(Event event, int data) {
    if (event < 0 || event >= EVENT_COUNT) {
        return 0;
    }
    
    Transition trans = state_table[current_state][event];
    
    if (trans.requires_data && data < 0) {
        return 0;
    }
    
    if (trans.requires_data) {
        data_value = data;
    }
    
    if (trans.next_state != current_state) {
        current_state = trans.next_state;
        if (current_state == STATE_IDLE) {
            data_value = 0;
            process_count = 0;
        }
        return 1;
    }
    
    return 0;
}

void print_menu(void) {
    printf("\nAvailable events:\n");
    printf("0 - START\n");
    printf("1 - DATA (requires value)\n");
    printf("2 - PROCESS\n");
    printf("3 - SUCCESS\n");
    printf("4 - FAILURE\n");
    printf("5 - RESET\n");
    printf("6 - EXIT\n");
    printf("Current state: ");
}

int main(void) {
    int running = 1;
    
    printf("State Machine Demo\n");
    
    while (running) {
        state_handlers[current_state]();
        print_menu();
        
        int choice;
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input\n");
            continue;
        }
        
        if (choice == 6) {
            running = 0;
            continue;
        }
        
        if (choice < 0 || choice > 5) {
            printf("Invalid event\n");
            continue;
        }
        
        Event event = (Event)choice;
        int data = -1;
        
        if (event == EVENT_DATA) {
            printf("Enter data value: ");
            if (scanf("%d", &data) != 1) {
                while (getchar() != '\n');
                printf("Invalid data\n");
                continue;
            }
            if (data < 0) {
                printf("Data must be non-negative\n");
                continue;
            }
        }
        
        if (!process_event(event, data)) {
            printf("Event not allowed in current state\n");
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}