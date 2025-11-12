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
        printf("%-8s: %d entries\n", level_to_string(level), counts[level]);
    }
    
    int total = 0;
    for (int i = 0; i <= LOG_CRITICAL; i++) {
        total += counts[i];
    }
    printf("Total   : %d entries\n", total);
}

int main() {
    struct LogEntry entries[1000];
    int counts[LOG_CRITICAL + 1] = {0};
    int entry_count = 0;
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Enter 'END' on a separate line to finish input\n");
    printf("Example: 2024-01-15T10:30:00 INFO User logged in\n\n");
    
    while (entry_count < 1000) {
        printf("Log entry %d: ", entry_count + 1);
        
        struct LogEntry entry;
        int result = read_log_entry(stdin, &entry);
        
        if (result == 0) {
            break;
        }
        
        if (result == -1) {
            printf("Invalid format. Please use: TIMESTAMP LEVEL MESSAGE\n");
            char buffer[512];
            fgets(buffer, sizeof(buffer), stdin);
            continue;
        }
        
        if (strcmp(entry.message, "END") == 0) {
            break;
        }
        
        entries[entry_count] = entry;
        analyze_log_level(entry.level, counts);
        entry_count++;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    printf("\n");
    print_analysis(counts);
    
    printf("\nDetailed entries by severity:\n");
    for (enum LogLevel level = LOG_DEBUG; level <= LOG_CRITICAL; level++) {
        if (counts[level] > 0) {
            printf("\n%s entries:\n", level_to_string(level));
            for (int i = 0; i < entry_count; i++) {
                if (entries[i].level == level) {
                    printf("  %s %s %s\n", 
                           entries[i].timestamp, 
                           level_to_string(entries[i].level), 
                           entries[i].message);
                }
            }
        }
    }
    
    return 0;
}