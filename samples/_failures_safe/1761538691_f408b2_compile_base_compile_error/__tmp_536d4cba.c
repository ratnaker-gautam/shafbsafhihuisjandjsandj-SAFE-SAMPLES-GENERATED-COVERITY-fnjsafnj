//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

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
    int capitalized_count;
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->word_count = 0;
    analyzer->total_words = 0;
    analyzer->longest_word_length = 0;
    analyzer->shortest_word_length = MAX_WORD_LEN;
    analyzer->capitalized_count = 0;
}

int is_valid_char(char c) {
    return isalpha((unsigned char)c) || c == ' ' || c == '\n' || c == '\t';
}

int process_word(struct TextAnalyzer *analyzer, const char *word, int length) {
    if (length <= 0 || length >= MAX_WORD_LEN) {
        return 0;
    }

    for (int i = 0; i < length; i++) {
        if (!isalpha((unsigned char)word[i])) {
            return 0;
        }
    }

    int found_index = -1;
    for (int i = 0; i < analyzer->word_count; i++) {
        if (strncmp(analyzer->words[i].word, word, length) == 0 && 
            analyzer->words[i].word[length] == '\0') {
            found_index = i;
            break;
        }
    }

    if (found_index >= 0) {
        if (analyzer->words[found_index].count < INT_MAX) {
            analyzer->words[found_index].count++;
        }
        return 1;
    }

    if (analyzer->word_count >= MAX_WORDS) {
        return 0;
    }

    struct WordStats *new_word = &analyzer->words[analyzer->word_count];
    strncpy(new_word->word, word, length);
    new_word->word[length] = '\0';
    new_word->count = 1;
    new_word->length = length;
    new_word->is_capitalized = isupper((unsigned char)word[0]) ? 1 : 0;

    if (length > analyzer->longest_word_length) {
        analyzer->longest_word_length = length;
    }
    if (length < analyzer->shortest_word_length) {
        analyzer->shortest_word_length = length;
    }
    if (new_word->is_capitalized) {
        analyzer->capitalized_count++;
    }

    analyzer->word_count++;
    return 1;
}

int process_text(struct TextAnalyzer *analyzer, const char *text) {
    if (text == NULL) {
        return 0;
    }

    int text_len = strlen(text);
    if (text_len == 0 || text_len > 10000) {
        return 0;
    }

    for (int i = 0; i < text_len; i++) {
        if (!is_valid_char(text[i])) {
            return 0;
        }
    }

    char buffer[MAX_WORD_LEN];
    int buffer_pos = 0;
    int in_word = 0;

    for (int i = 0; i <= text_len; i++) {
        char c = text[i];
        
        if (isalpha((unsigned char)c)) {
            if (buffer_pos < MAX_WORD_LEN - 1) {
                buffer[buffer_pos++] = c;
            }
            in_word = 1;
        } else if (in_word) {
            buffer[buffer_pos] = '\0';
            if (process_word(analyzer, buffer, buffer_pos)) {
                analyzer->total_words++;
            }
            buffer_pos = 0;
            in_word = 0;
        }
    }

    return analyzer->total_words > 0;
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text Analysis Results:\n");
    printf("Total words: %d\n", analyzer->total_words);
    printf("Unique words: %d\n", analyzer->word_count);
    printf("Longest word length: %d\n", analyzer->longest_word_length);
    printf("Shortest word length: %d\n", analyzer->shortest_word_length);
    printf("Capitalized words: %d\n", analyzer->capitalized_count);
    printf("\nWord frequency:\n");

    for (int i = 0; i < analyzer->word_count; i++) {
        const struct WordStats *word = &analyzer->words[i];
        printf("%s: %d\n", word->word, word->count);
    }
}

int main(void) {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);

    char input_buffer[10001];
    printf("Enter text to analyze (max 10000 characters):\n");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }

    if (!process_text(&analyzer, input_buffer)) {
        fprintf(stderr, "