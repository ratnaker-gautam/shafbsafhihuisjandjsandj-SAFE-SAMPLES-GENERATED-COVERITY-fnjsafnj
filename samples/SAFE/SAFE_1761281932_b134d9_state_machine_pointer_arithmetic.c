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

int is_valid_input(const char *input) {
    if (input == NULL) return 0;
    const char *p = input;
    while (*p != '\0') {
        if (!isalnum(*p) && *p != ' ') return 0;
        p++;
    }
    return 1;
}

void process_word(char *start, char *end) {
    if (start >= end) return;
    
    char *p = start;
    char *q = end - 1;
    
    while (p < q) {
        char temp = *p;
        *p = *q;
        *q = temp;
        p++;
        q--;
    }
}

int main(void) {
    char input[MAX_INPUT + 1];
    int state = STATE_IDLE;
    char buffer[MAX_INPUT + 1];
    char *buf_ptr = buffer;
    char *input_ptr = input;
    
    printf("Enter text (alphanumeric and spaces only): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return EXIT_FAILURE;
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input characters\n");
        return EXIT_FAILURE;
    }
    
    char *current = input;
    char *end = input + len;
    
    while (state != STATE_DONE) {
        switch (state) {
            case STATE_IDLE:
                if (current < end) {
                    state = STATE_READING;
                    buf_ptr = buffer;
                } else {
                    state = STATE_DONE;
                }
                break;
                
            case STATE_READING:
                if (current < end && *current != ' ') {
                    if (buf_ptr < buffer + MAX_INPUT) {
                        *buf_ptr = *current;
                        buf_ptr++;
                    }
                    current++;
                } else {
                    state = STATE_PROCESSING;
                }
                break;
                
            case STATE_PROCESSING:
                if (buf_ptr > buffer) {
                    *buf_ptr = '\0';
                    process_word(buffer, buf_ptr);
                    printf("%s", buffer);
                }
                
                if (current < end && *current == ' ') {
                    printf(" ");
                    current++;
                }
                
                state = STATE_IDLE;
                break;
                
            default:
                state = STATE_DONE;
                break;
        }
    }
    
    printf("\n");
    return EXIT_SUCCESS;
}