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

int read_log_entry(FILE* file, struct LogEntry* entry) {
    char line[512];
    char level_str[16];
    
    if (fgets(line, sizeof(line), file) == NULL) {
        return 0;
    }
    
    if (sscanf(line, "%19s %15s %255[^\n]", 
               entry->timestamp, level_str, entry->message) != 3) {
        return -1;
    }
    
    int level = parse_log_level(level_str);
    if (level == -1) {
        return -1;
    }
    
    entry->level = (enum LogLevel)level;
    return 1;
}

void analyze_log_level(enum LogLevel level, int* counts) {
    if (level >= LOG_DEBUG && level <= LOG_CRITICAL) {
        counts[level]++;
    }
}

void print_analysis(int* counts, int total_entries) {
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    printf("Total entries: %d\n", total_entries);
    
    for (enum LogLevel level = LOG_DEBUG; level <= LOG_CRITICAL; level++) {
        const char* level_name = level_to_string(level);
        int count = counts[level];
        float percentage = total_entries > 0 ? (count * 100.0f) / total_entries : 0.0f;
        printf("%-8s: %3d (%5.1f%%)\n", level_name, count, percentage);
    }
}

int main(void) {
    struct LogEntry entry;
    int level_counts[5] = {0};
    int total_entries = 0;
    int valid_entries = 0;
    
    printf("Log Analyzer\n");
    printf("Enter log entries in format: TIMESTAMP LEVEL MESSAGE\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Enter 'END' on a separate line to finish input\n");
    printf("Example: 2024-01-15T10:30:00 INFO User logged in\n\n");
    
    while (1) {
        printf("Log entry %d: ", total_entries + 1);
        
        if (read_log_entry(stdin, &entry) == 0) {
            break;
        }
        
        total_entries++;
        
        if (strcmp(entry.timestamp, "END") == 0) {
            break;
        }
        
        if (entry.level >= LOG_DEBUG && entry.level <= LOG_CRITICAL) {
            analyze_log_level(entry.level, level_counts);
            valid_entries++;
            printf("  Added: %s %s %s\n", 
                   entry.timestamp, level_to_string(entry.level), entry.message);
        } else {
            printf("  Invalid level - skipping entry\n");
        }
    }
    
    if (total_entries == 0) {
        printf("No log entries provided.\n");
        return 1;
    }
    
    print_analysis(level_counts, valid_entries);
    
    if (valid_entries < total_entries) {
        printf("\nWarning: %d invalid entries were skipped\n", 
               total_entries - valid_entries);
    }
    
    return 0;
}