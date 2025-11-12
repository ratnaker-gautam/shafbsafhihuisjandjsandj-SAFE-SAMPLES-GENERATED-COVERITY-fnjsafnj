//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_DONE
};

int main(void) {
    char buffer[256];
    int numbers[50];
    char words[50][50];
    int num_count = 0;
    int word_count = 0;
    enum State current_state = STATE_START;
    int buffer_pos = 0;
    int ch;
    
    printf("Enter text (max 255 chars, end with newline): ");
    
    while ((ch = getchar()) != '\n' && ch != EOF) {
        if (buffer_pos >= 255) {
            current_state = STATE_ERROR;
            break;
        }
        
        buffer[buffer_pos++] = (char)ch;
        
        switch (current_state) {
            case STATE_START:
                if (isdigit(ch)) {
                    current_state = STATE_READING_NUMBER;
                    numbers[num_count] = ch - '0';
                } else if (isalpha(ch)) {
                    current_state = STATE_READING_WORD;
                    words[word_count][0] = (char)ch;
                    words[word_count][1] = '\0';
                } else if (!isspace(ch)) {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (isdigit(ch)) {
                    if (num_count < 50) {
                        numbers[num_count] = numbers[num_count] * 10 + (ch - '0');
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (isalpha(ch)) {
                    current_state = STATE_READING_WORD;
                    num_count++;
                    if (word_count < 50) {
                        words[word_count][0] = (char)ch;
                        words[word_count][1] = '\0';
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (isspace(ch)) {
                    current_state = STATE_START;
                    num_count++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (isalnum(ch)) {
                    size_t len = strlen(words[word_count]);
                    if (len < 49) {
                        words[word_count][len] = (char)ch;
                        words[word_count][len + 1] = '\0';
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (isdigit(ch)) {
                    current_state = STATE_READING_NUMBER;
                    word_count++;
                    if (num_count < 50) {
                        numbers[num_count] = ch - '0';
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (isspace(ch)) {
                    current_state = STATE_START;
                    word_count++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                break;
                
            case STATE_DONE:
                break;
        }
        
        if (current_state == STATE_ERROR) {
            break;
        }
    }
    
    buffer[buffer_pos] = '\0';
    
    if (current_state == STATE_READING_NUMBER) {
        num_count++;
    } else if (current_state == STATE_READING_WORD) {
        word_count++;
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Input processed successfully.\n");
    printf("Numbers found: ");
    for (int i = 0; i < num_count && i < 50; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
    
    printf("Words found: ");
    for (int i = 0; i < word_count && i < 50; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    return 0;
}