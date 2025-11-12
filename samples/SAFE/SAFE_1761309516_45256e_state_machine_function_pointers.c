//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 5

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_OUTPUT,
    STATE_ERROR
} State;

typedef void (*StateHandler)(char, State*);

void handle_idle(char c, State* state) {
    if (c == 'S') {
        *state = STATE_READING;
        printf("Started reading sequence\n");
    } else if (c != ' ' && c != '\n' && c != '\t') {
        *state = STATE_ERROR;
        printf("Error: Invalid start character\n");
    }
}

void handle_reading(char c, State* state) {
    if (c == 'E') {
        *state = STATE_PROCESSING;
        printf("End of sequence detected\n");
    } else if (c >= '0' && c <= '9') {
        printf("Read digit: %c\n", c);
    } else if (c == ' ' || c == '\n' || c == '\t') {
        return;
    } else {
        *state = STATE_ERROR;
        printf("Error: Invalid character in sequence\n");
    }
}

void handle_processing(char c, State* state) {
    if (c == 'P') {
        *state = STATE_OUTPUT;
        printf("Processing complete\n");
    } else if (c == ' ' || c == '\n' || c == '\t') {
        return;
    } else {
        *state = STATE_ERROR;
        printf("Error: Expected processing command\n");
    }
}

void handle_output(char c, State* state) {
    if (c == 'O') {
        printf("Output generated successfully\n");
        *state = STATE_IDLE;
    } else if (c == ' ' || c == '\n' || c == '\t') {
        return;
    } else {
        *state = STATE_ERROR;
        printf("Error: Expected output command\n");
    }
}

void handle_error(char c, State* state) {
    if (c == 'R') {
        *state = STATE_IDLE;
        printf("Reset to idle state\n");
    } else if (c == ' ' || c == '\n' || c == '\t') {
        return;
    } else {
        printf("Error state: waiting for reset\n");
    }
}

int main() {
    State current_state = STATE_IDLE;
    char input[MAX_INPUT_LEN + 1];
    StateHandler handlers[MAX_STATES] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_output,
        handle_error
    };

    printf("State Machine Controller\n");
    printf("Commands: S (start), 0-9 (digits), E (end), P (process), O (output), R (reset)\n");
    printf("Enter commands: ");

    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }

    if (len == 0) {
        printf("No input provided\n");
        return 1;
    }

    for (size_t i = 0; i < len; i++) {
        if (current_state >= 0 && current_state < MAX_STATES) {
            handlers[current_state](input[i], &current_state);
        } else {
            printf("Invalid state encountered\n");
            return 1;
        }
    }

    if (current_state != STATE_IDLE && current_state != STATE_ERROR) {
        printf("Warning: Sequence not properly completed\n");
    }

    return 0;
}