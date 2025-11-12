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
    EVENT_PROCESS_DONE,
    EVENT_WRITE_DONE,
    EVENT_ERROR,
    EVENT_RESET,
    EVENT_COUNT
} Event;

typedef State (*StateHandler)(Event, void*);

typedef struct {
    char buffer[256];
    size_t length;
    int data_value;
} Context;

State handle_idle(Event event, void* ctx) {
    Context* context = (Context*)ctx;
    switch (event) {
        case EVENT_START:
            printf("Starting data processing\n");
            context->length = 0;
            context->data_value = 0;
            return STATE_READING;
        case EVENT_ERROR:
            printf("Error in idle state\n");
            return STATE_ERROR;
        default:
            return STATE_IDLE;
    }
}

State handle_reading(Event event, void* ctx) {
    Context* context = (Context*)ctx;
    switch (event) {
        case EVENT_DATA_READY:
            if (context->length < sizeof(context->buffer) - 1) {
                printf("Enter data (max 255 chars): ");
                if (fgets(context->buffer, sizeof(context->buffer), stdin)) {
                    context->length = strlen(context->buffer);
                    if (context->length > 0 && context->buffer[context->length - 1] == '\n') {
                        context->buffer[context->length - 1] = '\0';
                        context->length--;
                    }
                    printf("Read %zu characters\n", context->length);
                    return STATE_PROCESSING;
                } else {
                    return STATE_ERROR;
                }
            } else {
                return STATE_ERROR;
            }
        case EVENT_ERROR:
            printf("Error during reading\n");
            return STATE_ERROR;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_READING;
    }
}

State handle_processing(Event event, void* ctx) {
    Context* context = (Context*)ctx;
    switch (event) {
        case EVENT_PROCESS_DONE:
            if (context->length > 0) {
                context->data_value = 0;
                for (size_t i = 0; i < context->length; i++) {
                    if (context->data_value > 1000000 - context->buffer[i]) {
                        return STATE_ERROR;
                    }
                    context->data_value += context->buffer[i];
                }
                printf("Processed data, sum: %d\n", context->data_value);
                return STATE_WRITING;
            } else {
                return STATE_ERROR;
            }
        case EVENT_ERROR:
            printf("Error during processing\n");
            return STATE_ERROR;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_PROCESSING;
    }
}

State handle_writing(Event event, void* ctx) {
    Context* context = (Context*)ctx;
    switch (event) {
        case EVENT_WRITE_DONE:
            printf("Writing result: %d\n", context->data_value);
            return STATE_IDLE;
        case EVENT_ERROR:
            printf("Error during writing\n");
            return STATE_ERROR;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_WRITING;
    }
}

State handle_error(Event event, void* ctx) {
    Context* context = (Context*)ctx;
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from error state\n");
            context->length = 0;
            context->data_value = 0;
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

int main() {
    State current_state = STATE_IDLE;
    Context context;
    context.length = 0;
    context.data_value = 0;
    
    StateHandler handlers[STATE_COUNT] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_error
    };
    
    printf("State Machine Demo\n");
    printf("Commands: start, read, process, write, reset, quit\n");
    
    char input[32];
    while (1) {
        printf("Current state: ");
        switch (current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READING: printf("READING"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_WRITING: printf("WRITING"); break;
            case STATE_ERROR: printf("ERROR"); break;
            default: printf("UNKNOWN"); break;
        }
        printf("\n> ");
        
        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        Event event;
        if (strcmp(input, "start") == 0) {
            event = EVENT_START;
        } else if (strcmp(input, "read") == 0) {
            event = EVENT_DATA_READY;
        } else if (strcmp(input, "process") == 0) {
            event = EVENT_PROCESS_DONE;
        } else if (strcmp(input, "write") == 0) {
            event = EVENT_WRITE_DONE;
        } else if (strcmp(input,