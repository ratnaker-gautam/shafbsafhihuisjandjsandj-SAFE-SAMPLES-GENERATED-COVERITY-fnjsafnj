//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_WRITING 3
#define STATE_ERROR 4

#define MAX_INPUT_LEN 100
#define MAX_WORDS 50
#define MAX_WORD_LEN 20

#define TRANSITION(new_state) do { current_state = new_state; } while(0)
#define IS_VALID_CHAR(c) ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ' || c == '\n' || c == '\0')

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count;
    char input_buffer[MAX_INPUT_LEN + 1];
    int input_len;
} context_t;

void initialize_context(context_t *ctx) {
    memset(ctx, 0, sizeof(context_t));
    ctx->word_count = 0;
    ctx->input_len = 0;
}

int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (!IS_VALID_CHAR(input[i])) return 0;
    }
    return 1;
}

int process_idle_state(context_t *ctx, const char *input) {
    if (!validate_input(input)) {
        return STATE_ERROR;
    }
    
    strncpy(ctx->input_buffer, input, MAX_INPUT_LEN);
    ctx->input_buffer[MAX_INPUT_LEN] = '\0';
    ctx->input_len = strlen(ctx->input_buffer);
    
    return STATE_READING;
}

int process_reading_state(context_t *ctx) {
    char *ptr = ctx->input_buffer;
    char current_word[MAX_WORD_LEN];
    int word_len = 0;
    
    while (*ptr && ctx->word_count < MAX_WORDS) {
        if ((*ptr >= 'A' && *ptr <= 'Z') || (*ptr >= 'a' && *ptr <= 'z')) {
            if (word_len < MAX_WORD_LEN - 1) {
                current_word[word_len++] = *ptr;
            }
        } else if (word_len > 0) {
            current_word[word_len] = '\0';
            strncpy(ctx->words[ctx->word_count], current_word, MAX_WORD_LEN);
            ctx->words[ctx->word_count][MAX_WORD_LEN - 1] = '\0';
            ctx->word_count++;
            word_len = 0;
        }
        ptr++;
    }
    
    if (word_len > 0 && ctx->word_count < MAX_WORDS) {
        current_word[word_len] = '\0';
        strncpy(ctx->words[ctx->word_count], current_word, MAX_WORD_LEN);
        ctx->words[ctx->word_count][MAX_WORD_LEN - 1] = '\0';
        ctx->word_count++;
    }
    
    return ctx->word_count > 0 ? STATE_PROCESSING : STATE_ERROR;
}

int process_processing_state(context_t *ctx) {
    for (int i = 0; i < ctx->word_count; i++) {
        for (int j = 0; j < strlen(ctx->words[i]); j++) {
            if (ctx->words[i][j] >= 'a' && ctx->words[i][j] <= 'z') {
                ctx->words[i][j] = ctx->words[i][j] - 'a' + 'A';
            }
        }
    }
    return STATE_WRITING;
}

int process_writing_state(context_t *ctx) {
    printf("Processed words (%d):\n", ctx->word_count);
    for (int i = 0; i < ctx->word_count; i++) {
        printf("%d: %s\n", i + 1, ctx->words[i]);
    }
    return STATE_IDLE;
}

int process_error_state(context_t *ctx) {
    printf("Error: Invalid input detected\n");
    initialize_context(ctx);
    return STATE_IDLE;
}

int main(void) {
    context_t ctx;
    int current_state = STATE_IDLE;
    char user_input[MAX_INPUT_LEN + 1];
    
    initialize_context(&ctx);
    
    printf("Word Processor State Machine\n");
    printf("Enter text (letters and spaces only, max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(user_input);
    if (len > 0 && user_input[len - 1] == '\n') {
        user_input[len - 1] = '\0';
    }
    
    while (current_state != STATE_IDLE || ctx.input_len == 0) {
        switch (current_state) {
            case STATE_IDLE:
                TRANSITION(process_idle_state(&ctx, user_input));
                break;
            case STATE_READING:
                TRANSITION(process_reading_state(&ctx));
                break;
            case STATE_PROCESSING:
                TRANSITION(process_processing_state(&ctx));
                break;
            case STATE_WRITING:
                TRANSITION(process_writing_state(&ctx));