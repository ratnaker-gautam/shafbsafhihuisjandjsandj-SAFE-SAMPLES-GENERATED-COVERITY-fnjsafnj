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
#define IS_VALID_CHAR(c) ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ' || c == '\n' || c == '\0')

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count;
    char input_buffer[MAX_INPUT_LEN + 1];
    int input_len;
} context_t;

void initialize_context(context_t *ctx) {
    memset(ctx, 0, sizeof(context_t));
    ctx->input_buffer[0] = '\0';
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

int process_idle_state(context_t *ctx, int *current_state, const char *input) {
    if (!validate_input(input)) {
        TRANSITION(STATE_ERROR);
        return 0;
    }
    
    strncpy(ctx->input_buffer, input, MAX_INPUT_LEN);
    ctx->input_buffer[MAX_INPUT_LEN] = '\0';
    ctx->input_len = strlen(ctx->input_buffer);
    TRANSITION(STATE_READING);
    return 1;
}

int process_reading_state(context_t *ctx, int *current_state) {
    char *ptr = ctx->input_buffer;
    char word[MAX_WORD_LEN];
    int word_len = 0;
    
    ctx->word_count = 0;
    
    while (*ptr != '\0' && ctx->word_count < MAX_WORDS) {
        if ((*ptr >= 'a' && *ptr <= 'z') || (*ptr >= 'A' && *ptr <= 'Z')) {
            if (word_len < MAX_WORD_LEN - 1) {
                word[word_len++] = *ptr;
            }
        } else if (word_len > 0) {
            word[word_len] = '\0';
            strncpy(ctx->words[ctx->word_count], word, MAX_WORD_LEN);
            ctx->words[ctx->word_count][MAX_WORD_LEN - 1] = '\0';
            ctx->word_count++;
            word_len = 0;
        }
        ptr++;
    }
    
    if (word_len > 0 && ctx->word_count < MAX_WORDS) {
        word[word_len] = '\0';
        strncpy(ctx->words[ctx->word_count], word, MAX_WORD_LEN);
        ctx->words[ctx->word_count][MAX_WORD_LEN - 1] = '\0';
        ctx->word_count++;
    }
    
    if (ctx->word_count > 0) {
        TRANSITION(STATE_PROCESSING);
        return 1;
    } else {
        TRANSITION(STATE_ERROR);
        return 0;
    }
}

int process_processing_state(context_t *ctx, int *current_state) {
    for (int i = 0; i < ctx->word_count; i++) {
        for (int j = 0; j < ctx->word_count - i - 1; j++) {
            if (strcmp(ctx->words[j], ctx->words[j + 1]) > 0) {
                char temp[MAX_WORD_LEN];
                strncpy(temp, ctx->words[j], MAX_WORD_LEN);
                temp[MAX_WORD_LEN - 1] = '\0';
                strncpy(ctx->words[j], ctx->words[j + 1], MAX_WORD_LEN);
                ctx->words[j][MAX_WORD_LEN - 1] = '\0';
                strncpy(ctx->words[j + 1], temp, MAX_WORD_LEN);
                ctx->words[j + 1][MAX_WORD_LEN - 1] = '\0';
            }
        }
    }
    TRANSITION(STATE_WRITING);
    return 1;
}

int process_writing_state(context_t *ctx, int *current_state) {
    printf("Sorted words:\n");
    for (int i = 0; i < ctx->word_count; i++) {
        printf("%d: %s\n", i + 1, ctx->words[i]);
    }
    TRANSITION(STATE_IDLE);
    return 1;
}

int process_error_state(context_t *ctx, int *current_state) {
    printf("Error: Invalid input detected\n");
    TRANSITION(STATE_IDLE);
    return 0;
}

int main(void) {
    context_t ctx;
    int current_state = STATE_IDLE;
    char user_input[MAX_INPUT_LEN + 1];
    
    initialize_context(&ctx);
    
    printf("Word sorter state machine\n");
    printf("Enter text (letters and spaces only, max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(user_input