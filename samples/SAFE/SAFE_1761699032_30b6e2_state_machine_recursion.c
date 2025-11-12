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
            if (c == '\0') {
                ctx->current_state = FINISHED;
            } else if (c >= '0' && c <= '9') {
                ctx->current_state = COUNT_DIGITS;
                ctx->digit_count = 1;
            } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                ctx->current_state = PROCESS_WORD;
                ctx->word_count = 1;
            } else {
                ctx->current_state = READ_CHAR;
            }
            break;
            
        case READ_CHAR:
            if (c == '\0') {
                ctx->current_state = FINISHED;
            } else if (c >= '0' && c <= '9') {
                ctx->current_state = COUNT_DIGITS;
                ctx->digit_count = 1;
            } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                ctx->current_state = PROCESS_WORD;
                ctx->word_count = 1;
            }
            break;
            
        case COUNT_DIGITS:
            if (c >= '0' && c <= '9') {
                ctx->digit_count++;
            } else if (c == '\0') {
                ctx->current_state = FINISHED;
            } else {
                ctx->current_state = READ_CHAR;
            }
            break;
            
        case PROCESS_WORD:
            if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                ctx->word_count++;
            } else if (c == '\0') {
                ctx->current_state = FINISHED;
            } else {
                ctx->current_state = READ_CHAR;
            }
            break;
            
        case FINISHED:
        case ERROR:
            break;
    }
}

void recursive_processor(struct Context *ctx, const char *str, size_t pos) {
    if (ctx->current_state == FINISHED || ctx->current_state == ERROR) {
        return;
    }
    
    if (pos >= strlen(str)) {
        process_state_machine(ctx, '\0');
        return;
    }
    
    char current_char = str[pos];
    process_state_machine(ctx, current_char);
    
    recursive_processor(ctx, str, pos + 1);
}

int main(void) {
    struct Context ctx;
    char input[256];
    
    printf("Enter a string to analyze: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    ctx.current_state = START;
    ctx.input_pos = 0;
    ctx.digit_count = 0;
    ctx.word_count = 0;
    memset(ctx.input_buffer, 0, sizeof(ctx.input_buffer));
    
    recursive_processor(&ctx, input, 0);
    
    printf("Final state: %d\n", ctx.current_state);
    printf("Digits counted: %d\n", ctx.digit_count);
    printf("Words processed: %d\n", ctx.word_count);
    
    return 0;
}