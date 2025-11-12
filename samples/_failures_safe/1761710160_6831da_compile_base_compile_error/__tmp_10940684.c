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
LogEntry entries[MAX_ENTRIES];
int entry_count = 0;

int parse_log_level(const char* str) {
    if (str == NULL) return -1;
    if (strcasecmp(str, "ERROR") == 0) return LOG_LEVEL_ERROR;
    if (strcasecmp(str, "WARN") == 0) return LOG_LEVEL_WARN;
    if (strcasecmp(str, "INFO") == 0) return LOG_LEVEL_INFO;
    if (strcasecmp(str, "DEBUG") == 0) return LOG_LEVEL_DEBUG;
    return -1;
}

int parse_timestamp(const char* str, char* output) {
    if (str == NULL || output == NULL) return 0;
    if (strlen(str) < TIMESTAMP_LEN - 1) return 0;
    
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        char c = str[i];
        if (!(isdigit(c) || c == '-' || c == ':' || c == ' ' || c == '.')) {
            return 0;
        }
    }
    
    strncpy(output, str, TIMESTAMP_LEN - 1);
    output[TIMESTAMP_LEN - 1] = '\0';
    return 1;
}

int parse_log_line(const char* line, LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char level_str[16];
    char timestamp[TIMESTAMP_LEN];
    char message[MAX_LINE_LEN];
    
    if (sscanf(line, "%19s %15s %1023[^\n]", timestamp, level_str, message) != 3) {
        return 0;
    }
    
    if (!parse_timestamp(timestamp, entry->timestamp)) {
        return 0;
    }
    
    int level = parse_log_level(level_str);
    if (!IS_VALID_LEVEL(level)) {
        return 0;
    }
    
    entry->level = level;
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void print_summary(void) {
    printf("Log Analysis Summary:\n");
    printf("====================\n");
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%s: %d\n", level_names[i], level_counts[i]);
    }
    printf("Total entries: %d\n", entry_count);
}

void print_entries_by_level(int level) {
    if (!IS_VALID_LEVEL(level)) return;
    
    printf("\n%s entries:\n", level_names[level]);
    printf("------------\n");
    
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].level == level) {
            printf("%s %s %s\n", entries[i].timestamp, level_names[level], entries[i].message);
        }
    }
}

int main(void) {
    char line[MAX_LINE_LEN];
    int line_num = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Example: 2024-01-15 10:30:00 INFO System started\n");
    printf("Enter empty line to finish.\n");
    
    while (fgets(line, sizeof(line), stdin) != NULL) {
        line_num++;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strlen(line) == 0) {
            break;
        }
        
        if (entry_count >= MAX_ENTRIES) {
            printf("Maximum entries reached (%d)\n", MAX_ENTRIES);
            break;
        }
        
        LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            level_counts[entry.level]++;
            entry_count++;
        } else {
            printf("Invalid log format on line %d: %s\n", line_num, line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 1;
    }
    
    print_summary();
    
    printf("\nSelect level to view (0=ERROR, 1=WARN, 2=INFO, 3=DEBUG, -1=exit): ");
    
    int choice;
    while (scanf("%