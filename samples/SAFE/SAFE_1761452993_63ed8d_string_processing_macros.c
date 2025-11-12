//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define IS_TERMINATOR(c) ((c) == '.' || (c) == '!' || (c) == '?')
#define TO_UPPER(c) (((c) >= 'a' && (c) <= 'z') ? ((c) - 'a' + 'A') : (c))
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) - 'A' + 'a') : (c))

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int safe_strlen(const char *str, size_t max_len) {
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

int extract_words(const char *text, WordCount words[], int max_words) {
    int word_count = 0;
    int i = 0;
    int text_len = safe_strlen(text, MAX_INPUT_LEN);
    
    while (i < text_len && word_count < max_words) {
        while (i < text_len && IS_WHITESPACE(text[i])) {
            i++;
        }
        
        if (i >= text_len) {
            break;
        }
        
        int word_start = i;
        int word_len = 0;
        
        while (i < text_len && !IS_WHITESPACE(text[i]) && word_len < MAX_WORD_LEN - 1) {
            word_len++;
            i++;
        }
        
        if (word_len > 0) {
            int j;
            for (j = 0; j < word_len; j++) {
                words[word_count].word[j] = TO_LOWER(text[word_start + j]);
            }
            words[word_count].word[word_len] = '\0';
            
            int k;
            int found = 0;
            for (k = 0; k < word_count; k++) {
                if (strcmp(words[k].word, words[word_count].word) == 0) {
                    words[k].count++;
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                words[word_count].count = 1;
                word_count++;
            }
        }
    }
    
    return word_count;
}

void capitalize_sentences(char *text) {
    int len = safe_strlen(text, MAX_INPUT_LEN);
    int capitalize_next = 1;
    int i;
    
    for (i = 0; i < len; i++) {
        if (capitalize_next && isalpha(text[i])) {
            text[i] = TO_UPPER(text[i]);
            capitalize_next = 0;
        }
        
        if (IS_TERMINATOR(text[i])) {
            capitalize_next = 1;
        }
    }
}

int compare_word_count(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    return wb->count - wa->count;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    
    printf("Enter text (max %d characters):\n", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    int input_len = safe_strlen(input, sizeof(input));
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (safe_strlen(input, sizeof(input)) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    char processed[MAX_INPUT_LEN];
    if (safe_strlen(input, sizeof(input)) >= sizeof(processed)) {
        printf("Input too long\n");
        return 1;
    }
    
    strncpy(processed, input, sizeof(processed) - 1);
    processed[sizeof(processed) - 1] = '\0';
    
    capitalize_sentences(processed);
    
    int word_count = extract_words(input, words, MAX_WORDS);
    
    printf("\nCapitalized text:\n%s\n\n", processed);
    
    if (word_count > 0) {
        qsort(words, word_count, sizeof(WordCount), compare_word_count);
        
        printf("Word frequency (top 10):\n");
        int display_count = word_count > 10 ? 10 : word_count;
        int i;
        for (i = 0; i < display_count; i++) {
            printf("%s: %d\n", words[i].word, words[i].count);
        }
    } else {
        printf("No words found in input\n");
    }
    
    return 0;
}