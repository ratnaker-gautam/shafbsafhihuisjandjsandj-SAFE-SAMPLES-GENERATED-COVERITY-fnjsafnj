//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_IDENTIFIER,
    STATE_NUMBER,
    STATE_OPERATOR,
    STATE_END,
    STATE_ERROR
} state_t;

typedef struct {
    state_t current;
    char *input;
    size_t pos;
    size_t len;
} context_t;

state_t transition_start(context_t *ctx);
state_t transition_identifier(context_t *ctx);
state_t transition_number(context_t *ctx);
state_t transition_operator(context_t *ctx);
state_t transition_end(context_t *ctx);
state_t transition_error(context_t *ctx);

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    printf("Enter expression: ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && *(input_buffer + input_len - 1) == '\n') {
        *(input_buffer + input_len - 1) = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return EXIT_FAILURE;
    }
    
    if (input_len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return EXIT_FAILURE;
    }
    
    context_t ctx;
    ctx.current = STATE_START;
    ctx.input = input_buffer;
    ctx.pos = 0;
    ctx.len = input_len;
    
    state_t (*transitions[])(context_t *) = {
        transition_start,
        transition_identifier,
        transition_number,
        transition_operator,
        transition_end,
        transition_error
    };
    
    int step_count = 0;
    while (ctx.current != STATE_END && ctx.current != STATE_ERROR && step_count < 100) {
        if (ctx.current < 0 || ctx.current >= sizeof(transitions)/sizeof(transitions[0])) {
            fprintf(stderr, "Invalid state\n");
            return EXIT_FAILURE;
        }
        
        state_t next_state = transitions[ctx.current](&ctx);
        ctx.current = next_state;
        step_count++;
        
        if (ctx.pos > ctx.len) {
            ctx.current = STATE_ERROR;
            break;
        }
    }
    
    if (ctx.current == STATE_END) {
        printf("Valid expression\n");
    } else {
        printf("Invalid expression at position %zu\n", ctx.pos);
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

state_t transition_start(context_t *ctx) {
    if (ctx->pos >= ctx->len) {
        return STATE_END;
    }
    
    char current_char = *(ctx->input + ctx->pos);
    
    if (isalpha(current_char)) {
        return STATE_IDENTIFIER;
    } else if (isdigit(current_char)) {
        return STATE_NUMBER;
    } else if (strchr("+-*/", current_char) != NULL) {
        return STATE_OPERATOR;
    }
    
    return STATE_ERROR;
}

state_t transition_identifier(context_t *ctx) {
    while (ctx->pos < ctx->len) {
        char current_char = *(ctx->input + ctx->pos);
        
        if (!isalnum(current_char) && current_char != '_') {
            break;
        }
        
        ctx->pos++;
    }
    
    if (ctx->pos >= ctx->len) {
        return STATE_END;
    }
    
    char current_char = *(ctx->input + ctx->pos);
    
    if (strchr("+-*/", current_char) != NULL) {
        ctx->pos++;
        return STATE_OPERATOR;
    }
    
    return STATE_ERROR;
}

state_t transition_number(context_t *ctx) {
    while (ctx->pos < ctx->len) {
        char current_char = *(ctx->input + ctx->pos);
        
        if (!isdigit(current_char)) {
            break;
        }
        
        ctx->pos++;
    }
    
    if (ctx->pos >= ctx->len) {
        return STATE_END;
    }
    
    char current_char = *(ctx->input + ctx->pos);
    
    if (strchr("+-*/", current_char) != NULL) {
        ctx->pos++;
        return STATE_OPERATOR;
    }
    
    return STATE_ERROR;
}

state_t transition_operator(context_t *ctx) {
    if (ctx->pos >= ctx->len) {
        return STATE_ERROR;
    }
    
    char current_char = *(ctx->input + ctx->pos);
    
    if (isalpha(current_char)) {
        return STATE_IDENTIFIER;
    } else if (isdigit(current_char)) {
        return STATE_NUMBER;
    }
    
    return STATE_ERROR;
}

state_t transition_end(context_t *ctx) {
    return STATE_END;
}

state_t transition_error(context_t *ctx) {
    return STATE_ERROR;
}