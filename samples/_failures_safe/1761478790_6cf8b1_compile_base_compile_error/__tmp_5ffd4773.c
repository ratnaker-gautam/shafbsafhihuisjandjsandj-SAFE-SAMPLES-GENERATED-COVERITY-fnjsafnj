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

int validate_input(const char *input, size_t max_len) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= max_len) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

void state_machine_step(struct context *ctx, enum event evt, const char *data) {
    if (ctx == NULL) return;
    
    switch (ctx->current_state) {
        case STATE_IDLE:
            switch (evt) {
                case EVENT_START:
                    printf("State: IDLE -> READING\n");
                    ctx->current_state = STATE_READING;
                    break;
                case EVENT_RESET:
                    printf("State: IDLE (already reset)\n");
                    break;
                default:
                    printf("Invalid event for IDLE state\n");
                    ctx->current_state = STATE_ERROR;
                    ctx->error_code = 1;
            }
            break;
            
        case STATE_READING:
            switch (evt) {
                case EVENT_DATA_READY:
                    if (data != NULL && validate_input(data, sizeof(ctx->buffer))) {
                        size_t len = strlen(data);
                        if (len < sizeof(ctx->buffer)) {
                            strncpy(ctx->buffer, data, sizeof(ctx->buffer) - 1);
                            ctx->buffer[sizeof(ctx->buffer) - 1] = '\0';
                            ctx->buffer_len = len;
                            printf("State: READING -> PROCESSING\n");
                            ctx->current_state = STATE_PROCESSING;
                        } else {
                            printf("Data too large\n");
                            ctx->current_state = STATE_ERROR;
                            ctx->error_code = 2;
                        }
                    } else {
                        printf("Invalid data\n");
                        ctx->current_state = STATE_ERROR;
                        ctx->error_code = 3;
                    }
                    break;
                case EVENT_RESET:
                    printf("State: READING -> IDLE\n");
                    ctx->current_state = STATE_IDLE;
                    memset(ctx->buffer, 0, sizeof(ctx->buffer));
                    ctx->buffer_len = 0;
                    break;
                default:
                    printf("Invalid event for READING state\n");
                    ctx->current_state = STATE_ERROR;
                    ctx->error_code = 4;
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
                        printf("State: PROCESSING -> WRITING\n");
                        ctx->current_state = STATE_WRITING;
                    } else {
                        printf("No data to process\n");
                        ctx->current_state = STATE_ERROR;
                        ctx->error_code = 5;
                    }
                    break;
                case EVENT_RESET:
                    printf("State: PROCESSING -> IDLE\n");
                    ctx->current_state = STATE_IDLE;
                    memset(ctx->buffer, 0, sizeof(ctx->buffer));
                    ctx->buffer_len = 0;
                    break;
                default:
                    printf("Invalid event for PROCESSING state\n");
                    ctx->current_state = STATE_ERROR;
                    ctx->error_code = 6;
            }
            break;
            
        case STATE_WRITING:
            switch (evt) {
                case EVENT_WRITE_COMPLETE:
                    printf("Processed data: %s\n", ctx->buffer);
                    printf("State: WRITING -> DONE\n");
                    ctx->current_state = STATE_DONE;
                    break;
                case EVENT_RESET:
                    printf("State: WRITING -> IDLE\n");
                    ctx->current_state = STATE_IDLE;
                    memset(ctx->buffer, 0, sizeof(ctx->buffer));
                    ctx->buffer_len = 0;
                    break;
                default:
                    printf("Invalid event for WRITING state\n");
                    ctx->current_state = STATE_ERROR;
                    ctx->error_code = 7;
            }
            break;
            
        case STATE_ERROR:
            switch (evt) {
                case EVENT_RESET:
                    printf("State: ERROR -> IDLE\n");
                    ctx->current_state = STATE_IDLE;
                    memset(ctx->buffer, 0, sizeof(ctx->buffer));
                    ctx->buffer_len = 0;
                    ctx->error_code = 0;
                    break;
                default:
                    printf("Can only reset from ERROR state\n");
            }
            break;
            
        case STATE_DONE:
            switch (evt) {
                case EVENT_RESET:
                    printf("State: DONE -> IDLE\n");
                    ctx->current_state = STATE_IDLE;
                    memset(ctx->