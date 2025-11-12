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
    
    int i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        int j = 0;
        while (i < len && j < MAX_WORD_LEN && !isspace((unsigned char)input[i])) {
            temp[j++] = input[i++];
        }
        temp[j] = '\0';
        
        if (j > 0) {
            strncpy(words[word_count], temp, MAX_WORD_LEN);
            words[word_count][MAX_WORD_LEN] = '\0';
            word_count++;
        }
    }
    
    printf("Found %d words:\n", word_count);
    
    for (int idx = 0; idx < word_count; idx++) {
        printf("Word %d: ", idx + 1);
        
        int k = 0;
        while (words[idx][k] != '\0' && k < MAX_WORD_LEN) {
            putchar(words[idx][k]);
            k++;
        }
        putchar('\n');
    }
    
    printf("\nProcessing words:\n");
    int idx = 0;
    do {
        if (word_count == 0) break;
        
        int has_upper = 0;
        int has_lower = 0;
        int char_idx = 0;
        
        while (words[idx][char_idx] != '\0' && char_idx < MAX_WORD_LEN) {
            if (isupper((unsigned char)words[idx][char_idx])) {
                has_upper = 1;
            }
            if (islower((unsigned char)words[idx][char_idx])) {
                has_lower = 1;
            }
            char_idx++;
        }
        
        printf("Word '%s': ", words[idx]);
        if (has_upper && has_lower) {
            printf("Mixed case\n");
        } else if (has_upper) {
            printf("All uppercase\n");
        } else if (has_lower) {
            printf("All lowercase\n");
        } else {
            printf("No letters\n");
        }
        
        idx++;
    } while (idx < word_count);
    
    printf("\nWord lengths:\n");
    for (int pos = 0; pos < word_count; pos++) {
        size_t length = 0;
        while (words[pos][length] != '\0' && length < MAX_WORD_LEN) {
            length++;
        }
        printf("'%s' has %zu characters\n", words[pos], length);
    }
    
    return 0;
}