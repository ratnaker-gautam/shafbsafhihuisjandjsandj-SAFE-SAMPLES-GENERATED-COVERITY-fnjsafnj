//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdint.h>
#include <string.h>

enum State {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_DONE
};

enum Event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
};

struct Context {
    enum State current_state;
    char buffer[256];
    size_t data_length;
    uint32_t processed_count;
};

void initialize_context(struct Context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_IDLE;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->data_length = 0;
    ctx->processed_count = 0;
}

enum State handle_event(struct Context *ctx, enum Event evt) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (ctx->current_state) {
        case STATE_IDLE:
            switch (evt) {
                case EVENT_START:
                    printf("Starting data processing\n");
                    return STATE_READING;
                case EVENT_RESET:
                    printf("Already in idle state\n");
                    return STATE_IDLE;
                default:
                    printf("Invalid event for idle state\n");
                    return STATE_ERROR;
            }
            
        case STATE_READING:
            switch (evt) {
                case EVENT_DATA_READY:
                    if (ctx->data_length < sizeof(ctx->buffer)) {
                        strncpy(ctx->buffer, "Sample data", sizeof(ctx->buffer) - 1);
                        ctx->buffer[sizeof(ctx->buffer) - 1] = '\0';
                        ctx->data_length = strlen(ctx->buffer);
                        printf("Data read: %s\n", ctx->buffer);
                        return STATE_PROCESSING;
                    } else {
                        printf("Buffer overflow prevented\n");
                        return STATE_ERROR;
                    }
                case EVENT_RESET:
                    printf("Resetting from reading state\n");
                    return STATE_IDLE;
                case EVENT_ERROR:
                    printf("Error during reading\n");
                    return STATE_ERROR;
                default:
                    printf("Invalid event for reading state\n");
                    return STATE_ERROR;
            }
            
        case STATE_PROCESSING:
            switch (evt) {
                case EVENT_PROCESS_COMPLETE:
                    if (ctx->data_length > 0) {
                        for (size_t i = 0; i < ctx->data_length && i < sizeof(ctx->buffer); i++) {
                            if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
                                ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
                            }
                        }
                        ctx->processed_count++;
                        printf("Data processed: %s\n", ctx->buffer);
                        return STATE_WRITING;
                    } else {
                        printf("No data to process\n");
                        return STATE_ERROR;
                    }
                case EVENT_RESET:
                    printf("Resetting from processing state\n");
                    return STATE_IDLE;
                case EVENT_ERROR:
                    printf("Error during processing\n");
                    return STATE_ERROR;
                default:
                    printf("Invalid event for processing state\n");
                    return STATE_ERROR;
            }
            
        case STATE_WRITING:
            switch (evt) {
                case EVENT_WRITE_COMPLETE:
                    printf("Data written successfully. Processed count: %u\n", ctx->processed_count);
                    if (ctx->processed_count < 3) {
                        return STATE_READING;
                    } else {
                        return STATE_DONE;
                    }
                case EVENT_RESET:
                    printf("Resetting from writing state\n");
                    return STATE_IDLE;
                case EVENT_ERROR:
                    printf("Error during writing\n");
                    return STATE_ERROR;
                default:
                    printf("Invalid event for writing state\n");
                    return STATE_ERROR;
            }
            
        case STATE_ERROR:
            switch (evt) {
                case EVENT_RESET:
                    printf("Resetting from error state\n");
                    return STATE_IDLE;
                default:
                    printf("Must reset from error state\n");
                    return STATE_ERROR;
            }
            
        case STATE_DONE:
            switch (evt) {
                case EVENT_RESET:
                    printf("Resetting from done state\n");
                    return STATE_IDLE;
                default:
                    printf("Processing complete. Reset to start over.\n");
                    return STATE_DONE;
            }
            
        default:
            return STATE_ERROR;
    }
}

int main(void) {
    struct Context ctx;
    initialize_context(&ctx);
    
    const enum Event events[] = {
        EVENT_START,
        EVENT_DATA_READY,
        EVENT_PROCESS_COMPLETE,
        EVENT_WRITE_COMPLETE,
        EVENT_DATA_READY,
        EVENT_PROCESS_COMPLETE,
        EVENT_WRITE_COMPLETE,
        EVENT_DATA_READY,
        EVENT_PROCESS_COMPLETE,
        EVENT_WRITE_COMPLETE,
        EVENT_RESET
    };
    
    const size_t num_events = sizeof(events) / sizeof(events[0]);
    
    for (size_t i = 0; i < num_events; i++) {
        enum State new_state = handle_event(&ctx, events[i]);
        ctx.current_state = new_state;
        
        if (ctx.current_state == STATE_ERROR) {
            printf("Entering error state, resetting...\n");
            ctx.current_state = handle_event(&ctx, EVENT_RESET);
        }
        
        if (ctx.current_state == STATE_DONE) {
            printf("Processing