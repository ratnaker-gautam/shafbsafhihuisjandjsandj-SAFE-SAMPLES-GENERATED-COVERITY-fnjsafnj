//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 5

typedef enum {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR
} State;

typedef void (*StateHandler)(State*, const char*);

void handle_idle(State* state, const char* input) {
    if (input == NULL) {
        *state = STATE_ERROR;
        return;
    }
    
    if (strlen(input) == 0) {
        *state = STATE_IDLE;
    } else if (strcmp(input, "start") == 0) {
        *state = STATE_READY;
    } else {
        *state = STATE_ERROR;
    }
}

void handle_ready(State* state, const char* input) {
    if (input == NULL) {
        *state = STATE_ERROR;
        return;
    }
    
    if (strlen(input) == 0) {
        *state = STATE_READY;
    } else if (strcmp(input, "process") == 0) {
        *state = STATE_PROCESSING;
    } else if (strcmp(input, "reset") == 0) {
        *state = STATE_IDLE;
    } else {
        *state = STATE_ERROR;
    }
}

void handle_processing(State* state, const char* input) {
    if (input == NULL) {
        *state = STATE_ERROR;
        return;
    }
    
    if (strlen(input) == 0) {
        *state = STATE_PROCESSING;
    } else if (strcmp(input, "complete") == 0) {
        *state = STATE_COMPLETE;
    } else if (strcmp(input, "error") == 0) {
        *state = STATE_ERROR;
    } else {
        *state = STATE_ERROR;
    }
}

void handle_complete(State* state, const char* input) {
    if (input == NULL) {
        *state = STATE_ERROR;
        return;
    }
    
    if (strlen(input) == 0) {
        *state = STATE_COMPLETE;
    } else if (strcmp(input, "reset") == 0) {
        *state = STATE_IDLE;
    } else {
        *state = STATE_ERROR;
    }
}

void handle_error(State* state, const char* input) {
    if (input == NULL) {
        *state = STATE_ERROR;
        return;
    }
    
    if (strlen(input) == 0) {
        *state = STATE_ERROR;
    } else if (strcmp(input, "reset") == 0) {
        *state = STATE_IDLE;
    } else {
        *state = STATE_ERROR;
    }
}

StateHandler get_handler(State state) {
    static StateHandler handlers[] = {
        handle_idle,
        handle_ready,
        handle_processing,
        handle_complete,
        handle_error
    };
    
    if (state >= 0 && state < MAX_STATES) {
        return handlers[state];
    }
    return handle_error;
}

const char* state_to_string(State state) {
    static const char* names[] = {
        "IDLE",
        "READY",
        "PROCESSING",
        "COMPLETE",
        "ERROR"
    };
    
    if (state >= 0 && state < MAX_STATES) {
        return names[state];
    }
    return "UNKNOWN";
}

int main() {
    State current_state = STATE_IDLE;
    char input[MAX_INPUT_LEN + 1];
    
    printf("State Machine Controller\n");
    printf("Available commands: start, process, complete, error, reset\n");
    printf("Enter empty line to exit\n\n");
    
    while (1) {
        printf("Current state: %s\n", state_to_string(current_state));
        printf("Enter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) {
            break;
        }
        
        if (len >= MAX_INPUT_LEN) {
            printf("Input too long. Maximum %d characters allowed.\n", MAX_INPUT_LEN);
            continue;
        }
        
        StateHandler handler = get_handler(current_state);
        handler(&current_state, input);
    }
    
    printf("Final state: %s\n", state_to_string(current_state));
    printf("Goodbye!\n");
    
    return 0;
}