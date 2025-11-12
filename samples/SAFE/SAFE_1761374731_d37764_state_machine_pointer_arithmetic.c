//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define STATE_START 0
#define STATE_WORD 1
#define STATE_NUMBER 2
#define STATE_PUNCT 3
#define STATE_END 4

typedef struct {
    int state;
    int word_count;
    int number_count;
    int punct_count;
} context_t;

int is_valid_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    return 1;
}

void process_character(char c, context_t *ctx) {
    switch (ctx->state) {
        case STATE_START:
            if (isalpha(c)) {
                ctx->state = STATE_WORD;
                ctx->word_count++;
            } else if (isdigit(c)) {
                ctx->state = STATE_NUMBER;
                ctx->number_count++;
            } else if (ispunct(c)) {
                ctx->state = STATE_PUNCT;
                ctx->punct_count++;
            }
            break;
            
        case STATE_WORD:
            if (!isalpha(c)) {
                if (isdigit(c)) {
                    ctx->state = STATE_NUMBER;
                    ctx->number_count++;
                } else if (ispunct(c)) {
                    ctx->state = STATE_PUNCT;
                    ctx->punct_count++;
                } else if (isspace(c)) {
                    ctx->state = STATE_START;
                }
            }
            break;
            
        case STATE_NUMBER:
            if (!isdigit(c)) {
                if (isalpha(c)) {
                    ctx->state = STATE_WORD;
                    ctx->word_count++;
                } else if (ispunct(c)) {
                    ctx->state = STATE_PUNCT;
                    ctx->punct_count++;
                } else if (isspace(c)) {
                    ctx->state = STATE_START;
                }
            }
            break;
            
        case STATE_PUNCT:
            if (!ispunct(c)) {
                if (isalpha(c)) {
                    ctx->state = STATE_WORD;
                    ctx->word_count++;
                } else if (isdigit(c)) {
                    ctx->state = STATE_NUMBER;
                    ctx->number_count++;
                } else if (isspace(c)) {
                    ctx->state = STATE_START;
                }
            }
            break;
    }
}

void analyze_text(const char *input, context_t *ctx) {
    if (!is_valid_input(input) || ctx == NULL) return;
    
    ctx->state = STATE_START;
    ctx->word_count = 0;
    ctx->number_count = 0;
    ctx->punct_count = 0;
    
    const char *ptr = input;
    while (*ptr != '\0') {
        process_character(*ptr, ctx);
        ptr++;
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    context_t ctx;
    
    printf("Enter text to analyze: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    analyze_text(input, &ctx);
    
    printf("Analysis results:\n");
    printf("Words: %d\n", ctx.word_count);
    printf("Numbers: %d\n", ctx.number_count);
    printf("Punctuation sequences: %d\n", ctx.punct_count);
    
    return 0;
}