//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

typedef struct {
    char word[64];
    int count;
} WordCount;

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_';
}

void to_lower_case(char *str) {
    for (size_t i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int find_word_index(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int compare_word_counts(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    if (wa->count != wb->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
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
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char buffer[64];
    size_t buf_pos = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (is_word_char(c) && buf_pos < sizeof(buffer) - 1) {
            buffer[buf_pos++] = c;
            in_word = 1;
        } else {
            if (in_word) {
                buffer[buf_pos] = '\0';
                
                if (buf_pos > 0) {
                    to_lower_case(buffer);
                    
                    int index = find_word_index(words, word_count, buffer);
                    if (index >= 0) {
                        words[index].count++;
                    } else if (word_count < MAX_WORDS) {
                        if (strlen(buffer) < sizeof(words[word_count].word)) {
                            strcpy(words[word_count].word, buffer);
                            words[word_count].count = 1;
                            word_count++;
                        }
                    }
                }
                
                buf_pos = 0;
                in_word = 0;
            }
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    qsort(words, word_count, sizeof(WordCount), compare_word_counts);
    
    printf("\nWord frequency analysis:\n");
    printf("------------------------\n");
    for (int i = 0; i < word_count; i++) {
        if (i < 10 || words[i].count > 1) {
            printf("%-20s: %d\n", words[i].word, words[i].count);
        }
    }
    
    return 0;
}