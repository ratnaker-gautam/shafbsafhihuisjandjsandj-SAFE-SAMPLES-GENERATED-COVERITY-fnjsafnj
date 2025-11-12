//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

typedef struct {
    char *word;
    int count;
} WordCount;

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(str[i])) return 0;
    }
    return 1;
}

char *to_lowercase(const char *str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str);
    char *result = malloc(len + 1);
    if (result == NULL) return NULL;
    for (size_t i = 0; i < len; i++) {
        result[i] = tolower(str[i]);
    }
    result[len] = '\0';
    return result;
}

int is_word_char(char c) {
    return isalnum(c) || c == '\'' || c == '-';
}

int find_word(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void free_words(WordCount *words, int count) {
    for (int i = 0; i < count; i++) {
        free(words[i].word);
    }
}

int compare_word_count(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    return wb->count - wa->count;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words_stack[MAX_WORDS];
    WordCount *words_heap = NULL;
    int word_count = 0;
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        printf("Invalid input\n");
        return 1;
    }
    
    char *lower_input = to_lowercase(input);
    if (lower_input == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    char *text = lower_input;
    while (*text && word_count < MAX_WORDS) {
        while (*text && !is_word_char(*text)) {
            text++;
        }
        if (!*text) break;
        
        char *word_start = text;
        while (*text && is_word_char(*text)) {
            text++;
        }
        
        size_t word_len = text - word_start;
        if (word_len > 0) {
            char temp_word[256];
            if (word_len >= sizeof(temp_word)) {
                word_len = sizeof(temp_word) - 1;
            }
            strncpy(temp_word, word_start, word_len);
            temp_word[word_len] = '\0';
            
            int found_idx = find_word(words_stack, word_count, temp_word);
            if (found_idx >= 0) {
                words_stack[found_idx].count++;
            } else {
                char *new_word = malloc(word_len + 1);
                if (new_word == NULL) {
                    free(lower_input);
                    free_words(words_stack, word_count);
                    printf("Memory allocation failed\n");
                    return 1;
                }
                strcpy(new_word, temp_word);
                words_stack[word_count].word = new_word;
                words_stack[word_count].count = 1;
                word_count++;
            }
        }
    }
    
    if (word_count > 0) {
        words_heap = malloc(sizeof(WordCount) * word_count);
        if (words_heap == NULL) {
            free(lower_input);
            free_words(words_stack, word_count);
            printf("Memory allocation failed\n");
            return 1;
        }
        
        for (int i = 0; i < word_count; i++) {
            words_heap[i] = words_stack[i];
        }
        
        qsort(words_heap, word_count, sizeof(WordCount), compare_word_count);
        
        printf("\nWord frequencies (sorted by count):\n");
        for (int i = 0; i < word_count; i++) {
            printf("%s: %d\n", words_heap[i].word, words_heap[i].count);
        }
        
        free(words_heap);
    } else {
        printf("No words found in input\n");
    }
    
    free(lower_input);
    free_words(words_stack, word_count);
    
    return 0;
}