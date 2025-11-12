//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 100
#define MAX_TEXT_LEN 1024

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int first_position;
    int last_position;
};

struct TextProcessor {
    struct WordStats words[MAX_WORDS];
    int word_count;
    char processed_text[MAX_TEXT_LEN];
    int total_words;
    int total_chars;
};

void init_text_processor(struct TextProcessor *tp) {
    tp->word_count = 0;
    tp->total_words = 0;
    tp->total_chars = 0;
    tp->processed_text[0] = '\0';
    for (int i = 0; i < MAX_WORDS; i++) {
        tp->words[i].word[0] = '\0';
        tp->words[i].count = 0;
        tp->words[i].first_position = -1;
        tp->words[i].last_position = -1;
    }
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\t' || c == '\n';
}

int safe_strncpy(char *dest, const char *src, size_t n) {
    if (dest == NULL || src == NULL || n == 0) return 0;
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return 1;
}

int find_word_index(struct TextProcessor *tp, const char *word) {
    for (int i = 0; i < tp->word_count; i++) {
        if (strcmp(tp->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int add_word(struct TextProcessor *tp, const char *word, int position) {
    if (tp->word_count >= MAX_WORDS) return 0;
    if (!safe_strncpy(tp->words[tp->word_count].word, word, MAX_WORD_LEN)) return 0;
    tp->words[tp->word_count].count = 1;
    tp->words[tp->word_count].first_position = position;
    tp->words[tp->word_count].last_position = position;
    tp->word_count++;
    return 1;
}

void process_text(struct TextProcessor *tp, const char *input) {
    if (input == NULL) return;
    
    char buffer[MAX_TEXT_LEN];
    if (!safe_strncpy(buffer, input, MAX_TEXT_LEN)) return;
    
    int pos = 0;
    char *token = strtok(buffer, " \t\n");
    
    while (token != NULL && tp->total_words < MAX_WORDS) {
        int valid = 1;
        for (int i = 0; token[i] != '\0'; i++) {
            if (!isalnum((unsigned char)token[i])) {
                valid = 0;
                break;
            }
        }
        
        if (valid && strlen(token) < MAX_WORD_LEN) {
            int index = find_word_index(tp, token);
            if (index == -1) {
                if (add_word(tp, token, pos)) {
                    tp->total_words++;
                }
            } else {
                tp->words[index].count++;
                tp->words[index].last_position = pos;
            }
            pos++;
        }
        token = strtok(NULL, " \t\n");
    }
    
    tp->total_chars = (int)strlen(input);
    safe_strncpy(tp->processed_text, input, MAX_TEXT_LEN);
}

void print_stats(struct TextProcessor *tp) {
    printf("Text Statistics:\n");
    printf("Total characters: %d\n", tp->total_chars);
    printf("Total words: %d\n", tp->total_words);
    printf("Unique words: %d\n", tp->word_count);
    printf("\nWord Details:\n");
    
    for (int i = 0; i < tp->word_count; i++) {
        printf("Word: %-15s Count: %-3d First: %-3d Last: %-3d\n",
               tp->words[i].word,
               tp->words[i].count,
               tp->words[i].first_position,
               tp->words[i].last_position);
    }
}

int main() {
    struct TextProcessor processor;
    init_text_processor(&processor);
    
    char input_text[MAX_TEXT_LEN];
    printf("Enter text to analyze (max %d chars): ", MAX_TEXT_LEN - 1);
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_text);
    if (len > 0 && input_text[len - 1] == '\n') {
        input_text[len - 1] = '\0';
    }
    
    int valid_input = 1;
    for (size_t i = 0; i < strlen(input_text); i++) {
        if (!is_valid_char(input_text[i])) {
            valid_input = 0;
            break;
        }
    }
    
    if (!valid_input) {
        printf("Invalid characters in input\n");
        return