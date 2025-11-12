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
    int error_code;
};

int validate_input(const char* input, size_t max_len) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= max_len) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

enum event get_user_event(void) {
    char input[32];
    printf("Enter event (0=START, 1=DATA_READY, 2=PROCESS_COMPLETE, 3=WRITE_COMPLETE, 4=ERROR, 5=RESET): ");
    if (fgets(input, sizeof(input), stdin) == NULL) return EVENT_ERROR;
    char* endptr;
    long value = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') return EVENT_ERROR;
    if (value < 0 || value > 5) return EVENT_ERROR;
    return (enum event)value;
}

void state_machine_step(struct context* ctx, enum event evt) {
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
                    printf("Enter data to process: ");
                    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) != NULL) {
                        ctx->buffer_len = strlen(ctx->buffer);
                        if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len-1] == '\n') {
                            ctx->buffer[--ctx->buffer_len] = '\0';
                        }
                        if (validate_input(ctx->buffer, sizeof(ctx->buffer))) {
                            printf("Transition: READING -> PROCESSING\n");
                            ctx->current_state = STATE_PROCESSING;
                        } else {
                            printf("Invalid input data\n");
                            ctx->current_state = STATE_ERROR;
                            ctx->error_code = 2;
                        }
                    } else {
                        printf("Failed to read data\n");
                        ctx->current_state = STATE_ERROR;
                        ctx->error_code = 3;
                    }
                    break;
                case EVENT_RESET:
                    printf("Transition: READING -> IDLE\n");
                    ctx->current_state = STATE_IDLE;
                    break;
                default:
                    printf("Invalid event for READING state\n");
                    ctx->current_state = STATE_ERROR;
                    ctx->error_code = 4;
                    break;
            }
            break;
            
        case STATE_PROCESSING:
            switch (evt) {
                case EVENT_PROCESS_COMPLETE:
                    if (ctx->buffer_len > 0) {
                        for (size_t i = 0; i < ctx->buffer_len; i++) {
                            if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
                                ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
                            }
                        }
                        printf("Transition: PROCESSING -> WRITING\n");
                        ctx->current_state = STATE_WRITING;
                    } else {
                        printf("No data to process\n");
                        ctx->current_state = STATE_ERROR;
                        ctx->error_code = 5;
                    }
                    break;
                case EVENT_RESET:
                    printf("Transition: PROCESSING -> IDLE\n");
                    ctx->current_state = STATE_IDLE;
                    break;
                default:
                    printf("Invalid event for PROCESSING state\n");
                    ctx->current_state = STATE_ERROR;
                    ctx->error_code = 6;
                    break;
            }
            break;
            
        case STATE_WRITING:
            switch (evt) {
                case EVENT_WRITE_COMPLETE:
                    printf("Processed data: %s\n", ctx->buffer);
                    printf("Transition: WRITING -> DONE\n");
                    ctx->current_state = STATE_DONE;
                    break;
                case EVENT_RESET:
                    printf("Transition: WRITING -> IDLE\n");
                    ctx->current_state = STATE_IDLE;
                    break;
                default:
                    printf("Invalid event for WRITING state\n");
                    ctx->current_state = STATE_ERROR;
                    ctx->error_code = 7;
                    break;
            }
            break;
            
        case STATE_ERROR:
            switch (evt) {
                case EVENT_RESET:
                    printf("Transition: ERROR -> IDLE\n");
                    ctx->current_state = STATE