//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_FINISHED,
    STATE_ERROR
} state_t;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_DONE,
    EVENT_ERROR,
    EVENT_INVALID
} event_t;

typedef struct {
    state_t current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_count;
} context_t;

static event_t get_next_event(void) {
    char input[MAX_INPUT_LEN];
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_ERROR;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        return EVENT_INVALID;
    }
    
    if (strcmp(input, "start") == 0) {
        return EVENT_START;
    } else if (strcmp(input, "data") == 0) {
        return EVENT_DATA;
    } else if (strcmp(input, "process") == 0) {
        return EVENT_PROCESS;
    } else if (strcmp(input, "done") == 0) {
        return EVENT_DONE;
    } else if (strcmp(input, "error") == 0) {
        return EVENT_ERROR;
    } else {
        return EVENT_INVALID;
    }
}

static state_t handle_start_state(event_t event, context_t *ctx) {
    if (event == EVENT_START) {
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        ctx->buffer_len = 0;
        ctx->data_count = 0;
        printf("State machine started\n");
        return STATE_READING;
    }
    return STATE_ERROR;
}

static state_t handle_reading_state(event_t event, context_t *ctx) {
    if (event == EVENT_DATA) {
        if (ctx->data_count < 5) {
            ctx->data_count++;
            printf("Data received: %d/5\n", ctx->data_count);
            return STATE_READING;
        } else {
            printf("Maximum data count reached\n");
            return STATE_PROCESSING;
        }
    } else if (event == EVENT_PROCESS) {
        return STATE_PROCESSING;
    }
    return STATE_ERROR;
}

static state_t handle_processing_state(event_t event, context_t *ctx) {
    if (event == EVENT_PROCESS) {
        if (ctx->data_count > 0) {
            printf("Processing %d data items\n", ctx->data_count);
            return STATE_FINISHED;
        } else {
            printf("No data to process\n");
            return STATE_ERROR;
        }
    }
    return STATE_ERROR;
}

static state_t handle_finished_state(event_t event, context_t *ctx) {
    if (event == EVENT_DONE) {
        printf("Processing completed successfully\n");
        return STATE_START;
    }
    return STATE_ERROR;
}

static state_t handle_error_state(event_t event, context_t *ctx) {
    printf("Error state encountered\n");
    return STATE_START;
}

static state_t process_event(state_t current_state, event_t event, context_t *ctx) {
    switch (current_state) {
        case STATE_START:
            return handle_start_state(event, ctx);
        case STATE_READING:
            return handle_reading_state(event, ctx);
        case STATE_PROCESSING:
            return handle_processing_state(event, ctx);
        case STATE_FINISHED:
            return handle_finished_state(event, ctx);
        case STATE_ERROR:
            return handle_error_state(event, ctx);
        default:
            return STATE_ERROR;
    }
}

static void print_state_info(state_t state) {
    const char *state_names[] = {
        "START", "READING", "PROCESSING", "FINISHED", "ERROR"
    };
    
    if (state < STATE_START || state > STATE_ERROR) {
        printf("Current state: UNKNOWN\n");
    } else {
        printf("Current state: %s\n", state_names[state]);
    }
}

static void print_instructions(void) {
    printf("Available commands: start, data, process, done, error\n");
    printf("Enter command: ");
}

int main(void) {
    context_t ctx;
    state_t current_state = STATE_START;
    event_t event;
    
    printf("State Machine Demo\n");
    printf("Type 'quit' to exit\n\n");
    
    while (1) {
        print_state_info(current_state);
        print_instructions();
        
        event = get_next_event();
        if (event == EVENT_INVALID) {
            printf("Invalid command. Try again.\n");
            continue;
        }
        
        if (event == EVENT_ERROR) {
            printf("Input error or EOF detected\n");
            break;
        }
        
        state_t next_state = process_event(current_state, event, &ctx);
        
        if (next_state == STATE_ERROR && current_state != STATE_ERROR) {
            printf("Transition to error state\n");
        }
        
        current_state = next_state;
        
        if (current_state == STATE_START) {
            printf("Returned to start state\n");
        }
        
        printf("\n");
    }
    
    printf("State machine terminated\n");
    return 0;
}