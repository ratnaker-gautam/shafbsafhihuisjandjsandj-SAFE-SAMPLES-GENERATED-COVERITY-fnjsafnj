//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int main() {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][64];
    int word_count = 0;
    int i, j, k;
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (len >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        j = 0;
        while (i < len && !isspace((unsigned char)input[i]) && j < 63) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        words[word_count][j] = '\0';
        
        if (j > 0) {
            word_count++;
        }
    }
    
    if (word_count == 0) {
        fprintf(stderr, "No words found\n");
        return 1;
    }
    
    for (i = 0; i < word_count - 1; i++) {
        for (j = 0; j < word_count - i - 1; j++) {
            if (strcmp(words[j], words[j+1]) > 0) {
                char temp[64];
                if (strlen(words[j]) < 64 && strlen(words[j+1]) < 64) {
                    strcpy(temp, words[j]);
                    strcpy(words[j], words[j+1]);
                    strcpy(words[j+1], temp);
                }
            }
        }
    }
    
    printf("Sorted words:\n");
    for (i = 0; i < word_count; i++) {
        if (printf("%s\n", words[i]) < 0) {
            fprintf(stderr, "Error writing output\n");
            return 1;
        }
    }
    
    int unique_count = 0;
    printf("\nUnique words:\n");
    for (i = 0; i < word_count; i++) {
        int is_unique = 1;
        for (j = 0; j < i; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            if (printf("%s\n", words[i]) < 0) {
                fprintf(stderr, "Error writing output\n");
                return 1;
            }
            unique_count++;
        }
    }
    
    printf("\nStatistics:\n");
    printf("Total words: %d\n", word_count);
    printf("Unique words: %d\n", unique_count);
    
    int max_len = 0;
    int min_len = 1000;
    for (i = 0; i < word_count; i++) {
        int current_len = strlen(words[i]);
        if (current_len > max_len) {
            max_len = current_len;
        }
        if (current_len < min_len) {
            min_len = current_len;
        }
    }
    printf("Longest word length: %d\n", max_len);
    printf("Shortest word length: %d\n", min_len);
    
    return 0;
}