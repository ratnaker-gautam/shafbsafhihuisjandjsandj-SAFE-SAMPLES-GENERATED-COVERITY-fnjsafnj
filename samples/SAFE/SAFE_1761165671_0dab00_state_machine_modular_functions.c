//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 64
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_FINISHED,
    STATE_ERROR
} State;

typedef enum {
    EVENT_NONE,
    EVENT_CHAR,
    EVENT_SPACE,
    EVENT_EOF,
    EVENT_INVALID
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_pos;
    int word_count;
} Context;

static Event get_next_event(int ch) {
    if (ch == EOF) return EVENT_EOF;
    if (ch >= 0 && ch <= 127) {
        if (ch == ' ' || ch == '\t' || ch == '\n') return EVENT_SPACE;
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) return EVENT_CHAR;
    }
    return EVENT_INVALID;
}

static State handle_start_state(Context *ctx, Event ev) {
    if (ev == EVENT_CHAR) {
        ctx->buffer[0] = (char)getchar();
        ctx->buffer_pos = 1;
        ctx->word_count = 1;
        return STATE_READING;
    } else if (ev == EVENT_SPACE) {
        return STATE_START;
    } else if (ev == EVENT_EOF) {
        return STATE_FINISHED;
    }
    return STATE_ERROR;
}

static State handle_reading_state(Context *ctx, Event ev) {
    if (ev == EVENT_CHAR) {
        if (ctx->buffer_pos < MAX_INPUT_LEN - 1) {
            ctx->buffer[ctx->buffer_pos++] = (char)getchar();
            return STATE_READING;
        }
        return STATE_ERROR;
    } else if (ev == EVENT_SPACE) {
        ctx->buffer[ctx->buffer_pos] = '\0';
        return STATE_PROCESSING;
    } else if (ev == EVENT_EOF) {
        ctx->buffer[ctx->buffer_pos] = '\0';
        return STATE_FINISHED;
    }
    return STATE_ERROR;
}

static State handle_processing_state(Context *ctx, Event ev) {
    printf("Word %d: %s\n", ctx->word_count, ctx->buffer);
    ctx->buffer_pos = 0;
    
    if (ev == EVENT_CHAR) {
        ctx->buffer[0] = (char)getchar();
        ctx->buffer_pos = 1;
        ctx->word_count++;
        return STATE_READING;
    } else if (ev == EVENT_SPACE) {
        return STATE_PROCESSING;
    } else if (ev == EVENT_EOF) {
        return STATE_FINISHED;
    }
    return STATE_ERROR;
}

static State handle_finished_state(Context *ctx, Event ev) {
    if (ctx->buffer_pos > 0) {
        ctx->buffer[ctx->buffer_pos] = '\0';
        printf("Word %d: %s\n", ctx->word_count, ctx->buffer);
    }
    printf("Total words: %d\n", ctx->word_count);
    return STATE_FINISHED;
}

static State handle_error_state(Context *ctx, Event ev) {
    printf("Error: Invalid input or buffer overflow\n");
    return STATE_ERROR;
}

static void initialize_context(Context *ctx) {
    ctx->current_state = STATE_START;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_pos = 0;
    ctx->word_count = 0;
}

static void run_state_machine(Context *ctx) {
    State (*handlers[MAX_STATES])(Context *, Event) = {
        handle_start_state,
        handle_reading_state,
        handle_processing_state,
        handle_finished_state,
        handle_error_state
    };
    
    while (ctx->current_state != STATE_FINISHED && ctx->current_state != STATE_ERROR) {
        int ch = getchar();
        Event ev = get_next_event(ch);
        
        if (ctx->current_state < MAX_STATES && handlers[ctx->current_state] != NULL) {
            State new_state = handlers[ctx->current_state](ctx, ev);
            ctx->current_state = new_state;
        } else {
            ctx->current_state = STATE_ERROR;
        }
    }
    
    if (ctx->current_state == STATE_FINISHED) {
        handle_finished_state(ctx, EVENT_NONE);
    }
}

int main(void) {
    Context ctx;
    initialize_context(&ctx);
    printf("Enter text (Ctrl+D to end):\n");
    run_state_machine(&ctx);
    return ctx.current_state == STATE_FINISHED ? EXIT_SUCCESS : EXIT_FAILURE;
}