//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 4096
#define MAX_WORDS 1000
#define MAX_WORD_LEN 100

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

int process_input(char *input, WordCount *words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) {
        return -1;
    }
    
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    int in_word = 0;
    
    for (int i = 0; input[i] != '\0'; i++) {
        if (is_valid_char(input[i])) {
            if (buf_pos < MAX_WORD_LEN - 1) {
                buffer[buf_pos++] = tolower((unsigned char)input[i]);
                in_word = 1;
            } else {
                return -1;
            }
        } else {
            if (in_word && buf_pos > 0) {
                buffer[buf_pos] = '\0';
                
                int found = 0;
                for (int j = 0; j < *word_count; j++) {
                    if (strcmp(words[j].word, buffer) == 0) {
                        if (words[j].count < INT_MAX) {
                            words[j].count++;
                        }
                        found = 1;
                        break;
                    }
                }
                
                if (!found && *word_count < MAX_WORDS) {
                    strncpy(words[*word_count].word, buffer, MAX_WORD_LEN - 1);
                    words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
                    words[*word_count].count = 1;
                    (*word_count)++;
                }
                
                buf_pos = 0;
                in_word = 0;
            }
        }
    }
    
    if (in_word && buf_pos > 0) {
        buffer[buf_pos] = '\0';
        
        int found = 0;
        for (int j = 0; j < *word_count; j++) {
            if (strcmp(words[j].word, buffer) == 0) {
                if (words[j].count < INT_MAX) {
                    words[j].count++;
                }
                found = 1;
                break;
            }
        }
        
        if (!found && *word_count < MAX_WORDS) {
            strncpy(words[*word_count].word, buffer, MAX_WORD_LEN - 1);
            words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
            words[*word_count].count = 1;
            (*word_count)++;
        }
    }
    
    return 0;
}

void sort_words(WordCount *words, int word_count) {
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
            if (words[j].count < words[j + 1].count || 
                (words[j].count == words[j + 1].count && 
                 strcmp(words[j].word, words[j + 1].word) > 0)) {
                WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text for word frequency analysis: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (process_input(input, words, &word_count) != 0) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found in input\n");
        return 0;
    }
    
    sort_words(words, word_count);
    
    printf("\nWord frequency analysis (sorted by frequency, then alphabetically):\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}