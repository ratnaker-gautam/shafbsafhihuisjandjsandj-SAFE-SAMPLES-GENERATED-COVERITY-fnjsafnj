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
        return 1;
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
        
        size_t word_len = j - i;
        if (word_len >= sizeof(words[0])) {
            fprintf(stderr, "Word too long\n");
            return 1;
        }
        
        memcpy(words[word_count], &input[i], word_len);
        words[word_count][word_len] = '\0';
        word_count++;
        
        i = j;
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 1;
    }
    
    for (i = 0; i < word_count - 1; i++) {
        for (j = 0; j < word_count - i - 1; j++) {
            if (strcmp(words[j], words[j + 1]) > 0) {
                char temp[MAX_INPUT_LEN + 1];
                if (strlen(words[j]) >= sizeof(temp) || strlen(words[j + 1]) >= sizeof(temp)) {
                    fprintf(stderr, "Word too long for swapping\n");
                    return 1;
                }
                strcpy(temp, words[j]);
                strcpy(words[j], words[j + 1]);
                strcpy(words[j + 1], temp);
            }
        }
    }
    
    printf("Sorted words:\n");
    for (i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    
    printf("\nWord frequencies:\n");
    i = 0;
    while (i < word_count) {
        int count = 1;
        j = i + 1;
        while (j < word_count && strcmp(words[i], words[j]) == 0) {
            count++;
            j++;
        }
        printf("%s: %d\n", words[i], count);
        i = j;
    }
    
    return 0;
}