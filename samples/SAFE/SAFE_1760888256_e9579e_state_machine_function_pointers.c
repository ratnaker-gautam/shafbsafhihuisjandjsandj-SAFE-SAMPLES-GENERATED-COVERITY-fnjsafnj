//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_COUNT
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_DONE,
    EVENT_WRITE_DONE,
    EVENT_ERROR,
    EVENT_RESET,
    EVENT_COUNT
} Event;

typedef struct {
    char buffer[256];
    size_t position;
    int data_ready;
} Context;

typedef State (*StateHandler)(Context*, Event);

State handle_idle(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    switch (evt) {
        case EVENT_START:
            ctx->position = 0;
            ctx->data_ready = 0;
            return STATE_READING;
        default:
            return STATE_IDLE;
    }
}

State handle_reading(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    switch (evt) {
        case EVENT_DATA_READY:
            if (ctx->position < sizeof(ctx->buffer)) {
                ctx->data_ready = 1;
                return STATE_PROCESSING;
            }
            return STATE_ERROR;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_READING;
    }
}

State handle_processing(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    switch (evt) {
        case EVENT_PROCESS_DONE:
            if (ctx->data_ready && ctx->position > 0) {
                return STATE_WRITING;
            }
            return STATE_ERROR;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_PROCESSING;
    }
}

State handle_writing(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    switch (evt) {
        case EVENT_WRITE_DONE:
            ctx->position = 0;
            ctx->data_ready = 0;
            return STATE_IDLE;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_WRITING;
    }
}

State handle_error(Context* ctx, Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    switch (evt) {
        case EVENT_RESET:
            ctx->position = 0;
            ctx->data_ready = 0;
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

State process_event(State current, Context* ctx, Event evt) {
    static const StateHandler handlers[STATE_COUNT] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_error
    };
    
    if (current >= STATE_COUNT || evt >= EVENT_COUNT || ctx == NULL) {
        return STATE_ERROR;
    }
    
    return handlers[current](ctx, evt);
}

void simulate_data_read(Context* ctx) {
    if (ctx == NULL || ctx->position >= sizeof(ctx->buffer)) return;
    const char* sample_data = "Sample data line ";
    size_t data_len = strlen(sample_data);
    if (ctx->position + data_len < sizeof(ctx->buffer)) {
        memcpy(ctx->buffer + ctx->position, sample_data, data_len);
        ctx->position += data_len;
    }
}

int main(void) {
    Context ctx;
    memset(&ctx, 0, sizeof(Context));
    
    State current_state = STATE_IDLE;
    int cycle = 0;
    const int max_cycles = 10;
    
    while (cycle < max_cycles) {
        Event event = EVENT_COUNT;
        
        switch (current_state) {
            case STATE_IDLE:
                event = EVENT_START;
                break;
            case STATE_READING:
                simulate_data_read(&ctx);
                if (ctx.position > 0 && ctx.position < sizeof(ctx.buffer)) {
                    event = EVENT_DATA_READY;
                } else {
                    event = EVENT_ERROR;
                }
                break;
            case STATE_PROCESSING:
                if (ctx.data_ready && ctx.position > 0) {
                    event = EVENT_PROCESS_DONE;
                } else {
                    event = EVENT_ERROR;
                }
                break;
            case STATE_WRITING:
                if (ctx.position > 0) {
                    printf("Writing: %.*s\n", (int)ctx.position, ctx.buffer);
                    event = EVENT_WRITE_DONE;
                } else {
                    event = EVENT_ERROR;
                }
                break;
            case STATE_ERROR:
                printf("Error state detected, resetting\n");
                event = EVENT_RESET;
                break;
            default:
                event = EVENT_ERROR;
                break;
        }
        
        State next_state = process_event(current_state, &ctx, event);
        if (next_state >= STATE_COUNT) {
            next_state = STATE_ERROR;
        }
        
        current_state = next_state;
        cycle++;
        
        if (current_state == STATE_IDLE && cycle >= max_cycles - 1) {
            break;
        }
    }
    
    return 0;
}