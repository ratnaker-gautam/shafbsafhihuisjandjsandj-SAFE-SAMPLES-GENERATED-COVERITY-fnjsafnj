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
    int processed_count;
} Context;

State handle_idle(Event event, void* ctx) {
    Context* context = (Context*)ctx;
    switch (event) {
        case EVENT_START:
            printf("Starting data processing\n");
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
            printf("Enter data to process (max 255 chars): ");
            if (fgets(context->buffer, sizeof(context->buffer), stdin) != NULL) {
                context->length = strlen(context->buffer);
                if (context->length > 0 && context->buffer[context->length - 1] == '\n') {
                    context->buffer[context->length - 1] = '\0';
                    context->length--;
                }
                if (context->length > 0) {
                    printf("Read %zu characters\n", context->length);
                    return STATE_PROCESSING;
                }
            }
            return STATE_ERROR;
        case EVENT_ERROR:
            printf("Error reading data\n");
            return STATE_ERROR;
        default:
            return STATE_READING;
    }
}

State handle_processing(Event event, void* ctx) {
    Context* context = (Context*)ctx;
    switch (event) {
        case EVENT_PROCESS_DONE:
            if (context->length > 0) {
                for (size_t i = 0; i < context->length; i++) {
                    if (context->buffer[i] >= 'a' && context->buffer[i] <= 'z') {
                        context->buffer[i] = context->buffer[i] - 'a' + 'A';
                    }
                }
                context->processed_count++;
                printf("Processed data: %s\n", context->buffer);
                return STATE_WRITING;
            }
            return STATE_ERROR;
        case EVENT_ERROR:
            printf("Error processing data\n");
            return STATE_ERROR;
        default:
            return STATE_PROCESSING;
    }
}

State handle_writing(Event event, void* ctx) {
    Context* context = (Context*)ctx;
    switch (event) {
        case EVENT_WRITE_DONE:
            printf("Data written successfully. Processed %d items total.\n", context->processed_count);
            printf("Continue? (y/n): ");
            char response[16];
            if (fgets(response, sizeof(response), stdin) != NULL) {
                if (response[0] == 'y' || response[0] == 'Y') {
                    return STATE_READING;
                }
            }
            return STATE_IDLE;
        case EVENT_ERROR:
            printf("Error writing data\n");
            return STATE_ERROR;
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
            context->buffer[0] = '\0';
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

int main() {
    State current_state = STATE_IDLE;
    Context context;
    context.length = 0;
    context.processed_count = 0;
    context.buffer[0] = '\0';
    
    StateHandler handlers[STATE_COUNT] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_error
    };
    
    printf("State Machine Demo - Data Processor\n");
    printf("Commands: start, read, process, write, error, reset, quit\n");
    
    char input[32];
    while (1) {
        printf("\nCurrent state: ");
        switch (current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READING: printf("READING"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_WRITING: printf("WRITING"); break;
            case STATE_ERROR: printf("ERROR"); break;
            default: printf("UNKNOWN"); break;
        }
        printf("\nEnter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
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
        } else if (strcmp(input, "