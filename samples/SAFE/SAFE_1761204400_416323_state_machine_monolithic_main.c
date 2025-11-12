//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUMBER, READING_WORD, ERROR, DONE };

int main(void) {
    enum State current_state = START;
    char input_buffer[1024];
    char number_buffer[64];
    char word_buffer[64];
    int num_index = 0;
    int word_index = 0;
    int input_index = 0;
    int number_count = 0;
    int word_count = 0;
    
    printf("Enter text (max 1023 chars): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    while (current_state != DONE && current_state != ERROR && input_index <= input_len) {
        char current_char = input_index < input_len ? input_buffer[input_index] : ' ';
        
        switch (current_state) {
            case START:
                if (isdigit(current_char)) {
                    if (num_index < sizeof(number_buffer) - 1) {
                        number_buffer[num_index++] = current_char;
                    }
                    current_state = READING_NUMBER;
                } else if (isalpha(current_char)) {
                    if (word_index < sizeof(word_buffer) - 1) {
                        word_buffer[word_index++] = tolower(current_char);
                    }
                    current_state = READING_WORD;
                } else if (current_char == ' ' || current_char == '\t' || current_char == '\0') {
                    current_state = START;
                } else if (current_char != '\0') {
                    current_state = ERROR;
                }
                input_index++;
                break;
                
            case READING_NUMBER:
                if (isdigit(current_char)) {
                    if (num_index < sizeof(number_buffer) - 1) {
                        number_buffer[num_index++] = current_char;
                    }
                    input_index++;
                } else if (current_char == ' ' || current_char == '\t' || current_char == '\0') {
                    if (num_index > 0) {
                        number_buffer[num_index] = '\0';
                        number_count++;
                        printf("Found number: %s\n", number_buffer);
                        num_index = 0;
                    }
                    current_state = START;
                    input_index++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(current_char)) {
                    if (word_index < sizeof(word_buffer) - 1) {
                        word_buffer[word_index++] = tolower(current_char);
                    }
                    input_index++;
                } else if (current_char == ' ' || current_char == '\t' || current_char == '\0') {
                    if (word_index > 0) {
                        word_buffer[word_index] = '\0';
                        word_count++;
                        printf("Found word: %s\n", word_buffer);
                        word_index = 0;
                    }
                    current_state = START;
                    input_index++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                fprintf(stderr, "Error: Invalid character at position %d\n", input_index);
                current_state = DONE;
                break;
                
            case DONE:
                break;
        }
    }
    
    if (current_state == READING_NUMBER && num_index > 0) {
        number_buffer[num_index] = '\0';
        number_count++;
        printf("Found number: %s\n", number_buffer);
    } else if (current_state == READING_WORD && word_index > 0) {
        word_buffer[word_index] = '\0';
        word_count++;
        printf("Found word: %s\n", word_buffer);
    }
    
    printf("\nSummary: %d numbers, %d words found\n", number_count, word_count);
    
    return current_state == ERROR ? 1 : 0;
}