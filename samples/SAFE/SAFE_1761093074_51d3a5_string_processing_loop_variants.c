//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: string_processing
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
        return 0;
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
        if (word_len < 63) {
            strncpy(words[word_count], &input[start], word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    printf("Found %d words:\n", word_count);
    
    for (int j = 0; j < word_count; j++) {
        int k = 0;
        while (words[j][k] != '\0') {
            words[j][k] = toupper((unsigned char)words[j][k]);
            k++;
        }
    }
    
    int swapped;
    int n = word_count;
    do {
        swapped = 0;
        for (int j = 0; j < n - 1; j++) {
            if (strcmp(words[j], words[j+1]) > 0) {
                char temp[64];
                strcpy(temp, words[j]);
                strcpy(words[j], words[j+1]);
                strcpy(words[j+1], temp);
                swapped = 1;
            }
        }
        n--;
    } while (swapped);
    
    int idx = 0;
    while (idx < word_count) {
        printf("%d: %s\n", idx + 1, words[idx]);
        idx++;
    }
    
    int total_chars = 0;
    for (int j = 0; j < word_count; j++) {
        int k = 0;
        while (words[j][k] != '\0') {
            total_chars++;
            k++;
        }
    }
    
    printf("Total characters in words: %d\n", total_chars);
    
    return 0;
}