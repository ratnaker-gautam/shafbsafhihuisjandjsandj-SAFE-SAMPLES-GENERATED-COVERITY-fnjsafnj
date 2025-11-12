//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LOG_LEVELS 4

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARN 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3

#define IS_VALID_LEVEL(level) ((level) >= 0 && (level) < LOG_LEVELS)

struct log_entry {
    char timestamp[TIMESTAMP_LEN + 1];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
};

static const char *level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};
static int level_counts[LOG_LEVELS] = {0};
static struct log_entry entries[MAX_ENTRIES];
static int entry_count = 0;

int parse_timestamp(const char *str) {
    if (!str || strlen(str) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (str[i] != '-') return 0;
        } else if (i == 13 || i == 16) {
            if (str[i] != ':') return 0;
        } else if (!isdigit(str[i])) {
            return 0;
        }
    }
    return 1;
}

int parse_log_level(const char *str) {
    if (!str) return -1;
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strcmp(str, level_names[i]) == 0) {
            return i;
        }
    }
    return -1;
}

int read_log_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return 0;
    }

    char line[MAX_LINE_LEN];
    entry_count = 0;
    memset(level_counts, 0, sizeof(level_counts));

    while (fgets(line, sizeof(line), file) && entry_count < MAX_ENTRIES) {
        if (strlen(line) >= MAX_LINE_LEN - 1) {
            fprintf(stderr, "Warning: Line too long, skipping\n");
            continue;
        }

        line[strcspn(line, "\n")] = 0;

        char timestamp[TIMESTAMP_LEN + 1];
        char level_str[10];
        char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];

        if (sscanf(line, "%19s %9s %[^\n]", timestamp, level_str, message) != 3) {
            fprintf(stderr, "Warning: Invalid log format, skipping: %s\n", line);
            continue;
        }

        if (!parse_timestamp(timestamp)) {
            fprintf(stderr, "Warning: Invalid timestamp, skipping: %s\n", timestamp);
            continue;
        }

        int level = parse_log_level(level_str);
        if (!IS_VALID_LEVEL(level)) {
            fprintf(stderr, "Warning: Invalid log level, skipping: %s\n", level_str);
            continue;
        }

        struct log_entry *entry = &entries[entry_count];
        strncpy(entry->timestamp, timestamp, TIMESTAMP_LEN);
        entry->timestamp[TIMESTAMP_LEN] = '\0';
        entry->level = level;
        strncpy(entry->message, message, sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\0';

        level_counts[level]++;
        entry_count++;
    }

    if (ferror(file)) {
        fprintf(stderr, "Error: Reading file failed\n");
        fclose(file);
        return 0;
    }

    fclose(file);
    return 1;
}

void print_summary(void) {
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", entry_count);
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d\n", level_names[i], level_counts[i]);
    }
    printf("\n");
}

void print_entries_by_level(int level) {
    if (!IS_VALID_LEVEL(level)) {
        fprintf(stderr, "Error: Invalid log level\n");
        return;
    }

    printf("Entries with level %s:\n", level_names[level]);
    int count = 0;
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].level == level) {
            printf("%s %s %s\n", entries[i].timestamp, level_names[level], entries[i].message);
            count++;
        }
    }
    if (count == 0) {
        printf("No entries found\n");
    }
    printf("\n");
}

int main(void) {
    char filename[256];
    printf("Enter log file name: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        fprintf(stderr, "Error: Reading input failed\n");
        return 1;
    }
    filename[strcspn(filename, "\n")] = 0;

    if (strlen(filename) == 0) {
        fprintf(stderr, "Error: Empty filename\n");