//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][64];
    int word_count = 0;
    char temp_word[64];
    int temp_len = 0;
    
    printf("Enter text to process: ");
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
            if (temp_len < 63) {
                temp_word[temp_len++] = tolower((unsigned char)input[i]);
            }
        } else {
            if (temp_len > 0) {
                temp_word[temp_len] = '\0';
                if (word_count < MAX_WORDS) {
                    strcpy(words[word_count], temp_word);
                    word_count++;
                }
                temp_len = 0;
            }
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
            if (strcmp(words[j], words[j + 1]) > 0) {
                char temp[64];
                strcpy(temp, words[j]);
                strcpy(words[j], words[j + 1]);
                strcpy(words[j + 1], temp);
            }
        }
    }
    
    printf("Sorted unique words:\n");
    for (int i = 0; i < word_count; i++) {
        if (i == 0 || strcmp(words[i], words[i - 1]) != 0) {
            printf("%s\n", words[i]);
        }
    }
    
    int char_count[26] = {0};
    for (int i = 0; i < word_count; i++) {
        for (size_t j = 0; j < strlen(words[i]); j++) {
            char c = words[i][j];
            if (c >= 'a' && c <= 'z') {
                char_count[c - 'a']++;
            }
        }
    }
    
    printf("\nCharacter frequency in words:\n");
    for (int i = 0; i < 26; i++) {
        if (char_count[i] > 0) {
            printf("%c: %d\n", 'a' + i, char_count[i]);
        }
    }
    
    int total_chars = 0;
    for (int i = 0; i < 26; i++) {
        total_chars += char_count[i];
    }
    
    if (total_chars > 0) {
        printf("\nTotal characters in words: %d\n", total_chars);
        printf("Average word length: %.2f\n", (float)total_chars / word_count);
    }
    
    return 0;
}