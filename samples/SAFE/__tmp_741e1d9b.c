//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <errno.h>

#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_DONE 3
#define STATE_ERROR 4

#define MAX_INPUT_LEN 100
#define MAX_TOKENS 10

#define TRANSITION(from, to) ((from) * 10 + (to))
#define VALID_TRANSITION(from, to) \
    ((from) == STATE_IDLE && (to) == STATE_READING) || \
    ((from) == STATE_READING && (to) == STATE_PROCESSING) || \
    ((from) == STATE_PROCESSING && (to) == STATE_DONE) || \
    ((from) == STATE_READING && (to) == STATE_ERROR) || \
    ((from) == STATE_PROCESSING && (to) == STATE_ERROR) || \
    ((from) == STATE_ERROR && (to) == STATE_IDLE) || \
    ((from) == STATE_DONE && (to) == STATE_IDLE)

#define IS_DIGIT(c) ((c) >= '0' && (c) <= '9')
#define IS_SPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    return 1;
}

int process_tokens(char* input, int* tokens, size_t* count) {
    if (input == NULL || tokens == NULL || count == NULL) return 0;
    
    char* ptr = input;
    *count = 0;
    
    while (*ptr != '\0' && *count < MAX_TOKENS) {
        while (IS_SPACE(*ptr)) ptr++;
        
        if (*ptr == '\0') break;
        
        if (IS_DIGIT(*ptr) || *ptr == '-') {
            char* end;
            errno = 0;
            long val = strtol(ptr, &end, 10);
            if (ptr == end) return 0;
            if (errno == ERANGE) return 0;
            if (val < -1000 || val > 1000) return 0;
            tokens[*count] = (int)val;
            (*count)++;
            ptr = end;
        } else {
            return 0;
        }
    }
    
    return 1;
}

int main(void) {
    int state = STATE_IDLE;
    char input[MAX_INPUT_LEN + 2];
    int tokens[MAX_TOKENS];
    size_t token_count = 0;
    int sum = 0;
    
    printf("Enter numbers separated by spaces (max %d numbers): ", MAX_TOKENS);
    
    while (state != STATE_DONE) {
        switch (state) {
            case STATE_IDLE:
                if (fgets(input, sizeof(input), stdin) != NULL) {
                    size_t len = strlen(input);
                    if (len > 0 && input[len-1] == '\n') {
                        input[len-1] = '\0';
                    }
                    if (validate_input(input)) {
                        state = STATE_READING;
                    } else {
                        state = STATE_ERROR;
                    }
                } else {
                    state = STATE_ERROR;
                }
                break;
                
            case STATE_READING:
                if (process_tokens(input, tokens, &token_count)) {
                    state = STATE_PROCESSING;
                } else {
                    state = STATE_ERROR;
                }
                break;
                
            case STATE_PROCESSING:
                sum = 0;
                for (size_t i = 0; i < token_count; i++) {
                    if (sum > 0 && tokens[i] > INT_MAX - sum) {
                        state = STATE_ERROR;
                        break;
                    }
                    if (sum < 0 && tokens[i] < INT_MIN - sum) {
                        state = STATE_ERROR;
                        break;
                    }
                    sum += tokens[i];
                }
                if (state != STATE_ERROR) {
                    state = STATE_DONE;
                }
                break;
                
            case STATE_DONE:
                printf("Sum: %d\n", sum);
                printf("Processed %zu numbers\n", token_count);
                state = STATE_IDLE;
                break;
                
            case STATE_ERROR:
                printf("Error processing input. Try again.\n");
                state = STATE_IDLE;
                break;
        }
        
        if (!VALID_TRANSITION(state, state)) {
            state = STATE_ERROR;
        }
    }
    
    return 0;
}