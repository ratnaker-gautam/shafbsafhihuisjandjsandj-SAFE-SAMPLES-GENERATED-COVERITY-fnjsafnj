//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 1000

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int length;
    int is_capitalized;
};

struct TextAnalyzer {
    struct WordStats words[MAX_WORDS];
    int word_count;
    int total_words;
    int longest_word_length;
    int shortest_word_length;
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_words = 0;
    analyzer->longest_word_length = 0;
    analyzer->shortest_word_length = MAX_WORD_LEN;
}

int is_valid_char(char c) {
    return isalpha((unsigned char)c) || c == ' ' || c == '\n' || c == '\t';
}

int process_word(struct TextAnalyzer *analyzer, const char *word) {
    if (analyzer->word_count >= MAX_WORDS) {
        return 0;
    }
    
    size_t len = strlen(word);
    if (len == 0 || len >= MAX_WORD_LEN) {
        return 0;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (!isalpha((unsigned char)word[i])) {
            return 0;
        }
    }
    
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            analyzer->words[i].count++;
            return 1;
        }
    }
    
    struct WordStats *ws = &analyzer->words[analyzer->word_count];
    strncpy(ws->word, word, MAX_WORD_LEN - 1);
    ws->word[MAX_WORD_LEN - 1] = '\0';
    ws->count = 1;
    ws->length = (int)len;
    ws->is_capitalized = isupper((unsigned char)word[0]) ? 1 : 0;
    
    analyzer->word_count++;
    
    if (len > (size_t)analyzer->longest_word_length) {
        analyzer->longest_word_length = (int)len;
    }
    if (len < (size_t)analyzer->shortest_word_length) {
        analyzer->shortest_word_length = (int)len;
    }
    
    return 1;
}

int parse_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) {
        return 0;
    }
    
    size_t text_len = strlen(text);
    if (text_len == 0 || text_len > 10000) {
        return 0;
    }
    
    for (size_t i = 0; i < text_len; i++) {
        if (!is_valid_char(text[i])) {
            return 0;
        }
    }
    
    char buffer[MAX_WORD_LEN];
    size_t buf_pos = 0;
    
    for (size_t i = 0; i <= text_len; i++) {
        char c = text[i];
        
        if (isalpha((unsigned char)c)) {
            if (buf_pos < MAX_WORD_LEN - 1) {
                buffer[buf_pos++] = c;
            }
        } else if (buf_pos > 0) {
            buffer[buf_pos] = '\0';
            if (process_word(analyzer, buffer)) {
                analyzer->total_words++;
            }
            buf_pos = 0;
        }
    }
    
    return 1;
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    if (analyzer->total_words == 0) {
        printf("No words found in text.\n");
        return;
    }
    
    printf("Text Analysis Results:\n");
    printf("Total words processed: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->word_count);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("Shortest word length: %d\n", analyzer->shortest_word_length);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        const struct WordStats *ws = &analyzer->words[i];
        printf("'%s': count=%d, length=%d, capitalized=%d\n", 
               ws->word, ws->count, ws->length, ws->is_capitalized);
    }
}

int main(void) {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input_text[10001];
    printf("Enter text to analyze (max 10000 characters):\n");
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        fprintf(stderr, "Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input_text);
    if (len > 0 && input_text[len - 1] == '\n') {
        input_text[len - 1] = '\0';
    }
    
    if (!parse_text(&analyzer, input_text)) {
        fprintf(stderr, "Invalid input text.\n");
        return 1;
    }
    
    print_analysis(&analyzer);
    
    return 0;
}