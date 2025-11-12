//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_INPUT_LEN + 1];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
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
        printf("No input provided\n");
        return 0;
    }
    
    if (len > MAX_INPUT_LEN) {
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
        
        j = i;
        while (j < len && !isspace((unsigned char)input[j])) {
            j++;
        }
        
        int word_len = j - i;
        if (word_len > 0 && word_len <= MAX_INPUT_LEN) {
            strncpy(words[word_count], input + i, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
        
        i = j;
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    for (i = 0; i < word_count; i++) {
        for (j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) > 0) {
                char temp[MAX_INPUT_LEN + 1];
                strcpy(temp, words[i]);
                strcpy(words[i], words[j]);
                strcpy(words[j], temp);
            }
        }
    }
    
    printf("Sorted words:\n");
    for (i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    
    int freq[MAX_WORDS] = {0};
    int unique_count = 0;
    char unique_words[MAX_WORDS][MAX_INPUT_LEN + 1];
    
    for (i = 0; i < word_count; i++) {
        int found = 0;
        for (j = 0; j < unique_count; j++) {
            if (strcmp(words[i], unique_words[j]) == 0) {
                freq[j]++;
                found = 1;
                break;
            }
        }
        
        if (!found && unique_count < MAX_WORDS) {
            strcpy(unique_words[unique_count], words[i]);
            freq[unique_count] = 1;
            unique_count++;
        }
    }
    
    printf("\nWord frequencies:\n");
    for (i = 0; i < unique_count; i++) {
        printf("%s: %d\n", unique_words[i], freq[i]);
    }
    
    return 0;
}