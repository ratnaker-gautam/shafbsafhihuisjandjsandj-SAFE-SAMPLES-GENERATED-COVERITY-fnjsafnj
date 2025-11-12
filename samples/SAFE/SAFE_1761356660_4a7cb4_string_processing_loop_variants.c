//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 1024
#define MAX_WORDS 512

int main() {
    char input[MAX_INPUT];
    char words[MAX_WORDS][64];
    int word_count = 0;
    
    printf("Enter text: ");
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
        printf("No input provided\n");
        return 1;
    }
    
    int i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) break;
        
        int start = i;
        while (i < len && !isspace((unsigned char)input[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len > 0 && word_len < 64) {
            strncpy(words[word_count], input + start, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    printf("Found %d words:\n", word_count);
    
    for (int j = 0; j < word_count; j++) {
        int k = 0;
        char processed[64];
        
        while (words[j][k] != '\0' && k < 63) {
            processed[k] = tolower((unsigned char)words[j][k]);
            k++;
        }
        processed[k] = '\0';
        
        printf("%s\n", processed);
    }
    
    int total_chars = 0;
    int idx = 0;
    do {
        if (idx < word_count) {
            total_chars += strlen(words[idx]);
        }
        idx++;
    } while (idx < word_count);
    
    printf("Total characters in words: %d\n", total_chars);
    
    char search[64];
    printf("Enter word to search: ");
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
        printf("No search term provided\n");
        return 1;
    }
    
    int found = 0;
    for (int j = 0; j < word_count; j++) {
        char temp[64];
        int k = 0;
        while (words[j][k] != '\0' && k < 63) {
            temp[k] = tolower((unsigned char)words[j][k]);
            k++;
        }
        temp[k] = '\0';
        
        if (strcmp(temp, search) == 0) {
            found = 1;
            break;
        }
    }
    
    if (found) {
        printf("Word '%s' found in text\n", search);
    } else {
        printf("Word '%s' not found in text\n", search);
    }
    
    return 0;
}