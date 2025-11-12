//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
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
                    
                    if (word_count < MAX_WORDS) {
                        strncpy(words[word_count], temp_word, MAX_WORD_LEN);
                        words[word_count][MAX_WORD_LEN] = '\0';
                        word_count++;
                    } else {
                        fprintf(stderr, "Too many words\n");
                        break;
                    }
                }
                temp_len = 0;
            }
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    printf("\nProcessing %d words:\n", word_count);
    
    int total_chars = 0;
    int longest_len = 0;
    int shortest_len = MAX_WORD_LEN;
    
    for (int i = 0; i < word_count; i++) {
        int len = strlen(words[i]);
        total_chars += len;
        
        if (len > longest_len) {
            longest_len = len;
        }
        if (len < shortest_len) {
            shortest_len = len;
        }
        
        printf("Word %d: %s (length: %d)\n", i + 1, words[i], len);
    }
    
    printf("\nStatistics:\n");
    printf("Total words: %d\n", word_count);
    printf("Total characters: %d\n", total_chars);
    printf("Average word length: %.2f\n", word_count > 0 ? (double)total_chars / word_count : 0.0);
    printf("Longest word length: %d\n", longest_len);
    printf("Shortest word length: %d\n", shortest_len);
    
    printf("\nWords sorted alphabetically:\n");
    
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) > 0) {
                char temp[MAX_WORD_LEN + 1];
                strncpy(temp, words[i], MAX_WORD_LEN);
                temp[MAX_WORD_LEN] = '\0';
                strncpy(words[i], words[j], MAX_WORD_LEN);
                words[i][MAX_WORD_LEN] = '\0';
                strncpy(words[j], temp, MAX_WORD_LEN);
                words[j][MAX_WORD_LEN] = '\0';
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