//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_END
} State;

typedef enum {
    EVENT_DIGIT,
    EVENT_LETTER,
    EVENT_SPACE,
    EVENT_EOF,
    EVENT_INVALID
} Event;

Event get_event(char c) {
    if (c == '\0' || c == '\n') return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    int numbers[MAX_STATES];
    char words[MAX_STATES][MAX_INPUT_LEN];
    int num_count = 0;
    int word_count = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    State current_state = STATE_START;
    char buffer[MAX_INPUT_LEN + 1];
    int buf_pos = 0;
    
    for (int i = 0; input[i] != '\0'; i++) {
        Event ev = get_event(input[i]);
        
        switch (current_state) {
            case STATE_START:
                if (ev == EVENT_DIGIT) {
                    buffer[buf_pos++] = input[i];
                    current_state = STATE_READING_NUMBER;
                } else if (ev == EVENT_LETTER) {
                    buffer[buf_pos++] = input[i];
                    current_state = STATE_READING_WORD;
                } else if (ev != EVENT_SPACE && ev != EVENT_EOF) {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (ev == EVENT_DIGIT) {
                    if (buf_pos < MAX_INPUT_LEN) {
                        buffer[buf_pos++] = input[i];
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (ev == EVENT_SPACE || ev == EVENT_EOF) {
                    buffer[buf_pos] = '\0';
                    if (num_count < MAX_STATES) {
                        numbers[num_count++] = atoi(buffer);
                    }
                    buf_pos = 0;
                    current_state = STATE_START;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (ev == EVENT_LETTER) {
                    if (buf_pos < MAX_INPUT_LEN) {
                        buffer[buf_pos++] = input[i];
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (ev == EVENT_SPACE || ev == EVENT_EOF) {
                    buffer[buf_pos] = '\0';
                    if (word_count < MAX_STATES) {
                        strncpy(words[word_count], buffer, MAX_INPUT_LEN - 1);
                        words[word_count][MAX_INPUT_LEN - 1] = '\0';
                        word_count++;
                    }
                    buf_pos = 0;
                    current_state = STATE_START;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                break;
                
            case STATE_END:
                break;
        }
        
        if (current_state == STATE_ERROR) {
            break;
        }
    }
    
    if (current_state == STATE_READING_NUMBER || current_state == STATE_READING_WORD) {
        buffer[buf_pos] = '\0';
        if (current_state == STATE_READING_NUMBER && num_count < MAX_STATES) {
            numbers[num_count++] = atoi(buffer);
        } else if (current_state == STATE_READING_WORD && word_count < MAX_STATES) {
            strncpy(words[word_count], buffer, MAX_INPUT_LEN - 1);
            words[word_count][MAX_INPUT_LEN - 1] = '\0';
            word_count++;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Numbers found: ");
    for (int i = 0; i < num_count; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
    
    printf("Words found: ");
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    return 0;
}