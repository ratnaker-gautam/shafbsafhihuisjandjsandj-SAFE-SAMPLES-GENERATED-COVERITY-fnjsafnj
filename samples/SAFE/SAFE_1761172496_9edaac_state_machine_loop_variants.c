//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { IDLE, READING, PROCESSING, WRITING, ERROR, DONE };

struct Context {
    enum State current_state;
    char buffer[256];
    size_t buffer_pos;
    int word_count;
    int char_count;
};

void initialize_context(struct Context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = IDLE;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_pos = 0;
    ctx->word_count = 0;
    ctx->char_count = 0;
}

int is_valid_char(int c) {
    return c >= 0 && c <= 127;
}

int is_word_char(int c) {
    return isalpha(c) || isdigit(c);
}

void process_state_machine(struct Context *ctx) {
    if (ctx == NULL) return;
    
    int input_char;
    int transition_occurred;
    
    do {
        transition_occurred = 0;
        
        switch (ctx->current_state) {
            case IDLE:
                printf("Enter text (Ctrl+D to end): ");
                ctx->current_state = READING;
                transition_occurred = 1;
                break;
                
            case READING:
                input_char = getchar();
                if (!is_valid_char(input_char)) {
                    ctx->current_state = ERROR;
                    transition_occurred = 1;
                } else if (input_char == EOF) {
                    ctx->current_state = DONE;
                    transition_occurred = 1;
                } else if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buffer_pos++] = (char)input_char;
                    ctx->char_count++;
                    if (input_char == '\n') {
                        ctx->current_state = PROCESSING;
                        transition_occurred = 1;
                    }
                } else {
                    ctx->current_state = ERROR;
                    transition_occurred = 1;
                }
                break;
                
            case PROCESSING:
                for (size_t i = 0; i < ctx->buffer_pos; i++) {
                    if (i == 0 || (i > 0 && !is_word_char(ctx->buffer[i-1]))) {
                        if (is_word_char(ctx->buffer[i])) {
                            ctx->word_count++;
                        }
                    }
                }
                ctx->current_state = WRITING;
                transition_occurred = 1;
                break;
                
            case WRITING:
                printf("Buffer: %.*s\n", (int)ctx->buffer_pos, ctx->buffer);
                printf("Words: %d, Characters: %d\n", ctx->word_count, ctx->char_count);
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                ctx->buffer_pos = 0;
                ctx->word_count = 0;
                ctx->char_count = 0;
                ctx->current_state = IDLE;
                transition_occurred = 1;
                break;
                
            case ERROR:
                printf("Error: Invalid input or buffer overflow\n");
                ctx->current_state = DONE;
                transition_occurred = 1;
                break;
                
            case DONE:
                break;
        }
    } while (transition_occurred && ctx->current_state != DONE);
}

int main(void) {
    struct Context ctx;
    initialize_context(&ctx);
    
    while (ctx.current_state != DONE) {
        process_state_machine(&ctx);
    }
    
    printf("Processing complete.\n");
    return 0;
}