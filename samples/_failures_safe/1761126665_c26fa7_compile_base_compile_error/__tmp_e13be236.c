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
    EVENT_RESET
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_len;
    int processed_count;
    int max_items;
};

static int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= 256) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (!isalnum((unsigned char)input[i]) && input[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

static enum event get_next_event(void) {
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_ERROR;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "data") == 0) return EVENT_DATA_READY;
    if (strcmp(input, "process") == 0) return EVENT_PROCESS_COMPLETE;
    if (strcmp(input, "write") == 0) return EVENT_WRITE_COMPLETE;
    if (strcmp(input, "error") == 0) return EVENT_ERROR;
    if (strcmp(input, "reset") == 0) return EVENT_RESET;
    
    return EVENT_ERROR;
}

static void state_machine_step(struct context *ctx, enum event evt) {
    if (ctx == NULL) {
        return;
    }
    
    switch (ctx->current_state) {
        case STATE_IDLE:
            switch (evt) {
                case EVENT_START:
                    printf("Starting processing pipeline\n");
                    ctx->current_state = STATE_READING;
                    ctx->processed_count = 0;
                    ctx->buffer_len = 0;
                    break;
                case EVENT_RESET:
                    printf("Already in idle state\n");
                    break;
                default:
                    printf("Invalid event for idle state\n");
                    ctx->current_state = STATE_ERROR;
                    break;
            }
            break;
            
        case STATE_READING:
            switch (evt) {
                case EVENT_DATA_READY:
                    printf("Enter data to process: ");
                    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) != NULL) {
                        ctx->buffer_len = strcspn(ctx->buffer, "\n");
                        ctx->buffer[ctx->buffer_len] = '\0';
                        if (validate_input(ctx->buffer, ctx->buffer_len)) {
                            printf("Data validated: %s\n", ctx->buffer);
                            ctx->current_state = STATE_PROCESSING;
                        } else {
                            printf("Invalid input data\n");
                            ctx->current_state = STATE_ERROR;
                        }
                    } else {
                        printf("Failed to read input\n");
                        ctx->current_state = STATE_ERROR;
                    }
                    break;
                case EVENT_RESET:
                    printf("Resetting from reading state\n");
                    ctx->current_state = STATE_IDLE;
                    break;
                default:
                    printf("Invalid event for reading state\n");
                    ctx->current_state = STATE_ERROR;
                    break;
            }
            break;
            
        case STATE_PROCESSING:
            switch (evt) {
                case EVENT_PROCESS_COMPLETE:
                    if (ctx->buffer_len > 0) {
                        for (size_t i = 0; i < ctx->buffer_len; i++) {
                            ctx->buffer[i] = (char)toupper((unsigned char)ctx->buffer[i]);
                        }
                        printf("Processed data: %s\n", ctx->buffer);
                        ctx->processed_count++;
                        ctx->current_state = STATE_WRITING;
                    } else {
                        printf("No data to process\n");
                        ctx->current_state = STATE_ERROR;
                    }
                    break;
                case EVENT_RESET:
                    printf("Resetting from processing state\n");
                    ctx->current_state = STATE_IDLE;
                    break;
                default:
                    printf("Invalid event for processing state\n");
                    ctx->current_state = STATE_ERROR;
                    break;
            }
            break;
            
        case STATE_WRITING:
            switch (evt) {
                case EVENT_WRITE_COMPLETE:
                    printf("Data written successfully. Processed %d items\n", ctx->processed_count);
                    if (ctx->processed_count < ctx->max_items) {
                        ctx->current_state = STATE_READING;
                    } else {
                        ctx->current_state = STATE_DONE;
                    }
                    break;
                case EVENT_RESET:
                    printf("Resetting from writing state\n");
                    ctx->current_state = STATE_IDLE;
                    break;
                default:
                    printf("Invalid event for writing state\n");
                    ctx->current_state = STATE_ERROR;
                    break;
            }
            break;
            
        case STATE_ERROR:
            switch (evt) {
                case EVENT_RESET:
                    printf("Resetting from error state\n");
                    ctx->current_state = STATE_IDLE;
                    break;
                default:
                    printf("Must reset from error state\n");
                    break;