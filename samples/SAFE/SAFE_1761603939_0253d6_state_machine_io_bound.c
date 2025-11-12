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

void process_input(struct Context *ctx, const char *input) {
    if (input == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    size_t len = strlen(input);
    if (len >= sizeof(ctx->buffer) - ctx->buffer_pos) {
        ctx->current_state = ERROR;
        return;
    }
    
    strncpy(ctx->buffer + ctx->buffer_pos, input, sizeof(ctx->buffer) - ctx->buffer_pos - 1);
    ctx->buffer_pos += len;
    ctx->buffer[ctx->buffer_pos] = '\0';
    
    char *token = strtok(ctx->buffer, " \t\n");
    while (token != NULL && ctx->current_state != ERROR) {
        switch (ctx->current_state) {
            case INIT:
                if (is_valid_number(token)) {
                    ctx->current_state = READING_NUMBER;
                    ctx->number_count++;
                } else if (is_valid_string(token)) {
                    ctx->current_state = READING_STRING;
                    ctx->string_count++;
                } else {
                    ctx->current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (is_valid_number(token)) {
                    ctx->number_count++;
                } else if (is_valid_string(token)) {
                    ctx->current_state = READING_STRING;
                    ctx->string_count++;
                } else {
                    ctx->current_state = ERROR;
                }
                break;
                
            case READING_STRING:
                if (is_valid_string(token)) {
                    ctx->string_count++;
                } else if (is_valid_number(token)) {
                    ctx->current_state = READING_NUMBER;
                    ctx->number_count++;
                } else {
                    ctx->current_state = ERROR;
                }
                break;
                
            default:
                ctx->current_state = ERROR;
                break;
        }
        token = strtok(NULL, " \t\n");
    }
    
    if (ctx->current_state != ERROR) {
        ctx->buffer_pos = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
    }
}

int main(void) {
    struct Context ctx;
    init_context(&ctx);
    
    char input_line[512];
    
    printf("Enter inputs (numbers and strings, one per line):\n");
    printf("Type 'END' to finish processing.\n");
    
    while (ctx.current_state != ERROR && ctx.current_state != DONE) {
        if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
            break;
        }
        
        input_line[strcspn(input_line, "\n")] = '\0';
        
        if (strcmp(input_line, "END") == 0) {
            ctx.current_state = DONE;
            break;
        }
        
        process_input(&ctx, input_line);
        
        if (ctx.current_state == ERROR) {
            printf("Error: Invalid input detected.\n");
            break;
        }
    }
    
    if (ctx.current_state == DONE || ctx.current_state != ERROR) {
        printf("Processing complete.\n");
        printf("Numbers processed: %d\n", ctx.number_count);
        printf("Strings processed: %d\n", ctx.string_count);
    }
    
    return ctx.current_state == ERROR ? 1 : 0;
}