//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: state_machine
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
    if (ctx == NULL) return;
    ctx->current_state = IDLE;
    ctx->buffer_len = 0;
    ctx->processed_count = 0;
    ctx->error_code = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

enum State get_next_state(struct Context* ctx) {
    if (ctx == NULL) return ERROR;
    
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].from == ctx->current_state) {
            if (transitions[i].condition(ctx)) {
                return transitions[i].to;
            }
        }
    }
    return ctx->current_state;
}

void execute_state_action(struct Context* ctx) {
    if (ctx == NULL) return;
    
    switch (ctx->current_state) {
        case IDLE:
            break;
        case READING:
            if (ctx->buffer_len < sizeof(ctx->buffer) - 1) {
                int c = getchar();
                if (c != EOF && c != '\n') {
                    ctx->buffer[ctx->buffer_len++] = (char)c;
                }
            }
            break;
        case PROCESSING:
            if (ctx->buffer_len > 0) {
                for (size_t i = 0; i < ctx->buffer_len; i++) {
                    if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
                        ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
                    }
                }
                ctx->processed_count++;
            }
            break;
        case WRITING:
            if (ctx->buffer_len > 0) {
                for (size_t i = 0; i < ctx->buffer_len; i++) {
                    putchar(ctx->buffer[i]);
                }
                putchar('\n');
                ctx->buffer_len = 0;
            }
            break;
        case ERROR:
            ctx->error_code = 0;
            break;
        case DONE:
            break;
    }
}

int main(void) {
    struct Context ctx;
    initialize_context(&ctx);
    
    while (ctx.current_state != DONE) {
        execute_state_action(&ctx);
        enum State next_state = get_next_state(&ctx);
        
        if (next_state != ctx.current_state) {
            ctx.current_state = next_state;
        } else {
            break;
        }
        
        if (ctx.processed_count >= 10) {
            break;
        }
    }
    
    printf("Processed %d items\n", ctx.processed_count);
    return 0;
}