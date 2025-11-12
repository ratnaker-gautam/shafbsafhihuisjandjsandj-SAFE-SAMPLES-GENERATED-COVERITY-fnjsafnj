//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
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
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET,
    EVENT_COUNT
} Event;

typedef State (*StateHandler)(Event, void*);

State handle_idle(Event event, void* data);
State handle_reading(Event event, void* data);
State handle_processing(Event event, void* data);
State handle_writing(Event event, void* data);
State handle_error(Event event, void* data);

StateHandler state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_reading,
    handle_processing,
    handle_writing,
    handle_error
};

typedef struct {
    char buffer[256];
    size_t length;
    int processed_count;
} Context;

State handle_idle(Event event, void* data) {
    Context* ctx = (Context*)data;
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("Starting data processing\n");
            ctx->length = 0;
            ctx->processed_count = 0;
            return STATE_READING;
        case EVENT_RESET:
            printf("System reset\n");
            ctx->length = 0;
            ctx->processed_count = 0;
            return STATE_IDLE;
        default:
            return STATE_IDLE;
    }
}

State handle_reading(Event event, void* data) {
    Context* ctx = (Context*)data;
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA_READY:
            if (ctx->length < sizeof(ctx->buffer) - 1) {
                const char* sample_data = "Sample data chunk ";
                size_t copy_len = strlen(sample_data);
                if (ctx->length + copy_len < sizeof(ctx->buffer)) {
                    memcpy(ctx->buffer + ctx->length, sample_data, copy_len);
                    ctx->length += copy_len;
                    ctx->buffer[ctx->length] = '\0';
                    printf("Read data: %s\n", ctx->buffer);
                    return STATE_PROCESSING;
                }
            }
            return STATE_ERROR;
        case EVENT_ERROR:
            return STATE_ERROR;
        case EVENT_RESET:
            printf("Reset during reading\n");
            ctx->length = 0;
            ctx->processed_count = 0;
            return STATE_IDLE;
        default:
            return STATE_READING;
    }
}

State handle_processing(Event event, void* data) {
    Context* ctx = (Context*)data;
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_PROCESS_COMPLETE:
            if (ctx->length > 0) {
                for (size_t i = 0; i < ctx->length && i < sizeof(ctx->buffer); i++) {
                    if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
                        ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
                    }
                }
                ctx->processed_count++;
                printf("Processed data: %s\n", ctx->buffer);
                return STATE_WRITING;
            }
            return STATE_ERROR;
        case EVENT_ERROR:
            return STATE_ERROR;
        case EVENT_RESET:
            printf("Reset during processing\n");
            ctx->length = 0;
            ctx->processed_count = 0;
            return STATE_IDLE;
        default:
            return STATE_PROCESSING;
    }
}

State handle_writing(Event event, void* data) {
    Context* ctx = (Context*)data;
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_WRITE_COMPLETE:
            printf("Data written successfully. Processed %d chunks\n", ctx->processed_count);
            if (ctx->processed_count < 3) {
                ctx->length = 0;
                return STATE_READING;
            } else {
                printf("Processing complete\n");
                return STATE_IDLE;
            }
        case EVENT_ERROR:
            return STATE_ERROR;
        case EVENT_RESET:
            printf("Reset during writing\n");
            ctx->length = 0;
            ctx->processed_count = 0;
            return STATE_IDLE;
        default:
            return STATE_WRITING;
    }
}

State handle_error(Event event, void* data) {
    Context* ctx = (Context*)data;
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from error state\n");
            ctx->length = 0;
            ctx->processed_count = 0;
            return STATE_IDLE;
        default:
            printf("In error state - need reset\n");
            return STATE_ERROR;
    }
}

int main() {
    State current_state = STATE_IDLE;
    Context context;
    memset(&context, 0, sizeof(context));
    
    Event events[] = {
        EVENT_START,
        EVENT_DATA_READY,
        EVENT_PROCESS_COMPLETE,
        EVENT_WRITE_COMPLETE,
        EVENT_DATA_READY,
        EVENT_PROCESS_COMPLETE,
        EVENT_WRITE_COMPLETE,
        EVENT_DATA_READY,