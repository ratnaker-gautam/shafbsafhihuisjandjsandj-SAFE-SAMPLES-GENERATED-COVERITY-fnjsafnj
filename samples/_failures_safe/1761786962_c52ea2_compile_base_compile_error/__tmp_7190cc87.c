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

#define IS_VALID_LEVEL(l) ((l) >= 0 && (l) < LOG_LEVELS)

struct log_entry {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
};

static const char* level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};
static int level_counts[LOG_LEVELS] = {0};
static struct log_entry entries[MAX_ENTRIES];
static int entry_count = 0;

int parse_timestamp(const char* str) {
    if (strlen(str) != TIMESTAMP_LEN - 1) return 0;
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        if (i == 4 || i == 7) {
            if (str[i] != '-') return 0;
        } else if (i == 10) {
            if (str[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (str[i] != ':') return 0;
        } else {
            if (!isdigit(str[i])) return 0;
        }
    }
    return 1;
}

int parse_level(const char* str) {
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strcmp(str, level_names[i]) == 0) {
            return i;
        }
    }
    return -1;
}

int read_log_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return 0;
    }

    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), file) && entry_count < MAX_ENTRIES) {
        if (strlen(line) >= MAX_LINE_LEN - 1) {
            fprintf(stderr, "Warning: Line too long, skipping\n");
            continue;
        }

        char timestamp[TIMESTAMP_LEN];
        char level_str[10];
        char message[MAX_LINE_LEN];

        int parsed = sscanf(line, "%19s %9s %[^\n]", timestamp, level_str, message);
        if (parsed != 3) {
            fprintf(stderr, "Warning: Invalid log format, skipping\n");
            continue;
        }

        if (!parse_timestamp(timestamp)) {
            fprintf(stderr, "Warning: Invalid timestamp, skipping\n");
            continue;
        }

        int level = parse_level(level_str);
        if (!IS_VALID_LEVEL(level)) {
            fprintf(stderr, "Warning: Invalid log level, skipping\n");
            continue;
        }

        strncpy(entries[entry_count].timestamp, timestamp, TIMESTAMP_LEN - 1);
        entries[entry_count].timestamp[TIMESTAMP_LEN - 1] = '\0';
        entries[entry_count].level = level;
        strncpy(entries[entry_count].message, message, sizeof(entries[entry_count].message) - 1);
        entries[entry_count].message[sizeof(entries[entry_count].message) - 1] = '\0';

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
}

void print_entries_by_level(int level) {
    if (!IS_VALID_LEVEL(level)) {
        fprintf(stderr, "Error: Invalid log level\n");
        return;
    }

    printf("\n%s entries:\n", level_names[level]);
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].level == level) {
            printf("%s - %s\n", entries[i].timestamp, entries[i].message);
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }

    if (strlen(argv[1]) == 0) {
        fprintf(stderr, "Error: Empty filename\n");
        return 1;
    }

    if (!read_log_file(argv[1])) {
        return 1;
    }

    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return 0;
    }

    print_summary();

    int level;
    while (1) {
        printf("\nEnter log level to view (ERROR=0, WARN=1, INFO=2, DEBUG=3, -1 to exit