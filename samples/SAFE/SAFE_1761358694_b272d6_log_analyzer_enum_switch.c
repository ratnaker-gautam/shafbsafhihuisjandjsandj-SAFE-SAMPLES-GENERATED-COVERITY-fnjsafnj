//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
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

int parse_log_entry(const char* line, struct LogEntry* entry) {
    char level_str[16];
    if (sscanf(line, "%19s %15s %255[^\n]", 
               entry->timestamp, level_str, entry->message) != 3) {
        return 0;
    }
    
    int level = parse_log_level(level_str);
    if (level == -1) {
        return 0;
    }
    entry->level = level;
    return 1;
}

void analyze_log_level(enum LogLevel level, int* counts) {
    if (level >= LOG_DEBUG && level <= LOG_CRITICAL) {
        counts[level]++;
    }
}

void print_analysis(int* counts) {
    printf("Log Level Analysis:\n");
    printf("===================\n");
    
    for (enum LogLevel level = LOG_DEBUG; level <= LOG_CRITICAL; level++) {
        const char* level_name = level_to_string(level);
        printf("%-8s: %d entries\n", level_name, counts[level]);
    }
    printf("\n");
}

void process_log_entry(struct LogEntry* entry, int* counts) {
    switch (entry->level) {
        case LOG_DEBUG:
            printf("DEBUG: %s - %s\n", entry->timestamp, entry->message);
            break;
        case LOG_INFO:
            printf("INFO: %s - %s\n", entry->timestamp, entry->message);
            break;
        case LOG_WARNING:
            printf("WARNING: %s - %s\n", entry->timestamp, entry->message);
            break;
        case LOG_ERROR:
            printf("ERROR: %s - %s\n", entry->timestamp, entry->message);
            break;
        case LOG_CRITICAL:
            printf("CRITICAL: %s - %s\n", entry->timestamp, entry->message);
            break;
        default:
            printf("UNKNOWN: %s - %s\n", entry->timestamp, entry->message);
            break;
    }
    
    analyze_log_level(entry->level, counts);
}

int main(void) {
    struct LogEntry entries[] = {
        {"2024-01-15T10:30:00", LOG_INFO, "System started successfully"},
        {"2024-01-15T10:31:15", LOG_DEBUG, "User login: john_doe"},
        {"2024-01-15T10:32:30", LOG_WARNING, "High memory usage detected"},
        {"2024-01-15T10:33:45", LOG_ERROR, "Database connection failed"},
        {"2024-01-15T10:34:20", LOG_INFO, "Backup process started"},
        {"2024-01-15T10:35:10", LOG_CRITICAL, "Disk full - system unstable"},
        {"2024-01-15T10:36:00", LOG_ERROR, "Failed to write log file"},
        {"2024-01-15T10:37:30", LOG_INFO, "Recovery process initiated"}
    };
    
    int num_entries = sizeof(entries) / sizeof(entries[0]);
    int level_counts[LOG_CRITICAL + 1] = {0};
    
    printf("Processing %d log entries:\n\n", num_entries);
    
    for (int i = 0; i < num_entries; i++) {
        process_log_entry(&entries[i], level_counts);
    }
    
    printf("\n");
    print_analysis(level_counts);
    
    int total_entries = 0;
    for (int i = 0; i <= LOG_CRITICAL; i++) {
        total_entries += level_counts[i];
    }
    
    printf("Total entries processed: %d\n", total_entries);
    
    return 0;
}