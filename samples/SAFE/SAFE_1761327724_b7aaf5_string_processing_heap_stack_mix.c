//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 100

struct WordCount {
    char word[MAX_WORD_LEN];
    int count;
};

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

char* to_lowercase(const char* str) {
    if (str == NULL) return NULL;
    
    size_t len = strlen(str);
    if (len >= MAX_WORD_LEN) return NULL;
    
    char* result = malloc(len + 1);
    if (result == NULL) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        result[i] = tolower((unsigned char)str[i]);
    }
    result[len] = '\0';
    
    return result;
}

int find_word_index(struct WordCount* words, int count, const char* word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void process_text(const char* text, struct WordCount** result, int* result_count) {
    if (text == NULL || result == NULL || result_count == NULL) return;
    
    struct WordCount* words = malloc(MAX_WORDS * sizeof(struct WordCount));
    if (words == NULL) return;
    
    int word_count = 0;
    size_t text_len = strlen(text);
    char buffer[MAX_WORD_LEN];
    size_t buf_pos = 0;
    
    for (size_t i = 0; i <= text_len; i++) {
        char c = text[i];
        
        if (i < text_len && is_valid_char(c)) {
            if (buf_pos < MAX_WORD_LEN - 1) {
                buffer[buf_pos++] = c;
            }
        } else if (buf_pos > 0) {
            buffer[buf_pos] = '\0';
            
            char* lower_word = to_lowercase(buffer);
            if (lower_word != NULL) {
                int index = find_word_index(words, word_count, lower_word);
                
                if (index >= 0) {
                    words[index].count++;
                } else if (word_count < MAX_WORDS) {
                    strncpy(words[word_count].word, lower_word, MAX_WORD_LEN - 1);
                    words[word_count].word[MAX_WORD_LEN - 1] = '\0';
                    words[word_count].count = 1;
                    word_count++;
                }
                
                free(lower_word);
            }
            
            buf_pos = 0;
        }
    }
    
    *result = words;
    *result_count = word_count;
}

int compare_word_count(const void* a, const void* b) {
    const struct WordCount* wa = (const struct WordCount*)a;
    const struct WordCount* wb = (const struct WordCount*)b;
    
    if (wb->count != wa->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

int main(void) {
    char input[1024];
    
    printf("Enter text to analyze (max %zu characters): ", sizeof(input) - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    struct WordCount* words = NULL;
    int word_count = 0;
    
    process_text(input, &words, &word_count);
    
    if (words == NULL || word_count == 0) {
        printf("No words found in input\n");
        if (words != NULL) free(words);
        return 1;
    }
    
    qsort(words, word_count, sizeof(struct WordCount), compare_word_count);
    
    printf("\nWord frequency analysis:\n");
    printf("%-20s %s\n", "Word", "Count");
    printf("%-20s %s\n", "----", "-----");
    
    for (int i = 0; i < word_count; i++) {
        if (i < 10) {
            printf("%-20s %d\n", words[i].word, words[i].count);
        }
    }
    
    if (word_count > 10) {
        printf("... and %d more words\n", word_count - 10);
    }
    
    free(words);
    return 0;
}