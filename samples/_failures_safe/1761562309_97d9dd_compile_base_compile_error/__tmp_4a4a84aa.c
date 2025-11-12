//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_END
} state_t;

typedef struct {
    state_t current_state;
    int number_count;
    int word_count;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_pos;
} context_t;

void initialize_context(context_t *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int is_valid_input_char(char c) {
    return isalnum(c) || c == ' ' || c == '\t' || c == '\n' || c == '\0';
}

state_t handle_start(char c, context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (isdigit(c)) {
        if (ctx->buffer_pos < MAX_INPUT_LEN - 1) {
            ctx->buffer[ctx->buffer_pos++] = c;
        }
        return STATE_READING_NUMBER;
    } else if (isalpha(c)) {
        if (ctx->buffer_pos < MAX_INPUT_LEN - 1) {
            ctx->buffer[ctx->buffer_pos++] = c;
        }
        return STATE_READING_WORD;
    } else if (c == ' ' || c == '\t' || c == '\n') {
        return STATE_START;
    } else if (c == '\0') {
        return STATE_END;
    }
    return STATE_ERROR;
}

state_t handle_reading_number(char c, context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (isdigit(c)) {
        if (ctx->buffer_pos < MAX_INPUT_LEN - 1) {
            ctx->buffer[ctx->buffer_pos++] = c;
        }
        return STATE_READING_NUMBER;
    } else if (c == ' ' || c == '\t' || c == '\n' || c == '\0') {
        ctx->number_count++;
        ctx->buffer_pos = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        if (c == '\0') return STATE_END;
        return STATE_START;
    }
    return STATE_ERROR;
}

state_t handle_reading_word(char c, context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (isalpha(c)) {
        if (ctx->buffer_pos < MAX_INPUT_LEN - 1) {
            ctx->buffer[ctx->buffer_pos++] = c;
        }
        return STATE_READING_WORD;
    } else if (c == ' ' || c == '\t' || c == '\n' || c == '\0') {
        ctx->word_count++;
        ctx->buffer_pos = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        if (c == '\0') return STATE_END;
        return STATE_START;
    }
    return STATE_ERROR;
}

state_t process_character(char c, context_t *ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    if (!is_valid_input_char(c)) {
        return STATE_ERROR;
    }
    
    switch (ctx->current_state) {
        case STATE_START:
            return handle_start(c, ctx);
        case STATE_READING_NUMBER:
            return handle_reading_number(c, ctx);
        case STATE_READING_WORD:
            return handle_reading_word(c, ctx);
        case STATE_ERROR:
        case STATE_END:
            return ctx->current_state;
        default:
            return STATE_ERROR;
    }
}

int process_input(const char *input, context_t *ctx) {
    if (input == NULL || ctx == NULL) return 0;
    
    size_t len = strnlen(input, MAX_INPUT_LEN * 2);
    if (len >= MAX_INPUT_LEN * 2) return 0;
    
    for (size_t i = 0; i <= len; i++) {
        char c = input[i];
        state_t next_state = process_character(c, ctx);
        ctx->current_state = next_state;
        
        if (next_state == STATE_ERROR) {
            return 0;
        }
        if (next_state == STATE_END) {
            break;
        }
    }
    
    return 1;
}

void print_results(const context_t *ctx) {
    if (ctx == NULL) return;
    
    printf("Processing completed successfully.\n");
    printf("Numbers found: %d\n", ctx->number_count);
    printf("Words found: %d\n", ctx->word_count);
}

int main(void) {
    char input[MAX_INPUT_LEN * 2];
    context_t ctx;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN * 2 - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strnlen(input, sizeof(input));
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1]