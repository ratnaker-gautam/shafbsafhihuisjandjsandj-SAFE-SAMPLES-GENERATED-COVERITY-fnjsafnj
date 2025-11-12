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
    EVENT_RESET,
    EVENT_QUIT
} Event;

typedef struct {
    char buffer[MAX_INPUT_LEN];
    size_t length;
    int processed_count;
} Context;

State handle_start(Context* ctx, Event ev);
State handle_reading(Context* ctx, Event ev);
State handle_processing(Context* ctx, Event ev);
State handle_done(Context* ctx, Event ev);
State handle_error(Context* ctx, Event ev);

State (*state_handlers[])(Context*, Event) = {
    handle_start,
    handle_reading,
    handle_processing,
    handle_done,
    handle_error
};

State handle_start(Context* ctx, Event ev) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (ev) {
        case EVENT_START:
            memset(ctx->buffer, 0, MAX_INPUT_LEN);
            ctx->length = 0;
            ctx->processed_count = 0;
            printf("State machine started\n");
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

State handle_reading(Context* ctx, Event ev) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (ev) {
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
                        printf("Data added: %s\n", input);
                        return STATE_READING;
                    }
                }
            }
            return STATE_ERROR;
        case EVENT_COMPLETE:
            if (ctx->length > 0) {
                return STATE_PROCESSING;
            }
            return STATE_ERROR;
        case EVENT_RESET:
            return handle_start(ctx, EVENT_START);
        default:
            return STATE_ERROR;
    }
}

State handle_processing(Context* ctx, Event ev) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (ev) {
        case EVENT_COMPLETE:
            if (ctx->length > 0) {
                printf("Processing data: %s\n", ctx->buffer);
                ctx->processed_count++;
                
                int char_count = 0;
                int digit_count = 0;
                for (size_t i = 0; i < ctx->length; i++) {
                    if ((ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') || 
                        (ctx->buffer[i] >= 'A' && ctx->buffer[i] <= 'Z')) {
                        char_count++;
                    } else if (ctx->buffer[i] >= '0' && ctx->buffer[i] <= '9') {
                        digit_count++;
                    }
                }
                printf("Characters: %d, Digits: %d\n", char_count, digit_count);
                return STATE_DONE;
            }
            return STATE_ERROR;
        case EVENT_RESET:
            return handle_start(ctx, EVENT_START);
        default:
            return STATE_ERROR;
    }
}

State handle_done(Context* ctx, Event ev) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (ev) {
        case EVENT_START:
            return handle_start(ctx, EVENT_START);
        case EVENT_RESET:
            return handle_start(ctx, EVENT_START);
        default:
            return STATE_DONE;
    }
}

State handle_error(Context* ctx, Event ev) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (ev) {
        case EVENT_RESET:
            return handle_start(ctx, EVENT_START);
        default:
            printf("Error state - reset required\n");
            return STATE_ERROR;
    }
}

Event get_user_event(void) {
    char input[10];
    printf("Enter event (0=start, 1=data, 2=complete, 3=error, 4=reset, 5=quit): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_ERROR;
    }
    
    int choice;
    if (sscanf(input, "%d", &choice) != 1) {
        return EVENT_ERROR;
    }
    
    if (choice >= 0 && choice <= 5) {
        return (Event)choice;
    }
    
    return EVENT_ERROR;
}

int main(void) {
    Context context;
    State current_state = STATE_START;
    Event current_event = EVENT_START;
    int running = 1;
    
    printf("State Machine Demo\n");
    
    while (running) {
        if (current_state >= 0 && current_state < (int)(sizeof(state_handlers)/sizeof(state_handlers[0]))) {
            current_state = state_handlers[current_state](&context, current_event);
        } else {
            current