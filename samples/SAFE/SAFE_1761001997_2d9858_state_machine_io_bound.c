//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: state_machine
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

State handle_event(State current, Event event, Context *ctx) {
    switch (current) {
        case STATE_START:
            if (event == EVENT_ALPHA || event == EVENT_DIGIT) {
                ctx->buffer[0] = (char)event;
                ctx->buffer_pos = 1;
                ctx->char_count = 1;
                return STATE_READING;
            }
            return STATE_START;
            
        case STATE_READING:
            if (event == EVENT_ALPHA || event == EVENT_DIGIT) {
                if (ctx->buffer_pos < MAX_INPUT_LEN - 1) {
                    ctx->buffer[ctx->buffer_pos++] = (char)event;
                    ctx->char_count++;
                }
                return STATE_READING;
            } else if (event == EVENT_NEWLINE || event == EVENT_OTHER) {
                ctx->buffer[ctx->buffer_pos] = '\0';
                return STATE_PROCESSING;
            } else if (event == EVENT_EOF) {
                ctx->buffer[ctx->buffer_pos] = '\0';
                return STATE_PROCESSING;
            }
            return STATE_READING;
            
        case STATE_PROCESSING:
            if (ctx->buffer_pos > 0) {
                ctx->word_count++;
                printf("Word %d: %s (%d chars)\n", ctx->word_count, ctx->buffer, ctx->char_count);
            }
            ctx->buffer_pos = 0;
            ctx->char_count = 0;
            
            if (event == EVENT_EOF) {
                return STATE_END;
            }
            return STATE_OUTPUT;
            
        case STATE_OUTPUT:
            if (event == EVENT_ALPHA || event == EVENT_DIGIT) {
                ctx->buffer[0] = (char)event;
                ctx->buffer_pos = 1;
                ctx->char_count = 1;
                return STATE_READING;
            } else if (event == EVENT_EOF) {
                return STATE_END;
            }
            return STATE_OUTPUT;
            
        case STATE_END:
            return STATE_END;
            
        default:
            return STATE_START;
    }
}

int main(void) {
    Context ctx = {STATE_START, {0}, 0, 0, 0};
    State current_state = STATE_START;
    Event event;
    
    printf("Enter text (Ctrl+D to end):\n");
    
    while (current_state != STATE_END) {
        event = get_next_event(stdin);
        current_state = handle_event(current_state, event, &ctx);
        
        if (current_state == STATE_PROCESSING) {
            current_state = handle_event(current_state, event, &ctx);
        }
    }
    
    printf("\nTotal words processed: %d\n", ctx.word_count);
    
    if (ferror(stdin)) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}