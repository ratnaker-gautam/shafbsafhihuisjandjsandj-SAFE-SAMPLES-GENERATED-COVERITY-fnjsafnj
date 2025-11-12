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
    int word_lengths[MAX_WORDS] = {0};
    
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
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int in_word = 0;
    int current_word_len = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && isalpha((unsigned char)input[i])) {
            if (!in_word) {
                in_word = 1;
                current_word_len = 0;
            }
            
            if (current_word_len < MAX_WORD_LEN && word_count < MAX_WORDS) {
                words[word_count][current_word_len] = tolower((unsigned char)input[i]);
                current_word_len++;
            }
        } else {
            if (in_word) {
                if (current_word_len > 0 && word_count < MAX_WORDS) {
                    words[word_count][current_word_len] = '\0';
                    word_lengths[word_count] = current_word_len;
                    word_count++;
                    char_count += current_word_len;
                }
                in_word = 0;
                current_word_len = 0;
            }
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    printf("\nProcessing results:\n");
    printf("Total words: %d\n", word_count);
    printf("Total characters in words: %d\n", char_count);
    printf("Average word length: %.2f\n", (float)char_count / word_count);
    
    printf("\nWords found:\n");
    for (int i = 0; i < word_count; i++) {
        if (i < 10) {
            printf("%d. %s (length: %d)\n", i + 1, words[i], word_lengths[i]);
        } else {
            printf("%d. %s (length: %d)\n", i + 1, words[i], word_lengths[i]);
        }
    }
    
    printf("\nWord length distribution:\n");
    int max_len = 0;
    for (int i = 0; i < word_count; i++) {
        if (word_lengths[i] > max_len) {
            max_len = word_lengths[i];
        }
    }
    
    for (int len = 1; len <= max_len; len++) {
        int count = 0;
        for (int i = 0; i < word_count; i++) {
            if (word_lengths[i] == len) {
                count++;
            }
        }
        if (count > 0) {
            printf("Length %d: %d words\n", len, count);
        }
    }
    
    printf("\nAlphabetically sorted words:\n");
    char temp[MAX_WORD_LEN + 1];
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) > 0) {
                strncpy(temp, words[i], sizeof(temp) - 1);
                temp[sizeof(temp) - 1] = '\0';
                strncpy(words[i], words[j], sizeof(words[i]) - 1);
                words[i][sizeof(words[i]) - 1] = '\0';
                strncpy(words[j], temp, sizeof(words[j]) - 1);
                words[j][sizeof(words[j]) - 1] = '\0';
            }
        }
    }
    
    for (int i = 0; i < word_count; i++) {
        printf("%s", words[i]);
        if (i < word_count - 1) {
            printf(", ");
        }
    }
    printf("\n");
    
    return 0;
}