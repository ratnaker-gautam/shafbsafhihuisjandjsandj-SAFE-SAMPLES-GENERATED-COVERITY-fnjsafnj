//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 1000
#define TIMESTAMP_LEN 20
#define LOG_LEVELS 4

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARN 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3

#define IS_VALID_LEVEL(l) ((l) >= 0 && (l) < LOG_LEVELS)

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

const char* level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};
int level_counts[LOG_LEVELS] = {0};
LogEntry entries[MAX_ENTRIES];
int entry_count = 0;

int parse_timestamp(const char* str) {
    if (strlen(str) != 19) return 0;
    for (int i = 0; i < 19; i++) {
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
        printf("Error: Cannot open file %s\n", filename);
        return 0;
    }

    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), file) && entry_count < MAX_ENTRIES) {
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
            len--;
        }

        if (len < TIMESTAMP_LEN + 7) continue;

        char timestamp[TIMESTAMP_LEN];
        strncpy(timestamp, line, TIMESTAMP_LEN-1);
        timestamp[TIMESTAMP_LEN-1] = '\0';

        if (!parse_timestamp(timestamp)) continue;

        const char* level_start = line + TIMESTAMP_LEN + 1;
        char level_str[10];
        int level_len = 0;
        while (level_start[level_len] && level_start[level_len] != ' ' && level_len < 9) {
            level_str[level_len] = level_start[level_len];
            level_len++;
        }
        level_str[level_len] = '\0';

        int level = parse_level(level_str);
        if (!IS_VALID_LEVEL(level)) continue;

        const char* message_start = level_start + level_len + 1;
        if (strlen(message_start) >= sizeof(entries[entry_count].message)) continue;

        strcpy(entries[entry_count].timestamp, timestamp);
        entries[entry_count].level = level;
        strcpy(entries[entry_count].message, message_start);

        level_counts[level]++;
        entry_count++;
    }

    fclose(file);
    return 1;
}

void print_summary() {
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", entry_count);
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d\n", level_names[i], level_counts[i]);
    }
}

void print_entries_by_level(int level) {
    if (!IS_VALID_LEVEL(level)) {
        printf("Invalid log level\n");
        return;
    }

    printf("\n%s entries:\n", level_names[level]);
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].level == level) {
            printf("%s [%s] %s\n", entries[i].timestamp, level_names[level], entries[i].message);
        }
    }
}

int main() {
    char filename[100];
    printf("Enter log file name: ");
    if (scanf("%99s", filename) != 1) {
        printf("Error reading filename\n");
        return 1;
    }

    if (!read_log_file(filename)) {
        return 1;
    }

    if (entry_count == 0) {
        printf("No valid log entries found\n");
        return 0;
    }

    print_summary();

    char command[10];
    while (1) {
        printf("\nCommands: error, warn, info, debug, quit\n");
        printf("Enter command: ");
        if (scanf("%9s", command) != 1) {
            break;
        }

        if (strcmp(command, "quit") == 0) {
            break;
        } else if (strcmp(command, "error") == 0) {
            print_entries_by_level(LOG_LEVEL_ERROR);
        } else if (strcmp(command, "warn") == 0