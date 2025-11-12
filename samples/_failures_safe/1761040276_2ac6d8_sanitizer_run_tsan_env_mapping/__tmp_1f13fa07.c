//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512
#define MAX_WORD_LEN 64

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

int find_word(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int compare_word_count(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    if (wa->count != wb->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
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
    
    size_t input_len = strlen(input);
    if (input_len == 0 || (input_len == 1 && input[0] == '\n')) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    char current_word[MAX_WORD_LEN];
    int word_pos = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (is_valid_char(c) && word_pos < MAX_WORD_LEN - 1) {
            current_word[word_pos++] = c;
            in_word = 1;
        } else {
            if (in_word) {
                current_word[word_pos] = '\0';
                
                if (strlen(current_word) > 0) {
                    to_lowercase(current_word);
                    
                    int found_idx = find_word(words, word_count, current_word);
                    if (found_idx >= 0) {
                        words[found_idx].count++;
                    } else if (word_count < MAX_WORDS) {
                        if (strlen(current_word) < MAX_WORD_LEN) {
                            strcpy(words[word_count].word, current_word);
                            words[word_count].count = 1;
                            word_count++;
                        }
                    }
                }
                
                word_pos = 0;
                in_word = 0;
            }
            
            if (c == '\0') {
                break;
            }
        }
    }
    
    qsort(words, word_count, sizeof(WordCount), compare_word_count);
    
    printf("\nWord frequency analysis (top 20):\n");
    printf("---------------------------------\n");
    
    int display_count = word_count > 20 ? 20 : word_count;
    for (int i = 0; i < display_count; i++) {
        printf("%-20s: %d\n", words[i].word, words[i].count);
    }
    
    if (word_count == 0) {
        printf("No valid words found in input.\n");
    } else if (word_count > 20) {
        printf("... and %d more words\n", word_count - 20);
    }
    
    return 0;
}