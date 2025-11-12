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
    int length;
    int is_capitalized;
};

struct TextProcessor {
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_chars;
    int total_words;
    char processed_text[MAX_TEXT_LEN];
};

void init_text_processor(struct TextProcessor *tp) {
    tp->word_count = 0;
    tp->total_chars = 0;
    tp->total_words = 0;
    memset(tp->processed_text, 0, sizeof(tp->processed_text));
    for (int i = 0; i < MAX_WORDS; i++) {
        memset(tp->words[i].word, 0, sizeof(tp->words[i].word));
        tp->words[i].count = 0;
        tp->words[i].length = 0;
        tp->words[i].is_capitalized = 0;
    }
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

int find_word_index(struct TextProcessor *tp, const char *word) {
    for (int i = 0; i < tp->word_count; i++) {
        if (strcmp(tp->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int add_word(struct TextProcessor *tp, const char *word, int is_capitalized) {
    if (tp->word_count >= MAX_WORDS || strlen(word) >= MAX_WORD_LEN) {
        return -1;
    }
    
    int index = find_word_index(tp, word);
    if (index >= 0) {
        tp->words[index].count++;
        return index;
    }
    
    strncpy(tp->words[tp->word_count].word, word, MAX_WORD_LEN - 1);
    tp->words[tp->word_count].word[MAX_WORD_LEN - 1] = '\0';
    tp->words[tp->word_count].count = 1;
    tp->words[tp->word_count].length = strlen(word);
    tp->words[tp->word_count].is_capitalized = is_capitalized;
    tp->word_count++;
    return tp->word_count - 1;
}

void process_text(struct TextProcessor *tp, const char *input) {
    if (input == NULL || strlen(input) >= MAX_TEXT_LEN) {
        return;
    }
    
    strncpy(tp->processed_text, input, MAX_TEXT_LEN - 1);
    tp->processed_text[MAX_TEXT_LEN - 1] = '\0';
    tp->total_chars = strlen(input);
    
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    int in_word = 0;
    
    for (int i = 0; input[i] != '\0' && i < MAX_TEXT_LEN; i++) {
        if (is_word_char(input[i])) {
            if (!in_word) {
                in_word = 1;
                buf_pos = 0;
            }
            if (buf_pos < MAX_WORD_LEN - 1) {
                buffer[buf_pos++] = input[i];
            }
        } else {
            if (in_word) {
                buffer[buf_pos] = '\0';
                if (buf_pos > 0) {
                    int is_cap = isupper((unsigned char)buffer[0]);
                    add_word(tp, buffer, is_cap);
                    tp->total_words++;
                }
                in_word = 0;
            }
        }
    }
    
    if (in_word && buf_pos > 0) {
        buffer[buf_pos] = '\0';
        int is_cap = isupper((unsigned char)buffer[0]);
        add_word(tp, buffer, is_cap);
        tp->total_words++;
    }
}

void print_statistics(struct TextProcessor *tp) {
    printf("Text Statistics:\n");
    printf("Total characters: %d\n", tp->total_chars);
    printf("Total words: %d\n", tp->total_words);
    printf("Unique words: %d\n", tp->word_count);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < tp->word_count; i++) {
        printf("%-20s: %d occurrences", tp->words[i].word, tp->words[i].count);
        if (tp->words[i].is_capitalized) {
            printf(" (capitalized)");
        }
        printf("\n");
    }
}

int main() {
    struct TextProcessor processor;
    init_text_processor(&processor);
    
    char input[MAX_TEXT_LEN];
    printf("Enter text to analyze (max %d characters):\n", MAX_TEXT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");