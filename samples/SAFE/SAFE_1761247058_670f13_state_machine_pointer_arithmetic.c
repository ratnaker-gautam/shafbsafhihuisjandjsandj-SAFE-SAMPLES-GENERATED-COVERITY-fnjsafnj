//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define STATE_COUNT 5

typedef enum {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING,
    FINISHED
} State;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || isspace((unsigned char)c);
}

int main(void) {
    char input[MAX_INPUT + 1];
    char *ptr = input;
    State current_state = START;
    int number_count = 0;
    int word_count = 0;
    
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
    
    char *end = ptr + len;
    
    while (current_state != FINISHED && ptr < end) {
        switch (current_state) {
            case START:
                if (isdigit((unsigned char)*ptr)) {
                    current_state = READING_NUMBER;
                    number_count++;
                } else if (isalpha((unsigned char)*ptr)) {
                    current_state = READING_WORD;
                    word_count++;
                } else if (isspace((unsigned char)*ptr)) {
                    current_state = SKIPPING;
                } else {
                    fprintf(stderr, "Invalid character: %c\n", *ptr);
                    return 1;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit((unsigned char)*ptr)) {
                    ptr++;
                } else if (isspace((unsigned char)*ptr)) {
                    current_state = SKIPPING;
                } else if (isalpha((unsigned char)*ptr)) {
                    current_state = READING_WORD;
                    word_count++;
                } else {
                    fprintf(stderr, "Invalid character in number: %c\n", *ptr);
                    return 1;
                }
                break;
                
            case READING_WORD:
                if (isalpha((unsigned char)*ptr)) {
                    ptr++;
                } else if (isspace((unsigned char)*ptr)) {
                    current_state = SKIPPING;
                } else if (isdigit((unsigned char)*ptr)) {
                    current_state = READING_NUMBER;
                    number_count++;
                } else {
                    fprintf(stderr, "Invalid character in word: %c\n", *ptr);
                    return 1;
                }
                break;
                
            case SKIPPING:
                if (isspace((unsigned char)*ptr)) {
                    ptr++;
                } else if (isdigit((unsigned char)*ptr)) {
                    current_state = READING_NUMBER;
                    number_count++;
                } else if (isalpha((unsigned char)*ptr)) {
                    current_state = READING_WORD;
                    word_count++;
                } else {
                    fprintf(stderr, "Invalid character: %c\n", *ptr);
                    return 1;
                }
                break;
                
            case FINISHED:
                break;
        }
        
        if (ptr >= end) {
            current_state = FINISHED;
        }
    }
    
    printf("Numbers found: %d\n", number_count);
    printf("Words found: %d\n", word_count);
    
    return 0;
}