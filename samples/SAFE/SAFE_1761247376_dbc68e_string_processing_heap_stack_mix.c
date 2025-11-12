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

char *to_lower_case(const char *str) {
    if (str == NULL) return NULL;
    
    size_t len = strlen(str);
    if (len == 0) return NULL;
    
    char *result = malloc(len + 1);
    if (result == NULL) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        result[i] = tolower((unsigned char)str[i]);
    }
    result[len] = '\0';
    return result;
}

int find_word_index(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int compare_word_count(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    return wb->count - wa->count;
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
    
    char *current_word = NULL;
    size_t word_start = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (is_valid_char(c) && !in_word) {
            word_start = i;
            in_word = 1;
        } else if (!is_valid_char(c) && in_word) {
            size_t word_len = i - word_start;
            if (word_len > 0 && word_len <= MAX_WORD_LEN && word_count < MAX_WORDS) {
                char temp_word[MAX_WORD_LEN + 1];
                if (word_len < sizeof(temp_word)) {
                    strncpy(temp_word, &input[word_start], word_len);
                    temp_word[word_len] = '\0';
                    
                    char *lower_word = to_lower_case(temp_word);
                    if (lower_word != NULL) {
                        int existing_index = find_word_index(words, word_count, lower_word);
                        if (existing_index >= 0) {
                            words[existing_index].count++;
                            free(lower_word);
                        } else {
                            words[word_count].word = lower_word;
                            words[word_count].count = 1;
                            word_count++;
                        }
                    }
                }
            }
            in_word = 0;
        }
    }
    
    if (word_count > 1) {
        qsort(words, word_count, sizeof(WordCount), compare_word_count);
    }
    
    printf("\nWord frequency (top %d):\n", word_count > 10 ? 10 : word_count);
    int display_count = word_count > 10 ? 10 : word_count;
    
    for (int i = 0; i < display_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    for (int i = 0; i < word_count; i++) {
        free(words[i].word);
    }
    
    return 0;
}