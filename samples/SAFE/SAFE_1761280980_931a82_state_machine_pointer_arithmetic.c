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
    char input[MAX_INPUT + 1] = {0};
    char *ptr = input;
    State current_state = START;
    int number_sum = 0;
    int word_count = 0;
    char current_word[MAX_INPUT] = {0};
    char *word_ptr = current_word;
    
    printf("Enter text (max %d chars): ", MAX_INPUT);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && *(ptr + input_len - 1) == '\n') {
        *(ptr + input_len - 1) = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty input\n");
        return 0;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        char c = *(ptr + i);
        
        if (!is_valid_char(c)) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
        
        switch (current_state) {
            case START:
                if (isdigit((unsigned char)c)) {
                    current_state = READING_NUMBER;
                    number_sum += (c - '0');
                } else if (isalpha((unsigned char)c)) {
                    current_state = READING_WORD;
                    *word_ptr = c;
                    word_ptr++;
                } else if (isspace((unsigned char)c)) {
                    current_state = SKIPPING;
                } else {
                    current_state = FINISHED;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit((unsigned char)c)) {
                    number_sum += (c - '0');
                } else if (isalpha((unsigned char)c)) {
                    current_state = READING_WORD;
                    word_ptr = current_word;
                    *word_ptr = c;
                    word_ptr++;
                } else if (isspace((unsigned char)c)) {
                    current_state = SKIPPING;
                } else {
                    current_state = FINISHED;
                }
                break;
                
            case READING_WORD:
                if (isalpha((unsigned char)c)) {
                    if ((size_t)(word_ptr - current_word) < sizeof(current_word) - 1) {
                        *word_ptr = c;
                        word_ptr++;
                    }
                } else if (isdigit((unsigned char)c)) {
                    current_state = READING_NUMBER;
                    word_count++;
                    word_ptr = current_word;
                    *word_ptr = '\0';
                    number_sum += (c - '0');
                } else if (isspace((unsigned char)c)) {
                    current_state = SKIPPING;
                    word_count++;
                    word_ptr = current_word;
                    *word_ptr = '\0';
                } else {
                    current_state = FINISHED;
                    word_count++;
                    word_ptr = current_word;
                    *word_ptr = '\0';
                }
                break;
                
            case SKIPPING:
                if (isdigit((unsigned char)c)) {
                    current_state = READING_NUMBER;
                    number_sum += (c - '0');
                } else if (isalpha((unsigned char)c)) {
                    current_state = READING_WORD;
                    word_ptr = current_word;
                    *word_ptr = c;
                    word_ptr++;
                } else if (!isspace((unsigned char)c)) {
                    current_state = FINISHED;
                }
                break;
                
            case FINISHED:
                break;
        }
        
        if (current_state == FINISHED) {
            break;
        }
    }
    
    if (current_state == READING_WORD) {
        word_count++;
        *word_ptr = '\0';
    }
    
    printf("Sum of digits: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}