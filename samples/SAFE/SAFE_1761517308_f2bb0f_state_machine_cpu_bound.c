//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_INPUT_LEN 100
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_FINALIZING,
    STATE_DONE
} State;

typedef enum {
    EVENT_CHAR,
    EVENT_SPACE,
    EVENT_END,
    EVENT_ERROR
} Event;

typedef struct {
    State current_state;
    int char_count;
    int word_count;
    int in_word;
} Context;

Event get_next_event(char input, int position, int max_len) {
    if (position >= max_len) return EVENT_END;
    if (input == '\0') return EVENT_END;
    if (input == ' ') return EVENT_SPACE;
    if ((input >= 'a' && input <= 'z') || (input >= 'A' && input <= 'Z')) return EVENT_CHAR;
    return EVENT_ERROR;
}

State state_transition(State current, Event event, Context *ctx) {
    switch (current) {
        case STATE_START:
            if (event == EVENT_CHAR) {
                ctx->in_word = 1;
                ctx->word_count = 1;
                return STATE_READING;
            }
            if (event == EVENT_SPACE) return STATE_START;
            if (event == EVENT_END) return STATE_DONE;
            return STATE_START;
            
        case STATE_READING:
            if (event == EVENT_CHAR) {
                ctx->char_count++;
                return STATE_READING;
            }
            if (event == EVENT_SPACE) {
                ctx->in_word = 0;
                return STATE_PROCESSING;
            }
            if (event == EVENT_END) return STATE_FINALIZING;
            return STATE_READING;
            
        case STATE_PROCESSING:
            if (event == EVENT_CHAR) {
                ctx->in_word = 1;
                ctx->word_count++;
                return STATE_READING;
            }
            if (event == EVENT_SPACE) return STATE_PROCESSING;
            if (event == EVENT_END) return STATE_FINALIZING;
            return STATE_PROCESSING;
            
        case STATE_FINALIZING:
            return STATE_DONE;
            
        case STATE_DONE:
            return STATE_DONE;
    }
    return STATE_START;
}

void process_state(State state, Context *ctx, char input) {
    switch (state) {
        case STATE_START:
            ctx->char_count = 0;
            ctx->word_count = 0;
            ctx->in_word = 0;
            break;
            
        case STATE_READING:
            if (input != '\0' && input != ' ') {
                ctx->char_count++;
            }
            break;
            
        case STATE_PROCESSING:
            break;
            
        case STATE_FINALIZING:
            if (ctx->in_word) {
                ctx->word_count++;
            }
            break;
            
        case STATE_DONE:
            break;
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    Context ctx;
    State current_state = STATE_START;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (int i = 0; i <= (int)len; i++) {
        Event event = get_next_event(input[i], i, (int)len);
        process_state(current_state, &ctx, input[i]);
        current_state = state_transition(current_state, event, &ctx);
        
        if (current_state == STATE_DONE) {
            break;
        }
    }
    
    printf("Character count: %d\n", ctx.char_count);
    printf("Word count: %d\n", ctx.word_count);
    
    return 0;
}