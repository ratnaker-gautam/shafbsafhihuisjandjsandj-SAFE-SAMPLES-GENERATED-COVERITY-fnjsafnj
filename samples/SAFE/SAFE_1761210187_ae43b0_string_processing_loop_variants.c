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
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
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
        
        strncpy(words[word_count], input + start, word_len);
        words[word_count][word_len] = '\0';
        word_count++;
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
        int word_idx = 0;
        while (words[idx][word_idx] != '\0') {
            total_chars++;
            word_idx++;
        }
        idx++;
    }
    
    printf("Total characters in all words: %d\n", total_chars);
    
    char search[MAX_INPUT];
    printf("Enter a word to search for: ");
    if (fgets(search, sizeof(search), stdin) == NULL) {
        fprintf(stderr, "Error reading search term\n");
        return 1;
    }
    
    len = strlen(search);
    if (len > 0 && search[len-1] == '\n') {
        search[len-1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty search term\n");
        return 0;
    }
    
    int found = 0;
    for (int j = 0; j < word_count; j++) {
        if (strcmp(words[j], search) == 0) {
            found = 1;
            break;
        }
    }
    
    if (found) {
        printf("Word '%s' was found in the sentence\n", search);
    } else {
        printf("Word '%s' was not found in the sentence\n", search);
    }
    
    return 0;
}