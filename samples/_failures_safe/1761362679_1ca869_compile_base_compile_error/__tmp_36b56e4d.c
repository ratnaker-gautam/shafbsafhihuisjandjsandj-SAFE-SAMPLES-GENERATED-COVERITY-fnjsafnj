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
    int total_chars;
    int total_words;
    int unique_words;
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_chars = 0;
    analyzer->total_words = 0;
    analyzer->unique_words = 0;
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

int find_word_index(struct TextAnalyzer *analyzer, const char *word) {
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strcmp(analyzer->words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void add_word(struct TextAnalyzer *analyzer, const char *word, int length) {
    if (length <= 0 || length >= MAX_WORD_LEN || analyzer->word_count >= MAX_WORDS) {
        return;
    }
    
    int index = find_word_index(analyzer, word);
    if (index >= 0) {
        analyzer->words[index].count++;
        return;
    }
    
    if (analyzer->word_count < MAX_WORDS) {
        struct WordStats *ws = &analyzer->words[analyzer->word_count];
        strncpy(ws->word, word, MAX_WORD_LEN - 1);
        ws->word[MAX_WORD_LEN - 1] = '\0';
        ws->count = 1;
        ws->length = length;
        ws->is_capitalized = isupper((unsigned char)word[0]) ? 1 : 0;
        analyzer->word_count++;
        analyzer->unique_words++;
    }
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (!text) return;
    
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    int in_word = 0;
    
    for (int i = 0; text[i] != '\0'; i++) {
        char c = text[i];
        
        if (is_word_char(c)) {
            if (buf_pos < MAX_WORD_LEN - 1) {
                buffer[buf_pos++] = c;
            }
            in_word = 1;
        } else {
            if (in_word && buf_pos > 0) {
                buffer[buf_pos] = '\0';
                add_word(analyzer, buffer, buf_pos);
                analyzer->total_words++;
                analyzer->total_chars += buf_pos;
                buf_pos = 0;
            }
            in_word = 0;
        }
    }
    
    if (in_word && buf_pos > 0) {
        buffer[buf_pos] = '\0';
        add_word(analyzer, buffer, buf_pos);
        analyzer->total_words++;
        analyzer->total_chars += buf_pos;
    }
}

void print_analysis(struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Total characters in words: %d\n", analyzer->total_chars);
    printf("Unique words: %d\n", analyzer->unique_words);
    printf("\nWord frequency (appearing more than once):\n");
    
    int printed = 0;
    for (int i = 0; i < analyzer->word_count; i++) {
        if (analyzer->words[i].count > 1) {
            printf("'%s': %d times\n", analyzer->words[i].word, analyzer->words[i].count);
            printed++;
        }
    }
    
    if (printed == 0) {
        printf("No words appear more than once.\n");
    }
    
    printf("\nLongest words:\n");
    int max_len = 0;
    for (int i = 0; i < analyzer->word_count; i++) {
        if (analyzer->words[i].length > max_len) {
            max_len = analyzer->words[i].length;
        }
    }
    
    for (int i = 0; i < analyzer->word_count; i++) {
        if (analyzer->words[i].length == max_len) {
            printf("'%s' (%d characters)\n", analyzer->words[i].word, max_len);
        }
    }
}

int main(void) {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    
    char input_buffer[4096];
    printf("Enter text to analyze (max 4095 characters):\n");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        printf("No input provided.\n");
        return 1;
    }