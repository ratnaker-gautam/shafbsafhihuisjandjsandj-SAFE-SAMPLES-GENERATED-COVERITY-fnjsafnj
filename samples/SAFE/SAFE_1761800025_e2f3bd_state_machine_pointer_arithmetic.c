//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define STATE_COUNT 5

typedef enum {
    START,
    READING_DIGITS,
    READING_LETTERS,
    ERROR,
    DONE
} State;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\n';
}

void process_input(const char *input, size_t len) {
    State current_state = START;
    const char *ptr = input;
    const char *end = input + len;
    
    while (ptr < end && current_state != ERROR && current_state != DONE) {
        char c = *ptr;
        
        if (!is_valid_char(c)) {
            current_state = ERROR;
            break;
        }
        
        switch (current_state) {
            case START:
                if (isdigit((unsigned char)c)) {
                    current_state = READING_DIGITS;
                    printf("Digits: %c", c);
                } else if (isalpha((unsigned char)c)) {
                    current_state = READING_LETTERS;
                    printf("Letters: %c", c);
                } else if (c == ' ') {
                    ptr++;
                    continue;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_DIGITS:
                if (isdigit((unsigned char)c)) {
                    printf("%c", c);
                } else if (isalpha((unsigned char)c)) {
                    current_state = READING_LETTERS;
                    printf("\nLetters: %c", c);
                } else if (c == ' ') {
                    printf(" ");
                } else if (c == '\n') {
                    current_state = DONE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_LETTERS:
                if (isalpha((unsigned char)c)) {
                    printf("%c", c);
                } else if (isdigit((unsigned char)c)) {
                    current_state = READING_DIGITS;
                    printf("\nDigits: %c", c);
                } else if (c == ' ') {
                    printf(" ");
                } else if (c == '\n') {
                    current_state = DONE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            default:
                current_state = ERROR;
                break;
        }
        
        ptr++;
    }
    
    if (current_state == ERROR) {
        printf("\nError: Invalid input character\n");
    } else if (current_state != DONE) {
        printf("\n");
    }
}

int main(void) {
    char input[MAX_INPUT + 1];
    size_t bytes_read;
    
    printf("Enter text (max %d chars): ", MAX_INPUT);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    bytes_read = strlen(input);
    if (bytes_read == 0 || (bytes_read == sizeof(input) - 1 && input[bytes_read - 1] != '\n')) {
        fprintf(stderr, "Input too long or invalid\n");
        return 1;
    }
    
    if (bytes_read > 0 && input[bytes_read - 1] == '\n') {
        input[bytes_read - 1] = '\0';
        bytes_read--;
    }
    
    if (bytes_read == 0) {
        printf("Empty input\n");
        return 0;
    }
    
    process_input(input, bytes_read);
    return 0;
}