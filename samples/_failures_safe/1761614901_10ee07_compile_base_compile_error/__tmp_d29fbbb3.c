//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

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
    EVENT_RESET,
    EVENT_DONE
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_len;
    int data_value;
    int processed_value;
    int write_count;
};

static int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > 255) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isdigit(input[i])) return 0;
    }
    return 1;
}

static enum event get_next_event(struct context *ctx) {
    if (ctx == NULL) return EVENT_ERROR;
    
    static int step = 0;
    switch (step) {
        case 0: step++; return EVENT_START;
        case 1: step++; return EVENT_DATA_READY;
        case 2: step++; return EVENT_PROCESS_COMPLETE;
        case 3: step++; return EVENT_WRITE_COMPLETE;
        case 4: step++; return EVENT_DONE;
        default: return EVENT_RESET;
    }
}

static void handle_idle(struct context *ctx, enum event evt) {
    switch (evt) {
        case EVENT_START:
            printf("State machine started\n");
            ctx->current_state = STATE_READING;
            break;
        case EVENT_RESET:
            printf("Reset in idle state\n");
            break;
        default:
            printf("Unexpected event in idle state\n");
            ctx->current_state = STATE_ERROR;
            break;
    }
}

static void handle_reading(struct context *ctx, enum event evt) {
    switch (evt) {
        case EVENT_DATA_READY:
            printf("Enter a number: ");
            if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) != NULL) {
                size_t len = strlen(ctx->buffer);
                if (len > 0 && ctx->buffer[len - 1] == '\n') {
                    ctx->buffer[len - 1] = '\0';
                }
                if (validate_input(ctx->buffer)) {
                    ctx->data_value = atoi(ctx->buffer);
                    if (ctx->data_value >= 0 && ctx->data_value <= 1000) {
                        ctx->current_state = STATE_PROCESSING;
                    } else {
                        printf("Number out of range (0-1000)\n");
                        ctx->current_state = STATE_ERROR;
                    }
                } else {
                    printf("Invalid input - digits only\n");
                    ctx->current_state = STATE_ERROR;
                }
            } else {
                printf("Failed to read input\n");
                ctx->current_state = STATE_ERROR;
            }
            break;
        case EVENT_RESET:
            printf("Reset in reading state\n");
            ctx->current_state = STATE_IDLE;
            break;
        default:
            printf("Unexpected event in reading state\n");
            ctx->current_state = STATE_ERROR;
            break;
    }
}

static void handle_processing(struct context *ctx, enum event evt) {
    switch (evt) {
        case EVENT_PROCESS_COMPLETE:
            if (ctx->data_value < 0) {
                ctx->current_state = STATE_ERROR;
                break;
            }
            ctx->processed_value = ctx->data_value * 2;
            if (ctx->processed_value < ctx->data_value) {
                printf("Integer overflow detected\n");
                ctx->current_state = STATE_ERROR;
                break;
            }
            printf("Processed value: %d\n", ctx->processed_value);
            ctx->current_state = STATE_WRITING;
            break;
        case EVENT_RESET:
            printf("Reset in processing state\n");
            ctx->current_state = STATE_IDLE;
            break;
        default:
            printf("Unexpected event in processing state\n");
            ctx->current_state = STATE_ERROR;
            break;
    }
}

static void handle_writing(struct context *ctx, enum event evt) {
    switch (evt) {
        case EVENT_WRITE_COMPLETE:
            printf("Writing result: %d\n", ctx->processed_value);
            ctx->write_count++;
            if (ctx->write_count < 3) {
                ctx->current_state = STATE_READING;
            } else {
                ctx->current_state = STATE_DONE;
            }
            break;
        case EVENT_RESET:
            printf("Reset in writing state\n");
            ctx->current_state = STATE_IDLE;
            ctx->write_count = 0;
            break;
        default:
            printf("Unexpected event in writing state\n");
            ctx->current_state = STATE_ERROR;
            break;
    }
}

static void handle_error(struct context *ctx, enum event evt) {
    switch (evt) {
        case EVENT_RESET:
            printf("Resetting from error state\n");
            ctx->current_state = STATE_IDLE;
            ctx->write_count = 0;
            memset(ctx->buffer, 0, sizeof(ctx->buffer));
            break;
        default:
            printf("Remaining in error state - reset required\n");
            break;
    }