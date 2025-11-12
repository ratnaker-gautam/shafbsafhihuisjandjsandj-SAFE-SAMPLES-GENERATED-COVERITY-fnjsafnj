//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_WRITING 3
#define STATE_ERROR 4

#define MAX_INPUT_LEN 100
#define MAX_WORDS 50
#define MAX_WORD_LEN 20

#define TRANSITION(new_state) do { current_state = new_state; } while(0)
#define IS_VALID_CHAR(c) ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ')

typedef struct {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count;
} WordBuffer;

int main(void) {
    int current_state = STATE_IDLE;
    char input[MAX_INPUT_LEN + 1];
    WordBuffer buffer;
    int i, j, word_idx;
    char c;
    
    memset(&buffer, 0, sizeof(buffer));
    
    printf("Enter text (letters and spaces only, max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        TRANSITION(STATE_ERROR);
        printf("Empty input\n");
        return 1;
    }
    
    if (len > MAX_INPUT_LEN) {
        TRANSITION(STATE_ERROR);
        printf("Input too long\n");
        return 1;
    }
    
    for (i = 0; i < len; i++) {
        if (!IS_VALID_CHAR(input[i])) {
            TRANSITION(STATE_ERROR);
            printf("Invalid character: %c\n", input[i]);
            return 1;
        }
    }
    
    TRANSITION(STATE_READING);
    
    word_idx = 0;
    j = 0;
    for (i = 0; i <= len; i++) {
        c = input[i];
        
        if (c == ' ' || c == '\0') {
            if (j > 0) {
                if (word_idx >= MAX_WORDS) {
                    TRANSITION(STATE_ERROR);
                    printf("Too many words\n");
                    return 1;
                }
                
                if (j >= MAX_WORD_LEN) {
                    TRANSITION(STATE_ERROR);
                    printf("Word too long\n");
                    return 1;
                }
                
                buffer.words[word_idx][j] = '\0';
                word_idx++;
                j = 0;
            }
        } else {
            if (j >= MAX_WORD_LEN - 1) {
                TRANSITION(STATE_ERROR);
                printf("Word too long\n");
                return 1;
            }
            
            buffer.words[word_idx][j] = c;
            j++;
        }
    }
    
    buffer.word_count = word_idx;
    
    if (buffer.word_count == 0) {
        TRANSITION(STATE_ERROR);
        printf("No valid words found\n");
        return 1;
    }
    
    TRANSITION(STATE_PROCESSING);
    
    for (i = 0; i < buffer.word_count; i++) {
        for (j = 0; buffer.words[i][j] != '\0'; j++) {
            if (buffer.words[i][j] >= 'a' && buffer.words[i][j] <= 'z') {
                buffer.words[i][j] = buffer.words[i][j] - 'a' + 'A';
            }
        }
    }
    
    TRANSITION(STATE_WRITING);
    
    printf("Processed words (%d total):\n", buffer.word_count);
    for (i = 0; i < buffer.word_count; i++) {
        printf("%d: %s\n", i + 1, buffer.words[i]);
    }
    
    printf("State machine completed successfully\n");
    return 0;
}