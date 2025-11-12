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

int state_idle(context_t *ctx) {
    printf("Enter text (letters and spaces only, max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(ctx->input_buffer, MAX_INPUT_LEN + 1, stdin) == NULL) {
        return STATE_ERROR;
    }
    
    size_t len = strlen(ctx->input_buffer);
    if (len > 0 && ctx->input_buffer[len - 1] == '\n') {
        ctx->input_buffer[len - 1] = '\0';
    }
    
    if (!validate_input(ctx->input_buffer)) {
        printf("Invalid input detected.\n");
        return STATE_ERROR;
    }
    
    ctx->input_len = strlen(ctx->input_buffer);
    return STATE_READING;
}

int state_reading(context_t *ctx) {
    const char *input = ctx->input_buffer;
    int word_start = -1;
    ctx->word_count = 0;
    
    for (int i = 0; i <= ctx->input_len && ctx->word_count < MAX_WORDS; i++) {
        char c = input[i];
        
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
            if (word_start == -1) {
                word_start = i;
            }
        } else {
            if (word_start != -1) {
                int word_len = i - word_start;
                if (word_len < MAX_WORD_LEN) {
                    strncpy(ctx->words[ctx->word_count], &input[word_start], word_len);
                    ctx->words[ctx->word_count][word_len] = '\0';
                    ctx->word_count++;
                }
                word_start = -1;
            }
        }
    }
    
    return STATE_PROCESSING;
}

int state_processing(context_t *ctx) {
    if (ctx->word_count == 0) {
        printf("No words found in input.\n");
        return STATE_IDLE;
    }
    
    for (int i = 0; i < ctx->word_count - 1; i++) {
        for (int j = i + 1; j < ctx->word_count; j++) {
            if (strcmp(ctx->words[i], ctx->words[j]) > 0) {
                char temp[MAX_WORD_LEN];
                strncpy(temp, ctx->words[i], MAX_WORD_LEN - 1);
                temp[MAX_WORD_LEN - 1] = '\0';
                strncpy(ctx->words[i], ctx->words[j], MAX_WORD_LEN - 1);
                ctx->words[i][MAX_WORD_LEN - 1] = '\0';
                strncpy(ctx->words[j], temp, MAX_WORD_LEN - 1);
                ctx->words[j][MAX_WORD_LEN - 1] = '\0';
            }
        }
    }
    
    return STATE_WRITING;
}

int state_writing(context_t *ctx) {
    printf("Sorted words (%d found):\n", ctx->word_count);
    for (int i = 0; i < ctx->word_count; i++) {
        printf("%d: %s\n", i + 1, ctx->words[i]);
    }
    return STATE_IDLE;
}

int state_error(context_t *ctx) {
    printf("An error occurred. Resetting state machine.\n");
    initialize_context(ctx);
    return STATE_IDLE;
}

int main(void) {
    context_t ctx;
    int current_state = STATE_IDLE;
    int running = 1;
    
    initialize_context(&ctx);
    
    while (running) {
        int next_state;
        
        switch (current_state) {
            case STATE_IDLE:
                next_state = state_idle(&ctx);
                break;
            case STATE_READING:
                next_state = state_reading(&ctx);
                break;
            case STATE_PROCESSING:
                next_state = state_processing(&ctx);
                break;
            case STATE_WRITING:
                next_state = state_writing(&ctx);