//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

enum State { START, READING_NUMBER, READING_WORD, ERROR, DONE };

int main(void) {
    enum State current_state = START;
    char input[256];
    int number_sum = 0;
    int word_count = 0;
    int pos = 0;
    char ch;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    while (current_state != DONE && current_state != ERROR) {
        ch = input[pos];
        
        switch (current_state) {
            case START:
                if (ch == '\0') {
                    current_state = DONE;
                } else if (isdigit(ch)) {
                    current_state = READING_NUMBER;
                } else if (isalpha(ch)) {
                    current_state = READING_WORD;
                } else if (isspace(ch)) {
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                {
                    int number = 0;
                    while (isdigit(ch)) {
                        if (number > (INT_MAX - (ch - '0')) / 10) {
                            current_state = ERROR;
                            break;
                        }
                        number = number * 10 + (ch - '0');
                        pos++;
                        ch = input[pos];
                    }
                    if (current_state != ERROR) {
                        if (number_sum > INT_MAX - number) {
                            current_state = ERROR;
                        } else {
                            number_sum += number;
                            current_state = START;
                        }
                    }
                }
                break;
                
            case READING_WORD:
                while (isalpha(ch)) {
                    pos++;
                    ch = input[pos];
                }
                word_count++;
                current_state = START;
                break;
                
            case ERROR:
                fprintf(stderr, "Invalid input format\n");
                break;
                
            case DONE:
                break;
        }
    }
    
    if (current_state == ERROR) {
        return 1;
    }
    
    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}