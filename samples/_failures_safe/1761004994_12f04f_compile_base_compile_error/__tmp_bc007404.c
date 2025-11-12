//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_COUNT
} state_t;

typedef enum {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_DONE,
    EVENT_WRITE_DONE,
    EVENT_ERROR,
    EVENT_RESET,
    EVENT_COUNT
} event_t;

typedef state_t (*state_handler_t)(event_t, void*);

typedef struct {
    char buffer[256];
    size_t length;
    int error_code;
} context_t;

state_t handle_idle(event_t event, void* ctx) {
    context_t* context = (context_t*)ctx;
    if (context == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("State: IDLE -> READING\n");
            return STATE_READING;
        case EVENT_RESET:
            printf("State: IDLE (reset)\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for IDLE state\n");
            return STATE_ERROR;
    }
}

state_t handle_reading(event_t event, void* ctx) {
    context_t* context = (context_t*)ctx;
    if (context == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA_READY:
            printf("Enter data (max 255 chars): ");
            if (fgets(context->buffer, sizeof(context->buffer), stdin) != NULL) {
                context->length = strlen(context->buffer);
                if (context->length > 0 && context->buffer[context->length - 1] == '\n') {
                    context->buffer[context->length - 1] = '\0';
                    context->length--;
                }
                printf("State: READING -> PROCESSING\n");
                return STATE_PROCESSING;
            } else {
                printf("Read error\n");
                return STATE_ERROR;
            }
        case EVENT_RESET:
            printf("State: READING -> IDLE\n");
            return STATE_IDLE;
        case EVENT_ERROR:
            printf("State: READING -> ERROR\n");
            return STATE_ERROR;
        default:
            printf("Invalid event for READING state\n");
            return STATE_ERROR;
    }
}

state_t handle_processing(event_t event, void* ctx) {
    context_t* context = (context_t*)ctx;
    if (context == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_PROCESS_DONE:
            if (context->length > 0) {
                for (size_t i = 0; i < context->length; i++) {
                    if (context->buffer[i] >= 'a' && context->buffer[i] <= 'z') {
                        context->buffer[i] = context->buffer[i] - 'a' + 'A';
                    }
                }
                printf("State: PROCESSING -> WRITING\n");
                return STATE_WRITING;
            } else {
                printf("No data to process\n");
                return STATE_ERROR;
            }
        case EVENT_RESET:
            printf("State: PROCESSING -> IDLE\n");
            return STATE_IDLE;
        case EVENT_ERROR:
            printf("State: PROCESSING -> ERROR\n");
            return STATE_ERROR;
        default:
            printf("Invalid event for PROCESSING state\n");
            return STATE_ERROR;
    }
}

state_t handle_writing(event_t event, void* ctx) {
    context_t* context = (context_t*)ctx;
    if (context == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_WRITE_DONE:
            printf("Processed data: %s\n", context->buffer);
            printf("State: WRITING -> IDLE\n");
            return STATE_IDLE;
        case EVENT_RESET:
            printf("State: WRITING -> IDLE\n");
            return STATE_IDLE;
        case EVENT_ERROR:
            printf("State: WRITING -> ERROR\n");
            return STATE_ERROR;
        default:
            printf("Invalid event for WRITING state\n");
            return STATE_ERROR;
    }
}

state_t handle_error(event_t event, void* ctx) {
    context_t* context = (context_t*)ctx;
    if (context == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            printf("State: ERROR -> IDLE\n");
            context->error_code = 0;
            memset(context->buffer, 0, sizeof(context->buffer));
            context->length = 0;
            return STATE_IDLE;
        default:
            printf("Invalid event for ERROR state\n");
            return STATE_ERROR;
    }
}

int main() {
    state_handler_t handlers[STATE_COUNT] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_error
    };
    
    context_t context;
    memset(&context, 0, sizeof(context));
    
    state_t current_state = STATE_IDLE;
    int running = 1;
    
    printf("State Machine Demo - Text Processor\n");
    printf("Commands: start, read, process, write, reset, quit\n");
    
    while (running) {
        char input[32];
        printf("\nEnter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        event_t event = EVENT_COUNT;
        
        if (strcmp(input, "start")