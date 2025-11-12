//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_DONE 3
#define STATE_ERROR 4

#define EVENT_START 0
#define EVENT_DATA 1
#define EVENT_END 2
#define EVENT_RESET 3
#define EVENT_INVALID 4

#define MAX_INPUT 100
#define MAX_WORDS 50
#define MAX_WORD_LEN 20

typedef struct {
    int state;
    char input[MAX_INPUT];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count;
    int char_count;
} context_t;

int get_next_event(const char* input, int pos) {
    if (pos >= (int)strlen(input)) return EVENT_END;
    if (pos == 0) return EVENT_START;
    if (isalnum((unsigned char)input[pos])) return EVENT_DATA;
    if (input[pos] == ' ') return EVENT_DATA;
    return EVENT_INVALID;
}

void state_machine_step(context_t* ctx, int event) {
    switch (ctx->state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                ctx->state = STATE_READING;
                ctx->word_count = 0;
                ctx->char_count = 0;
                memset(ctx->words, 0, sizeof(ctx->words));
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) {
                if (ctx->char_count < MAX_INPUT - 1) {
                    ctx->input[ctx->char_count++] = ctx->input[ctx->char_count];
                }
            } else if (event == EVENT_END) {
                ctx->input[ctx->char_count] = '\0';
                ctx->state = STATE_PROCESSING;
            } else if (event == EVENT_INVALID) {
                ctx->state = STATE_ERROR;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_END) {
                char* token = strtok(ctx->input, " ");
                while (token != NULL && ctx->word_count < MAX_WORDS) {
                    if (strlen(token) < MAX_WORD_LEN) {
                        strncpy(ctx->words[ctx->word_count], token, MAX_WORD_LEN - 1);
                        ctx->words[ctx->word_count][MAX_WORD_LEN - 1] = '\0';
                        ctx->word_count++;
                    }
                    token = strtok(NULL, " ");
                }
                ctx->state = STATE_DONE;
            }
            break;
            
        case STATE_DONE:
            if (event == EVENT_RESET) {
                ctx->state = STATE_IDLE;
                memset(ctx->input, 0, sizeof(ctx->input));
                memset(ctx->words, 0, sizeof(ctx->words));
                ctx->word_count = 0;
                ctx->char_count = 0;
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                ctx->state = STATE_IDLE;
                memset(ctx->input, 0, sizeof(ctx->input));
                memset(ctx->words, 0, sizeof(ctx->words));
                ctx->word_count = 0;
                ctx->char_count = 0;
            }
            break;
    }
}

int main(void) {
    context_t ctx;
    char user_input[MAX_INPUT * 2];
    int running = 1;
    
    while (running) {
        printf("Enter text to process (max %d chars): ", MAX_INPUT - 1);
        if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            return 1;
        }
        
        size_t len = strlen(user_input);
        if (len > 0 && user_input[len - 1] == '\n') {
            user_input[len - 1] = '\0';
        }
        
        if (strlen(user_input) >= MAX_INPUT) {
            fprintf(stderr, "Input too long\n");
            return 1;
        }
        
        ctx.state = STATE_IDLE;
        memset(ctx.input, 0, sizeof(ctx.input));
        memset(ctx.words, 0, sizeof(ctx.words));
        ctx.word_count = 0;
        ctx.char_count = 0;
        
        strncpy(ctx.input, user_input, MAX_INPUT - 1);
        ctx.input[MAX_INPUT - 1] = '\0';
        
        for (int i = 0; i <= (int)strlen(ctx.input); i++) {
            int event = get_next_event(ctx.input, i);
            state_machine_step(&ctx, event);
            
            if (ctx.state == STATE_ERROR) {
                printf("Error: Invalid character encountered\n");
                break;
            }
        }
        
        if (ctx.state == STATE_DONE) {
            printf("Processing complete. Found %d words:\n", ctx.word_count);
            for (int i = 0; i < ctx.word_count; i++) {
                printf("%d: '%s'\n", i + 1, ctx.words[i]);
            }
        }
        
        printf("Enter 'reset' to process another text or 'quit' to exit: ");
        if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
            break;
        }
        
        len = strlen(user_input);
        if (len > 0 && user_input[len -