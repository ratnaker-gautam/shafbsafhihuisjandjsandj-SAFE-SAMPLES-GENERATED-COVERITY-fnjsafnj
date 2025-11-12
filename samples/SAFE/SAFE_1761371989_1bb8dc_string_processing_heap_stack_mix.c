//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

struct WordCount {
    char *word;
    int count;
};

char *to_lower_case(const char *str) {
    if (str == NULL) return NULL;
    
    size_t len = strlen(str);
    if (len >= MAX_INPUT_LEN) return NULL;
    
    char *result = malloc(len + 1);
    if (result == NULL) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        result[i] = tolower((unsigned char)str[i]);
    }
    result[len] = '\0';
    return result;
}

int is_word_char(char c) {
    return isalpha((unsigned char)c);
}

char *extract_next_word(const char **text_ptr) {
    const char *text = *text_ptr;
    
    while (*text && !is_word_char(*text)) {
        text++;
    }
    
    if (*text == '\0') return NULL;
    
    const char *start = text;
    while (*text && is_word_char(*text)) {
        text++;
    }
    
    size_t word_len = text - start;
    if (word_len >= MAX_INPUT_LEN) return NULL;
    
    char *word = malloc(word_len + 1);
    if (word == NULL) return NULL;
    
    memcpy(word, start, word_len);
    word[word_len] = '\0';
    
    *text_ptr = text;
    return word;
}

int find_word_in_array(struct WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void free_word_array(struct WordCount *words, int count) {
    for (int i = 0; i < count; i++) {
        free(words[i].word);
    }
}

int compare_word_count(const void *a, const void *b) {
    const struct WordCount *wa = (const struct WordCount *)a;
    const struct WordCount *wb = (const struct WordCount *)b;
    return wb->count - wa->count;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    const char *text_ptr = input;
    
    while (word_count < MAX_WORDS) {
        char *raw_word = extract_next_word(&text_ptr);
        if (raw_word == NULL) break;
        
        char *word = to_lower_case(raw_word);
        free(raw_word);
        
        if (word == NULL) continue;
        
        int existing_index = find_word_in_array(words, word_count, word);
        
        if (existing_index >= 0) {
            words[existing_index].count++;
            free(word);
        } else {
            words[word_count].word = word;
            words[word_count].count = 1;
            word_count++;
        }
    }
    
    if (word_count > 0) {
        qsort(words, word_count, sizeof(struct WordCount), compare_word_count);
        
        printf("\nWord frequencies (top 10):\n");
        int limit = word_count > 10 ? 10 : word_count;
        
        for (int i = 0; i < limit; i++) {
            printf("%s: %d\n", words[i].word, words[i].count);
        }
    } else {
        printf("No words found in input\n");
    }
    
    free_word_array(words, word_count);
    return 0;
}