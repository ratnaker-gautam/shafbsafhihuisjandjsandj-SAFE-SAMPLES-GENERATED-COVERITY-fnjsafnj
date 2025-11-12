//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: state_machine
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
    EVENT_COMPLETE,
    EVENT_ERROR,
    EVENT_INVALID
} event_t;

typedef struct {
    state_t current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_count;
} context_t;

static event_t get_next_event(const char* input);
static state_t handle_event(state_t current_state, event_t event, context_t* ctx);
static void process_data(context_t* ctx);
static int is_valid_input(const char* input);

int main(void) {
    context_t ctx = {
        .current_state = STATE_START,
        .buffer_len = 0,
        .data_count = 0
    };
    
    char input[MAX_INPUT_LEN + 2];
    
    printf("State Machine Demo\n");
    printf("Commands: start, data <text>, process, complete, error, quit\n");
    
    while (ctx.current_state != STATE_FINISHED) {
        printf("Current state: %d > ", ctx.current_state);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        event_t event = get_next_event(input);
        state_t next_state = handle_event(ctx.current_state, event, &ctx);
        
        if (next_state == STATE_ERROR) {
            printf("Invalid transition from state %d with event %d\n", 
                   ctx.current_state, event);
        }
        
        ctx.current_state = next_state;
        
        if (ctx.data_count >= MAX_STATES) {
            ctx.current_state = STATE_FINISHED;
        }
    }
    
    printf("Final data count: %d\n", ctx.data_count);
    return 0;
}

static event_t get_next_event(const char* input) {
    if (input == NULL || strlen(input) == 0) {
        return EVENT_INVALID;
    }
    
    if (strcmp(input, "start") == 0) {
        return EVENT_START;
    } else if (strncmp(input, "data ", 5) == 0) {
        const char* data_part = input + 5;
        if (is_valid_input(data_part)) {
            return EVENT_DATA;
        }
    } else if (strcmp(input, "process") == 0) {
        return EVENT_PROCESS;
    } else if (strcmp(input, "complete") == 0) {
        return EVENT_COMPLETE;
    } else if (strcmp(input, "error") == 0) {
        return EVENT_ERROR;
    }
    
    return EVENT_INVALID;
}

static state_t handle_event(state_t current_state, event_t event, context_t* ctx) {
    if (ctx == NULL) {
        return STATE_ERROR;
    }
    
    switch (current_state) {
        case STATE_START:
            switch (event) {
                case EVENT_START:
                    printf("Machine started\n");
                    return STATE_READING;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_READING:
            switch (event) {
                case EVENT_DATA:
                    if (ctx->buffer_len < MAX_INPUT_LEN) {
                        const char* data_input = strchr(ctx->buffer, ' ');
                        if (data_input != NULL) {
                            data_input++;
                            size_t data_len = strlen(data_input);
                            if (data_len > 0 && data_len < MAX_INPUT_LEN) {
                                strncpy(ctx->buffer, data_input, MAX_INPUT_LEN - 1);
                                ctx->buffer[MAX_INPUT_LEN - 1] = '\0';
                                ctx->buffer_len = data_len;
                                printf("Data stored: %s\n", ctx->buffer);
                                return STATE_PROCESSING;
                            }
                        }
                    }
                    return STATE_ERROR;
                case EVENT_ERROR:
                    return STATE_ERROR;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_PROCESSING:
            switch (event) {
                case EVENT_PROCESS:
                    process_data(ctx);
                    return STATE_READING;
                case EVENT_ERROR:
                    return STATE_ERROR;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_FINISHED:
            return STATE_FINISHED;
            
        case STATE_ERROR:
            switch (event) {
                case EVENT_START:
                    printf("Restarting from error state\n");
                    ctx->buffer_len = 0;
                    ctx->data_count = 0;
                    memset(ctx->buffer, 0, sizeof(ctx->buffer));
                    return STATE_READING;
                default:
                    return STATE_ERROR;
            }
            
        default:
            return STATE_ERROR;
    }
}

static void process_data(context_t* ctx) {
    if (ctx == NULL || ctx->buffer_len == 0) {
        return;
    }
    
    printf("Processing: %s\n", ctx->buffer);
    ctx->data_count++;
    
    if (ctx->data_count < MAX_STATES) {
        printf("Processed %d items\n", ctx->data_count);
    } else {
        printf("Reached maximum processing limit\n