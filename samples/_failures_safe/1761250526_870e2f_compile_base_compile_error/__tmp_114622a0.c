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
    analyzer->words = malloc(initial_capacity * sizeof(struct WordStats));
    analyzer->capacity = initial_capacity;
    analyzer->word_count = 0;
    analyzer->text = NULL;
    analyzer->text_length = 0;
}

void free_analyzer(struct TextAnalyzer *analyzer) {
    free(analyzer->words);
    free(analyzer->text);
}

int is_valid_char(char c) {
    return isalpha(c) || c == ' ';
}

int count_vowels(const char *str, int len) {
    int count = 0;
    for (int i = 0; i < len; i++) {
        char c = tolower(str[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            count++;
        }
    }
    return count;
}

int count_consonants(const char *str, int len) {
    int count = 0;
    for (int i = 0; i < len; i++) {
        char c = tolower(str[i]);
        if (isalpha(c) && c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u') {
            count++;
        }
    }
    return count;
}

int add_word(struct TextAnalyzer *analyzer, const char *word, int len) {
    if (len <= 0 || len >= 64) return 0;
    
    if (analyzer->word_count >= analyzer->capacity) {
        int new_capacity = analyzer->capacity * 2;
        if (new_capacity <= analyzer->capacity) return 0;
        struct WordStats *new_words = realloc(analyzer->words, new_capacity * sizeof(struct WordStats));
        if (!new_words) return 0;
        analyzer->words = new_words;
        analyzer->capacity = new_capacity;
    }
    
    struct WordStats *ws = &analyzer->words[analyzer->word_count];
    strncpy(ws->word, word, len);
    ws->word[len] = '\0';
    ws->length = len;
    ws->vowel_count = count_vowels(word, len);
    ws->consonant_count = count_consonants(word, len);
    
    analyzer->word_count++;
    return 1;
}

int process_text(struct TextAnalyzer *analyzer, const char *input) {
    if (!input) return 0;
    
    int input_len = strlen(input);
    if (input_len <= 0 || input_len > 4096) return 0;
    
    for (int i = 0; i < input_len; i++) {
        if (!is_valid_char(input[i])) return 0;
    }
    
    free(analyzer->text);
    analyzer->text = malloc(input_len + 1);
    if (!analyzer->text) return 0;
    strcpy(analyzer->text, input);
    analyzer->text_length = input_len;
    
    analyzer->word_count = 0;
    
    int start = -1;
    for (int i = 0; i <= input_len; i++) {
        if (i < input_len && isalpha(input[i])) {
            if (start == -1) start = i;
        } else {
            if (start != -1) {
                int word_len = i - start;
                if (!add_word(analyzer, input + start, word_len)) {
                    return 0;
                }
                start = -1;
            }
        }
    }
    
    return 1;
}

void print_analysis(const struct TextAnalyzer *analyzer) {
    printf("Text analysis results:\n");
    printf("Total words: %d\n", analyzer->word_count);
    printf("\nWord breakdown:\n");
    
    for (int i = 0; i < analyzer->word_count; i++) {
        const struct WordStats *ws = &analyzer->words[i];
        printf("Word: %-15s Length: %2d  Vowels: %d  Consonants: %d\n",
               ws->word, ws->length, ws->vowel_count, ws->consonant_count);
    }
    
    int total_vowels = 0;
    int total_consonants = 0;
    for (int i = 0; i < analyzer->word_count; i++) {
        total_vowels += analyzer->words[i].vowel_count;
        total_consonants += analyzer->words[i].consonant_count;
    }
    
    printf("\nSummary:\n");
    printf("Total vowels: %d\n", total_vowels);
    printf("Total consonants: %d\n", total_consonants);
    printf("Vowel to consonant ratio: %.2f\n", 
           total_consonants > 0 ? (float)total_vowels / total_consonants : 0.0f);
}

int main() {
    struct TextAnalyzer analyzer;
    init_analyzer(&analyzer, 10);
    
    char input[4097];
    printf("Enter