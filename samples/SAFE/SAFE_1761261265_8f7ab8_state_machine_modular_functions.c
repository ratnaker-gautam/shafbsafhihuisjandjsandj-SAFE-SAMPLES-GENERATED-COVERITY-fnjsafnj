//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum state {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_DONE,
    STATE_ERROR
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_pos;
    int word_count;
    int char_count;
};

void state_idle(struct context *ctx, char input);
void state_reading(struct context *ctx, char input);
void state_processing(struct context *ctx, char input);
void state_done(struct context *ctx, char input);
void state_error(struct context *ctx, char input);

void initialize_context(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_IDLE;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_pos = 0;
    ctx->word_count = 0;
    ctx->char_count = 0;
}

void transition_state(struct context *ctx, enum state new_state) {
    if (ctx == NULL) return;
    ctx->current_state = new_state;
}

void state_idle(struct context *ctx, char input) {
    if (ctx == NULL) return;
    
    if (input == 'S' || input == 's') {
        printf("Starting text analysis...\n");
        transition_state(ctx, STATE_READING);
    } else if (input == 'Q' || input == 'q') {
        transition_state(ctx, STATE_DONE);
    } else if (input != '\n') {
        printf("Press 'S' to start or 'Q' to quit\n");
    }
}

void state_reading(struct context *ctx, char input) {
    if (ctx == NULL) return;
    
    if (input == '\n') {
        if (ctx->buffer_pos > 0) {
            transition_state(ctx, STATE_PROCESSING);
        } else {
            transition_state(ctx, STATE_DONE);
        }
    } else if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
        ctx->buffer[ctx->buffer_pos++] = input;
        ctx->char_count++;
    } else {
        printf("Buffer full, processing...\n");
        transition_state(ctx, STATE_PROCESSING);
    }
}

void state_processing(struct context *ctx, char input) {
    if (ctx == NULL) return;
    
    if (ctx->buffer_pos == 0) {
        transition_state(ctx, STATE_DONE);
        return;
    }
    
    ctx->buffer[ctx->buffer_pos] = '\0';
    
    int in_word = 0;
    for (size_t i = 0; i < ctx->buffer_pos; i++) {
        if (isalpha((unsigned char)ctx->buffer[i])) {
            if (!in_word) {
                ctx->word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
    }
    
    printf("Processed: %zu characters, %d words\n", ctx->buffer_pos, ctx->word_count);
    
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_pos = 0;
    
    if (input == 'C' || input == 'c') {
        transition_state(ctx, STATE_READING);
        printf("Continue entering text...\n");
    } else {
        transition_state(ctx, STATE_DONE);
    }
}

void state_done(struct context *ctx, char input) {
    if (ctx == NULL) return;
    
    printf("Final results: %d total characters, %d total words\n", 
           ctx->char_count, ctx->word_count);
    printf("Thank you for using the text analyzer.\n");
    
    if (input == 'R' || input == 'r') {
        initialize_context(ctx);
        printf("Reset complete. Press 'S' to start again.\n");
    } else {
        exit(0);
    }
}

void state_error(struct context *ctx, char input) {
    if (ctx == NULL) return;
    
    printf("Error occurred. Resetting...\n");
    initialize_context(ctx);
}

void process_input(struct context *ctx, char input) {
    if (ctx == NULL) return;
    
    switch (ctx->current_state) {
        case STATE_IDLE:
            state_idle(ctx, input);
            break;
        case STATE_READING:
            state_reading(ctx, input);
            break;
        case STATE_PROCESSING:
            state_processing(ctx, input);
            break;
        case STATE_DONE:
            state_done(ctx, input);
            break;
        case STATE_ERROR:
            state_error(ctx, input);
            break;
    }
}

int main(void) {
    struct context ctx;
    initialize_context(&ctx);
    
    printf("Text Analysis State Machine\n");
    printf("Commands: S=Start, Q=Quit, C=Continue, R=Reset\n");
    printf("Enter text line by line for analysis\n");
    
    int input_char;
    while (1) {
        input_char = getchar();
        if (input_char == EOF) {
            break;
        }
        
        process_input(&ctx, (char)input_char);
        
        if (ctx.current_state == STATE_DONE && input_char != 'R' && input_char != 'r') {
            break;
        }
    }
    
    return 0;
}