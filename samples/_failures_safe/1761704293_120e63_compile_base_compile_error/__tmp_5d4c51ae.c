//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    int processed_value;
};

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > 255) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') return 0;
    }
    return 1;
}

int parse_number(const char* input) {
    long value = 0;
    for (size_t i = 0; input[i] != '\0'; i++) {
        if (value > 214748364) return -1;
        value = value * 10 + (input[i] - '0');
        if (value > 2147483647) return -1;
    }
    return (int)value;
}

enum event get_user_event(void) {
    char input[16];
    printf("Enter event (0=START, 1=DATA_READY, 2=PROCESS_COMPLETE, 3=WRITE_COMPLETE, 4=ERROR, 5=RESET): ");
    if (fgets(input, sizeof(input), stdin) == NULL) return EVENT_ERROR;
    input[strcspn(input, "\n")] = '\0';
    if (!validate_input(input)) return EVENT_ERROR;
    int event_num = parse_number(input);
    if (event_num < 0 || event_num > 5) return EVENT_ERROR;
    return (enum event)event_num;
}

void state_machine_step(struct context* ctx, enum event evt) {
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
                    break;
            }
            break;
            
        case STATE_READING:
            switch (evt) {
                case EVENT_DATA_READY:
                    printf("Enter numeric data: ");
                    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
                        ctx->current_state = STATE_ERROR;
                        break;
                    }
                    ctx->buffer[strcspn(ctx->buffer, "\n")] = '\0';
                    if (!validate_input(ctx->buffer)) {
                        ctx->current_state = STATE_ERROR;
                        break;
                    }
                    ctx->data_value = parse_number(ctx->buffer);
                    if (ctx->data_value < 0) {
                        ctx->current_state = STATE_ERROR;
                        break;
                    }
                    printf("Transition: READING -> PROCESSING\n");
                    ctx->current_state = STATE_PROCESSING;
                    break;
                case EVENT_RESET:
                    printf("Transition: READING -> IDLE\n");
                    ctx->current_state = STATE_IDLE;
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
                    if (ctx->data_value > 1000000) {
                        printf("Value too large for processing\n");
                        ctx->current_state = STATE_ERROR;
                        break;
                    }
                    ctx->processed_value = ctx->data_value * 2;
                    printf("Processed value: %d\n", ctx->processed_value);
                    printf("Transition: PROCESSING -> WRITING\n");
                    ctx->current_state = STATE_WRITING;
                    break;
                case EVENT_RESET:
                    printf("Transition: PROCESSING -> IDLE\n");
                    ctx->current_state = STATE_IDLE;
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
                    printf("Writing result: %d\n", ctx->processed_value);
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
                    break;
            }
            break;
            
        case STATE_ERROR:
            switch (evt) {
                case EVENT_RESET:
                    printf("Transition: ERROR -> IDLE\n");
                    ctx->current_state = STATE_IDLE;
                    break;
                default:
                    printf("Must RESET from ERROR state\n");
                    break;
            }
            break;
            
        case STATE_DONE:
            switch (evt) {
                case EVENT