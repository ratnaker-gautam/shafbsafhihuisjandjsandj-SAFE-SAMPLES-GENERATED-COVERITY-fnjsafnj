//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: log_analyzer
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
#define IS_TIMESTAMP_CHAR(c) (isdigit((c)) || (c) == '-' || (c) == ':' || (c) == ' ')

static const char *level_names[] = {"DEBUG", "INFO", "WARNING", "ERROR"};

typedef struct {
    char timestamp[TIMESTAMP_LEN];
    int level;
    char message[MAX_LINE_LEN - TIMESTAMP_LEN - 10];
} LogEntry;

int parse_log_level(const char *str) {
    if (str == NULL) return -1;
    for (int i = 0; i < LOG_LEVELS; i++) {
        if (strcmp(str, level_names[i]) == 0) return i;
    }
    return -1;
}

int parse_timestamp(const char *str, char *timestamp) {
    if (str == NULL || timestamp == NULL) return 0;
    int len = 0;
    while (len < TIMESTAMP_LEN - 1 && str[len] != '\0' && IS_TIMESTAMP_CHAR(str[len])) {
        timestamp[len] = str[len];
        len++;
    }
    if (len == 0) return 0;
    timestamp[len] = '\0';
    return 1;
}

int read_log_entry(FILE *file, LogEntry *entry) {
    if (file == NULL || entry == NULL) return 0;
    char line[MAX_LINE_LEN];
    if (fgets(line, sizeof(line), file) == NULL) return 0;
    
    size_t len = strlen(line);
    if (len > 0 && line[len-1] == '\n') line[len-1] = '\0';
    
    char timestamp[TIMESTAMP_LEN];
    char level_str[16];
    char message[MAX_LINE_LEN];
    
    if (sscanf(line, "%19s %15s %1023[^\n]", timestamp, level_str, message) != 3) {
        return 0;
    }
    
    if (!parse_timestamp(timestamp, entry->timestamp)) return 0;
    
    int level = parse_log_level(level_str);
    if (!IS_VALID_LEVEL(level)) return 0;
    entry->level = level;
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
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
    
    printf("Log Analyzer - Enter log entries (timestamp LEVEL message)\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR\n");
    printf("Example: 2024-01-15 10:30:00 INFO System started\n");
    printf("Enter 'END' on a separate line to finish input\n\n");
    
    while (entry_count < MAX_ENTRIES) {
        printf("Log entry %d: ", entry_count + 1);
        
        LogEntry entry;
        if (!read_log_entry(stdin, &entry)) {
            char buffer[MAX_LINE_LEN];
            if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                buffer[strcspn(buffer, "\n")] = '\0';
                if (strcmp(buffer, "END") == 0) break;
            }
            printf("Invalid log format. Please use: TIMESTAMP LEVEL MESSAGE\n");
            continue;
        }
        
        entries[entry_count] = entry;
        entry_count++;
    }
    
    if (entry_count > 0) {
        analyze_logs(entries, entry_count);
        
        printf("\nRecent entries:\n");
        int start = entry_count > 5 ? entry_count - 5 : 0;
        for (int i = start; i < entry_count; i++) {
            printf("%s %s %s\n", entries[i].timestamp, LEVEL_NAME(entries[i].level), entries[i].message);
        }
    } else {
        printf("No valid log entries provided.\n");
    }
    
    return 0;
}