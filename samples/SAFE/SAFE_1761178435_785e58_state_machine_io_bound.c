//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 5

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_OUTPUT,
    STATE_END
} State;

typedef enum {
    EVENT_NEWLINE,
    EVENT_ALPHA,
    EVENT_DIGIT,
    EVENT_OTHER,
    EVENT_EOF
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_pos;
    int word_count;
    int char_count;
} Context;

Event get_next_event(FILE *input) {
    int c = fgetc(input);
    if (c == EOF) return EVENT_EOF;
    if (c == '\n') return EVENT_NEWLINE;
    if (isalpha(c)) return EVENT_ALPHA;
    if (isdigit(c)) return EVENT_DIGIT;
    return EVENT_OTHER;
}

State handle_state_start(Event event, Context *ctx) {
    switch (event) {
        case EVENT_ALPHA:
            ctx->buffer[0] = (char)event;
            ctx->buffer_pos = 1;
            ctx->char_count = 1;
            return STATE_READING;
        case EVENT_DIGIT:
        case EVENT_OTHER:
            return STATE_START;
        case EVENT_NEWLINE:
            return STATE_START;
        case EVENT_EOF:
            return STATE_END;
    }
    return STATE_START;
}

State handle_state_reading(Event event, Context *ctx) {
    switch (event) {
        case EVENT_ALPHA:
            if (ctx->buffer_pos < MAX_INPUT_LEN - 1) {
                ctx->buffer[ctx->buffer_pos++] = (char)event;
                ctx->char_count++;
            }
            return STATE_READING;
        case EVENT_DIGIT:
        case EVENT_OTHER:
        case EVENT_NEWLINE:
            ctx->buffer[ctx->buffer_pos] = '\0';
            return STATE_PROCESSING;
        case EVENT_EOF:
            ctx->buffer[ctx->buffer_pos] = '\0';
            return STATE_PROCESSING;
    }
    return STATE_READING;
}

State handle_state_processing(Event event, Context *ctx) {
    if (ctx->buffer_pos > 0) {
        ctx->word_count++;
        printf("Word %d: %s (%d chars)\n", ctx->word_count, ctx->buffer, ctx->char_count);
    }
    
    ctx->buffer_pos = 0;
    ctx->char_count = 0;
    
    switch (event) {
        case EVENT_ALPHA:
            ctx->buffer[0] = (char)event;
            ctx->buffer_pos = 1;
            ctx->char_count = 1;
            return STATE_READING;
        case EVENT_DIGIT:
        case EVENT_OTHER:
            return STATE_START;
        case EVENT_NEWLINE:
            return STATE_START;
        case EVENT_EOF:
            return STATE_END;
    }
    return STATE_START;
}

State handle_state_output(Context *ctx) {
    printf("Total words: %d\n", ctx->word_count);
    return STATE_END;
}

int main(void) {
    Context ctx = {STATE_START, {0}, 0, 0, 0};
    State current_state = STATE_START;
    Event event;
    
    printf("Enter text (Ctrl+D to end):\n");
    
    while (current_state != STATE_END) {
        event = get_next_event(stdin);
        
        switch (current_state) {
            case STATE_START:
                current_state = handle_state_start(event, &ctx);
                break;
            case STATE_READING:
                current_state = handle_state_reading(event, &ctx);
                break;
            case STATE_PROCESSING:
                current_state = handle_state_processing(event, &ctx);
                break;
            case STATE_OUTPUT:
                current_state = handle_state_output(&ctx);
                break;
            case STATE_END:
                break;
        }
        
        if (ctx.buffer_pos >= MAX_INPUT_LEN - 1) {
            ctx.buffer[MAX_INPUT_LEN - 1] = '\0';
            current_state = STATE_PROCESSING;
        }
    }
    
    if (ctx.word_count == 0) {
        printf("No words processed.\n");
    }
    
    return 0;
}