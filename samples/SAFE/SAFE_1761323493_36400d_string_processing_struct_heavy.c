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
    int total_chars;
    int total_words;
};

void init_text_processor(struct TextProcessor *tp) {
    tp->word_count = 0;
    tp->processed_text[0] = '\0';
    tp->total_chars = 0;
    tp->total_words = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        tp->words[i].word[0] = '\0';
        tp->words[i].count = 0;
        tp->words[i].first_position = -1;
        tp->words[i].last_position = -1;
    }
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\t' || c == '\n' || c == '.';
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

int process_text(struct TextProcessor *tp, const char *input) {
    if (tp == NULL || input == NULL) return 0;
    
    char clean_text[MAX_TEXT_LEN];
    int clean_len = 0;
    
    for (int i = 0; input[i] != '\0' && clean_len < MAX_TEXT_LEN - 1; i++) {
        if (is_valid_char(input[i])) {
            clean_text[clean_len++] = input[i];
        }
    }
    clean_text[clean_len] = '\0';
    
    if (!safe_strncpy(tp->processed_text, clean_text, MAX_TEXT_LEN)) return 0;
    tp->total_chars = clean_len;
    
    char temp_text[MAX_TEXT_LEN];
    if (!safe_strncpy(temp_text, clean_text, MAX_TEXT_LEN)) return 0;
    
    char *token = strtok(temp_text, " \t\n.");
    int position = 0;
    
    while (token != NULL && tp->word_count < MAX_WORDS) {
        if (strlen(token) >= MAX_WORD_LEN) {
            token = strtok(NULL, " \t\n.");
            continue;
        }
        
        int found = -1;
        for (int i = 0; i < tp->word_count; i++) {
            if (strcmp(tp->words[i].word, token) == 0) {
                found = i;
                break;
            }
        }
        
        if (found == -1) {
            if (!safe_strncpy(tp->words[tp->word_count].word, token, MAX_WORD_LEN)) {
                token = strtok(NULL, " \t\n.");
                continue;
            }
            tp->words[tp->word_count].count = 1;
            tp->words[tp->word_count].first_position = position;
            tp->words[tp->word_count].last_position = position;
            tp->word_count++;
        } else {
            tp->words[found].count++;
            tp->words[found].last_position = position;
        }
        
        position++;
        token = strtok(NULL, " \t\n.");
    }
    
    tp->total_words = position;
    return 1;
}

void print_statistics(const struct TextProcessor *tp) {
    if (tp == NULL) return;
    
    printf("Processed text: %s\n", tp->processed_text);
    printf("Total characters: %d\n", tp->total_chars);
    printf("Total words: %d\n", tp->total_words);
    printf("\nWord statistics:\n");
    
    for (int i = 0; i < tp->word_count; i++) {
        printf("Word: %s, Count: %d, First: %d, Last: %d\n",
               tp->words[i].word, tp->words[i].count,
               tp->words[i].first_position, tp->words[i].last_position);
    }
}

int main(void) {
    struct TextProcessor processor;
    init_text_processor(&processor);
    
    char input_text[MAX_TEXT_LEN];
    printf("Enter text to process: ");
    
    if (fgets(input_text, MAX_TEXT_LEN, stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_text);
    if (len > 0 && input_text[len - 1] == '\n') {
        input_text[len - 1] = '\0';
    }
    
    if (strlen(input_text) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (!process_text(&processor, input_text)) {
        printf("Error processing text\n");
        return 1;
    }
    
    print_statistics(&processor);
    
    return 0;
}