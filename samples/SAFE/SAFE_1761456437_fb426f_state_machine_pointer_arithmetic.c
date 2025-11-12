//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_DONE 3

int is_valid_input(const char *str) {
    if (!str || *str == '\0') return 0;
    const char *p = str;
    while (*p) {
        if (!isalnum(*p) && *p != ' ') return 0;
        p++;
    }
    return 1;
}

void process_word(char *start, char *end) {
    if (start >= end) return;
    
    char *left = start;
    char *right = end - 1;
    
    while (left < right) {
        char temp = *left;
        *left = *right;
        *right = temp;
        left++;
        right--;
    }
}

int main(void) {
    char input[MAX_INPUT + 1];
    int state = STATE_IDLE;
    char *buffer = input;
    char *word_start = NULL;
    char *current = NULL;
    
    printf("Enter text (alphanumeric and spaces only): ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 0;
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input characters\n");
        return 1;
    }
    
    current = buffer;
    
    while (state != STATE_DONE) {
        switch (state) {
            case STATE_IDLE:
                if (*current == '\0') {
                    state = STATE_DONE;
                } else if (!isspace(*current)) {
                    word_start = current;
                    state = STATE_READING;
                } else {
                    current++;
                }
                break;
                
            case STATE_READING:
                if (*current == '\0' || isspace(*current)) {
                    state = STATE_PROCESSING;
                } else {
                    current++;
                }
                break;
                
            case STATE_PROCESSING:
                process_word(word_start, current);
                state = STATE_IDLE;
                if (*current != '\0') {
                    current++;
                }
                break;
                
            case STATE_DONE:
                break;
        }
    }
    
    printf("Processed: %s\n", input);
    return 0;
}