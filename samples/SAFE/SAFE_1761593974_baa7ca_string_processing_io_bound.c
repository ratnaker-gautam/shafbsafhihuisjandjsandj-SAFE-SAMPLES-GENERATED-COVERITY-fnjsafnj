//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORD_LEN 64
#define MAX_WORDS 100

struct WordCount {
    char word[MAX_WORD_LEN];
    int count;
};

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

void to_lower_case(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int find_word_index(struct WordCount words[], int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void add_word(struct WordCount words[], int *count, const char *word) {
    if (*count >= MAX_WORDS) {
        return;
    }
    
    if (strlen(word) >= MAX_WORD_LEN) {
        return;
    }
    
    strcpy(words[*count].word, word);
    words[*count].count = 1;
    (*count)++;
}

void process_text(const char *text, struct WordCount words[], int *word_count) {
    char buffer[MAX_INPUT_LEN];
    char word[MAX_WORD_LEN];
    int buf_len, word_len;
    int i, j;
    
    if (text == NULL) {
        return;
    }
    
    buf_len = strlen(text);
    if (buf_len >= MAX_INPUT_LEN) {
        return;
    }
    
    strcpy(buffer, text);
    to_lower_case(buffer);
    
    i = 0;
    while (i < buf_len) {
        while (i < buf_len && !is_valid_char(buffer[i])) {
            i++;
        }
        
        if (i >= buf_len) {
            break;
        }
        
        j = 0;
        while (i < buf_len && is_valid_char(buffer[i]) && j < MAX_WORD_LEN - 1) {
            word[j++] = buffer[i++];
        }
        word[j] = '\0';
        
        if (strlen(word) > 0) {
            int index = find_word_index(words, *word_count, word);
            if (index >= 0) {
                words[index].count++;
            } else {
                add_word(words, word_count, word);
            }
        }
    }
}

void print_word_counts(struct WordCount words[], int count) {
    printf("Word frequencies:\n");
    for (int i = 0; i < count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text to analyze (max %d characters):\n", MAX_INPUT_LEN - 1);
    
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
    
    process_text(input, words, &word_count);
    
    if (word_count == 0) {
        printf("No valid words found in input\n");
        return 1;
    }
    
    print_word_counts(words, word_count);
    
    return 0;
}