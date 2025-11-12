//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define STATE_COUNT 5

typedef enum {
    START,
    READING_NUM,
    READING_WORD,
    SKIPPING,
    FINISHED
} State;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || isspace((unsigned char)c);
}

void process_input(char *input) {
    State current_state = START;
    char *ptr = input;
    char *end = input + strlen(input);
    
    while (current_state != FINISHED && ptr < end) {
        switch (current_state) {
            case START:
                if (isdigit((unsigned char)*ptr)) {
                    current_state = READING_NUM;
                    printf("Number: %c", *ptr);
                } else if (isalpha((unsigned char)*ptr)) {
                    current_state = READING_WORD;
                    printf("Word: %c", *ptr);
                } else if (isspace((unsigned char)*ptr)) {
                    current_state = SKIPPING;
                } else {
                    current_state = FINISHED;
                }
                ptr++;
                break;
                
            case READING_NUM:
                if (isdigit((unsigned char)*ptr)) {
                    putchar(*ptr);
                    ptr++;
                } else if (isspace((unsigned char)*ptr)) {
                    putchar('\n');
                    current_state = SKIPPING;
                    ptr++;
                } else {
                    putchar('\n');
                    current_state = FINISHED;
                }
                break;
                
            case READING_WORD:
                if (isalpha((unsigned char)*ptr)) {
                    putchar(*ptr);
                    ptr++;
                } else if (isspace((unsigned char)*ptr)) {
                    putchar('\n');
                    current_state = SKIPPING;
                    ptr++;
                } else {
                    putchar('\n');
                    current_state = FINISHED;
                }
                break;
                
            case SKIPPING:
                if (isspace((unsigned char)*ptr)) {
                    ptr++;
                } else if (isdigit((unsigned char)*ptr)) {
                    current_state = READING_NUM;
                    printf("Number: %c", *ptr);
                    ptr++;
                } else if (isalpha((unsigned char)*ptr)) {
                    current_state = READING_WORD;
                    printf("Word: %c", *ptr);
                    ptr++;
                } else {
                    current_state = FINISHED;
                }
                break;
                
            case FINISHED:
                break;
        }
    }
    
    if (current_state == READING_NUM || current_state == READING_WORD) {
        putchar('\n');
    }
}

int main(void) {
    char input[MAX_INPUT + 1] = {0};
    
    printf("Enter text (max %d chars): ", MAX_INPUT);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
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
    
    for (size_t i = 0; i < len; i++) {
        if (!is_valid_char(input[i])) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
    }
    
    process_input(input);
    return 0;
}