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
    while (i < len) {
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) break;
        
        int start = i;
        while (i < len && !isspace((unsigned char)input[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len > 0 && word_len < 64 && word_count < MAX_WORDS) {
            strncpy(words[word_count], input + start, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    printf("Words found: %d\n", word_count);
    
    for (int j = 0; j < word_count; j++) {
        int k = 0;
        char processed[64];
        
        char *ptr = words[j];
        while (*ptr) {
            if (isalnum((unsigned char)*ptr) && k < 63) {
                processed[k++] = tolower((unsigned char)*ptr);
            }
            ptr++;
        }
        processed[k] = '\0';
        
        if (k > 0) {
            printf("Word %d: %s (length: %d)\n", j+1, processed, k);
        }
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
    for (int m = 0; m < word_count; m++) {
        char temp[64];
        int n = 0;
        char *p = words[m];
        while (*p && n < 63) {
            temp[n++] = tolower((unsigned char)*p);
            p++;
        }
        temp[n] = '\0';
        
        char search_lower[64];
        int o = 0;
        char *q = search;
        while (*q && o < 63) {
            search_lower[o++] = tolower((unsigned char)*q);
            q++;
        }
        search_lower[o] = '\0';
        
        if (strcmp(temp, search_lower) == 0) {
            found = 1;
            break;
        }
    }
    
    if (found) {
        printf("Word found in text\n");
    } else {
        printf("Word not found in text\n");
    }
    
    return 0;
}