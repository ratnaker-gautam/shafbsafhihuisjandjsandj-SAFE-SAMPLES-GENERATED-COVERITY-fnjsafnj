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
    int data_processed;
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

enum event get_user_event(void) {
    char input[32];
    printf("Enter event (0=START, 1=DATA_READY, 2=PROCESS_COMPLETE, 3=WRITE_COMPLETE, 4=ERROR, 5=RESET): ");
    if (fgets(input, sizeof(input), stdin) == NULL) return EVENT_ERROR;
    input[strcspn(input, "\n")] = '\0';
    if (!validate_input(input, sizeof(input))) return EVENT_ERROR;
    long value = strtol(input, NULL, 10);
    if (value < 0 || value > 5) return EVENT_ERROR;
    return (enum event)value;
}

void state_machine_step(struct context *ctx, enum event evt) {
    if (ctx == NULL) return;
    
    switch (ctx->current_state) {
        case STATE_IDLE:
            switch (evt) {
                case EVENT_START:
                    printf("Transition: IDLE -> READING\n");
                    ctx->current_state = STATE_READING;
                    strncpy(ctx->buffer, "Sample data", sizeof(ctx->buffer) - 1);
                    ctx->buffer[sizeof(ctx->buffer) - 1] = '\0';
                    ctx->buffer_len = strlen(ctx->buffer);
                    break;
                case EVENT_RESET:
                    printf("Already in IDLE state\n");
                    break;
                default:
                    printf("Invalid event for IDLE state\n");
                    ctx->current_state = STATE_ERROR;
                    break;
            }
            break;
            
        case STATE_READING:
            switch (evt) {
                case EVENT_DATA_READY:
                    if (ctx->buffer_len > 0 && ctx->buffer_len < sizeof(ctx->buffer)) {
                        printf("Transition: READING -> PROCESSING\n");
                        ctx->current_state = STATE_PROCESSING;
                    } else {
                        printf("Invalid buffer state\n");
                        ctx->current_state = STATE_ERROR;
                    }
                    break;
                case EVENT_ERROR:
                    printf("Transition: READING -> ERROR\n");
                    ctx->current_state = STATE_ERROR;
                    ctx->error_code = 1;
                    break;
                case EVENT_RESET:
                    printf("Transition: READING -> IDLE\n");
                    ctx->current_state = STATE_IDLE;
                    memset(ctx->buffer, 0, sizeof(ctx->buffer));
                    ctx->buffer_len = 0;
                    break;
                default:
                    printf("Invalid event for READING state\n");
                    ctx->current_state = STATE_ERROR;
                    break;
            }
            break;
            
        case STATE_PROCESSING:
            switch (evt) {
                case EVENT_PROCESS_COMPLETE:
                    if (ctx->buffer_len > 0) {
                        printf("Transition: PROCESSING -> WRITING\n");
                        ctx->current_state = STATE_WRITING;
                        ctx->data_processed = 1;
                    } else {
                        printf("No data to process\n");
                        ctx->current_state = STATE_ERROR;
                    }
                    break;
                case EVENT_ERROR:
                    printf("Transition: PROCESSING -> ERROR\n");
                    ctx->current_state = STATE_ERROR;
                    ctx->error_code = 2;
                    break;
                case EVENT_RESET:
                    printf("Transition: PROCESSING -> IDLE\n");
                    ctx->current_state = STATE_IDLE;
                    memset(ctx->buffer, 0, sizeof(ctx->buffer));
                    ctx->buffer_len = 0;
                    ctx->data_processed = 0;
                    break;
                default:
                    printf("Invalid event for PROCESSING state\n");
                    ctx->current_state = STATE_ERROR;
                    break;
            }
            break;
            
        case STATE_WRITING:
            switch (evt) {
                case EVENT_WRITE_COMPLETE:
                    printf("Transition: WRITING -> DONE\n");
                    ctx->current_state = STATE_DONE;
                    break;
                case EVENT_ERROR:
                    printf("Transition: WRITING -> ERROR\n");
                    ctx->current_state = STATE_ERROR;
                    ctx->error_code = 3;
                    break;
                case EVENT_RESET:
                    printf("Transition: WRITING -> IDLE\n");
                    ctx->current_state = STATE_IDLE;
                    memset(ctx->buffer, 0, sizeof(ctx->buffer));
                    ctx->buffer_len = 0;
                    ctx->data_processed = 0;
                    break;
                default:
                    printf("Invalid event for WRITING state\n");
                    ctx->current_state = STATE_ERROR;
                    break;
            }
            break;
            
        case STATE_ERROR:
            switch (evt) {
                case