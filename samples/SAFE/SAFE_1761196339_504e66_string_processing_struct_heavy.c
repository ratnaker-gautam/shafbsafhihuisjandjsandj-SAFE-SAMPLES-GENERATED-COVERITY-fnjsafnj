//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct WordStats {
    char word[64];
    int length;
    int vowel_count;
    int consonant_count;
};

struct TextAnalyzer {
    struct WordStats *words;
    int word_count;
    int capacity;
    char text[1024];
};

void init_analyzer(struct TextAnalyzer *analyzer) {
    analyzer->words = NULL;
    analyzer->word_count = 0;
    analyzer->capacity = 0;
    analyzer->text[0] = '\0';
}

void free_analyzer(struct TextAnalyzer *analyzer) {
    if (analyzer->words != NULL) {
        free(analyzer->words);
        analyzer->words = NULL;
    }
    analyzer->word_count = 0;
    analyzer->capacity = 0;
}

int is_vowel(char c) {
    c = tolower(c);
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

int is_consonant(char c) {
    c = tolower(c);
    return isalpha(c) && !is_vowel(c);
}

int resize_words_array(struct TextAnalyzer *analyzer, int new_capacity) {
    if (new_capacity <= analyzer->capacity) return 1;
    struct WordStats *new_words = realloc(analyzer->words, new_capacity * sizeof(struct WordStats));
    if (new_words == NULL) return 0;
    analyzer->words = new_words;
    analyzer->capacity = new_capacity;
    return 1;
}

int add_word(struct TextAnalyzer *analyzer, const char *word) {
    if (word == NULL || strlen(word) == 0) return 0;
    if (analyzer->word_count >= analyzer->capacity) {
        int new_cap = (analyzer->capacity == 0) ? 16 : analyzer->capacity * 2;
        if (!resize_words_array(analyzer, new_cap)) return 0;
    }
    struct WordStats *ws = &analyzer->words[analyzer->word_count];
    size_t len = strlen(word);
    if (len >= sizeof(ws->word)) len = sizeof(ws->word) - 1;
    strncpy(ws->word, word, len);
    ws->word[len] = '\0';
    ws->length = (int)len;
    ws->vowel_count = 0;
    ws->consonant_count = 0;
    for (int i = 0; i < ws->length; i++) {
        if (is_vowel(ws->word[i])) ws->vowel_count++;
        else if (is_consonant(ws->word[i])) ws->consonant_count++;
    }
    analyzer->word_count++;
    return 1;
}

int parse_text(struct TextAnalyzer *analyzer, const char *input) {
    if (input == NULL) return 0;
    size_t input_len = strlen(input);
    if (input_len >= sizeof(analyzer->text)) return 0;
    strncpy(analyzer->text, input, sizeof(analyzer->text) - 1);
    analyzer->text[sizeof(analyzer->text) - 1] = '\0';
    analyzer->word_count = 0;
    char buffer[128];
    int buf_index = 0;
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && isalpha(input[i])) {
            if (buf_index < (int)sizeof(buffer) - 1) {
                buffer[buf_index++] = input[i];
            }
        } else {
            if (buf_index > 0) {
                buffer[buf_index] = '\0';
                if (!add_word(analyzer, buffer)) return 0;
                buf_index = 0;
            }
        }
    }
    return 1;
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text: \"%s\"\n", analyzer->text);
    printf("Words found: %d\n", analyzer->word_count);
    for (int i = 0; i < analyzer->word_count; i++) {
        const struct WordStats *ws = &analyzer->words[i];
        printf("Word %d: \"%s\" (length: %d, vowels: %d, consonants: %d)\n",
               i + 1, ws->word, ws->length, ws->vowel_count, ws->consonant_count);
    }
}

int main(void) {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer);
    char input[1024];
    printf("Enter text to analyze: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        free_analyzer(&analyzer);
        return 1;
    }
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    if (!parse_text(&analyzer, input)) {
        printf("Error processing text\n");
        free_analyzer(&analyzer);
        return 1;
    }
    print_analysis(&analyzer);
    free_analyzer(&analyzer);
    return 0;
}