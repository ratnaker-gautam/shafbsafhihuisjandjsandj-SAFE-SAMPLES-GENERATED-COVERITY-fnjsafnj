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

typedef struct {
    State next_state;
    int valid;
} Transition;

Transition state_table[STATE_COUNT][EVENT_COUNT] = {
    {{STATE_READY, 1}, {STATE_IDLE, 0}, {STATE_IDLE, 0}, {STATE_IDLE, 0}, {STATE_IDLE, 0}, {STATE_IDLE, 1}},
    {{STATE_READY, 0}, {STATE_READY, 1}, {STATE_PROCESSING, 1}, {STATE_READY, 0}, {STATE_ERROR, 1}, {STATE_IDLE, 1}},
    {{STATE_PROCESSING, 0}, {STATE_PROCESSING, 0}, {STATE_PROCESSING, 0}, {STATE_COMPLETE, 1}, {STATE_ERROR, 1}, {STATE_IDLE, 1}},
    {{STATE_COMPLETE, 0}, {STATE_COMPLETE, 0}, {STATE_COMPLETE, 0}, {STATE_COMPLETE, 0}, {STATE_COMPLETE, 0}, {STATE_IDLE, 1}},
    {{STATE_ERROR, 0}, {STATE_ERROR, 0}, {STATE_ERROR, 0}, {STATE_ERROR, 0}, {STATE_ERROR, 0}, {STATE_IDLE, 1}}
};

int process_event(Event event, int data) {
    if (event < 0 || event >= EVENT_COUNT) {
        return 0;
    }
    
    Transition trans = state_table[current_state][event];
    if (!trans.valid) {
        return 0;
    }
    
    switch (event) {
        case EVENT_DATA:
            if (data >= 0 && data <= 100) {
                data_value = data;
            } else {
                return 0;
            }
            break;
        case EVENT_PROCESS:
            if (data_value <= 0) {
                return 0;
            }
            break;
        default:
            break;
    }
    
    current_state = trans.next_state;
    
    if (event == EVENT_RESET) {
        data_value = 0;
        process_count = 0;
    }
    
    return 1;
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
    int running = 1;
    
    printf("State Machine Demo\n");
    printf("==================\n");
    
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
        int data = 0;
        
        if (event == EVENT_DATA) {
            printf("Enter data value (0-100): ");
            if (scanf("%d", &data) != 1) {
                while (getchar() != '\n');
                printf("Invalid data\n");
                continue;
            }
        }
        
        if (!process_event(event, data)) {
            printf("Event not allowed in current state or invalid data\n");
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}