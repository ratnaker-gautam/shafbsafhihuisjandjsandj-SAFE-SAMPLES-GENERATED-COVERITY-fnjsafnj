//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int char_count = 0;
    int line_count = 0;
    
    printf("Enter text (max %d characters):\n", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char_count = input_len;
    line_count = 1;
    
    int in_word = 0;
    int word_start = 0;
    int current_word_len = 0;
    
    for (int i = 0; i <= input_len; i++) {
        if (i < input_len && !isspace((unsigned char)input[i])) {
            if (!in_word) {
                in_word = 1;
                word_start = i;
                current_word_len = 0;
            }
            current_word_len++;
        } else {
            if (in_word) {
                if (word_count < MAX_WORDS && current_word_len <= MAX_WORD_LEN) {
                    strncpy(words[word_count], &input[word_start], current_word_len);
                    words[word_count][current_word_len] = '\0';
                    word_count++;
                }
                in_word = 0;
            }
        }
    }
    
    printf("\nText analysis results:\n");
    printf("Characters: %d\n", char_count);
    printf("Lines: %d\n", line_count);
    printf("Words: %d\n", word_count);
    
    if (word_count > 0) {
        printf("\nWords found:\n");
        for (int i = 0; i < word_count; i++) {
            if (i < 10) {
                printf("%d: %s\n", i + 1, words[i]);
            } else {
                printf("... and %d more words\n", word_count - 10);
                break;
            }
        }
    }
    
    char reversed[MAX_INPUT_LEN + 1];
    int rev_index = 0;
    for (int i = input_len - 1; i >= 0; i--) {
        if (rev_index < MAX_INPUT_LEN) {
            reversed[rev_index++] = input[i];
        }
    }
    reversed[rev_index] = '\0';
    
    printf("\nReversed text: %s\n", reversed);
    
    int alpha_count = 0;
    int digit_count = 0;
    int space_count = 0;
    int punct_count = 0;
    
    for (int i = 0; i < input_len; i++) {
        unsigned char c = input[i];
        if (isalpha(c)) {
            alpha_count++;
        } else if (isdigit(c)) {
            digit_count++;
        } else if (isspace(c)) {
            space_count++;
        } else if (ispunct(c)) {
            punct_count++;
        }
    }
    
    printf("\nCharacter types:\n");
    printf("Letters: %d\n", alpha_count);
    printf("Digits: %d\n", digit_count);
    printf("Spaces: %d\n", space_count);
    printf("Punctuation: %d\n", punct_count);
    
    return 0;
}