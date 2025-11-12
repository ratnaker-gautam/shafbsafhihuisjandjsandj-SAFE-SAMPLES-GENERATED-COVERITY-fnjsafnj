//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 256
#define MAX_WORDS 50

int main() {
    char input[MAX_INPUT];
    char words[MAX_WORDS][MAX_INPUT];
    int word_count = 0;
    
    printf("Enter a sentence: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 0;
    }
    
    int i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace(input[i])) {
            i++;
        }
        
        if (i >= len) break;
        
        int start = i;
        while (i < len && !isspace(input[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len >= MAX_INPUT) {
            word_len = MAX_INPUT - 1;
        }
        
        if (word_len > 0) {
            strncpy(words[word_count], input + start, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    printf("Words found: %d\n", word_count);
    
    for (int j = 0; j < word_count; j++) {
        char temp[MAX_INPUT];
        strncpy(temp, words[j], sizeof(temp) - 1);
        temp[sizeof(temp) - 1] = '\0';
        
        int k = 0;
        do {
            if (isalpha(temp[k])) {
                temp[k] = toupper(temp[k]);
            }
            k++;
        } while (temp[k] != '\0');
        
        printf("Word %d: %s -> %s\n", j + 1, words[j], temp);
    }
    
    int total_chars = 0;
    int idx = 0;
    while (idx < word_count) {
        total_chars += strlen(words[idx]);
        idx++;
    }
    
    printf("Total characters in words: %d\n", total_chars);
    
    char longest[MAX_INPUT] = "";
    char shortest[MAX_INPUT] = "";
    
    if (word_count > 0) {
        strncpy(longest, words[0], sizeof(longest) - 1);
        longest[sizeof(longest) - 1] = '\0';
        strncpy(shortest, words[0], sizeof(shortest) - 1);
        shortest[sizeof(shortest) - 1] = '\0';
        
        for (int m = 1; m < word_count; m++) {
            if (strlen(words[m]) > strlen(longest)) {
                strncpy(longest, words[m], sizeof(longest) - 1);
                longest[sizeof(longest) - 1] = '\0';
            }
            if (strlen(words[m]) < strlen(shortest)) {
                strncpy(shortest, words[m], sizeof(shortest) - 1);
                shortest[sizeof(shortest) - 1] = '\0';
            }
        }
        
        printf("Longest word: %s (%zu chars)\n", longest, strlen(longest));
        printf("Shortest word: %s (%zu chars)\n", shortest, strlen(shortest));
    }
    
    return 0;
}