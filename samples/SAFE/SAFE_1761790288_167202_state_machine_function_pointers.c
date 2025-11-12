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
    printf("State: IDLE\n");
    printf("Enter command (start/reset/quit): ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = '\0';
        if (strcmp(input, "start") == 0) {
            current_state = STATE_READY;
        } else if (strcmp(input, "reset") == 0) {
            current_state = STATE_IDLE;
            data_value = 0;
            process_count = 0;
        } else if (strcmp(input, "quit") == 0) {
            exit(0);
        }
    }
}

void handle_ready(void) {
    printf("State: READY\n");
    printf("Enter data value (0-100) or 'back': ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = '\0';
        if (strcmp(input, "back") == 0) {
            current_state = STATE_IDLE;
        } else {
            char *endptr;
            long val = strtol(input, &endptr, 10);
            if (endptr != input && *endptr == '\0' && val >= 0 && val <= 100) {
                data_value = (int)val;
                current_state = STATE_PROCESSING;
            } else {
                printf("Invalid input. Please enter 0-100 or 'back'.\n");
            }
        }
    }
}

void handle_processing(void) {
    printf("State: PROCESSING\n");
    printf("Data value: %d\n", data_value);
    printf("Enter 'process' to continue or 'back': ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = '\0';
        if (strcmp(input, "back") == 0) {
            current_state = STATE_READY;
        } else if (strcmp(input, "process") == 0) {
            if (data_value > 0 && data_value <= 100) {
                int result = data_value * 2;
                if (result > 150) {
                    current_state = STATE_ERROR;
                } else {
                    data_value = result;
                    process_count++;
                    if (process_count >= 3) {
                        current_state = STATE_COMPLETE;
                    }
                }
            } else {
                current_state = STATE_ERROR;
            }
        } else {
            printf("Invalid command.\n");
        }
    }
}

void handle_complete(void) {
    printf("State: COMPLETE\n");
    printf("Final data value: %d\n", data_value);
    printf("Processing steps: %d\n", process_count);
    printf("Enter 'reset' to start over or 'quit': ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = '\0';
        if (strcmp(input, "reset") == 0) {
            current_state = STATE_IDLE;
            data_value = 0;
            process_count = 0;
        } else if (strcmp(input, "quit") == 0) {
            exit(0);
        }
    }
}

void handle_error(void) {
    printf("State: ERROR\n");
    printf("Data value out of range: %d\n", data_value);
    printf("Enter 'reset' to start over or 'quit': ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = '\0';
        if (strcmp(input, "reset") == 0) {
            current_state = STATE_IDLE;
            data_value = 0;
            process_count = 0;
        } else if (strcmp(input, "quit") == 0) {
            exit(0);
        }
    }
}

int main(void) {
    StateHandler handlers[STATE_COUNT] = {
        handle_idle,
        handle_ready,
        handle_processing,
        handle_complete,
        handle_error
    };

    printf("State Machine Demo\n");
    printf("==================\n");

    while (1) {
        if (current_state >= 0 && current_state < STATE_COUNT) {
            handlers[current_state]();
        } else {
            current_state = STATE_IDLE;
        }
        printf("\n");
    }

    return 0;
}