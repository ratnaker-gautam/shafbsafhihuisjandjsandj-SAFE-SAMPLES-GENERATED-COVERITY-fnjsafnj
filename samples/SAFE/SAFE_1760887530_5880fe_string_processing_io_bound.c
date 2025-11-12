//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    char temp_word[MAX_WORD_LEN + 1];
    int temp_len = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
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
        return 1;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && isalpha((unsigned char)input[i])) {
            if (temp_len < MAX_WORD_LEN) {
                temp_word[temp_len++] = tolower((unsigned char)input[i]);
            }
        } else {
            if (temp_len > 0) {
                if (temp_len <= MAX_WORD_LEN) {
                    temp_word[temp_len] = '\0';
                    
                    int duplicate = 0;
                    for (int j = 0; j < word_count; j++) {
                        if (strcmp(words[j], temp_word) == 0) {
                            duplicate = 1;
                            break;
                        }
                    }
                    
                    if (!duplicate && word_count < MAX_WORDS) {
                        strncpy(words[word_count], temp_word, MAX_WORD_LEN);
                        words[word_count][MAX_WORD_LEN] = '\0';
                        word_count++;
                    }
                }
                temp_len = 0;
            }
        }
    }
    
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
            if (strcmp(words[j], words[j + 1]) > 0) {
                char temp[MAX_WORD_LEN + 1];
                strncpy(temp, words[j], MAX_WORD_LEN);
                temp[MAX_WORD_LEN] = '\0';
                strncpy(words[j], words[j + 1], MAX_WORD_LEN);
                words[j][MAX_WORD_LEN] = '\0';
                strncpy(words[j + 1], temp, MAX_WORD_LEN);
                words[j + 1][MAX_WORD_LEN] = '\0';
            }
        }
    }
    
    printf("Unique words in alphabetical order:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    
    printf("Total unique words: %d\n", word_count);
    
    return 0;
}