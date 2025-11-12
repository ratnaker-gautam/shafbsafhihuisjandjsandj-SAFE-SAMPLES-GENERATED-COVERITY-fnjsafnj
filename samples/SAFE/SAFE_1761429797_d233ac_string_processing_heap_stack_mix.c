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
    if (len == 0) return NULL;
    char *result = malloc(len + 1);
    if (result == NULL) return NULL;
    for (size_t i = 0; i < len; i++) {
        result[i] = tolower(str[i]);
    }
    result[len] = '\0';
    return result;
}

int is_word_char(char c) {
    return isalpha(c) || c == '\'';
}

int process_words(const char *input, WordCount *words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return 0;
    
    char *lower_input = to_lowercase(input);
    if (lower_input == NULL) return 0;
    
    int count = 0;
    size_t len = strlen(lower_input);
    size_t i = 0;
    
    while (i < len && count < MAX_WORDS) {
        while (i < len && !is_word_char(lower_input[i])) i++;
        if (i >= len) break;
        
        size_t start = i;
        while (i < len && is_word_char(lower_input[i])) i++;
        size_t word_len = i - start;
        
        if (word_len > 0 && word_len <= MAX_WORD_LEN) {
            char temp_word[MAX_WORD_LEN + 1];
            if (word_len < sizeof(temp_word)) {
                strncpy(temp_word, &lower_input[start], word_len);
                temp_word[word_len] = '\0';
                
                int found = 0;
                for (int j = 0; j < count; j++) {
                    if (strcmp(words[j].word, temp_word) == 0) {
                        words[j].count++;
                        found = 1;
                        break;
                    }
                }
                
                if (!found) {
                    words[count].word = malloc(word_len + 1);
                    if (words[count].word != NULL) {
                        strcpy(words[count].word, temp_word);
                        words[count].count = 1;
                        count++;
                    }
                }
            }
        }
    }
    
    free(lower_input);
    *word_count = count;
    return 1;
}

int compare_word_count(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    if (wb->count != wa->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

void cleanup_words(WordCount *words, int count) {
    if (words == NULL) return;
    for (int i = 0; i < count; i++) {
        if (words[i].word != NULL) {
            free(words[i].word);
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text for word frequency analysis: ");
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
    
    for (int i = 0; i < MAX_WORDS; i++) {
        words[i].word = NULL;
        words[i].count = 0;
    }
    
    if (!process_words(input, words, &word_count)) {
        printf("Error processing words\n");
        cleanup_words(words, word_count);
        return 1;
    }
    
    if (word_count > 0) {
        qsort(words, word_count, sizeof(WordCount), compare_word_count);
        
        printf("\nWord frequencies (sorted by count):\n");
        for (int i = 0; i < word_count; i++) {
            if (words[i].word != NULL) {
                printf("%s: %d\n", words[i].word, words[i].count);
            }
        }
    } else {
        printf("No valid words found in input\n");
    }
    
    cleanup_words(words, word_count);
    return 0;
}