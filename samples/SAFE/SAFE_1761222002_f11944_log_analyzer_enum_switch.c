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
    if (sscanf(line, "%19s %15[^:]: %255[^\n]", 
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

void analyze_log_level(enum LogLevel level, int count) {
    switch (level) {
        case LOG_DEBUG:
            printf("Debug messages: %d (Low priority diagnostics)\n", count);
            break;
        case LOG_INFO:
            printf("Info messages: %d (Normal operational events)\n", count);
            break;
        case LOG_WARNING:
            printf("Warning messages: %d (Potential issues detected)\n", count);
            break;
        case LOG_ERROR:
            printf("Error messages: %d (Operation failures)\n", count);
            break;
        case LOG_CRITICAL:
            printf("Critical messages: %d (System-threatening events)\n", count);
            break;
        default:
            printf("Unknown level messages: %d\n", count);
            break;
    }
}

int main(void) {
    struct LogEntry entries[100];
    int entry_count = 0;
    int level_counts[5] = {0};
    
    printf("Enter log entries (format: TIMESTAMP LEVEL: MESSAGE)\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
    char line[400];
    while (entry_count < 100) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        struct LogEntry entry;
        if (parse_log_entry(line, &entry)) {
            entries[entry_count] = entry;
            if (entry.level >= LOG_DEBUG && entry.level <= LOG_CRITICAL) {
                level_counts[entry.level]++;
            }
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    printf("\nLog Analysis Results:\n");
    printf("Total entries processed: %d\n\n", entry_count);
    
    printf("Breakdown by severity level:\n");
    for (enum LogLevel level = LOG_DEBUG; level <= LOG_CRITICAL; level++) {
        analyze_log_level(level, level_counts[level]);
    }
    
    printf("\nRecent log entries:\n");
    int start = (entry_count > 5) ? entry_count - 5 : 0;
    for (int i = start; i < entry_count; i++) {
        printf("%s %s: %s\n", 
               entries[i].timestamp, 
               level_to_string(entries[i].level), 
               entries[i].message);
    }
    
    int total_warnings = level_counts[LOG_WARNING] + level_counts[LOG_ERROR] + level_counts[LOG_CRITICAL];
    printf("\nSummary: %d entries require attention\n", total_warnings);
    
    return 0;
}