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
    char *text;
    int text_length;
};

void init_analyzer(struct TextAnalyzer *analyzer, int initial_capacity) {
    if (initial_capacity <= 0) initial_capacity = 10;
    analyzer->words = malloc(initial_capacity * sizeof(struct WordStats));
    if (!analyzer->words) exit(1);
    analyzer->capacity = initial_capacity;
    analyzer->word_count = 0;
    analyzer->text = NULL;
    analyzer->text_length = 0;
}

void free_analyzer(struct TextAnalyzer *analyzer) {
    if (analyzer->words) free(analyzer->words);
    if (analyzer->text) free(analyzer->text);
}

int is_vowel(char c) {
    c = tolower(c);
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

int is_consonant(char c) {
    c = tolower(c);
    return isalpha(c) && !is_vowel(c);
}

void analyze_word(struct TextAnalyzer *analyzer, const char *word, int len) {
    if (len <= 0 || len >= 64) return;
    if (analyzer->word_count >= analyzer->capacity) {
        int new_cap = analyzer->capacity * 2;
        if (new_cap <= analyzer->capacity) return;
        struct WordStats *new_words = realloc(analyzer->words, new_cap * sizeof(struct WordStats));
        if (!new_words) return;
        analyzer->words = new_words;
        analyzer->capacity = new_cap;
    }
    
    struct WordStats *ws = &analyzer->words[analyzer->word_count];
    strncpy(ws->word, word, len);
    ws->word[len] = '\0';
    ws->length = len;
    ws->vowel_count = 0;
    ws->consonant_count = 0;
    
    for (int i = 0; i < len; i++) {
        if (is_vowel(ws->word[i])) ws->vowel_count++;
        else if (is_consonant(ws->word[i])) ws->consonant_count++;
    }
    
    analyzer->word_count++;
}

void process_text(struct TextAnalyzer *analyzer, const char *input) {
    if (!input) return;
    int input_len = strlen(input);
    if (input_len <= 0) return;
    
    if (analyzer->text) free(analyzer->text);
    analyzer->text = malloc(input_len + 1);
    if (!analyzer->text) return;
    strncpy(analyzer->text, input, input_len);
    analyzer->text[input_len] = '\0';
    analyzer->text_length = input_len;
    
    analyzer->word_count = 0;
    
    int start = -1;
    for (int i = 0; i <= input_len; i++) {
        if (i < input_len && isalpha(input[i])) {
            if (start == -1) start = i;
        } else {
            if (start != -1) {
                int word_len = i - start;
                analyze_word(analyzer, input + start, word_len);
                start = -1;
            }
        }
    }
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    if (!analyzer || analyzer->word_count <= 0) {
        printf("No words to analyze.\n");
        return;
    }
    
    printf("Text analysis results:\n");
    printf("Total words: %d\n", analyzer->word_count);
    printf("\nWord breakdown:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        const struct WordStats *ws = &analyzer->words[i];
        printf("Word %d: '%s' (length: %d, vowels: %d, consonants: %d)\n",
               i + 1, ws->word, ws->length, ws->vowel_count, ws->consonant_count);
    }
}

int main() {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer, 20);
    
    char input[1024];
    printf("Enter text to analyze: ");
    if (!fgets(input, sizeof(input), stdin)) {
        printf("Error reading input.\n");
        free_analyzer(&analyzer);
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided.\n");
        free_analyzer(&analyzer);
        return 1;
    }
    
    process_text(&analyzer, input);
    print_analysis(&analyzer);
    
    free_analyzer(&analyzer);
    return 0;
}