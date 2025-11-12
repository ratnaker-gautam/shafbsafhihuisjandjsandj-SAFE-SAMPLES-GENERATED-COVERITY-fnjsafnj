//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

struct WordInfo {
    char *word;
    int count;
};

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(str[i])) return 0;
    }
    return 1;
}

char *safe_strdup(const char *str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str);
    if (len > MAX_INPUT_LEN) return NULL;
    char *new_str = malloc(len + 1);
    if (new_str == NULL) return NULL;
    strcpy(new_str, str);
    return new_str;
}

int compare_words(const void *a, const void *b) {
    const struct WordInfo *wa = (const struct WordInfo *)a;
    const struct WordInfo *wb = (const struct WordInfo *)b;
    return strcmp(wa->word, wb->word);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    struct WordInfo words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    char *input_copy = safe_strdup(input);
    if (input_copy == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    char *token = strtok(input_copy, " \t");
    while (token != NULL && word_count < MAX_WORDS) {
        if (strlen(token) > 0) {
            int found = 0;
            for (int i = 0; i < word_count; i++) {
                if (strcmp(words[i].word, token) == 0) {
                    words[i].count++;
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                char *word_copy = safe_strdup(token);
                if (word_copy == NULL) {
                    fprintf(stderr, "Memory allocation failed\n");
                    free(input_copy);
                    return 1;
                }
                words[word_count].word = word_copy;
                words[word_count].count = 1;
                word_count++;
            }
        }
        token = strtok(NULL, " \t");
    }
    
    free(input_copy);
    
    if (word_count > 0) {
        qsort(words, word_count, sizeof(struct WordInfo), compare_words);
        
        printf("Word frequencies:\n");
        for (int i = 0; i < word_count; i++) {
            printf("%s: %d\n", words[i].word, words[i].count);
            free(words[i].word);
        }
    } else {
        printf("No words found\n");
    }
    
    return 0;
}