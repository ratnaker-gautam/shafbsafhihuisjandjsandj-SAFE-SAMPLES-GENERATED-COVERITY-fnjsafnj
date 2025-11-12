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

State current_state = STATE_IDLE;
int data_value = 0;
int process_count = 0;

void handle_idle(void) {
    printf("State: IDLE - Waiting for start command\n");
}

void handle_ready(void) {
    printf("State: READY - Enter data value (0-100): ");
    int result = scanf("%d", &data_value);
    if (result != 1) {
        printf("Invalid input\n");
        current_state = STATE_ERROR;
        return;
    }
    if (data_value < 0 || data_value > 100) {
        printf("Data value out of range\n");
        current_state = STATE_ERROR;
        return;
    }
    printf("Data accepted: %d\n", data_value);
}

void handle_processing(void) {
    printf("State: PROCESSING - Processing data: %d\n", data_value);
    if (data_value > 0 && data_value < 50) {
        printf("Processing successful\n");
        current_state = STATE_COMPLETE;
    } else if (data_value >= 50) {
        printf("Processing failed\n");
        current_state = STATE_ERROR;
    } else {
        printf("Invalid data for processing\n");
        current_state = STATE_ERROR;
    }
    process_count++;
}

void handle_complete(void) {
    printf("State: COMPLETE - Processing finished successfully\n");
    printf("Total processes completed: %d\n", process_count);
}

void handle_error(void) {
    printf("State: ERROR - An error occurred\n");
}

int transition(Event event) {
    static const State transition_table[STATE_COUNT][EVENT_COUNT] = {
        {STATE_READY,     STATE_IDLE, STATE_IDLE,     STATE_IDLE,   STATE_IDLE,   STATE_IDLE},
        {STATE_READY,     STATE_READY, STATE_PROCESSING, STATE_READY, STATE_ERROR, STATE_IDLE},
        {STATE_PROCESSING, STATE_PROCESSING, STATE_PROCESSING, STATE_COMPLETE, STATE_ERROR, STATE_IDLE},
        {STATE_COMPLETE,  STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_COMPLETE, STATE_IDLE},
        {STATE_ERROR,     STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_IDLE}
    };

    if (event < 0 || event >= EVENT_COUNT) {
        return -1;
    }
    if (current_state < 0 || current_state >= STATE_COUNT) {
        return -1;
    }

    State new_state = transition_table[current_state][event];
    if (new_state < 0 || new_state >= STATE_COUNT) {
        return -1;
    }

    current_state = new_state;
    return 0;
}

void print_menu(void) {
    printf("\nAvailable events:\n");
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
    printf("State Machine Demo\n");
    printf("Initial state: IDLE\n");

    int running = 1;
    while (running) {
        if (current_state < 0 || current_state >= STATE_COUNT) {
            printf("Invalid state detected\n");
            break;
        }

        state_handlers[current_state]();

        print_menu();
        int choice;
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }

        if (choice == 6) {
            running = 0;
            continue;
        }

        if (choice < 0 || choice >= EVENT_COUNT) {
            printf("Invalid event choice\n");
            continue;
        }

        if (transition((Event)choice) != 0) {
            printf("Transition failed\n");
        }
    }

    printf("Program terminated\n");
    return 0;
}