//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    START,
    READ_CHAR,
    COUNT_DIGITS,
    PROCESS_WORD,
    FINISHED,
    ERROR
};

struct Context {
    enum State current_state;
    char input_buffer[256];
    size_t input_pos;
    int digit_count;
    int word_count;
};

void process_state_machine(struct Context *ctx, char c) {
    switch (ctx->current_state) {
        case START:
            if (c == '\0' || c == '\n') {
                ctx->current_state = FINISHED;
            } else if (isalpha(c)) {
                ctx->current_state = PROCESS_WORD;
                ctx->word_count++;
            } else if (isdigit(c)) {
                ctx->current_state = COUNT_DIGITS;
                ctx->digit_count++;
            } else {
                ctx->current_state = READ_CHAR;
            }
            break;
            
        case READ_CHAR:
            if (c == '\0' || c == '\n') {
                ctx->current_state = FINISHED;
            } else if (isalpha(c)) {
                ctx->current_state = PROCESS_WORD;
                ctx->word_count++;
            } else if (isdigit(c)) {
                ctx->current_state = COUNT_DIGITS;
                ctx->digit_count++;
            }
            break;
            
        case COUNT_DIGITS:
            if (c == '\0' || c == '\n') {
                ctx->current_state = FINISHED;
            } else if (isalpha(c)) {
                ctx->current_state = PROCESS_WORD;
                ctx->word_count++;
            } else if (!isdigit(c)) {
                ctx->current_state = READ_CHAR;
            } else {
                ctx->digit_count++;
            }
            break;
            
        case PROCESS_WORD:
            if (c == '\0' || c == '\n') {
                ctx->current_state = FINISHED;
            } else if (!isalpha(c)) {
                if (isdigit(c)) {
                    ctx->current_state = COUNT_DIGITS;
                    ctx->digit_count++;
                } else {
                    ctx->current_state = READ_CHAR;
                }
            }
            break;
            
        case FINISHED:
        case ERROR:
            break;
    }
}

void recursive_processor(struct Context *ctx, const char *input, size_t pos) {
    if (ctx->current_state == FINISHED || ctx->current_state == ERROR) {
        return;
    }
    
    if (pos >= strlen(input)) {
        process_state_machine(ctx, '\0');
        return;
    }
    
    char current_char = input[pos];
    process_state_machine(ctx, current_char);
    
    if (ctx->current_state != FINISHED && ctx->current_state != ERROR) {
        recursive_processor(ctx, input, pos + 1);
    }
}

int main(void) {
    struct Context ctx;
    char user_input[256];
    
    printf("Enter text to analyze: ");
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(user_input);
    if (len > 0 && user_input[len - 1] == '\n') {
        user_input[len - 1] = '\0';
    }
    
    if (strlen(user_input) >= sizeof(user_input) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    ctx.current_state = START;
    ctx.input_pos = 0;
    ctx.digit_count = 0;
    ctx.word_count = 0;
    memset(ctx.input_buffer, 0, sizeof(ctx.input_buffer));
    
    recursive_processor(&ctx, user_input, 0);
    
    if (ctx.current_state == ERROR) {
        fprintf(stderr, "Processing error occurred\n");
        return 1;
    }
    
    printf("Analysis results:\n");
    printf("Total digits: %d\n", ctx.digit_count);
    printf("Total words: %d\n", ctx.word_count);
    
    return 0;
}