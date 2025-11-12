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

typedef struct {
    char buffer[256];
    size_t length;
    int processed_count;
} Context;

State handle_idle(Event event, void* context_ptr) {
    Context* ctx = (Context*)context_ptr;
    if (event == EVENT_START) {
        printf("Starting data processing\n");
        return STATE_READING;
    }
    return STATE_IDLE;
}

State handle_reading(Event event, void* context_ptr) {
    Context* ctx = (Context*)context_ptr;
    if (event == EVENT_DATA_READY) {
        printf("Enter data to process: ");
        if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) != NULL) {
            ctx->length = strlen(ctx->buffer);
            if (ctx->length > 0 && ctx->buffer[ctx->length - 1] == '\n') {
                ctx->buffer[ctx->length - 1] = '\0';
                ctx->length--;
            }
            if (ctx->length > 0) {
                printf("Read %zu characters\n", ctx->length);
                return STATE_PROCESSING;
            }
        }
        return STATE_ERROR;
    }
    return STATE_READING;
}

State handle_processing(Event event, void* context_ptr) {
    Context* ctx = (Context*)context_ptr;
    if (event == EVENT_PROCESS_COMPLETE) {
        for (size_t i = 0; i < ctx->length; i++) {
            if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
                ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
            }
        }
        ctx->processed_count++;
        printf("Processed data: %s\n", ctx->buffer);
        return STATE_WRITING;
    }
    return STATE_PROCESSING;
}

State handle_writing(Event event, void* context_ptr) {
    Context* ctx = (Context*)context_ptr;
    if (event == EVENT_WRITE_COMPLETE) {
        printf("Data written successfully (processed %d times)\n", ctx->processed_count);
        return STATE_IDLE;
    }
    return STATE_WRITING;
}

State handle_error(Event event, void* context_ptr) {
    Context* ctx = (Context*)context_ptr;
    if (event == EVENT_RESET) {
        printf("Resetting from error state\n");
        ctx->length = 0;
        ctx->processed_count = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

StateHandler state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_reading,
    handle_processing,
    handle_writing,
    handle_error
};

Event get_user_event(State current_state) {
    printf("\nCurrent state: ");
    switch (current_state) {
        case STATE_IDLE: printf("IDLE"); break;
        case STATE_READING: printf("READING"); break;
        case STATE_PROCESSING: printf("PROCESSING"); break;
        case STATE_WRITING: printf("WRITING"); break;
        case STATE_ERROR: printf("ERROR"); break;
        default: break;
    }
    printf("\nAvailable events:\n");
    
    switch (current_state) {
        case STATE_IDLE:
            printf("1 - START\n");
            break;
        case STATE_READING:
            printf("2 - DATA_READY\n");
            break;
        case STATE_PROCESSING:
            printf("3 - PROCESS_COMPLETE\n");
            break;
        case STATE_WRITING:
            printf("4 - WRITE_COMPLETE\n");
            break;
        case STATE_ERROR:
            printf("5 - RESET\n");
            break;
        default:
            break;
    }
    printf("0 - EXIT\n");
    printf("Choose event: ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        return EVENT_ERROR;
    }
    while (getchar() != '\n');
    
    switch (current_state) {
        case STATE_IDLE:
            if (choice == 1) return EVENT_START;
            break;
        case STATE_READING:
            if (choice == 2) return EVENT_DATA_READY;
            break;
        case STATE_PROCESSING:
            if (choice == 3) return EVENT_PROCESS_COMPLETE;
            break;
        case STATE_WRITING:
            if (choice == 4) return EVENT_WRITE_COMPLETE;
            break;
        case STATE_ERROR:
            if (choice == 5) return EVENT_RESET;
            break;
        default:
            break;
    }
    
    if (choice == 0) return EVENT_COUNT;
    return EVENT_ERROR;
}

int main() {
    State current_state = STATE_IDLE;
    Context context;
    context.length = 0;
    context.processed_count = 0;
    memset(context