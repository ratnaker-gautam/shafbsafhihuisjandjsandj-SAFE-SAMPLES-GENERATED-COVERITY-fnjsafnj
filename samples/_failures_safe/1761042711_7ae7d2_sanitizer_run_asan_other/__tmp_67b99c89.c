//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: state_machine
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

#define TRANSITION(state_var, new_state) do { state_var = new_state; } while(0)
#define IS_VALID_CHAR(c) ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ' || c == '\n' || c == '\0')

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count;
    char input_buffer[MAX_INPUT_LEN + 1];
    int input_pos;
} context_t;

void initialize_context(context_t *ctx) {
    memset(ctx, 0, sizeof(context_t));
    ctx->word_count = 0;
    ctx->input_pos = 0;
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

int process_word(context_t *ctx, const char *word) {
    if (ctx->word_count >= MAX_WORDS) return 0;
    
    size_t word_len = strlen(word);
    if (word_len == 0 || word_len >= MAX_WORD_LEN) return 0;
    
    strncpy(ctx->words[ctx->word_count], word, MAX_WORD_LEN - 1);
    ctx->words[ctx->word_count][MAX_WORD_LEN - 1] = '\0';
    ctx->word_count++;
    return 1;
}

void process_input(context_t *ctx, int *current_state) {
    char *token;
    char *saveptr;
    char buffer[MAX_INPUT_LEN + 1];
    
    strncpy(buffer, ctx->input_buffer, MAX_INPUT_LEN);
    buffer[MAX_INPUT_LEN] = '\0';
    
    token = strtok_r(buffer, " ", &saveptr);
    while (token != NULL) {
        if (!process_word(ctx, token)) {
            TRANSITION(*current_state, STATE_ERROR);
            return;
        }
        token = strtok_r(NULL, " ", &saveptr);
    }
}

void print_results(const context_t *ctx) {
    printf("Processed %d words:\n", ctx->word_count);
    for (int i = 0; i < ctx->word_count; i++) {
        printf("%d: %s\n", i + 1, ctx->words[i]);
    }
}

int main(void) {
    context_t ctx;
    int current_state = STATE_IDLE;
    char input[MAX_INPUT_LEN + 1];
    
    initialize_context(&ctx);
    
    printf("Enter text (letters and spaces only, max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(current_state, STATE_ERROR);
    } else {
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (validate_input(input)) {
            strncpy(ctx.input_buffer, input, MAX_INPUT_LEN);
            ctx.input_buffer[MAX_INPUT_LEN] = '\0';
            TRANSITION(current_state, STATE_READING);
        } else {
            TRANSITION(current_state, STATE_ERROR);
        }
    }
    
    while (current_state != STATE_ERROR && current_state != STATE_WRITING) {
        switch (current_state) {
            case STATE_READING:
                TRANSITION(current_state, STATE_PROCESSING);
                break;
                
            case STATE_PROCESSING:
                process_input(&ctx, &current_state);
                if (current_state != STATE_ERROR) {
                    TRANSITION(current_state, STATE_WRITING);
                }
                break;
                
            default:
                TRANSITION(current_state, STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_WRITING) {
        print_results(&ctx);
        return EXIT_SUCCESS;
    } else {
        printf("Error: Invalid input or processing error\n");
        return EXIT_FAILURE;
    }
}