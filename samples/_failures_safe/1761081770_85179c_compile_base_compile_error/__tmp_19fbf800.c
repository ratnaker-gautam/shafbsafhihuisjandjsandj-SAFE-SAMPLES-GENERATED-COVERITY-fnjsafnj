//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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
    int data_value;
    int error_code;
};

int validate_input(const char* input, size_t max_len) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > max_len) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') return 0;
    }
    return 1;
}

enum event get_next_event(struct context* ctx) {
    char input[32];
    printf("Current state: %d\n", (int)ctx->current_state);
    printf("Enter event (0=START, 1=DATA_READY, 2=PROCESS_COMPLETE, 3=WRITE_COMPLETE, 4=ERROR, 5=RESET): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_ERROR;
    }
    
    if (!validate_input(input, 1)) {
        return EVENT_ERROR;
    }
    
    int event_num = atoi(input);
    if (event_num < 0 || event_num > 5) {
        return EVENT_ERROR;
    }
    
    return (enum event)event_num;
}

void handle_idle(struct context* ctx, enum event evt) {
    switch (evt) {
        case EVENT_START:
            printf("Starting data processing...\n");
            ctx->current_state = STATE_READING;
            break;
        case EVENT_RESET:
            printf("System reset.\n");
            break;
        default:
            printf("Invalid event for IDLE state.\n");
            ctx->current_state = STATE_ERROR;
            ctx->error_code = 1;
            break;
    }
}

void handle_reading(struct context* ctx, enum event evt) {
    switch (evt) {
        case EVENT_DATA_READY:
            printf("Enter numeric data: ");
            if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
                ctx->current_state = STATE_ERROR;
                ctx->error_code = 2;
                return;
            }
            
            ctx->buffer_len = strlen(ctx->buffer);
            if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len - 1] == '\n') {
                ctx->buffer[--ctx->buffer_len] = '\0';
            }
            
            if (!validate_input(ctx->buffer, 10)) {
                ctx->current_state = STATE_ERROR;
                ctx->error_code = 3;
                return;
            }
            
            ctx->data_value = atoi(ctx->buffer);
            printf("Data read: %d\n", ctx->data_value);
            ctx->current_state = STATE_PROCESSING;
            break;
        case EVENT_RESET:
            printf("Reset during reading.\n");
            ctx->current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for READING state.\n");
            ctx->current_state = STATE_ERROR;
            ctx->error_code = 4;
            break;
    }
}

void handle_processing(struct context* ctx, enum event evt) {
    switch (evt) {
        case EVENT_PROCESS_COMPLETE:
            if (ctx->data_value < 0) {
                printf("Processing negative value: %d\n", ctx->data_value);
            } else {
                ctx->data_value *= 2;
                printf("Processed value: %d\n", ctx->data_value);
            }
            ctx->current_state = STATE_WRITING;
            break;
        case EVENT_RESET:
            printf("Reset during processing.\n");
            ctx->current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for PROCESSING state.\n");
            ctx->current_state = STATE_ERROR;
            ctx->error_code = 5;
            break;
    }
}

void handle_writing(struct context* ctx, enum event evt) {
    switch (evt) {
        case EVENT_WRITE_COMPLETE:
            printf("Writing result: %d\n", ctx->data_value);
            ctx->current_state = STATE_DONE;
            break;
        case EVENT_RESET:
            printf("Reset during writing.\n");
            ctx->current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for WRITING state.\n");
            ctx->current_state = STATE_ERROR;
            ctx->error_code = 6;
            break;
    }
}

void handle_error(struct context* ctx, enum event evt) {
    switch (evt) {
        case EVENT_RESET:
            printf("Resetting from error state (code: %d)\n", ctx->error_code);
            ctx->current_state = STATE_IDLE;
            ctx->error_code = 0;
            memset(ctx->buffer, 0, sizeof(ctx->buffer));
            ctx->buffer_len = 0;
            ctx->data_value = 0;
            break;
        default:
            printf("