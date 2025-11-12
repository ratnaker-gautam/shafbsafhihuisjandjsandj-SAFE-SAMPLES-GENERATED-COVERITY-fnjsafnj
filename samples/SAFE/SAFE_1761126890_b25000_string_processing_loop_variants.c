//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    char temp[MAX_WORD_LEN + 1];
    int i, j, k;
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN);
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
        printf("Empty input string\n");
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
        
        j = 0;
        while (i < len && j < MAX_WORD_LEN && !isspace((unsigned char)input[i])) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        
        if (j > 0) {
            words[word_count][j] = '\0';
            word_count++;
        }
    }
    
    printf("Original words (%d found):\n", word_count);
    for (int idx = 0; idx < word_count; idx++) {
        printf("'%s' ", words[idx]);
    }
    printf("\n");
    
    printf("Words in reverse order:\n");
    for (int idx = word_count - 1; idx >= 0; idx--) {
        printf("'%s' ", words[idx]);
    }
    printf("\n");
    
    printf("Words with vowels removed:\n");
    for (int idx = 0; idx < word_count; idx++) {
        k = 0;
        for (j = 0; words[idx][j] != '\0' && k < MAX_WORD_LEN; j++) {
            char c = tolower((unsigned char)words[idx][j]);
            if (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u') {
                temp[k] = words[idx][j];
                k++;
            }
        }
        temp[k] = '\0';
        printf("'%s' ", temp);
    }
    printf("\n");
    
    printf("Word lengths:\n");
    int total_chars = 0;
    int idx = 0;
    do {
        int word_len = strlen(words[idx]);
        total_chars += word_len;
        printf("'%s': %d ", words[idx], word_len);
        idx++;
    } while (idx < word_count);
    printf("\nTotal characters in words: %d\n", total_chars);
    
    printf("Words sorted by length:\n");
    int lengths[MAX_WORDS];
    int indices[MAX_WORDS];
    
    for (i = 0; i < word_count; i++) {
        lengths[i] = strlen(words[i]);
        indices[i] = i;
    }
    
    for (i = 0; i < word_count - 1; i++) {
        for (j = 0; j < word_count - i - 1; j++) {
            if (lengths[j] > lengths[j + 1]) {
                int temp_len = lengths[j];
                lengths[j] = lengths[j + 1];
                lengths[j + 1] = temp_len;
                
                int temp_idx = indices[j];
                indices[j] = indices[j + 1];
                indices[j + 1] = temp_idx;
            }
        }
    }
    
    for (i = 0; i < word_count; i++) {
        printf("'%s' ", words[indices[i]]);
    }
    printf("\n");
    
    return 0;
}