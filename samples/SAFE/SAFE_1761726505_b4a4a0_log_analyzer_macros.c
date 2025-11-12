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
#define IS_TIMESTAMP_CHAR(c) (isdigit((c)) || (c) == '-' || (c) == ':' || (c) == ' ' || (c) == '.')

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
    int len = strlen(str);
    if (len < TIMESTAMP_LEN - 1) return 0;
    
    for (int i = 0; i < TIMESTAMP_LEN - 1; i++) {
        if (!IS_TIMESTAMP_CHAR(str[i])) return 0;
        timestamp[i] = str[i];
    }
    timestamp[TIMESTAMP_LEN - 1] = '\0';
    return 1;
}

int read_log_entry(FILE *file, LogEntry *entry) {
    if (file == NULL || entry == NULL) return 0;
    
    char line[MAX_LINE_LEN];
    if (fgets(line, sizeof(line), file) == NULL) return 0;
    
    line[strcspn(line, "\n")] = '\0';
    if (strlen(line) == 0) return 0;
    
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
        if (entries[i].level >= 0 && entries[i].level < LOG_LEVELS) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("Log Analysis Results:\n");
    printf("====================\n");
    for (int i = 0; i < LOG_LEVELS; i++) {
        printf("%-8s: %d entries\n", LEVEL_NAME(i), level_counts[i]);
    }
    
    if (count > 0) {
        printf("\nFirst entry: %s %s %s\n", 
               entries[0].timestamp, LEVEL_NAME(entries[0].level), entries[0].message);
        printf("Last entry:  %s %s %s\n", 
               entries[count-1].timestamp, LEVEL_NAME(entries[count-1].level), entries[count-1].message);
    }
}

int main(void) {
    LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Log Analyzer - Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR\n");
    printf("Enter empty line to finish input\n");
    
    while (entry_count < MAX_ENTRIES) {
        printf("Entry %d: ", entry_count + 1);
        
        LogEntry entry;
        if (!read_log_entry(stdin, &entry)) {
            break;
        }
        
        entries[entry_count] = entry;
        entry_count++;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    analyze_logs(entries, entry_count);
    
    return 0;
}