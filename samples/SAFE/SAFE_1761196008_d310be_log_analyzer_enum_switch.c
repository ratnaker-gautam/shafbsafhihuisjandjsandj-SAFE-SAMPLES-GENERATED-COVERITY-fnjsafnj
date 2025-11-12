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

void analyze_log_level(struct LogEntry* entries, int count, enum LogLevel target_level) {
    int matches = 0;
    printf("Entries with level %s:\n", level_to_string(target_level));
    
    for (int i = 0; i < count; i++) {
        if (entries[i].level == target_level) {
            printf("%s %s %s\n", entries[i].timestamp, 
                   level_to_string(entries[i].level), entries[i].message);
            matches++;
        }
    }
    printf("Total: %d entries\n\n", matches);
}

int main(void) {
    struct LogEntry entries[100];
    int entry_count = 0;
    char input[400];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (entry_count < 100) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "END") == 0) {
            break;
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        if (parse_log_entry(input, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log entry format: %s\n", input);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    
    for (enum LogLevel level = LOG_DEBUG; level <= LOG_CRITICAL; level++) {
        analyze_log_level(entries, entry_count, level);
    }
    
    int error_count = 0;
    int warning_count = 0;
    for (int i = 0; i < entry_count; i++) {
        switch (entries[i].level) {
            case LOG_ERROR:
            case LOG_CRITICAL:
                error_count++;
                break;
            case LOG_WARNING:
                warning_count++;
                break;
            default:
                break;
        }
    }
    
    printf("Summary:\n");
    printf("Total entries: %d\n", entry_count);
    printf("Errors/Critical: %d\n", error_count);
    printf("Warnings: %d\n", warning_count);
    
    return 0;
}