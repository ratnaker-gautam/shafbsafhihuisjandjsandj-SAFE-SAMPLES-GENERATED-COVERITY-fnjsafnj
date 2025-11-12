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

struct StateMachine {
    State current_state;
    int data_value;
    int error_code;
};

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

struct StateMachine sm = {STATE_IDLE, 0, 0};

void transition_state(State new_state) {
    if (new_state >= 0 && new_state < STATE_COUNT) {
        sm.current_state = new_state;
    }
}

void handle_idle(void) {
    printf("State: IDLE - Waiting for start event\n");
}

void handle_ready(void) {
    printf("State: READY - Data loaded: %d\n", sm.data_value);
}

void handle_processing(void) {
    printf("State: PROCESSING - Processing data: %d\n", sm.data_value);
    if (sm.data_value < 0) {
        sm.error_code = 1;
        transition_state(STATE_ERROR);
    } else if (sm.data_value > 1000) {
        sm.error_code = 2;
        transition_state(STATE_ERROR);
    }
}

void handle_complete(void) {
    printf("State: COMPLETE - Operation successful\n");
}

void handle_error(void) {
    printf("State: ERROR - Code: %d\n", sm.error_code);
}

int process_event(Event event, int data) {
    if (event < 0 || event >= EVENT_COUNT) {
        return -1;
    }

    switch (sm.current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                sm.data_value = data;
                transition_state(STATE_READY);
            }
            break;
        case STATE_READY:
            if (event == EVENT_DATA) {
                if (data >= 0 && data <= 10000) {
                    sm.data_value = data;
                } else {
                    return -1;
                }
            } else if (event == EVENT_PROCESS) {
                transition_state(STATE_PROCESSING);
            }
            break;
        case STATE_PROCESSING:
            if (event == EVENT_SUCCESS) {
                transition_state(STATE_COMPLETE);
            } else if (event == EVENT_FAILURE) {
                sm.error_code = 3;
                transition_state(STATE_ERROR);
            }
            break;
        case STATE_COMPLETE:
            if (event == EVENT_RESET) {
                sm.data_value = 0;
                sm.error_code = 0;
                transition_state(STATE_IDLE);
            }
            break;
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                sm.data_value = 0;
                sm.error_code = 0;
                transition_state(STATE_IDLE);
            }
            break;
        default:
            return -1;
    }
    return 0;
}

void print_menu(void) {
    printf("\nAvailable events:\n");
    printf("0 - START (with initial data)\n");
    printf("1 - DATA (update data)\n");
    printf("2 - PROCESS\n");
    printf("3 - SUCCESS\n");
    printf("4 - FAILURE\n");
    printf("5 - RESET\n");
    printf("6 - EXIT\n");
    printf("Current state: ");
}

int main(void) {
    int running = 1;
    int choice;
    int data_input;

    printf("State Machine Demo\n");

    while (running) {
        state_handlers[sm.current_state]();
        print_menu();

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input\n");
            continue;
        }

        if (choice == 6) {
            running = 0;
            continue;
        }

        if (choice < 0 || choice >= EVENT_COUNT) {
            printf("Invalid event\n");
            continue;
        }

        if (choice == EVENT_START || choice == EVENT_DATA) {
            printf("Enter data (0-10000): ");
            if (scanf("%d", &data_input) != 1) {
                while (getchar() != '\n');
                printf("Invalid data\n");
                continue;
            }
        } else {
            data_input = 0;
        }

        if (process_event((Event)choice, data_input) != 0) {
            printf("Event not allowed in current state\n");
        }
    }

    printf("Exiting state machine\n");
    return 0;
}