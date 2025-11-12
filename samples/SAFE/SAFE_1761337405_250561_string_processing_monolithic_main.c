//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][64];
    int word_count = 0;
    int word_length = 0;
    int in_word = 0;
    
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
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && isalpha((unsigned char)input[i])) {
            if (!in_word) {
                in_word = 1;
                word_length = 0;
            }
            if (word_length < 63) {
                words[word_count][word_length++] = tolower((unsigned char)input[i]);
            }
        } else {
            if (in_word) {
                if (word_length > 0) {
                    words[word_count][word_length] = '\0';
                    word_count++;
                    if (word_count >= MAX_WORDS) {
                        break;
                    }
                }
                in_word = 0;
                word_length = 0;
            }
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    printf("\nProcessing results:\n");
    printf("Total words: %d\n", word_count);
    
    int unique_count = 0;
    int frequencies[MAX_WORDS] = {0};
    char unique_words[MAX_WORDS][64];
    
    for (int i = 0; i < word_count; i++) {
        if (words[i][0] == '\0') continue;
        
        int found = 0;
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(words[i], unique_words[j]) == 0) {
                frequencies[j]++;
                found = 1;
                break;
            }
        }
        
        if (!found && unique_count < MAX_WORDS) {
            strncpy(unique_words[unique_count], words[i], 63);
            unique_words[unique_count][63] = '\0';
            frequencies[unique_count] = 1;
            unique_count++;
        }
    }
    
    printf("Unique words: %d\n", unique_count);
    printf("\nWord frequencies:\n");
    
    for (int i = 0; i < unique_count; i++) {
        printf("%s: %d\n", unique_words[i], frequencies[i]);
    }
    
    printf("\nSorted by frequency:\n");
    for (int i = 0; i < unique_count - 1; i++) {
        for (int j = i + 1; j < unique_count; j++) {
            if (frequencies[j] > frequencies[i] || 
                (frequencies[j] == frequencies[i] && strcmp(unique_words[j], unique_words[i]) < 0)) {
                int temp_freq = frequencies[i];
                frequencies[i] = frequencies[j];
                frequencies[j] = temp_freq;
                
                char temp_word[64];
                strncpy(temp_word, unique_words[i], 63);
                temp_word[63] = '\0';
                strncpy(unique_words[i], unique_words[j], 63);
                unique_words[i][63] = '\0';
                strncpy(unique_words[j], temp_word, 63);
                unique_words[j][63] = '\0';
            }
        }
    }
    
    for (int i = 0; i < unique_count; i++) {
        printf("%s: %d\n", unique_words[i], frequencies[i]);
    }
    
    return 0;
}