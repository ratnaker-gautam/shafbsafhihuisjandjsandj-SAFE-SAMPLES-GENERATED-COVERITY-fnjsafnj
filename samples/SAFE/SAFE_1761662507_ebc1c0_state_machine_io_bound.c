//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state { IDLE, READING, PROCESSING, WRITING, ERROR, DONE };

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_pos;
    int word_count;
    int char_count;
    int line_count;
};

void state_idle(struct context *ctx, int input_char);
void state_reading(struct context *ctx, int input_char);
void state_processing(struct context *ctx, int input_char);
void state_writing(struct context *ctx, int input_char);
void state_error(struct context *ctx, int input_char);
void state_done(struct context *ctx, int input_char);

int main(void) {
    struct context ctx;
    ctx.current_state = IDLE;
    ctx.buffer_pos = 0;
    ctx.word_count = 0;
    ctx.char_count = 0;
    ctx.line_count = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));

    printf("Enter text (Ctrl+D to end):\n");

    while (ctx.current_state != DONE && ctx.current_state != ERROR) {
        int c = getchar();
        
        if (c == EOF && ctx.current_state != IDLE) {
            c = '\n';
        }

        switch (ctx.current_state) {
            case IDLE:
                state_idle(&ctx, c);
                break;
            case READING:
                state_reading(&ctx, c);
                break;
            case PROCESSING:
                state_processing(&ctx, c);
                break;
            case WRITING:
                state_writing(&ctx, c);
                break;
            case ERROR:
                state_error(&ctx, c);
                break;
            case DONE:
                break;
        }
    }

    if (ctx.current_state == ERROR) {
        fprintf(stderr, "Error occurred during processing\n");
        return EXIT_FAILURE;
    }

    printf("\nProcessing complete:\n");
    printf("Lines: %d\n", ctx.line_count);
    printf("Words: %d\n", ctx.word_count);
    printf("Characters: %d\n", ctx.char_count);

    return EXIT_SUCCESS;
}

void state_idle(struct context *ctx, int input_char) {
    if (input_char == EOF) {
        ctx->current_state = DONE;
        return;
    }
    
    if (input_char == '\n') {
        ctx->line_count++;
        return;
    }
    
    if (!isspace(input_char)) {
        ctx->buffer[0] = (char)input_char;
        ctx->buffer_pos = 1;
        ctx->char_count++;
        ctx->current_state = READING;
    }
}

void state_reading(struct context *ctx, int input_char) {
    if (input_char == EOF) {
        ctx->current_state = PROCESSING;
        return;
    }
    
    if (isspace(input_char)) {
        ctx->current_state = PROCESSING;
        return;
    }
    
    if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
        ctx->buffer[ctx->buffer_pos++] = (char)input_char;
        ctx->char_count++;
    } else {
        ctx->current_state = ERROR;
    }
}

void state_processing(struct context *ctx, int input_char) {
    if (ctx->buffer_pos > 0) {
        ctx->buffer[ctx->buffer_pos] = '\0';
        ctx->word_count++;
    }
    
    if (input_char == '\n') {
        ctx->line_count++;
    }
    
    ctx->buffer_pos = 0;
    ctx->current_state = WRITING;
}

void state_writing(struct context *ctx, int input_char) {
    if (ctx->buffer[0] != '\0') {
        printf("Word: %s\n", ctx->buffer);
        ctx->buffer[0] = '\0';
    }
    
    if (input_char == EOF) {
        ctx->current_state = DONE;
    } else if (isspace(input_char)) {
        ctx->current_state = IDLE;
    } else {
        ctx->buffer[0] = (char)input_char;
        ctx->buffer_pos = 1;
        ctx->char_count++;
        ctx->current_state = READING;
    }
}

void state_error(struct context *ctx, int input_char) {
    (void)input_char;
}

void state_done(struct context *ctx, int input_char) {
    (void)ctx;
    (void)input_char;
}