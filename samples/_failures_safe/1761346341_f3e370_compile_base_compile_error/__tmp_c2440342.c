//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10
#define MAX_TRANSITIONS 50

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_OUTPUT,
    STATE_ERROR,
    STATE_DONE
} State;

typedef struct {
    State current;
    char input_char;
    State next;
} Transition;

typedef struct {
    char buffer[MAX_INPUT_LEN];
    size_t pos;
    int processed_count;
    State current_state;
} Context;

static const Transition transitions[] = {
    {STATE_IDLE, 'S', STATE_READING},
    {STATE_READING, 'E', STATE_PROCESSING},
    {STATE_READING, '\0', STATE_PROCESSING},
    {STATE_PROCESSING, 0, STATE_OUTPUT},
    {STATE_OUTPUT, 0, STATE_DONE},
    {STATE_IDLE, 0, STATE_ERROR},
    {STATE_READING, 0, STATE_ERROR},
    {STATE_PROCESSING, 0, STATE_ERROR},
    {STATE_OUTPUT, 0, STATE_ERROR}
};
static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

static State get_next_state(State current, char input) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current == current) {
            if (transitions[i].input_char == input) {
                return transitions[i].next;
            }
            if (transitions[i].input_char == 0) {
                return transitions[i].next;
            }
        }
    }
    return STATE_ERROR;
}

static void process_buffer(Context *ctx) {
    if (ctx->pos == 0) return;
    
    for (size_t i = 0; i < ctx->pos; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    ctx->processed_count++;
}

static void state_idle(Context *ctx, char input) {
    if (input == 'S') {
        ctx->current_state = STATE_READING;
        ctx->pos = 0;
        ctx->processed_count = 0;
    } else {
        ctx->current_state = STATE_ERROR;
    }
}

static void state_reading(Context *ctx, char input) {
    if (input == 'E') {
        ctx->current_state = STATE_PROCESSING;
    } else if (ctx->pos < MAX_INPUT_LEN - 1) {
        if (isalpha(input) || input == ' ') {
            ctx->buffer[ctx->pos++] = input;
            ctx->buffer[ctx->pos] = '\0';
        }
    } else {
        ctx->current_state = STATE_ERROR;
    }
}

static void state_processing(Context *ctx, char input) {
    process_buffer(ctx);
    ctx->current_state = STATE_OUTPUT;
}

static void state_output(Context *ctx, char input) {
    if (ctx->pos > 0) {
        printf("Processed: %s (count: %d)\n", ctx->buffer, ctx->processed_count);
    }
    ctx->current_state = STATE_DONE;
}

static void state_error(Context *ctx, char input) {
    printf("Error in state machine\n");
    ctx->current_state = STATE_DONE;
}

static void state_done(Context *ctx, char input) {
}

static void (*state_handlers[])(Context*, char) = {
    state_idle,
    state_reading,
    state_processing,
    state_output,
    state_error,
    state_done
};

static void run_state_machine(const char *input) {
    Context ctx = {0};
    ctx.current_state = STATE_IDLE;
    
    for (size_t i = 0; input[i] != '\0' && ctx.current_state != STATE_DONE; i++) {
        char c = input[i];
        if (ctx.current_state < sizeof(state_handlers)/sizeof(state_handlers[0])) {
            state_handlers[ctx.current_state](&ctx, c);
        } else {
            ctx.current_state = STATE_ERROR;
        }
    }
    
    if (ctx.current_state == STATE_READING) {
        state_handlers[STATE_READING](&ctx, '\0');
    }
    
    while (ctx.current_state != STATE_DONE && ctx.current_state != STATE_ERROR) {
        if (ctx.current_state < sizeof(state_handlers)/sizeof(state_handlers[0])) {
            state_handlers[ctx.current_state](&ctx, 0);
        } else {
            ctx.current_state = STATE_ERROR;
        }
    }
    
    if (ctx.current_state == STATE_ERROR) {
        state_handlers[STATE_ERROR](&ctx, 0);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN * 2];
    
    printf("Enter input sequence (start with S, end with E): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';