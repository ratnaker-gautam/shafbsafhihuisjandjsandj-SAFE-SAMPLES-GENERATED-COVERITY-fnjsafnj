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

#define IS_VALID_LEVEL(l) ((l) >= 0 && (l) < LOG_LEVELS)
#define LEVEL_NAME(l) (level_names[(l)])
#define IS_EMPTY_LINE(s) ((s)[0] == '\0' || (s)[0] == '\n')

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

const char *level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR"};

int parse_log_level(const char *str) {
    if (str == NULL) return -1;
    if (strncmp(str, "DEBUG", 5) == 0) return 0;
    if (strncmp(str, "INFO", 4) == 0) return 1;
    if (strncmp(str, "WARNING", 7) == 0) return 2;
    if (strncmp(str, "ERROR", 5) == 0) return 3;
    return -1;
}

int parse_timestamp(const char *str, char *timestamp) {
    if (str == NULL || timestamp == NULL) return 0;
    if (strlen(str) < TIMESTAMP_LEN - 1) return 0;
    
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        char c = str[i];
        if (i == 4 || i == 7) {
            if (c != '-') return 0;
        } else if (i == 10) {
            if (c != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (c != ':') return 0;
        } else {
            if (!isdigit(c)) return 0;
        }
    }
    
    strncpy(timestamp, str, TIMESTAMP_LEN - 1);
    timestamp[TIMESTAMP_LEN - 1] = '\0';
    return 1;
}

int parse_log_line(const char *line, LogEntry *entry) {
    if (line == NULL || entry == NULL) return 0;
    if (IS_EMPTY_LINE(line)) return 0;
    
    char temp[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN - 1) return 0;
    strcpy(temp, line);
    
    char *timestamp_part = strtok(temp, " ");
    if (timestamp_part == NULL) return 0;
    
    if (!parse_timestamp(timestamp_part, entry->timestamp)) return 0;
    
    char *level_part = strtok(NULL, " ");
    if (level_part == NULL) return 0;
    
    int level = parse_log_level(level_part);
    if (!IS_VALID_LEVEL(level)) return 0;
    entry->level = level;
    
    char *message_part = strtok(NULL, "\n");
    if (message_part == NULL) return 0;
    
    if (strlen(message_part) >= sizeof(entry->message)) return 0;
    strcpy(entry->message, message_part);
    
    return 1;
}

void analyze_logs(LogEntry *entries, int count) {
    if (entries == NULL || count <= 0) return;
    
    int level_counts[LOG_LEVELS] = {0};
    
    for (int i = 0; i < count; i++) {
        if (IS_VALID_LEVEL(entries[i].level)) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("Log Analysis Summary:\n");
    printf("====================\n");
    printf("Total entries: %d\n", count);
    
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d (%.1f%%)\n", LEVEL_NAME(i), level_counts[i],
               count > 0 ? (level_counts[i] * 100.0 / count) : 0.0);
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (empty line to finish):\n");
    printf("Format: YYYY-MM-DD HH:MM:SS LEVEL message\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR\n\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        if (IS_EMPTY_LINE(line)) break;
        
        line[strcspn(line, "\n")] = '\0';
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
            printf("Added: %s\n", line);
        } else {
            printf("Invalid format: %s\n", line);
        }
    }
    
    if (entry_count > 0) {
        analyze_logs(entries, entry_count);
    } else {
        printf("No valid log entries to analyze.\n");
    }
    
    return 0;
}