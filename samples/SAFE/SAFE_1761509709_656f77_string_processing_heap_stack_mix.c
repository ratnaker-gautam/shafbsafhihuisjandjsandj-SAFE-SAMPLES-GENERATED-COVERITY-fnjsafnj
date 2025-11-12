//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

typedef struct {
    char *word;
    int count;
} WordCount;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

char *strdup_safe(const char *s) {
    if (s == NULL) return NULL;
    size_t len = strlen(s);
    if (len == 0) return NULL;
    char *copy = malloc(len + 1);
    if (copy == NULL) return NULL;
    memcpy(copy, s, len + 1);
    return copy;
}

int compare_words(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    return strcmp(wa->word, wb->word);
}

void free_word_counts(WordCount *words, int count) {
    for (int i = 0; i < count; i++) {
        free(words[i].word);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text: ");
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
        return 0;
    }
    
    char *word_buffer = malloc(MAX_WORD_LEN + 1);
    if (word_buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    int buffer_pos = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (is_valid_char(c) && buffer_pos < MAX_WORD_LEN) {
            word_buffer[buffer_pos++] = tolower((unsigned char)c);
            in_word = 1;
        } else {
            if (in_word && buffer_pos > 0) {
                word_buffer[buffer_pos] = '\0';
                
                int found = 0;
                for (int j = 0; j < word_count; j++) {
                    if (strcmp(words[j].word, word_buffer) == 0) {
                        words[j].count++;
                        found = 1;
                        break;
                    }
                }
                
                if (!found && word_count < MAX_WORDS) {
                    char *word_copy = strdup_safe(word_buffer);
                    if (word_copy != NULL) {
                        words[word_count].word = word_copy;
                        words[word_count].count = 1;
                        word_count++;
                    }
                }
                
                buffer_pos = 0;
                in_word = 0;
            }
            
            if (c == '\0') break;
        }
    }
    
    free(word_buffer);
    
    if (word_count > 0) {
        qsort(words, word_count, sizeof(WordCount), compare_words);
        
        printf("\nWord frequencies:\n");
        for (int i = 0; i < word_count; i++) {
            printf("%s: %d\n", words[i].word, words[i].count);
        }
    } else {
        printf("No valid words found\n");
    }
    
    free_word_counts(words, word_count);
    
    return 0;
}