//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    INIT,
    READING_NUMBER,
    READING_STRING,
    ERROR,
    DONE
};

struct Context {
    enum State current_state;
    int number_count;
    int string_count;
    char buffer[256];
    int buffer_pos;
};

void init_context(struct Context *ctx) {
    ctx->current_state = INIT;
    ctx->number_count = 0;
    ctx->string_count = 0;
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int is_valid_number(const char *str) {
    if (str == NULL || *str == '\0') return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}

int is_valid_string(const char *str) {
    if (str == NULL || *str == '\0') return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha(str[i])) return 0;
    }
    return 1;
}

void process_token(struct Context *ctx) {
    if (ctx->buffer_pos == 0) return;
    
    ctx->buffer[ctx->buffer_pos] = '\0';
    
    if (is_valid_number(ctx->buffer)) {
        ctx->number_count++;
        printf("Found number: %s\n", ctx->buffer);
    } else if (is_valid_string(ctx->buffer)) {
        ctx->string_count++;
        printf("Found string: %s\n", ctx->buffer);
    } else {
        printf("Invalid token: %s\n", ctx->buffer);
    }
    
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

void state_machine_step(struct Context *ctx, char c) {
    switch (ctx->current_state) {
        case INIT:
            if (c == ' ' || c == '\t' || c == '\n') {
                break;
            } else if (isdigit(c)) {
                ctx->current_state = READING_NUMBER;
                if (ctx->buffer_pos < 255) {
                    ctx->buffer[ctx->buffer_pos++] = c;
                }
            } else if (isalpha(c)) {
                ctx->current_state = READING_STRING;
                if (ctx->buffer_pos < 255) {
                    ctx->buffer[ctx->buffer_pos++] = c;
                }
            } else {
                ctx->current_state = ERROR;
            }
            break;
            
        case READING_NUMBER:
            if (c == ' ' || c == '\t' || c == '\n') {
                process_token(ctx);
                ctx->current_state = INIT;
            } else if (isdigit(c)) {
                if (ctx->buffer_pos < 255) {
                    ctx->buffer[ctx->buffer_pos++] = c;
                }
            } else {
                ctx->current_state = ERROR;
            }
            break;
            
        case READING_STRING:
            if (c == ' ' || c == '\t' || c == '\n') {
                process_token(ctx);
                ctx->current_state = INIT;
            } else if (isalpha(c)) {
                if (ctx->buffer_pos < 255) {
                    ctx->buffer[ctx->buffer_pos++] = c;
                }
            } else {
                ctx->current_state = ERROR;
            }
            break;
            
        case ERROR:
            if (c == ' ' || c == '\t' || c == '\n') {
                ctx->current_state = INIT;
                ctx->buffer_pos = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
            }
            break;
            
        case DONE:
            break;
    }
}

int main(void) {
    struct Context ctx;
    init_context(&ctx);
    
    printf("Enter text (numbers and strings separated by spaces):\n");
    printf("Press Ctrl+D (Unix) or Ctrl+Z (Windows) to finish.\n");
    
    int c;
    while ((c = getchar()) != EOF) {
        if (c == '\0') continue;
        state_machine_step(&ctx, (char)c);
        
        if (ctx.current_state == ERROR) {
            printf("Invalid input detected. Resetting...\n");
            ctx.current_state = INIT;
            ctx.buffer_pos = 0;
            memset(ctx.buffer, 0, sizeof(ctx.buffer));
        }
    }
    
    if (ctx.buffer_pos > 0) {
        process_token(&ctx);
    }
    
    printf("\nProcessing complete.\n");
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Strings found: %d\n", ctx.string_count);
    
    return 0;
}