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

void init_processor(struct TextProcessor *proc) {
    proc->word_count = 0;
    proc->processed_text[0] = '\0';
    proc->total_words = 0;
    proc->total_chars = 0;
    for (int i = 0; i < MAX_WORDS; i++) {
        proc->words[i].word[0] = '\0';
        proc->words[i].count = 0;
        proc->words[i].first_position = -1;
        proc->words[i].last_position = -1;
    }
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\t' || c == '\n' || c == '.';
}

int add_word(struct TextProcessor *proc, const char *word, int position) {
    if (proc->word_count >= MAX_WORDS || strlen(word) >= MAX_WORD_LEN) {
        return 0;
    }

    for (int i = 0; i < proc->word_count; i++) {
        if (strcmp(proc->words[i].word, word) == 0) {
            proc->words[i].count++;
            proc->words[i].last_position = position;
            return 1;
        }
    }

    strncpy(proc->words[proc->word_count].word, word, MAX_WORD_LEN - 1);
    proc->words[proc->word_count].word[MAX_WORD_LEN - 1] = '\0';
    proc->words[proc->word_count].count = 1;
    proc->words[proc->word_count].first_position = position;
    proc->words[proc->word_count].last_position = position;
    proc->word_count++;
    return 1;
}

int process_text(struct TextProcessor *proc, const char *input) {
    if (input == NULL || strlen(input) >= MAX_TEXT_LEN) {
        return 0;
    }

    char buffer[MAX_TEXT_LEN];
    strncpy(buffer, input, MAX_TEXT_LEN - 1);
    buffer[MAX_TEXT_LEN - 1] = '\0';

    for (int i = 0; buffer[i] != '\0'; i++) {
        if (!is_valid_char(buffer[i])) {
            return 0;
        }
    }

    strncpy(proc->processed_text, buffer, MAX_TEXT_LEN - 1);
    proc->processed_text[MAX_TEXT_LEN - 1] = '\0';
    proc->total_chars = strlen(proc->processed_text);

    char word[MAX_WORD_LEN];
    int word_len = 0;
    int word_position = 0;

    for (int i = 0; proc->processed_text[i] != '\0'; i++) {
        char c = proc->processed_text[i];
        if (isalnum((unsigned char)c)) {
            if (word_len < MAX_WORD_LEN - 1) {
                word[word_len++] = tolower((unsigned char)c);
            }
        } else {
            if (word_len > 0) {
                word[word_len] = '\0';
                if (!add_word(proc, word, word_position)) {
                    return 0;
                }
                word_position++;
                word_len = 0;
            }
        }
    }

    if (word_len > 0) {
        word[word_len] = '\0';
        if (!add_word(proc, word, word_position)) {
            return 0;
        }
        word_position++;
    }

    proc->total_words = word_position;
    return 1;
}

void print_stats(const struct TextProcessor *proc) {
    printf("Text Statistics:\n");
    printf("Total characters: %d\n", proc->total_chars);
    printf("Total words: %d\n", proc->total_words);
    printf("Unique words: %d\n", proc->word_count);
    printf("\nWord Details:\n");

    for (int i = 0; i < proc->word_count; i++) {
        printf("Word: %-15s Count: %2d  First: %2d  Last: %2d\n",
               proc->words[i].word,
               proc->words[i].count,
               proc->words[i].first_position,
               proc->words[i].last_position);
    }
}

int main() {
    struct TextProcessor processor;
    init_processor(&processor);

    char input[MAX_TEXT_LEN];
    printf("Enter text to analyze (max %d characters):\n", MAX_TEXT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    if (!process_text(&processor, input)) {
        fprintf(stderr, "Error processing text\n");
        return 1