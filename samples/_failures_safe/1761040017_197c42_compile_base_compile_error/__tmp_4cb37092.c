//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum state {
    STATE_START,
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
    int max_iterations;
};

static void initialize_context(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_START;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_len = 0;
    ctx->processed_count = 0;
    ctx->max_iterations = 5;
}

static enum event get_user_input(void) {
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_ERROR;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        return EVENT_ERROR;
    }
    
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "data") == 0) return EVENT_DATA_READY;
    if (strcmp(input, "process") == 0) return EVENT_PROCESS_COMPLETE;
    if (strcmp(input, "write") == 0) return EVENT_WRITE_COMPLETE;
    if (strcmp(input, "error") == 0) return EVENT_ERROR;
    if (strcmp(input, "reset") == 0) return EVENT_RESET;
    
    return EVENT_ERROR;
}

static void handle_start(struct context *ctx) {
    if (ctx == NULL) return;
    printf("State machine started.\n");
    ctx->current_state = STATE_READING;
}

static void handle_reading(struct context *ctx) {
    if (ctx == NULL) return;
    printf("Reading data...\n");
    
    char data[64];
    printf("Enter data to process (max 63 chars): ");
    if (fgets(data, sizeof(data), stdin) == NULL) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    data[strcspn(data, "\n")] = '\0';
    size_t data_len = strlen(data);
    
    if (data_len == 0) {
        printf("No data entered.\n");
        return;
    }
    
    if (data_len >= sizeof(ctx->buffer)) {
        printf("Data too large.\n");
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    memcpy(ctx->buffer, data, data_len);
    ctx->buffer_len = data_len;
    ctx->current_state = STATE_PROCESSING;
}

static void handle_processing(struct context *ctx) {
    if (ctx == NULL) return;
    printf("Processing data...\n");
    
    if (ctx->buffer_len == 0) {
        printf("No data to process.\n");
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        ctx->buffer[i] = (char)toupper((unsigned char)ctx->buffer[i]);
    }
    
    ctx->processed_count++;
    printf("Processed: %s\n", ctx->buffer);
    ctx->current_state = STATE_WRITING;
}

static void handle_writing(struct context *ctx) {
    if (ctx == NULL) return;
    printf("Writing results...\n");
    
    if (ctx->buffer_len == 0) {
        printf("No data to write.\n");
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    printf("Result written: %s\n", ctx->buffer);
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_len = 0;
    
    if (ctx->processed_count >= ctx->max_iterations) {
        ctx->current_state = STATE_DONE;
    } else {
        ctx->current_state = STATE_READING;
    }
}

static void handle_error(struct context *ctx) {
    if (ctx == NULL) return;
    printf("Error state encountered.\n");
    printf("Enter 'reset' to continue or any other key to exit: ");
    
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "reset") == 0) {
        initialize_context(ctx);
        ctx->current_state = STATE_START;
    }
}

static void handle_done(struct context *ctx) {
    if (ctx == NULL) return;
    printf("Processing complete. Processed %d items.\n", ctx->processed_count);
}

static void process_event(struct context *ctx, enum event evt) {
    if (ctx == NULL) return;
    
    switch (ctx->current_state) {
        case STATE_START:
            if (evt == EVENT_START) {
                handle_start(ctx);
            }
            break;
            
        case STATE_READING:
            if (evt == EVENT_DATA_READY) {
                handle_reading