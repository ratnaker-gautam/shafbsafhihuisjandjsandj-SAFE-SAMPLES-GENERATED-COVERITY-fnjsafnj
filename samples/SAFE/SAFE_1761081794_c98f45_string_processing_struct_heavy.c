//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 1000

struct WordInfo {
    char word[MAX_WORD_LEN];
    int length;
    int vowel_count;
    int consonant_count;
};

struct TextStats {
    struct WordInfo words[MAX_WORDS];
    int word_count;
    int total_chars;
    int total_vowels;
    int total_consonants;
};

int is_vowel(char c) {
    c = tolower(c);
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
}

int is_consonant(char c) {
    c = tolower(c);
    return (c >= 'a' && c <= 'z' && !is_vowel(c));
}

void init_text_stats(struct TextStats *stats) {
    stats->word_count = 0;
    stats->total_chars = 0;
    stats->total_vowels = 0;
    stats->total_consonants = 0;
}

int process_word(struct TextStats *stats, const char *word) {
    if (stats->word_count >= MAX_WORDS) return 0;
    
    size_t len = strlen(word);
    if (len == 0 || len >= MAX_WORD_LEN) return 0;
    
    struct WordInfo *info = &stats->words[stats->word_count];
    strncpy(info->word, word, MAX_WORD_LEN - 1);
    info->word[MAX_WORD_LEN - 1] = '\0';
    info->length = (int)len;
    
    info->vowel_count = 0;
    info->consonant_count = 0;
    
    for (int i = 0; i < info->length; i++) {
        if (is_vowel(info->word[i])) {
            info->vowel_count++;
        } else if (is_consonant(info->word[i])) {
            info->consonant_count++;
        }
    }
    
    stats->total_chars += info->length;
    stats->total_vowels += info->vowel_count;
    stats->total_consonants += info->consonant_count;
    stats->word_count++;
    
    return 1;
}

void parse_text(struct TextStats *stats, const char *text) {
    if (!text) return;
    
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    size_t text_len = strlen(text);
    
    for (size_t i = 0; i <= text_len; i++) {
        char c = text[i];
        
        if (isalnum(c) && buf_pos < MAX_WORD_LEN - 1) {
            buffer[buf_pos++] = c;
        } else {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                process_word(stats, buffer);
                buf_pos = 0;
            }
        }
    }
}

void print_stats(const struct TextStats *stats) {
    printf("Text Statistics:\n");
    printf("Total words: %d\n", stats->word_count);
    printf("Total characters: %d\n", stats->total_chars);
    printf("Total vowels: %d\n", stats->total_vowels);
    printf("Total consonants: %d\n", stats->total_consonants);
    printf("\nWord details:\n");
    
    for (int i = 0; i < stats->word_count; i++) {
        const struct WordInfo *info = &stats->words[i];
        printf("Word: %-15s Length: %2d Vowels: %2d Consonants: %2d\n",
               info->word, info->length, info->vowel_count, info->consonant_count);
    }
}

int main(void) {
    struct TextStats stats;
    init_text_stats(&stats);
    
    char input_text[4096];
    printf("Enter text to analyze: ");
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_text);
    if (len > 0 && input_text[len - 1] == '\n') {
        input_text[len - 1] = '\0';
    }
    
    if (strlen(input_text) == 0) {
        fprintf(stderr, "No input provided\n");
        return 1;
    }
    
    parse_text(&stats, input_text);
    
    if (stats.word_count == 0) {
        fprintf(stderr, "No valid words found\n");
        return 1;
    }
    
    print_stats(&stats);
    
    return 0;
}