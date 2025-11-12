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
    int number_count;
    int word_count;
    char buffer[256];
    size_t buffer_pos;
};

void init_context(struct Context *ctx) {
    ctx->current_state = START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int is_valid_input_char(char c) {
    return isalnum((unsigned char)c) || isspace((unsigned char)c);
}

enum State handle_start(char c, struct Context *ctx) {
    if (isdigit((unsigned char)c)) {
        ctx->buffer[0] = c;
        ctx->buffer_pos = 1;
        return READING_NUMBER;
    } else if (isalpha((unsigned char)c)) {
        ctx->buffer[0] = c;
        ctx->buffer_pos = 1;
        return READING_WORD;
    } else if (isspace((unsigned char)c)) {
        return SKIPPING_SPACE;
    }
    return ERROR;
}

enum State handle_reading_number(char c, struct Context *ctx) {
    if (isdigit((unsigned char)c)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = c;
            return READING_NUMBER;
        }
        return ERROR;
    } else if (isspace((unsigned char)c)) {
        ctx->number_count++;
        ctx->buffer_pos = 0;
        return SKIPPING_SPACE;
    }
    return ERROR;
}

enum State handle_reading_word(char c, struct Context *ctx) {
    if (isalpha((unsigned char)c)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = c;
            return READING_WORD;
        }
        return ERROR;
    } else if (isspace((unsigned char)c)) {
        ctx->word_count++;
        ctx->buffer_pos = 0;
        return SKIPPING_SPACE;
    }
    return ERROR;
}

enum State handle_skipping_space(char c, struct Context *ctx) {
    if (isspace((unsigned char)c)) {
        return SKIPPING_SPACE;
    } else if (isdigit((unsigned char)c)) {
        ctx->buffer[0] = c;
        ctx->buffer_pos = 1;
        return READING_NUMBER;
    } else if (isalpha((unsigned char)c)) {
        ctx->buffer[0] = c;
        ctx->buffer_pos = 1;
        return READING_WORD;
    }
    return ERROR;
}

int process_input(const char *input, struct Context *ctx) {
    size_t i = 0;
    while (input[i] != '\0') {
        if (!is_valid_input_char(input[i])) {
            return -1;
        }
        
        enum State next_state;
        switch (ctx->current_state) {
            case START:
                next_state = handle_start(input[i], ctx);
                break;
            case READING_NUMBER:
                next_state = handle_reading_number(input[i], ctx);
                break;
            case READING_WORD:
                next_state = handle_reading_word(input[i], ctx);
                break;
            case SKIPPING_SPACE:
                next_state = handle_skipping_space(input[i], ctx);
                break;
            default:
                return -1;
        }
        
        if (next_state == ERROR) {
            return -1;
        }
        ctx->current_state = next_state;
        i++;
    }
    
    if (ctx->current_state == READING_NUMBER) {
        ctx->number_count++;
    } else if (ctx->current_state == READING_WORD) {
        ctx->word_count++;
    }
    
    ctx->current_state = FINISHED;
    return 0;
}

int main(void) {
    char input[1024];
    struct Context ctx;
    
    printf("Enter text (alphanumeric and spaces only): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || len >= sizeof(input)) {
        fprintf(stderr, "Invalid input length\n");
        return 1;
    }
    
    init_context(&ctx);
    
    if (process_input(input, &ctx) != 0) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    
    return 0;
}