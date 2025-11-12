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
    ERROR,
    FINISHED
} State;

int is_valid_char(char c) {
    return isalnum(c) || c == ' ' || c == '\n' || c == '\t';
}

void process_input(const char *input, int *numbers, char **words, int *num_count, int *word_count) {
    State current_state = START;
    const char *ptr = input;
    char buffer[MAX_INPUT];
    char *buf_ptr = buffer;
    
    *num_count = 0;
    *word_count = 0;
    
    while (*ptr != '\0' && current_state != ERROR && current_state != FINISHED) {
        if (!is_valid_char(*ptr)) {
            current_state = ERROR;
            break;
        }
        
        switch (current_state) {
            case START:
                if (isdigit(*ptr)) {
                    current_state = READING_NUMBER;
                    buf_ptr = buffer;
                    *(buf_ptr++) = *ptr;
                } else if (isalpha(*ptr)) {
                    current_state = READING_WORD;
                    buf_ptr = buffer;
                    *(buf_ptr++) = *ptr;
                } else {
                    ptr++;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(*ptr)) {
                    if ((buf_ptr - buffer) < MAX_INPUT - 1) {
                        *(buf_ptr++) = *ptr;
                    }
                    ptr++;
                } else {
                    *buf_ptr = '\0';
                    if (*num_count < MAX_INPUT) {
                        numbers[(*num_count)++] = atoi(buffer);
                    }
                    current_state = START;
                }
                break;
                
            case READING_WORD:
                if (isalpha(*ptr)) {
                    if ((buf_ptr - buffer) < MAX_INPUT - 1) {
                        *(buf_ptr++) = *ptr;
                    }
                    ptr++;
                } else {
                    *buf_ptr = '\0';
                    if (*word_count < MAX_INPUT) {
                        words[(*word_count)++] = strdup(buffer);
                    }
                    current_state = START;
                }
                break;
                
            default:
                break;
        }
    }
    
    if (current_state == READING_NUMBER) {
        *buf_ptr = '\0';
        if (*num_count < MAX_INPUT) {
            numbers[(*num_count)++] = atoi(buffer);
        }
    } else if (current_state == READING_WORD) {
        *buf_ptr = '\0';
        if (*word_count < MAX_INPUT) {
            words[(*word_count)++] = strdup(buffer);
        }
    }
}

int main(void) {
    char input[MAX_INPUT * 2];
    int numbers[MAX_INPUT];
    char *words[MAX_INPUT];
    int num_count = 0;
    int word_count = 0;
    
    printf("Enter text with numbers and words: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || len >= sizeof(input)) {
        fprintf(stderr, "Invalid input length\n");
        return 1;
    }
    
    process_input(input, numbers, words, &num_count, &word_count);
    
    printf("\nExtracted numbers: ");
    for (int i = 0; i < num_count; i++) {
        printf("%d ", numbers[i]);
    }
    
    printf("\nExtracted words: ");
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i]);
        free(words[i]);
    }
    printf("\n");
    
    return 0;
}