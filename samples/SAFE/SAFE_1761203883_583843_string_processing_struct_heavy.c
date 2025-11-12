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
    for (int i = 0; i < MAX_WORDS; i++) {
        analyzer->words[i].count = 0;
        analyzer->words[i].length = 0;
        analyzer->words[i].is_capitalized = 0;
        analyzer->words[i].word[0] = '\0';
    }
}

int is_word_char(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

void add_word(struct TextAnalyzer *analyzer, const char *word, int length) {
    if (length <= 0 || length >= MAX_WORD_LEN || analyzer->word_count >= MAX_WORDS) {
        return;
    }

    for (int i = 0; i < analyzer->word_count; i++) {
        if (strncmp(analyzer->words[i].word, word, length) == 0 && 
            analyzer->words[i].word[length] == '\0') {
            analyzer->words[i].count++;
            return;
        }
    }

    if (analyzer->word_count < MAX_WORDS) {
        struct WordStats *ws = &analyzer->words[analyzer->word_count];
        strncpy(ws->word, word, length);
        ws->word[length] = '\0';
        ws->count = 1;
        ws->length = length;
        ws->is_capitalized = isupper((unsigned char)word[0]) ? 1 : 0;
        analyzer->word_count++;
    }
}

void process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) return;

    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    int text_len = strlen(text);

    for (int i = 0; i <= text_len; i++) {
        char c = text[i];
        if (is_word_char(c) && buf_pos < MAX_WORD_LEN - 1) {
            buffer[buf_pos++] = c;
        } else {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                add_word(analyzer, buffer, buf_pos);
                analyzer->total_words++;
                
                if (buf_pos > analyzer->longest_word_length) {
                    analyzer->longest_word_length = buf_pos;
                }
                if (buf_pos < analyzer->shortest_word_length) {
                    analyzer->shortest_word_length = buf_pos;
                }
                buf_pos = 0;
            }
        }
    }
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words processed: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->word_count);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("Shortest word length: %d\n", analyzer->shortest_word_length);
    printf("\nWord frequency:\n");

    for (int i = 0; i < analyzer->word_count; i++) {
        const struct WordStats *ws = &analyzer->words[i];
        printf("'%s': %d occurrences", ws->word, ws->count);
        if (ws->is_capitalized) {
            printf(" (capitalized)");
        }
        printf("\n");
    }
}

int main(void) {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);

    char input_buffer[4096];
    printf("Enter text to analyze (max 4095 characters):\n");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
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

    process_text(&analyzer, input_buffer);
    print_analysis(&analyzer);

    return 0;
}