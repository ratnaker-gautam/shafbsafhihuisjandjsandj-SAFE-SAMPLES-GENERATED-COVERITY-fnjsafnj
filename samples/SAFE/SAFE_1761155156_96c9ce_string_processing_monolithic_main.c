//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][64];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter text to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (input_len >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (i < input_len && word_count < MAX_WORDS) {
        while (i < input_len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        j = i;
        while (j < input_len && !isspace((unsigned char)input[j])) {
            j++;
        }
        
        int word_len = j - i;
        if (word_len >= 64) {
            word_len = 63;
        }
        
        if (word_len > 0) {
            for (k = 0; k < word_len; k++) {
                words[word_count][k] = tolower((unsigned char)input[i + k]);
            }
            words[word_count][word_len] = '\0';
            word_count++;
        }
        
        i = j;
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    printf("\nProcessing results:\n");
    printf("Total words: %d\n", word_count);
    printf("Unique words: ");
    
    int unique_count = 0;
    for (i = 0; i < word_count; i++) {
        int is_unique = 1;
        for (j = 0; j < i; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            unique_count++;
            if (unique_count == 1) {
                printf("%s", words[i]);
            } else {
                printf(", %s", words[i]);
            }
        }
    }
    printf("\n");
    
    printf("Word frequencies:\n");
    for (i = 0; i < word_count; i++) {
        int counted = 0;
        for (j = 0; j < i; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                counted = 1;
                break;
            }
        }
        
        if (!counted) {
            int freq = 1;
            for (j = i + 1; j < word_count; j++) {
                if (strcmp(words[i], words[j]) == 0) {
                    freq++;
                }
            }
            printf("'%s': %d\n", words[i], freq);
        }
    }
    
    printf("Sorted words: ");
    char temp[64];
    for (i = 0; i < word_count - 1; i++) {
        for (j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) > 0) {
                strncpy(temp, words[i], sizeof(temp) - 1);
                temp[sizeof(temp) - 1] = '\0';
                strncpy(words[i], words[j], sizeof(words[i]) - 1);
                words[i][sizeof(words[i]) - 1] = '\0';
                strncpy(words[j], temp, sizeof(words[j]) - 1);
                words[j][sizeof(words[j]) - 1] = '\0';
            }
        }
    }
    
    for (i = 0; i < word_count; i++) {
        if (i == 0) {
            printf("%s", words[i]);
        } else {
            printf(", %s", words[i]);
        }
    }
    printf("\n");
    
    return 0;
}