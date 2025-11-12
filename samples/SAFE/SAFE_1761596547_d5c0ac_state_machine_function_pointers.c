//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_DONE,
    STATE_ERROR
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET
} Event;

typedef void (*StateHandler)(void);

struct StateMachine {
    State current_state;
    int data_value;
    int result;
};

void handle_idle(void);
void handle_ready(void);
void handle_processing(void);
void handle_done(void);
void handle_error(void);

StateHandler handlers[] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_done,
    handle_error
};

struct StateMachine sm;

void handle_idle(void) {
    printf("State: IDLE\n");
    printf("Enter command (start/reset/quit): ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "start") == 0) {
            sm.current_state = STATE_READY;
        } else if (strcmp(input, "reset") == 0) {
            sm.current_state = STATE_IDLE;
        } else if (strcmp(input, "quit") == 0) {
            exit(0);
        }
    }
}

void handle_ready(void) {
    printf("State: READY\n");
    printf("Enter data value (0-100): ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) != NULL) {
        int value = atoi(input);
        if (value >= 0 && value <= 100) {
            sm.data_value = value;
            sm.current_state = STATE_PROCESSING;
        } else {
            printf("Invalid input. Must be 0-100.\n");
        }
    }
}

void handle_processing(void) {
    printf("State: PROCESSING\n");
    printf("Processing data: %d\n", sm.data_value);
    
    if (sm.data_value < 0 || sm.data_value > 100) {
        sm.current_state = STATE_ERROR;
        return;
    }
    
    if (sm.data_value == 0) {
        sm.current_state = STATE_ERROR;
        return;
    }
    
    sm.result = sm.data_value * 2;
    
    if (sm.result > 150) {
        sm.current_state = STATE_ERROR;
    } else {
        sm.current_state = STATE_DONE;
    }
}

void handle_done(void) {
    printf("State: DONE\n");
    printf("Result: %d\n", sm.result);
    printf("Enter command (reset/quit): ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "reset") == 0) {
            sm.current_state = STATE_IDLE;
            sm.data_value = 0;
            sm.result = 0;
        } else if (strcmp(input, "quit") == 0) {
            exit(0);
        }
    }
}

void handle_error(void) {
    printf("State: ERROR\n");
    printf("An error occurred during processing.\n");
    printf("Enter command (reset/quit): ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "reset") == 0) {
            sm.current_state = STATE_IDLE;
            sm.data_value = 0;
            sm.result = 0;
        } else if (strcmp(input, "quit") == 0) {
            exit(0);
        }
    }
}

int main(void) {
    sm.current_state = STATE_IDLE;
    sm.data_value = 0;
    sm.result = 0;
    
    printf("State Machine Demo Started\n");
    
    while (1) {
        if (sm.current_state >= STATE_IDLE && sm.current_state <= STATE_ERROR) {
            handlers[sm.current_state]();
        } else {
            sm.current_state = STATE_ERROR;
        }
    }
    
    return 0;
}