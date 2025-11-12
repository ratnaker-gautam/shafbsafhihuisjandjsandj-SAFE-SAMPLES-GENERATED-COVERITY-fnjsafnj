//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 30

int main() {
    char input[MAX_INPUT + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter a string (max %d characters): ", MAX_INPUT);
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
    
    if (len > MAX_INPUT) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) break;
        
        j = 0;
        while (i < len && !isspace((unsigned char)input[i]) && j < MAX_WORD_LEN) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        
        if (j > 0) {
            words[word_count][j] = '\0';
            word_count++;
        }
    }
    
    printf("Found %d words:\n", word_count);
    for (k = 0; k < word_count; k++) {
        printf("Word %d: %s\n", k + 1, words[k]);
    }
    
    printf("\nProcessing words with different loop styles:\n");
    
    printf("1. For loop (reverse words):\n");
    for (int idx = word_count - 1; idx >= 0; idx--) {
        printf("%s ", words[idx]);
    }
    printf("\n");
    
    printf("2. While loop (uppercase first letters):\n");
    int idx = 0;
    while (idx < word_count) {
        if (words[idx][0] != '\0') {
            char temp[MAX_WORD_LEN + 1];
            strncpy(temp, words[idx], sizeof(temp) - 1);
            temp[sizeof(temp) - 1] = '\0';
            temp[0] = toupper((unsigned char)temp[0]);
            printf("%s ", temp);
        }
        idx++;
    }
    printf("\n");
    
    printf("3. Do-while loop (word lengths):\n");
    idx = 0;
    if (word_count > 0) {
        do {
            printf("%s: %zu chars, ", words[idx], strlen(words[idx]));
            idx++;
        } while (idx < word_count);
        printf("\n");
    }
    
    printf("4. Nested loops (character analysis):\n");
    for (int w = 0; w < word_count; w++) {
        int letter_count = 0;
        int digit_count = 0;
        char *ptr = words[w];
        
        while (*ptr != '\0') {
            if (isalpha((unsigned char)*ptr)) {
                letter_count++;
            } else if (isdigit((unsigned char)*ptr)) {
                digit_count++;
            }
            ptr++;
        }
        
        printf("Word '%s': %d letters, %d digits\n", words[w], letter_count, digit_count);
    }
    
    return 0;
}