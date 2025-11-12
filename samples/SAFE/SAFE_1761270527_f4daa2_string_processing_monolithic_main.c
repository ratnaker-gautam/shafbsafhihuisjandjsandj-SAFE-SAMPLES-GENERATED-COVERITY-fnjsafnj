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
    int word_lengths[MAX_WORDS];
    
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
        printf("Empty input\n");
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int in_word = 0;
    int current_word_len = 0;
    char current_word[MAX_WORD_LEN + 1];
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (isalpha((unsigned char)c) && current_word_len < MAX_WORD_LEN) {
            if (!in_word) {
                in_word = 1;
                current_word_len = 0;
            }
            current_word[current_word_len++] = tolower((unsigned char)c);
        } else {
            if (in_word && current_word_len > 0 && word_count < MAX_WORDS) {
                current_word[current_word_len] = '\0';
                strncpy(words[word_count], current_word, MAX_WORD_LEN);
                words[word_count][MAX_WORD_LEN] = '\0';
                word_lengths[word_count] = current_word_len;
                word_count++;
                char_count += current_word_len;
            }
            in_word = 0;
            current_word_len = 0;
        }
    }
    
    printf("\nText analysis results:\n");
    printf("Total characters: %zu\n", input_len);
    printf("Total words: %d\n", word_count);
    printf("Average word length: ");
    
    if (word_count > 0) {
        double avg = (double)char_count / word_count;
        printf("%.2f\n", avg);
    } else {
        printf("0.00\n");
    }
    
    printf("\nWord frequency analysis:\n");
    
    for (int i = 0; i < word_count; i++) {
        if (words[i][0] == '\0') continue;
        
        int count = 1;
        for (int j = i + 1; j < word_count; j++) {
            if (words[j][0] != '\0' && strcmp(words[i], words[j]) == 0) {
                count++;
                words[j][0] = '\0';
            }
        }
        
        printf("'%s': length %d, count %d\n", words[i], word_lengths[i], count);
    }
    
    printf("\nProcessed text (lowercase, words only):\n");
    for (int i = 0; i < word_count; i++) {
        if (words[i][0] != '\0') {
            printf("%s ", words[i]);
        }
    }
    printf("\n");
    
    return 0;
}