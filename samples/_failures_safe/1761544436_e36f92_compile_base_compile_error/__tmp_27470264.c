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
        printf("%-6s: %d entries\n", level_names[i], level_counts[i]);
    }
    printf("Total : %d entries\n", entry_count);
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
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: ERROR, WARN, INFO, DEBUG\n");
    printf("Enter 'quit' to finish input\n\n");
    
    while (entry_count < MAX_ENTRIES) {
        printf("Log entry %d: ", entry_count + 1);
        
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "quit") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        if (entry_count < MAX_ENTRIES) {
            if (parse_log_line(line, &entries[entry_count])) {
                level_counts[entries[entry_count].level]++;
                entry_count++;
            } else {
                printf("Invalid log format. Use: YYYY-MM-DD HH:MM:SS LEVEL message\n");
            }
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    printf("\n");
    print_summary();
    
    int choice;
    do {
        printf("\nOptions:\n");
        printf("1. Show ERROR entries\n");
        printf("2. Show WARN entries\n");
        printf("3. Show INFO entries\n");
        printf