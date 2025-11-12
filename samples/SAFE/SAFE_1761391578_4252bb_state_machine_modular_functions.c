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
        return EVENT_INVALID;
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

static void handle_start(context_t *ctx) {
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_len = 0;
    ctx->data_count = 0;
    printf("State machine started\n");
}

static void handle_reading(context_t *ctx) {
    if (ctx->data_count < MAX_STATES) {
        ctx->data_count++;
        printf("Reading data %d\n", ctx->data_count);
    } else {
        printf("Maximum data count reached\n");
    }
}

static void handle_processing(context_t *ctx) {
    if (ctx->data_count > 0) {
        printf("Processing %d data items\n", ctx->data_count);
    } else {
        printf("No data to process\n");
    }
}

static state_t state_transition(state_t current, event_t event, context_t *ctx) {
    switch (current) {
        case STATE_START:
            if (event == EVENT_START) {
                handle_start(ctx);
                return STATE_READING;
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) {
                handle_reading(ctx);
                return STATE_READING;
            } else if (event == EVENT_PROCESS) {
                return STATE_PROCESSING;
            } else if (event == EVENT_ERROR) {
                return STATE_ERROR;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_DONE) {
                printf("Processing completed\n");
                return STATE_FINISHED;
            } else if (event == EVENT_ERROR) {
                return STATE_ERROR;
            }
            break;
            
        case STATE_FINISHED:
            if (event == EVENT_START) {
                handle_start(ctx);
                return STATE_READING;
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_START) {
                handle_start(ctx);
                return STATE_READING;
            }
            break;
            
        default:
            break;
    }
    
    printf("Invalid transition from state %d with event %d\n", current, event);
    return current;
}

static void print_state_info(state_t state) {
    const char *state_names[] = {
        "START", "READING", "PROCESSING", "FINISHED", "ERROR"
    };
    
    if (state >= STATE_START && state <= STATE_ERROR) {
        printf("Current state: %s\n", state_names[state]);
    }
}

static void print_instructions(void) {
    printf("Available commands: start, data, process, done, error\n");
    printf("Enter 'quit' to exit\n");
}

int main(void) {
    context_t ctx;
    state_t current_state = STATE_START;
    event_t event;
    int running = 1;
    
    memset(&ctx, 0, sizeof(ctx));
    
    print_instructions();
    print_state_info(current_state);
    
    while (running) {
        printf("> ");
        fflush(stdout);
        
        event = get_next_event();
        
        if (event == EVENT_INVALID) {
            printf("Invalid command\n");
            print_instructions();
            continue;
        }
        
        state_t new_state = state_transition(current_state, event, &ctx);
        
        if (new_state != current_state) {
            current_state = new_state;
            print_state_info(current_state);
        }
        
        if (current_state == STATE_FINISHED) {
            printf("Work completed successfully\n");
        } else if (current_state == STATE_ERROR) {
            printf("Error state encountered\n");
        }
    }
    
    return 0;
}