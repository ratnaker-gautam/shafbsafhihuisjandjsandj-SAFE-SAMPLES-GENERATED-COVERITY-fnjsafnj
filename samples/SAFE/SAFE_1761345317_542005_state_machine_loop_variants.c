//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum state {
    START,
    READ_CHAR,
    PROCESS_WORD,
    COUNT_LETTERS,
    FINALIZE,
    ERROR,
    DONE
};

struct context {
    enum state current_state;
    char input_buffer[256];
    size_t input_len;
    size_t position;
    unsigned int word_count;
    unsigned int letter_count;
    unsigned int current_word_letters;
};

void initialize_context(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = START;
    memset(ctx->input_buffer, 0, sizeof(ctx->input_buffer));
    ctx->input_len = 0;
    ctx->position = 0;
    ctx->word_count = 0;
    ctx->letter_count = 0;
    ctx->current_word_letters = 0;
}

enum state handle_start(struct context *ctx) {
    if (ctx == NULL) return ERROR;
    printf("Enter text (max 255 characters): ");
    
    if (fgets(ctx->input_buffer, sizeof(ctx->input_buffer), stdin) == NULL) {
        return ERROR;
    }
    
    ctx->input_len = strlen(ctx->input_buffer);
    if (ctx->input_len > 0 && ctx->input_buffer[ctx->input_len - 1] == '\n') {
        ctx->input_buffer[ctx->input_len - 1] = '\0';
        ctx->input_len--;
    }
    
    ctx->position = 0;
    return READ_CHAR;
}

enum state handle_read_char(struct context *ctx) {
    if (ctx == NULL) return ERROR;
    if (ctx->position >= ctx->input_len) {
        return FINALIZE;
    }
    
    char c = ctx->input_buffer[ctx->position];
    if (isalpha((unsigned char)c)) {
        ctx->current_word_letters++;
        ctx->letter_count++;
        ctx->position++;
        return COUNT_LETTERS;
    } else if (isspace((unsigned char)c) || ispunct((unsigned char)c)) {
        ctx->position++;
        return PROCESS_WORD;
    }
    
    ctx->position++;
    return READ_CHAR;
}

enum state handle_process_word(struct context *ctx) {
    if (ctx == NULL) return ERROR;
    if (ctx->current_word_letters > 0) {
        ctx->word_count++;
        ctx->current_word_letters = 0;
    }
    return READ_CHAR;
}

enum state handle_count_letters(struct context *ctx) {
    if (ctx == NULL) return ERROR;
    return READ_CHAR;
}

enum state handle_finalize(struct context *ctx) {
    if (ctx == NULL) return ERROR;
    if (ctx->current_word_letters > 0) {
        ctx->word_count++;
    }
    
    printf("Analysis complete:\n");
    printf("Words: %u\n", ctx->word_count);
    printf("Letters: %u\n", ctx->letter_count);
    
    return DONE;
}

enum state handle_error(struct context *ctx) {
    if (ctx == NULL) return ERROR;
    printf("Error occurred during processing.\n");
    return DONE;
}

int main(void) {
    struct context ctx;
    initialize_context(&ctx);
    
    int loop_counter = 0;
    const int max_iterations = 1000;
    
    while (ctx.current_state != DONE && loop_counter < max_iterations) {
        switch (ctx.current_state) {
            case START:
                ctx.current_state = handle_start(&ctx);
                break;
            case READ_CHAR:
                ctx.current_state = handle_read_char(&ctx);
                break;
            case PROCESS_WORD:
                ctx.current_state = handle_process_word(&ctx);
                break;
            case COUNT_LETTERS:
                ctx.current_state = handle_count_letters(&ctx);
                break;
            case FINALIZE:
                ctx.current_state = handle_finalize(&ctx);
                break;
            case ERROR:
                ctx.current_state = handle_error(&ctx);
                break;
            case DONE:
                break;
            default:
                ctx.current_state = ERROR;
                break;
        }
        
        loop_counter++;
        if (loop_counter >= max_iterations) {
            printf("State machine exceeded maximum iterations.\n");
            break;
        }
    }
    
    return 0;
}