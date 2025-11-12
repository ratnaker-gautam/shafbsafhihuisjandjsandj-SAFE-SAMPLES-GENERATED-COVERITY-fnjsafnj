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
    if (end <= start) return;
    char *p = start;
    while (p < end) {
        if (islower(*p)) *p = toupper(*p);
        p++;
    }
}

int main(void) {
    char input[MAX_INPUT + 1];
    int state = STATE_IDLE;
    char *buffer = input;
    char *current = NULL;
    char *word_start = NULL;
    
    printf("Enter text (alphanumeric and spaces only): ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    char *newline = strchr(input, '\n');
    if (newline) *newline = '\0';
    
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
                break;
                
            default:
                state = STATE_DONE;
                break;
        }
    }
    
    printf("Processed: %s\n", buffer);
    return 0;
}