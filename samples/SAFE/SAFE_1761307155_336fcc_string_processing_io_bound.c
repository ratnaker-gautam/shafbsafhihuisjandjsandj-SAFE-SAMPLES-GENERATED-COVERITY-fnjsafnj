//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORD_COUNT 512

struct WordCount {
    char word[64];
    int count;
};

int is_word_char(char c) {
    return isalnum(c) || c == '\'' || c == '-';
}

void to_lower_case(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

int find_word_index(struct WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void sort_words(struct WordCount *words, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(words[j].word, words[j + 1].word) > 0) {
                struct WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    struct WordCount words[MAX_WORD_COUNT];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    
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
    
    char buffer[MAX_INPUT_LEN];
    if (strlen(input) >= sizeof(buffer)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    strcpy(buffer, input);
    
    char *current = buffer;
    while (*current) {
        while (*current && !is_word_char(*current)) {
            current++;
        }
        
        if (!*current) {
            break;
        }
        
        char *word_start = current;
        while (*current && is_word_char(*current)) {
            current++;
        }
        
        char saved_char = *current;
        *current = '\0';
        
        if (word_count >= MAX_WORD_COUNT) {
            fprintf(stderr, "Too many words\n");
            return 1;
        }
        
        to_lower_case(word_start);
        
        int existing_index = find_word_index(words, word_count, word_start);
        if (existing_index != -1) {
            words[existing_index].count++;
        } else {
            if (strlen(word_start) >= sizeof(words[0].word)) {
                fprintf(stderr, "Word too long\n");
                return 1;
            }
            strcpy(words[word_count].word, word_start);
            words[word_count].count = 1;
            word_count++;
        }
        
        *current = saved_char;
        if (*current) {
            current++;
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    sort_words(words, word_count);
    
    printf("\nWord frequency analysis:\n");
    printf("%-20s %s\n", "Word", "Count");
    printf("%-20s %s\n", "----", "-----");
    
    for (int i = 0; i < word_count; i++) {
        printf("%-20s %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}