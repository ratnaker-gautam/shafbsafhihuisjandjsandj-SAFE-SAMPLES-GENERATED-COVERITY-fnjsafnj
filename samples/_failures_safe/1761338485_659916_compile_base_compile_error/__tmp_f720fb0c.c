//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_ENTRIES 1000

enum LogLevel {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_DEBUG,
    LOG_UNKNOWN
};

struct LogEntry {
    time_t timestamp;
    enum LogLevel level;
    char message[MAX_LINE_LEN];
};

const char* level_to_string(enum LogLevel level) {
    switch (level) {
        case LOG_INFO: return "INFO";
        case LOG_WARNING: return "WARNING";
        case LOG_ERROR: return "ERROR";
        case LOG_DEBUG: return "DEBUG";
        default: return "UNKNOWN";
    }
}

enum LogLevel string_to_level(const char* str) {
    if (strcmp(str, "INFO") == 0) return LOG_INFO;
    if (strcmp(str, "WARNING") == 0) return LOG_WARNING;
    if (strcmp(str, "ERROR") == 0) return LOG_ERROR;
    if (strcmp(str, "DEBUG") == 0) return LOG_DEBUG;
    return LOG_UNKNOWN;
}

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char level_str[16];
    char timestamp_str[32];
    char message[MAX_LINE_LEN];
    
    if (sscanf(line, "%31s %15s %255[^\n]", timestamp_str, level_str, message) != 3) {
        return 0;
    }
    
    struct tm tm = {0};
    if (strptime(timestamp_str, "%Y-%m-%dT%H:%M:%S", &tm) == NULL) {
        return 0;
    }
    
    entry->timestamp = mktime(&tm);
    if (entry->timestamp == (time_t)-1) {
        return 0;
    }
    
    entry->level = string_to_level(level_str);
    strncpy(entry->message, message, MAX_LINE_LEN - 1);
    entry->message[MAX_LINE_LEN - 1] = '\0';
    
    return 1;
}

void analyze_logs(struct LogEntry* entries, int count) {
    if (!entries || count <= 0) return;
    
    int level_counts[5] = {0};
    time_t earliest = entries[0].timestamp;
    time_t latest = entries[0].timestamp;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].level >= 0 && entries[i].level <= 4) {
            level_counts[entries[i].level]++;
        }
        
        if (entries[i].timestamp < earliest) earliest = entries[i].timestamp;
        if (entries[i].timestamp > latest) latest = entries[i].timestamp;
    }
    
    printf("Log Analysis Summary:\n");
    printf("Total entries: %d\n", count);
    printf("Time range: %s to %s", ctime(&earliest), ctime(&latest));
    printf("Level distribution:\n");
    
    for (enum LogLevel level = LOG_INFO; level <= LOG_UNKNOWN; level++) {
        printf("  %s: %d\n", level_to_string(level), level_counts[level]);
    }
}

void process_log_level(struct LogEntry* entries, int count, enum LogLevel filter_level) {
    if (!entries || count <= 0) return;
    
    printf("\nEntries with level %s:\n", level_to_string(filter_level));
    
    for (int i = 0; i < count; i++) {
        if (entries[i].level == filter_level) {
            printf("  [%s] %s\n", ctime(&entries[i].timestamp), entries[i].message);
        }
    }
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LEN];
    
    printf("Enter log entries (format: YYYY-MM-DDTHH:MM:SS LEVEL message)\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    while (entry_count < MAX_ENTRIES && fgets(line, MAX_LINE_LEN, stdin)) {
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    analyze_logs(entries, entry_count);
    
    int choice;
    printf("\nSelect log level to filter (0=INFO, 1=WARNING, 2=ERROR, 3=DEBUG, 4=UNKNOWN, -1=Exit): ");
    
    while (scanf("%d", &choice) == 1 && choice != -1) {
        if (choice >= 0 && choice <= 4) {
            process_log_level(entries, entry_count, (enum LogLevel)choice);
        } else {
            printf("Invalid choice. Please enter 0-4 or -