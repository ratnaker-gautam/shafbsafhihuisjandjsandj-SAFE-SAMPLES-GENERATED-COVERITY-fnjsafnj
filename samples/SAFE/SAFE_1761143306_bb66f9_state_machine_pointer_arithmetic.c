//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define STATE_START 0
#define STATE_WORD 1
#define STATE_NUMBER 2
#define STATE_END 3

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char *ptr = input;
    int state = STATE_START;
    int word_count = 0;
    int number_count = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && *(ptr + len - 1) == '\n') {
        *(ptr + len - 1) = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    while (state != STATE_END && ptr < input + len) {
        switch (state) {
            case STATE_START:
                if (isalpha(*ptr)) {
                    state = STATE_WORD;
                    word_count++;
                } else if (isdigit(*ptr)) {
                    state = STATE_NUMBER;
                    number_count++;
                }
                ptr++;
                break;
                
            case STATE_WORD:
                if (!isalpha(*ptr)) {
                    if (isdigit(*ptr)) {
                        state = STATE_NUMBER;
                        number_count++;
                    } else {
                        state = STATE_START;
                    }
                }
                ptr++;
                break;
                
            case STATE_NUMBER:
                if (!isdigit(*ptr)) {
                    if (isalpha(*ptr)) {
                        state = STATE_WORD;
                        word_count++;
                    } else {
                        state = STATE_START;
                    }
                }
                ptr++;
                break;
        }
    }
    
    printf("Words: %d\n", word_count);
    printf("Numbers: %d\n", number_count);
    
    return 0;
}