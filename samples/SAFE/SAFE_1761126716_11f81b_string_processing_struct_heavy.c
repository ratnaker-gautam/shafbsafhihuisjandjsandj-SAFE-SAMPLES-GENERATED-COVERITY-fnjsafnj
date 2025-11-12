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
    int first_position;
    int last_position;
};

struct TextProcessor {
    struct WordStats words[MAX_WORDS];
    int word_count;
    char processed_text[MAX_TEXT_LEN];
    int total_words;
    int total_chars;
};

void init_text_processor(struct TextProcessor *tp) {
    tp->word_count = 0;
    tp->processed_text[0] = '\0';
    tp->total_words = 0;
    tp->total_chars = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        tp->words[i].word[0] = '\0';
        tp->words[i].count = 0;
        tp->words[i].first_position = -1;
        tp->words[i].last_position = -1;
    }
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

int add_word(struct TextProcessor *tp, const char *word, int position) {
    if (tp->word_count >= MAX_WORDS || strlen(word) >= MAX_WORD_LEN) {
        return 0;
    }

    for (int i = 0; i < tp->word_count; i++) {
        if (strcmp(tp->words[i].word, word) == 0) {
            tp->words[i].count++;
            tp->words[i].last_position = position;
            return 1;
        }
    }

    strncpy(tp->words[tp->word_count].word, word, MAX_WORD_LEN - 1);
    tp->words[tp->word_count].word[MAX_WORD_LEN - 1] = '\0';
    tp->words[tp->word_count].count = 1;
    tp->words[tp->word_count].first_position = position;
    tp->words[tp->word_count].last_position = position;
    tp->word_count++;
    return 1;
}

void process_text(struct TextProcessor *tp, const char *text) {
    if (text == NULL || strlen(text) >= MAX_TEXT_LEN) {
        return;
    }

    strncpy(tp->processed_text, text, MAX_TEXT_LEN - 1);
    tp->processed_text[MAX_TEXT_LEN - 1] = '\0';
    tp->total_chars = strlen(tp->processed_text);

    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    int word_position = 0;

    for (int i = 0; tp->processed_text[i] != '\0'; i++) {
        if (is_word_char(tp->processed_text[i])) {
            if (buf_pos < MAX_WORD_LEN - 1) {
                buffer[buf_pos++] = tolower((unsigned char)tp->processed_text[i]);
            }
        } else {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                if (add_word(tp, buffer, word_position)) {
                    word_position++;
                    tp->total_words++;
                }
                buf_pos = 0;
            }
        }
    }

    if (buf_pos > 0) {
        buffer[buf_pos] = '\0';
        if (add_word(tp, buffer, word_position)) {
            tp->total_words++;
        }
    }
}

void print_statistics(const struct TextProcessor *tp) {
    printf("Text Statistics:\n");
    printf("Total characters: %d\n", tp->total_chars);
    printf("Total words: %d\n", tp->total_words);
    printf("Unique words: %d\n", tp->word_count);
    printf("\nWord Details:\n");

    for (int i = 0; i < tp->word_count; i++) {
        printf("Word: %-15s Count: %2d  First: %2d  Last: %2d\n",
               tp->words[i].word,
               tp->words[i].count,
               tp->words[i].first_position,
               tp->words[i].last_position);
    }
}

int main() {
    struct TextProcessor processor;
    char input_text[MAX_TEXT_LEN];

    printf("Enter text to analyze (max %d characters):\n", MAX_TEXT_LEN - 1);
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strlen(input_text);
    if (len > 0 && input_text[len - 1] == '\n') {
        input_text[len - 1] = '\0';
    }

    if (strlen(input_text) == 0) {
        printf("No input provided.\n");
        return 1;
    }

    init_text_processor(&processor);
    process_text(&processor, input_text);
    print_statistics(&processor);

    return 0;
}