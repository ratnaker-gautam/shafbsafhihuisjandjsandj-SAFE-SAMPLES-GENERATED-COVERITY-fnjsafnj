//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

const char* level_names[] = {"ERROR", "WARN", "INFO", "DEBUG"};
int level_counts[LOG_LEVELS] = {0};

int parse_log_level(const char* str) {
    if (str == NULL) return -1;
    if (strcasecmp(str, "ERROR") == 0) return LOG_LEVEL_ERROR;
    if (strcasecmp(str, "WARN") == 0) return LOG_LEVEL_WARN;
    if (strcasecmp(str, "INFO") == 0) return LOG_LEVEL_INFO;
    if (strcasecmp(str, "DEBUG") == 0) return LOG_LEVEL_DEBUG;
    return -1;
}

int validate_timestamp(const char* timestamp) {
    if (timestamp == NULL || strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 10) {
            if (timestamp[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int read_log_entries(const char* filename, LogEntry entries[], int max_entries) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Cannot open file %s\n", filename);
        return -1;
    }

    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) != NULL && count < max_entries) {
        if (strlen(line) >= MAX_LINE_LEN - 1) {
            printf("Warning: Line too long, skipping\n");
            continue;
        }

        char timestamp[TIMESTAMP_LEN];
        char level_str[10];
        char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
        
        int parsed = sscanf(line, "%19s %9s %[^\n]", timestamp, level_str, message);
        if (parsed != 3) {
            printf("Warning: Malformed log entry, skipping\n");
            continue;
        }

        if (!validate_timestamp(timestamp)) {
            printf("Warning: Invalid timestamp format, skipping\n");
            continue;
        }

        int level = parse_log_level(level_str);
        if (!IS_VALID_LEVEL(level)) {
            printf("Warning: Unknown log level, skipping\n");
            continue;
        }

        strncpy(entries[count].timestamp, timestamp, TIMESTAMP_LEN - 1);
        entries[count].timestamp[TIMESTAMP_LEN - 1] = '\0';
        entries[count].level = level;
        strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
        entries[count].message[sizeof(entries[count].message) - 1] = '\0';
        
        level_counts[level]++;
        count++;
    }

    if (ferror(file)) {
        printf("Error: Reading file failed\n");
        fclose(file);
        return -1;
    }

    fclose(file);
    return count;
}

void print_summary(int total_entries) {
    printf("\nLog Analysis Summary:\n");
    printf("Total entries: %d\n", total_entries);
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d (%.1f%%)\n", level_names[i], level_counts[i], 
               total_entries > 0 ? (level_counts[i] * 100.0 / total_entries) : 0.0);
    }
}

void print_entries_by_level(LogEntry entries[], int count, int target_level) {
    if (!IS_VALID_LEVEL(target_level)) return;
    
    printf("\n%s entries:\n", level_names[target_level]);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (entries[i].level == target_level) {
            printf("%s - %s\n", entries[i].timestamp, entries[i].message);
            found++;
        }
    }
    if (found == 0) {
        printf("No %s entries found\n", level_names[target_level]);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }

    LogEntry entries[MAX_ENTRIES];
    int