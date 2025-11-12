//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <limits.h>

enum state {
    INIT,
    READING_NUMBER,
    READING_WORD,
    FINISHED,
    ERROR
};

struct context {
    enum state current;
    int number;
    char word[32];
    size_t word_len;
};

int main(void) {
    struct context ctx = {INIT, 0, {0}, 0};
    char input[256];
    char output[512] = {0};
    size_t output_len = 0;
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len >= sizeof(input)) {
        return 1;
    }
    
    for (size_t i = 0; i < input_len && ctx.current != FINISHED && ctx.current != ERROR; i++) {
        char c = input[i];
        
        switch (ctx.current) {
            case INIT:
                if (isdigit((unsigned char)c)) {
                    ctx.current = READING_NUMBER;
                    ctx.number = c - '0';
                } else if (isalpha((unsigned char)c)) {
                    ctx.current = READING_WORD;
                    ctx.word[0] = c;
                    ctx.word_len = 1;
                } else if (c == '\n' || c == '\0') {
                    ctx.current = FINISHED;
                } else if (!isspace((unsigned char)c)) {
                    ctx.current = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit((unsigned char)c)) {
                    if (ctx.number > INT_MAX / 10) {
                        ctx.current = ERROR;
                        break;
                    }
                    int new_number = ctx.number * 10 + (c - '0');
                    if (new_number < ctx.number) {
                        ctx.current = ERROR;
                    } else {
                        ctx.number = new_number;
                    }
                } else if (isalpha((unsigned char)c)) {
                    int written = snprintf(output + output_len, 
                                         sizeof(output) - output_len,
                                         "Number: %d ", ctx.number);
                    if (written > 0 && (size_t)written < sizeof(output) - output_len) {
                        output_len += written;
                    }
                    ctx.current = READING_WORD;
                    ctx.word[0] = c;
                    ctx.word_len = 1;
                } else if (c == '\n' || c == '\0') {
                    int written = snprintf(output + output_len,
                                         sizeof(output) - output_len,
                                         "Number: %d", ctx.number);
                    if (written > 0 && (size_t)written < sizeof(output) - output_len) {
                        output_len += written;
                    }
                    ctx.current = FINISHED;
                } else if (!isspace((unsigned char)c)) {
                    ctx.current = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha((unsigned char)c)) {
                    if (ctx.word_len < sizeof(ctx.word) - 1) {
                        ctx.word[ctx.word_len++] = c;
                    } else {
                        ctx.current = ERROR;
                    }
                } else if (isdigit((unsigned char)c)) {
                    ctx.word[ctx.word_len] = '\0';
                    int written = snprintf(output + output_len,
                                         sizeof(output) - output_len,
                                         "Word: %s ", ctx.word);
                    if (written > 0 && (size_t)written < sizeof(output) - output_len) {
                        output_len += written;
                    }
                    ctx.current = READING_NUMBER;
                    ctx.number = c - '0';
                } else if (c == '\n' || c == '\0') {
                    ctx.word[ctx.word_len] = '\0';
                    int written = snprintf(output + output_len,
                                         sizeof(output) - output_len,
                                         "Word: %s", ctx.word);
                    if (written > 0 && (size_t)written < sizeof(output) - output_len) {
                        output_len += written;
                    }
                    ctx.current = FINISHED;
                } else if (!isspace((unsigned char)c)) {
                    ctx.current = ERROR;
                }
                break;
                
            case FINISHED:
            case ERROR:
                break;
        }
    }
    
    if (ctx.current == READING_NUMBER) {
        int written = snprintf(output + output_len,
                             sizeof(output) - output_len,
                             "Number: %d", ctx.number);
        if (written > 0 && (size_t)written < sizeof(output) - output_len) {
            output_len += written;
        }
    } else if (ctx.current == READING_WORD) {
        ctx.word[ctx.word_len] = '\0';
        int written = snprintf(output + output_len,
                             sizeof(output) - output_len,
                             "Word: %s", ctx.word);
        if (written > 0 && (size_t)written < sizeof(output) - output_len) {
            output_len += written;
        }
    }
    
    if (ctx.current == ERROR) {
        fputs("Error: Invalid input format\n", stdout);
        return 1;
    }
    
    if (output_len > 0 && output_len < sizeof(output)) {
        puts(output);
    }
    
    return 0;
}