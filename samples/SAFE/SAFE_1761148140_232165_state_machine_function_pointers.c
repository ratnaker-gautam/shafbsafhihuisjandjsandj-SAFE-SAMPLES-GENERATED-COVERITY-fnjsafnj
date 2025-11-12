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
static char buffer[256];
static size_t buffer_pos = 0;

void state_idle_handler(void) {
    printf("State: IDLE - Enter 'start' to begin: ");
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;
        if (strcmp(buffer, "start") == 0) {
            current_state = STATE_READY;
        } else {
            printf("Invalid command. Only 'start' allowed.\n");
        }
    }
}

void state_ready_handler(void) {
    printf("State: READY - Enter data (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;
        size_t len = strlen(buffer);
        if (len > 0 && len < sizeof(buffer)) {
            buffer_pos = len;
            current_state = STATE_PROCESSING;
        } else {
            printf("Invalid data length.\n");
            current_state = STATE_ERROR;
        }
    }
}

void state_processing_handler(void) {
    printf("State: PROCESSING - Processing data: %s\n", buffer);
    size_t valid_chars = 0;
    for (size_t i = 0; i < buffer_pos; i++) {
        if (buffer[i] >= 'A' && buffer[i] <= 'Z') {
            valid_chars++;
        }
    }
    if (valid_chars > 0) {
        printf("Found %zu uppercase letters. Processing successful.\n", valid_chars);
        current_state = STATE_COMPLETE;
    } else {
        printf("No uppercase letters found. Processing failed.\n");
        current_state = STATE_ERROR;
    }
}

void state_complete_handler(void) {
    printf("State: COMPLETE - Enter 'reset' to restart: ");
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;
        if (strcmp(buffer, "reset") == 0) {
            current_state = STATE_IDLE;
            buffer_pos = 0;
            memset(buffer, 0, sizeof(buffer));
        } else {
            printf("Invalid command. Only 'reset' allowed.\n");
        }
    }
}

void state_error_handler(void) {
    printf("State: ERROR - Enter 'reset' to restart: ");
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;
        if (strcmp(buffer, "reset") == 0) {
            current_state = STATE_IDLE;
            buffer_pos = 0;
            memset(buffer, 0, sizeof(buffer));
        } else {
            printf("Invalid command. Only 'reset' allowed.\n");
        }
    }
}

int main(void) {
    StateHandler handlers[STATE_COUNT] = {
        state_idle_handler,
        state_ready_handler,
        state_processing_handler,
        state_complete_handler,
        state_error_handler
    };

    printf("State Machine Demo Started\n");
    printf("Available commands: start, reset\n\n");

    while (1) {
        if (current_state >= 0 && current_state < STATE_COUNT) {
            handlers[current_state]();
        } else {
            printf("Invalid state detected. Resetting to IDLE.\n");
            current_state = STATE_IDLE;
        }
    }

    return 0;
}