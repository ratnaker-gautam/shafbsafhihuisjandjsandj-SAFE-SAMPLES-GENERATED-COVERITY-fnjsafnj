//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_END
} parser_state_t;

typedef struct {
    parser_state_t current_state;
    int number_count;
    int word_count;
    char buffer[MAX_INPUT_LEN + 1];
} parser_context_t;

void initialize_parser(parser_context_t *ctx) {
    ctx->current_state = STATE_START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

parser_state_t handle_start_state(char c, parser_context_t *ctx) {
    if (c == '\n' || c == '\0') {
        return STATE_END;
    } else if (isdigit(c)) {
        ctx->buffer[0] = c;
        ctx->buffer[1] = '\0';
        return STATE_READING_NUMBER;
    } else if (isalpha(c)) {
        ctx->buffer[0] = c;
        ctx->buffer[1] = '\0';
        return STATE_READING_WORD;
    } else if (isspace(c)) {
        return STATE_START;
    } else {
        return STATE_ERROR;
    }
}

parser_state_t handle_reading_number(char c, parser_context_t *ctx) {
    size_t len = strlen(ctx->buffer);
    if (len >= MAX_INPUT_LEN) {
        return STATE_ERROR;
    }
    
    if (isdigit(c)) {
        ctx->buffer[len] = c;
        ctx->buffer[len + 1] = '\0';
        return STATE_READING_NUMBER;
    } else if (isspace(c) || c == '\n' || c == '\0') {
        ctx->number_count++;
        printf("Found number: %s\n", ctx->buffer);
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        if (c == '\n' || c == '\0') {
            return STATE_END;
        }
        return STATE_START;
    } else {
        return STATE_ERROR;
    }
}

parser_state_t handle_reading_word(char c, parser_context_t *ctx) {
    size_t len = strlen(ctx->buffer);
    if (len >= MAX_INPUT_LEN) {
        return STATE_ERROR;
    }
    
    if (isalpha(c)) {
        ctx->buffer[len] = c;
        ctx->buffer[len + 1] = '\0';
        return STATE_READING_WORD;
    } else if (isspace(c) || c == '\n' || c == '\0') {
        ctx->word_count++;
        printf("Found word: %s\n", ctx->buffer);
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        if (c == '\n' || c == '\0') {
            return STATE_END;
        }
        return STATE_START;
    } else {
        return STATE_ERROR;
    }
}

int process_input(const char *input, parser_context_t *ctx) {
    size_t i = 0;
    char c;
    
    while ((c = input[i]) != '\0') {
        switch (ctx->current_state) {
            case STATE_START:
                ctx->current_state = handle_start_state(c, ctx);
                break;
            case STATE_READING_NUMBER:
                ctx->current_state = handle_reading_number(c, ctx);
                break;
            case STATE_READING_WORD:
                ctx->current_state = handle_reading_word(c, ctx);
                break;
            case STATE_ERROR:
                printf("Error: Invalid input format\n");
                return -1;
            case STATE_END:
                return 0;
        }
        
        if (ctx->current_state == STATE_ERROR) {
            printf("Error: Invalid input format\n");
            return -1;
        }
        i++;
    }
    
    if (ctx->current_state == STATE_READING_NUMBER || ctx->current_state == STATE_READING_WORD) {
        if (ctx->current_state == STATE_READING_NUMBER) {
            ctx->number_count++;
            printf("Found number: %s\n", ctx->buffer);
        } else {
            ctx->word_count++;
            printf("Found word: %s\n", ctx->buffer);
        }
    }
    
    return 0;
}

int main(void) {
    char input[MAX_INPUT_LEN * 2];
    parser_context_t ctx;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN * 2 - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len >= sizeof(input) - 1) {
        printf("Error: Input too long\n");
        return 1;
    }
    
    initialize_parser(&ctx);
    
    if (process_input(input, &ctx) != 0) {
        return 1;
    }
    
    printf("\nSummary: %d numbers, %d words\n", ctx.number_count, ctx.word_count);
    
    return 0;
}