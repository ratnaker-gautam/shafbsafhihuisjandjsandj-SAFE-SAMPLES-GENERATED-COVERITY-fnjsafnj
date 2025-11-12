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
    int total_chars;
    int total_words;
};

void init_analyzer(struct TextAnalyzer *analyzer, int capacity) {
    analyzer->words = malloc(capacity * sizeof(struct WordStats));
    if (analyzer->words == NULL) {
        exit(EXIT_FAILURE);
    }
    analyzer->capacity = capacity;
    analyzer->word_count = 0;
    analyzer->text = NULL;
    analyzer->total_chars = 0;
    analyzer->total_words = 0;
}

void free_analyzer(struct TextAnalyzer *analyzer) {
    free(analyzer->words);
    free(analyzer->text);
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
    if (analyzer->word_count >= analyzer->capacity || len >= 64) {
        return;
    }
    
    struct WordStats *stats = &analyzer->words[analyzer->word_count];
    strncpy(stats->word, word, len);
    stats->word[len] = '\0';
    stats->length = len;
    stats->vowel_count = 0;
    stats->consonant_count = 0;
    
    for (int i = 0; i < len; i++) {
        if (is_vowel(stats->word[i])) {
            stats->vowel_count++;
        } else if (is_consonant(stats->word[i])) {
            stats->consonant_count++;
        }
    }
    
    analyzer->word_count++;
}

void process_text(struct TextAnalyzer *analyzer, const char *input) {
    if (input == NULL) {
        return;
    }
    
    int len = strlen(input);
    if (len <= 0) {
        return;
    }
    
    analyzer->text = malloc(len + 1);
    if (analyzer->text == NULL) {
        return;
    }
    strncpy(analyzer->text, input, len);
    analyzer->text[len] = '\0';
    analyzer->total_chars = len;
    
    char buffer[64];
    int buf_index = 0;
    
    for (int i = 0; i <= len; i++) {
        if (i < len && isalpha(input[i])) {
            if (buf_index < 63) {
                buffer[buf_index++] = input[i];
            }
        } else {
            if (buf_index > 0) {
                buffer[buf_index] = '\0';
                analyze_word(analyzer, buffer, buf_index);
                analyzer->total_words++;
                buf_index = 0;
            }
        }
    }
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    if (analyzer == NULL || analyzer->word_count == 0) {
        printf("No words to analyze.\n");
        return;
    }
    
    printf("Text Analysis Results:\n");
    printf("Total characters: %d\n", analyzer->total_chars);
    printf("Total words: %d\n", analyzer->total_words);
    printf("\nWord Statistics:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        const struct WordStats *stats = &analyzer->words[i];
        printf("Word: %-15s Length: %2d  Vowels: %d  Consonants: %d\n",
               stats->word, stats->length, stats->vowel_count, stats->consonant_count);
    }
}

int main() {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer, 100);
    
    char input[1024];
    printf("Enter text to analyze: ");
    
    if (fgets(input, sizeof(input), stdin) != NULL) {
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        process_text(&analyzer, input);
        print_analysis(&analyzer);
    } else {
        printf("Error reading input.\n");
    }
    
    free_analyzer(&analyzer);
    return 0;
}