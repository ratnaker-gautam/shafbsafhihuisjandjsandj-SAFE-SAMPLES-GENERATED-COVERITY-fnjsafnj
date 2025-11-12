//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
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

void sort_words(WordCount *words, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(words[j].word, words[j + 1].word) > 0) {
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
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Invalid input length\n");
        return 1;
    }
    
    int in_word = 0;
    char current_word[MAX_WORD_LEN];
    int word_pos = 0;
    
    for (size_t i = 0; i <= len; i++) {
        char c = input[i];
        
        if (is_valid_char(c) && word_pos < MAX_WORD_LEN - 1) {
            if (!in_word) {
                in_word = 1;
                word_pos = 0;
            }
            current_word[word_pos++] = c;
        } else {
            if (in_word) {
                current_word[word_pos] = '\0';
                
                if (strlen(current_word) > 0) {
                    to_lowercase(current_word);
                    
                    int found_idx = find_word(words, word_count, current_word);
                    if (found_idx >= 0) {
                        words[found_idx].count++;
                    } else {
                        if (word_count < MAX_WORDS) {
                            strncpy(words[word_count].word, current_word, MAX_WORD_LEN - 1);
                            words[word_count].word[MAX_WORD_LEN - 1] = '\0';
                            words[word_count].count = 1;
                            word_count++;
                        }
                    }
                }
                in_word = 0;
                word_pos = 0;
            }
            
            if (c == '\0') {
                break;
            }
        }
    }
    
    sort_words(words, word_count);
    
    for (int i = 0; i < word_count; i++) {
        if (printf("%s: %d\n", words[i].word, words[i].count) < 0) {
            fprintf(stderr, "Error writing output\n");
            return 1;
        }
    }
    
    return 0;
}