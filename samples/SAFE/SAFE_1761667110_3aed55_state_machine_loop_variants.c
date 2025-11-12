//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum State {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING_SPACE,
    FINISHED,
    ERROR
};

struct Context {
    enum State current_state;
    char input_buffer[256];
    size_t input_len;
    size_t position;
    int number_count;
    int word_count;
};

void initialize_context(struct Context *ctx, const char *input) {
    ctx->current_state = START;
    ctx->input_len = strnlen(input, sizeof(ctx->input_buffer) - 1);
    if (ctx->input_len >= sizeof(ctx->input_buffer)) {
        ctx->current_state = ERROR;
        return;
    }
    memcpy(ctx->input_buffer, input, ctx->input_len);
    ctx->input_buffer[ctx->input_len] = '\0';
    ctx->position = 0;
    ctx->number_count = 0;
    ctx->word_count = 0;
}

int is_digit_char(char c) {
    return isdigit((unsigned char)c);
}

int is_alpha_char(char c) {
    return isalpha((unsigned char)c);
}

int is_space_char(char c) {
    return c == ' ' || c == '\t' || c == '\n';
}

void process_state_machine(struct Context *ctx) {
    while (ctx->current_state != FINISHED && ctx->current_state != ERROR) {
        char current_char = ctx->position < ctx->input_len ? 
                           ctx->input_buffer[ctx->position] : '\0';
        
        switch (ctx->current_state) {
            case START:
                if (is_space_char(current_char)) {
                    ctx->current_state = SKIPPING_SPACE;
                } else if (is_digit_char(current_char)) {
                    ctx->current_state = READING_NUMBER;
                    ctx->number_count++;
                } else if (is_alpha_char(current_char)) {
                    ctx->current_state = READING_WORD;
                    ctx->word_count++;
                } else if (current_char == '\0') {
                    ctx->current_state = FINISHED;
                } else {
                    ctx->current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (is_digit_char(current_char)) {
                    ctx->position++;
                } else if (is_space_char(current_char)) {
                    ctx->current_state = SKIPPING_SPACE;
                } else if (current_char == '\0') {
                    ctx->current_state = FINISHED;
                } else {
                    ctx->current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (is_alpha_char(current_char)) {
                    ctx->position++;
                } else if (is_space_char(current_char)) {
                    ctx->current_state = SKIPPING_SPACE;
                } else if (current_char == '\0') {
                    ctx->current_state = FINISHED;
                } else {
                    ctx->current_state = ERROR;
                }
                break;
                
            case SKIPPING_SPACE:
                if (is_space_char(current_char)) {
                    ctx->position++;
                } else if (is_digit_char(current_char)) {
                    ctx->current_state = READING_NUMBER;
                    ctx->number_count++;
                } else if (is_alpha_char(current_char)) {
                    ctx->current_state = READING_WORD;
                    ctx->word_count++;
                } else if (current_char == '\0') {
                    ctx->current_state = FINISHED;
                } else {
                    ctx->current_state = ERROR;
                }
                break;
                
            case FINISHED:
            case ERROR:
                break;
        }
        
        if (ctx->current_state != SKIPPING_SPACE && 
            ctx->current_state != FINISHED && 
            ctx->current_state != ERROR) {
            ctx->position++;
        }
        
        if (ctx->position > ctx->input_len) {
            ctx->current_state = FINISHED;
        }
    }
}

int main(void) {
    char user_input[256];
    
    printf("Enter text to analyze (max 255 characters): ");
    
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strnlen(user_input, sizeof(user_input));
    if (input_len > 0 && user_input[input_len - 1] == '\n') {
        user_input[input_len - 1] = '\0';
    }
    
    struct Context ctx;
    initialize_context(&ctx, user_input);
    
    if (ctx.current_state == ERROR) {
        fprintf(stderr, "Input too long or invalid\n");
        return 1;
    }
    
    process_state_machine(&ctx);
    
    if (ctx.current_state == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Analysis complete:\n");
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    printf("Total characters processed: %zu\n", ctx.position);
    
    return 0;
}