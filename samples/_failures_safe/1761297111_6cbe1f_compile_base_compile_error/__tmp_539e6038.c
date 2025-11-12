//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
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

static int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > 255) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') return 0;
    }
    return 1;
}

static enum event get_user_event(void) {
    char input[16];
    printf("Enter event (0=START, 1=DATA_READY, 2=PROCESS_COMPLETE, 3=WRITE_COMPLETE, 4=ERROR, 5=RESET): ");
    if (fgets(input, sizeof(input), stdin) == NULL) return EVENT_ERROR;
    char *endptr;
    long value = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') return EVENT_ERROR;
    if (value < 0 || value > 5) return EVENT_ERROR;
    return (enum event)value;
}

static void state_machine_step(struct context *ctx, enum event evt) {
    if (ctx == NULL) return;
    
    switch (ctx->current_state) {
        case STATE_IDLE:
            switch (evt) {
                case EVENT_START:
                    printf("Transition: IDLE -> READING\n");
                    ctx->current_state = STATE_READING;
                    break;
                case EVENT_RESET:
                    printf("Already in IDLE state\n");
                    break;
                default:
                    printf("Invalid event for IDLE state\n");
                    ctx->current_state = STATE_ERROR;
                    ctx->error_code = 1;
                    break;
            }
            break;
            
        case STATE_READING:
            switch (evt) {
                case EVENT_DATA_READY:
                    printf("Enter numeric data: ");
                    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
                        ctx->current_state = STATE_ERROR;
                        ctx->error_code = 2;
                        break;
                    }
                    ctx->buffer_len = strlen(ctx->buffer);
                    if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len - 1] == '\n') {
                        ctx->buffer[--ctx->buffer_len] = '\0';
                    }
                    if (!validate_input(ctx->buffer)) {
                        ctx->current_state = STATE_ERROR;
                        ctx->error_code = 3;
                        break;
                    }
                    ctx->data_value = atoi(ctx->buffer);
                    printf("Transition: READING -> PROCESSING\n");
                    ctx->current_state = STATE_PROCESSING;
                    break;
                case EVENT_RESET:
                    printf("Transition: READING -> IDLE\n");
                    ctx->current_state = STATE_IDLE;
                    break;
                case EVENT_ERROR:
                    printf("Transition: READING -> ERROR\n");
                    ctx->current_state = STATE_ERROR;
                    ctx->error_code = 4;
                    break;
                default:
                    printf("Invalid event for READING state\n");
                    ctx->current_state = STATE_ERROR;
                    ctx->error_code = 5;
                    break;
            }
            break;
            
        case STATE_PROCESSING:
            switch (evt) {
                case EVENT_PROCESS_COMPLETE:
                    if (ctx->data_value < 0) {
                        ctx->current_state = STATE_ERROR;
                        ctx->error_code = 6;
                        break;
                    }
                    ctx->data_value *= 2;
                    printf("Processed value: %d\n", ctx->data_value);
                    printf("Transition: PROCESSING -> WRITING\n");
                    ctx->current_state = STATE_WRITING;
                    break;
                case EVENT_RESET:
                    printf("Transition: PROCESSING -> IDLE\n");
                    ctx->current_state = STATE_IDLE;
                    break;
                case EVENT_ERROR:
                    printf("Transition: PROCESSING -> ERROR\n");
                    ctx->current_state = STATE_ERROR;
                    ctx->error_code = 7;
                    break;
                default:
                    printf("Invalid event for PROCESSING state\n");
                    ctx->current_state = STATE_ERROR;
                    ctx->error_code = 8;
                    break;
            }
            break;
            
        case STATE_WRITING:
            switch (evt) {
                case EVENT_WRITE_COMPLETE:
                    printf("Writing result: %d\n", ctx->data_value);
                    printf("Transition: WRITING -> DONE\n");
                    ctx->current_state = STATE_DONE;
                    break;
                case EVENT_RESET:
                    printf("Transition: WRITING -> IDLE\n");
                    ctx->current_state = STATE_IDLE;
                    break;
                case EVENT_ERROR:
                    printf("Transition: WRITING -> ERROR\n");
                    ctx->current_state = STATE_ERROR;
                    ctx->error_code = 9;
                    break;
                default:
                    printf("Invalid event for WRITING state\n");
                    ctx->current_state = STATE_ERROR;
                    ctx->error