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
    int processed_count;
    int error_code;
};

struct Transition {
    enum State from;
    enum State to;
    int (*condition)(struct Context*);
};

int condition_idle_to_reading(struct Context* ctx) {
    return ctx->buffer_len == 0;
}

int condition_reading_to_processing(struct Context* ctx) {
    return ctx->buffer_len > 0 && ctx->buffer_len < sizeof(ctx->buffer);
}

int condition_processing_to_writing(struct Context* ctx) {
    return ctx->processed_count >= 0;
}

int condition_processing_to_error(struct Context* ctx) {
    return ctx->error_code != 0;
}

int condition_writing_to_idle(struct Context* ctx) {
    return ctx->buffer_len == 0;
}

int condition_error_to_idle(struct Context* ctx) {
    return ctx->error_code == 0;
}

int condition_idle_to_done(struct Context* ctx) {
    return ctx->processed_count >= 10;
}

struct Transition transitions[] = {
    {IDLE, READING, condition_idle_to_reading},
    {READING, PROCESSING, condition_reading_to_processing},
    {PROCESSING, WRITING, condition_processing_to_writing},
    {PROCESSING, ERROR, condition_processing_to_error},
    {WRITING, IDLE, condition_writing_to_idle},
    {ERROR, IDLE, condition_error_to_idle},
    {IDLE, DONE, condition_idle_to_done}
};

const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

void initialize_context(struct Context* ctx) {
    ctx->current_state = IDLE;
    ctx->buffer_len = 0;
    ctx->processed_count = 0;
    ctx->error_code = 0;
}

void handle_idle(struct Context* ctx) {
    printf("State: IDLE\n");
}

void handle_reading(struct Context* ctx) {
    printf("State: READING\n");
    printf("Enter data (max 255 chars): ");
    
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) != NULL) {
        ctx->buffer_len = strlen(ctx->buffer);
        if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len - 1] == '\n') {
            ctx->buffer[ctx->buffer_len - 1] = '\0';
            ctx->buffer_len--;
        }
    } else {
        ctx->error_code = 1;
    }
}

void handle_processing(struct Context* ctx) {
    printf("State: PROCESSING\n");
    
    if (ctx->buffer_len == 0) {
        ctx->error_code = 2;
        return;
    }
    
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    
    ctx->processed_count++;
}

void handle_writing(struct Context* ctx) {
    printf("State: WRITING\n");
    printf("Processed data: %s\n", ctx->buffer);
    ctx->buffer_len = 0;
}

void handle_error(struct Context* ctx) {
    printf("State: ERROR\n");
    printf("Error code: %d\n", ctx->error_code);
    ctx->error_code = 0;
}

void handle_done(struct Context* ctx) {
    printf("State: DONE\n");
    printf("Total processed items: %d\n", ctx->processed_count);
}

void (*state_handlers[])(struct Context*) = {
    handle_idle,
    handle_reading,
    handle_processing,
    handle_writing,
    handle_error,
    handle_done
};

enum State find_next_state(struct Context* ctx) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].from == ctx->current_state) {
            if (transitions[i].condition(ctx)) {
                return transitions[i].to;
            }
        }
    }
    return ctx->current_state;
}

void run_state_machine(struct Context* ctx) {
    while (ctx->current_state != DONE) {
        if (ctx->current_state >= 0 && ctx->current_state < 6) {
            state_handlers[ctx->current_state](ctx);
        }
        
        enum State next_state = find_next_state(ctx);
        if (next_state == ctx->current_state) {
            break;
        }
        ctx->current_state = next_state;
    }
    
    if (ctx->current_state == DONE) {
        handle_done(ctx);
    }
}

int main(void) {
    struct Context ctx;
    initialize_context(&ctx);
    run_state_machine(&ctx);
    return 0;
}