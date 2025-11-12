//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_PUNCTUATION,
    STATE_END
} State;

typedef enum {
    EVENT_SPACE,
    EVENT_DIGIT,
    EVENT_ALPHA,
    EVENT_PUNCT,
    EVENT_END,
    EVENT_INVALID
} Event;

Event get_event(char c) {
    if (c == '\0') return EVENT_END;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_ALPHA;
    if (ispunct(c)) return EVENT_PUNCT;
    return EVENT_INVALID;
}

void process_token(char *start, char *end, int *word_count, int *number_count) {
    if (start >= end) return;
    
    int length = end - start;
    if (length <= 0) return;
    
    if (isdigit(*start)) {
        (*number_count)++;
        printf("Number: ");
    } else if (isalpha(*start)) {
        (*word_count)++;
        printf("Word: ");
    } else {
        printf("Punctuation: ");
    }
    
    for (char *p = start; p < end && p < start + MAX_INPUT; p++) {
        putchar(*p);
    }
    putchar('\n');
}

int main(void) {
    char input[MAX_INPUT + 1] = {0};
    char *buffer = input;
    
    printf("Enter text (max %d chars): ", MAX_INPUT);
    if (fgets(buffer, MAX_INPUT, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    State current_state = STATE_START;
    char *token_start = buffer;
    char *current_pos = buffer;
    int word_count = 0;
    int number_count = 0;
    
    while (current_state != STATE_END) {
        Event event = get_event(*current_pos);
        
        switch (current_state) {
            case STATE_START:
                switch (event) {
                    case EVENT_ALPHA:
                        current_state = STATE_READING_WORD;
                        token_start = current_pos;
                        break;
                    case EVENT_DIGIT:
                        current_state = STATE_READING_NUMBER;
                        token_start = current_pos;
                        break;
                    case EVENT_PUNCT:
                        current_state = STATE_PUNCTUATION;
                        token_start = current_pos;
                        break;
                    case EVENT_SPACE:
                        break;
                    case EVENT_END:
                        current_state = STATE_END;
                        break;
                    case EVENT_INVALID:
                        current_state = STATE_END;
                        break;
                }
                break;
                
            case STATE_READING_WORD:
                switch (event) {
                    case EVENT_SPACE:
                    case EVENT_PUNCT:
                    case EVENT_END:
                        process_token(token_start, current_pos, &word_count, &number_count);
                        current_state = (event == EVENT_END) ? STATE_END : STATE_START;
                        break;
                    case EVENT_DIGIT:
                    case EVENT_INVALID:
                        current_state = STATE_END;
                        break;
                    case EVENT_ALPHA:
                        break;
                }
                break;
                
            case STATE_READING_NUMBER:
                switch (event) {
                    case EVENT_SPACE:
                    case EVENT_PUNCT:
                    case EVENT_END:
                        process_token(token_start, current_pos, &word_count, &number_count);
                        current_state = (event == EVENT_END) ? STATE_END : STATE_START;
                        break;
                    case EVENT_ALPHA:
                    case EVENT_INVALID:
                        current_state = STATE_END;
                        break;
                    case EVENT_DIGIT:
                        break;
                }
                break;
                
            case STATE_PUNCTUATION:
                switch (event) {
                    case EVENT_SPACE:
                    case EVENT_ALPHA:
                    case EVENT_DIGIT:
                    case EVENT_END:
                        process_token(token_start, current_pos, &word_count, &number_count);
                        current_state = (event == EVENT_END) ? STATE_END : STATE_START;
                        break;
                    case EVENT_PUNCT:
                        break;
                    case EVENT_INVALID:
                        current_state = STATE_END;
                        break;
                }
                break;
                
            case STATE_END:
                break;
        }
        
        if (current_state != STATE_END) {
            current_pos++;
            if (current_pos - buffer >= MAX_INPUT) {
                current_state = STATE_END;
            }
        }
    }
    
    printf("\nSummary: %d words, %d numbers\n", word_count, number_count);
    
    return 0;
}