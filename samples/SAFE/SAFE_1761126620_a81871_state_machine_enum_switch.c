//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum state {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_DONE
};

enum event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_len;
    int processed_count;
    int max_cycles;
};

static int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= 256) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i])) {
            return 0;
        }
    }
    return 1;
}

static enum event get_next_event(struct context *ctx) {
    if (ctx == NULL) {
        return EVENT_ERROR;
    }
    
    static int event_count = 0;
    event_count++;
    
    switch (ctx->current_state) {
        case STATE_IDLE:
            return EVENT_START;
        case STATE_READING:
            return EVENT_DATA_READY;
        case STATE_PROCESSING:
            return EVENT_PROCESS_COMPLETE;
        case STATE_WRITING:
            return EVENT_WRITE_COMPLETE;
        case STATE_ERROR:
            return EVENT_RESET;
        case STATE_DONE:
            return EVENT_ERROR;
    }
    return EVENT_ERROR;
}

static void process_state_machine(struct context *ctx, enum event evt) {
    if (ctx == NULL) {
        return;
    }
    
    switch (ctx->current_state) {
        case STATE_IDLE:
            if (evt == EVENT_START) {
                printf("Starting processing cycle %d\n", ctx->processed_count + 1);
                ctx->current_state = STATE_READING;
            }
            break;
            
        case STATE_READING:
            if (evt == EVENT_DATA_READY) {
                const char *sample_data = "Sample input data for processing";
                size_t data_len = strlen(sample_data);
                if (data_len < sizeof(ctx->buffer)) {
                    memcpy(ctx->buffer, sample_data, data_len);
                    ctx->buffer_len = data_len;
                    ctx->buffer[ctx->buffer_len] = '\0';
                    ctx->current_state = STATE_PROCESSING;
                } else {
                    ctx->current_state = STATE_ERROR;
                }
            }
            break;
            
        case STATE_PROCESSING:
            if (evt == EVENT_PROCESS_COMPLETE) {
                if (validate_input(ctx->buffer, ctx->buffer_len)) {
                    for (size_t i = 0; i < ctx->buffer_len; i++) {
                        if (islower(ctx->buffer[i])) {
                            ctx->buffer[i] = toupper(ctx->buffer[i]);
                        }
                    }
                    ctx->current_state = STATE_WRITING;
                } else {
                    ctx->current_state = STATE_ERROR;
                }
            }
            break;
            
        case STATE_WRITING:
            if (evt == EVENT_WRITE_COMPLETE) {
                printf("Processed: %s\n", ctx->buffer);
                ctx->processed_count++;
                if (ctx->processed_count >= ctx->max_cycles) {
                    ctx->current_state = STATE_DONE;
                } else {
                    ctx->current_state = STATE_IDLE;
                }
            }
            break;
            
        case STATE_ERROR:
            if (evt == EVENT_RESET) {
                printf("Error occurred, resetting state machine\n");
                ctx->current_state = STATE_IDLE;
                ctx->buffer_len = 0;
                ctx->buffer[0] = '\0';
            }
            break;
            
        case STATE_DONE:
            break;
    }
}

int main(void) {
    struct context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.buffer_len = 0;
    ctx.buffer[0] = '\0';
    ctx.processed_count = 0;
    ctx.max_cycles = 3;
    
    printf("State Machine Demo - Processing %d cycles\n", ctx.max_cycles);
    
    int max_iterations = 50;
    int iteration = 0;
    
    while (ctx.current_state != STATE_DONE && iteration < max_iterations) {
        enum event next_event = get_next_event(&ctx);
        process_state_machine(&ctx, next_event);
        iteration++;
    }
    
    if (ctx.current_state == STATE_DONE) {
        printf("Successfully completed %d processing cycles\n", ctx.processed_count);
    } else {
        printf("State machine did not complete within expected iterations\n");
        return 1;
    }
    
    return 0;
}