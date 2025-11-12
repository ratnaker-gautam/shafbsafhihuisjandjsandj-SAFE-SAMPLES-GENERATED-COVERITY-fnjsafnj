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
    STATE_DONE,
    STATE_ERROR
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
} Event;

typedef struct {
    char buffer[MAX_INPUT_LEN];
    size_t length;
    int processed_count;
} Context;

State handle_start(Context* ctx, Event evt);
State handle_reading(Context* ctx, Event evt);
State handle_processing(Context* ctx, Event evt);
State handle_done(Context* ctx, Event evt);
State handle_error(Context* ctx, Event evt);

State (*state_handlers[])(Context*, Event) = {
    handle_start,
    handle_reading,
    handle_processing,
    handle_done,
    handle_error
};

State handle_start(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_START:
            memset(ctx->buffer, 0, sizeof(ctx->buffer));
            ctx->length = 0;
            ctx->processed_count = 0;
            printf("State machine started\n");
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

State handle_reading(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_DATA:
            if (ctx->length < MAX_INPUT_LEN - 1) {
                char input[MAX_INPUT_LEN];
                if (fgets(input, sizeof(input), stdin) != NULL) {
                    size_t len = strlen(input);
                    if (len > 0 && input[len-1] == '\n') {
                        input[len-1] = '\0';
                        len--;
                    }
                    if (ctx->length + len < MAX_INPUT_LEN) {
                        strncpy(ctx->buffer + ctx->length, input, len);
                        ctx->length += len;
                        ctx->buffer[ctx->length] = '\0';
                        printf("Data added. Current length: %zu\n", ctx->length);
                    } else {
                        printf("Input too long\n");
                        return STATE_ERROR;
                    }
                } else {
                    return STATE_ERROR;
                }
            }
            return STATE_READING;
        case EVENT_COMPLETE:
            if (ctx->length > 0) {
                return STATE_PROCESSING;
            }
            return STATE_READING;
        case EVENT_RESET:
            return handle_start(ctx, EVENT_START);
        default:
            return STATE_ERROR;
    }
}

State handle_processing(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_COMPLETE:
            if (ctx->length > 0) {
                int word_count = 0;
                int in_word = 0;
                
                for (size_t i = 0; i < ctx->length; i++) {
                    if (ctx->buffer[i] != ' ' && ctx->buffer[i] != '\t') {
                        if (!in_word) {
                            word_count++;
                            in_word = 1;
                        }
                    } else {
                        in_word = 0;
                    }
                }
                
                ctx->processed_count = word_count;
                printf("Processing complete. Found %d words\n", word_count);
                return STATE_DONE;
            }
            return STATE_ERROR;
        case EVENT_RESET:
            return handle_start(ctx, EVENT_START);
        default:
            return STATE_ERROR;
    }
}

State handle_done(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_RESET:
            return handle_start(ctx, EVENT_START);
        case EVENT_START:
            return handle_start(ctx, EVENT_START);
        default:
            return STATE_DONE;
    }
}

State handle_error(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_RESET:
            return handle_start(ctx, EVENT_START);
        case EVENT_START:
            return handle_start(ctx, EVENT_START);
        default:
            return STATE_ERROR;
    }
}

Event get_user_event(void) {
    char input[10];
    printf("Enter event (0=start, 1=data, 2=complete, 3=error, 4=reset, 5=quit): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_ERROR;
    }
    
    char* endptr;
    long choice = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') {
        return EVENT_ERROR;
    }
    
    if (choice < 0 || choice > 5) {
        return EVENT_ERROR;
    }
    
    if (choice == 5) {
        exit(0);
    }
    
    return (Event)choice;
}

int main(void) {
    State current_state = STATE_START;
    Context context;
    memset(&context, 0, sizeof(context));
    
    printf("State Machine Demo - Word Counter\n");
    
    while (1) {
        Event evt = get_user_event();
        
        if (current_state < 0 || current_state >= (int