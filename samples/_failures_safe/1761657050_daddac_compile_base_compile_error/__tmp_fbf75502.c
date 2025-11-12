//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum State {
    IDLE,
    READING,
    PROCESSING,
    WRITING,
    ERROR,
    DONE
};

struct Context {
    enum State current_state;
    char buffer[256];
    size_t buffer_len;
    int data_count;
    int max_data;
};

struct Transition {
    enum State from;
    enum State to;
    int (*condition)(struct Context*);
};

int condition_idle_to_reading(struct Context* ctx) {
    return ctx->data_count < ctx->max_data;
}

int condition_reading_to_processing(struct Context* ctx) {
    return ctx->buffer_len > 0 && ctx->buffer_len < sizeof(ctx->buffer);
}

int condition_processing_to_writing(struct Context* ctx) {
    return ctx->buffer_len > 0;
}

int condition_writing_to_idle(struct Context* ctx) {
    return ctx->data_count < ctx->max_data;
}

int condition_any_to_error(struct Context* ctx) {
    return ctx->buffer_len >= sizeof(ctx->buffer) || ctx->data_count >= ctx->max_data + 10;
}

int condition_any_to_done(struct Context* ctx) {
    return ctx->data_count >= ctx->max_data;
}

struct Transition transitions[] = {
    {IDLE, READING, condition_idle_to_reading},
    {READING, PROCESSING, condition_reading_to_processing},
    {PROCESSING, WRITING, condition_processing_to_writing},
    {WRITING, IDLE, condition_writing_to_idle},
    {IDLE, ERROR, condition_any_to_error},
    {READING, ERROR, condition_any_to_error},
    {PROCESSING, ERROR, condition_any_to_error},
    {WRITING, ERROR, condition_any_to_error},
    {IDLE, DONE, condition_any_to_done},
    {READING, DONE, condition_any_to_done},
    {PROCESSING, DONE, condition_any_to_done},
    {WRITING, DONE, condition_any_to_done}
};

const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

void state_idle(struct Context* ctx) {
    printf("State: IDLE (processed %d/%d items)\n", ctx->data_count, ctx->max_data);
}

void state_reading(struct Context* ctx) {
    printf("Enter data (max %zu chars): ", sizeof(ctx->buffer) - 1);
    
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) != NULL) {
        ctx->buffer_len = strlen(ctx->buffer);
        if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len - 1] == '\n') {
            ctx->buffer[ctx->buffer_len - 1] = '\0';
            ctx->buffer_len--;
        }
        printf("Read %zu characters\n", ctx->buffer_len);
    } else {
        ctx->buffer_len = 0;
        printf("Read failed\n");
    }
}

void state_processing(struct Context* ctx) {
    printf("Processing: '%.*s'\n", (int)ctx->buffer_len, ctx->buffer);
    
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    
    printf("Processed: '%.*s'\n", (int)ctx->buffer_len, ctx->buffer);
}

void state_writing(struct Context* ctx) {
    printf("Writing: '%.*s'\n", (int)ctx->buffer_len, ctx->buffer);
    ctx->data_count++;
    ctx->buffer_len = 0;
}

void state_error(struct Context* ctx) {
    printf("ERROR: Invalid state detected\n");
    printf("Buffer length: %zu, Data count: %d/%d\n", 
           ctx->buffer_len, ctx->data_count, ctx->max_data);
}

void state_done(struct Context* ctx) {
    printf("DONE: Processed %d items successfully\n", ctx->data_count);
}

typedef void (*state_handler)(struct Context*);

state_handler state_handlers[] = {
    state_idle,
    state_reading,
    state_processing,
    state_writing,
    state_error,
    state_done
};

enum State get_next_state(struct Context* ctx) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].from == ctx->current_state) {
            if (transitions[i].condition(ctx)) {
                return transitions[i].to;
            }
        }
    }
    return ctx->current_state;
}

void initialize_context(struct Context* ctx, int max_data) {
    if (max_data <= 0 || max_data > 1000) {
        max_data = 5;
    }
    ctx->current_state = IDLE;
    ctx->buffer_len = 0;
    ctx->data_count = 0;
    ctx->max_data = max_data;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int main(void) {
    struct Context ctx;
    int max_items;
    
    printf("Enter number of items to process (1-1000): ");
    if (scanf("%d", &max_items) != 1) {