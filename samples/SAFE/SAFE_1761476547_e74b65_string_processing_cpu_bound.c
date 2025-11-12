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
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
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
            if (words[j].count < words[j + 1].count || 
                (words[j].count == words[j + 1].count && 
                 strcmp(words[j].word, words[j + 1].word) > 0)) {
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
    if (len >= sizeof(input) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int i = 0;
    while (input[i] != '\0') {
        while (input[i] != '\0' && !is_valid_char(input[i])) {
            i++;
        }
        
        if (input[i] == '\0') {
            break;
        }
        
        int start = i;
        while (input[i] != '\0' && is_valid_char(input[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len >= MAX_WORD_LEN - 1) {
            word_len = MAX_WORD_LEN - 2;
        }
        
        if (word_count >= MAX_WORDS) {
            fprintf(stderr, "Too many words\n");
            return 1;
        }
        
        char temp_word[MAX_WORD_LEN];
        strncpy(temp_word, &input[start], word_len);
        temp_word[word_len] = '\0';
        to_lowercase(temp_word);
        
        if (strlen(temp_word) == 0) {
            continue;
        }
        
        int found_index = find_word(words, word_count, temp_word);
        if (found_index != -1) {
            words[found_index].count++;
        } else {
            strcpy(words[word_count].word, temp_word);
            words[word_count].count = 1;
            word_count++;
        }
    }
    
    sort_words(words, word_count);
    
    for (int i = 0; i < word_count && i < 10; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}