//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum LogLevel {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_CRITICAL
};

struct LogEntry {
    char timestamp[20];
    enum LogLevel level;
    char message[256];
};

int parse_log_level(const char* level_str) {
    if (strcmp(level_str, "DEBUG") == 0) return LOG_DEBUG;
    if (strcmp(level_str, "INFO") == 0) return LOG_INFO;
    if (strcmp(level_str, "WARNING") == 0) return LOG_WARNING;
    if (strcmp(level_str, "ERROR") == 0) return LOG_ERROR;
    if (strcmp(level_str, "CRITICAL") == 0) return LOG_CRITICAL;
    return -1;
}

const char* level_to_string(enum LogLevel level) {
    switch (level) {
        case LOG_DEBUG: return "DEBUG";
        case LOG_INFO: return "INFO";
        case LOG_WARNING: return "WARNING";
        case LOG_ERROR: return "ERROR";
        case LOG_CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

int read_log_entry(FILE* file, struct LogEntry* entry) {
    char line[300];
    char level_str[20];
    
    if (fgets(line, sizeof(line), file) == NULL) {
        return 0;
    }
    
    if (sscanf(line, "%19s %19s %255[^\n]", 
               entry->timestamp, level_str, entry->message) != 3) {
        return -1;
    }
    
    int level = parse_log_level(level_str);
    if (level == -1) {
        return -1;
    }
    
    entry->level = level;
    return 1;
}

void analyze_log_level(enum LogLevel level, int* counts) {
    switch (level) {
        case LOG_DEBUG:
            counts[0]++;
            break;
        case LOG_INFO:
            counts[1]++;
            break;
        case LOG_WARNING:
            counts[2]++;
            break;
        case LOG_ERROR:
            counts[3]++;
            break;
        case LOG_CRITICAL:
            counts[4]++;
            break;
        default:
            break;
    }
}

void print_analysis(int* counts) {
    const char* levels[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    
    printf("Log Level Analysis:\n");
    printf("===================\n");
    
    for (int i = 0; i < 5; i++) {
        printf("%-10s: %d entries\n", levels[i], counts[i]);
    }
    
    int total = 0;
    for (int i = 0; i < 5; i++) {
        total += counts[i];
    }
    
    printf("===================\n");
    printf("Total entries: %d\n", total);
}

int main(void) {
    struct LogEntry entry;
    int level_counts[5] = {0};
    int total_entries = 0;
    int valid_entries = 0;
    
    printf("Log Analyzer - Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Enter 'END' on a line by itself to finish input\n");
    printf("Example: 2024-01-15T10:30:00 INFO User logged in\n\n");
    
    while (1) {
        printf("Enter log entry %d: ", total_entries + 1);
        
        char input[300];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if (strlen(input) > 0 && input[strlen(input) - 1] == '\n') {
            input[strlen(input) - 1] = '\0';
        }
        
        if (strcmp(input, "END") == 0) {
            break;
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        total_entries++;
        
        char timestamp[20];
        char level_str[20];
        char message[256];
        
        if (sscanf(input, "%19s %19s %255[^\n]", timestamp, level_str, message) != 3) {
            printf("Invalid format. Skipping entry.\n");
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (level == -1) {
            printf("Invalid log level: %s. Skipping entry.\n", level_str);
            continue;
        }
        
        strncpy(entry.timestamp, timestamp, sizeof(entry.timestamp) - 1);
        entry.timestamp[sizeof(entry.timestamp) - 1] = '\0';
        entry.level = level;
        strncpy(entry.message, message, sizeof(entry.message) - 1);
        entry.message[sizeof(entry.message) - 1] = '\0';
        
        analyze_log_level(entry.level, level_counts);
        valid_entries++;
        
        printf("Added: %s %s %s\n", entry.timestamp, level_to_string(entry.level), entry.message);
    }
    
    printf("\n");
    
    if (valid_entries > 0) {
        print_analysis(level_counts