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

void analyze_log_level(struct LogEntry* entries, int count, enum LogLevel filter_level) {
    int matches = 0;
    
    for (int i = 0; i < count; i++) {
        if (entries[i].level == filter_level) {
            matches++;
            printf("Match %d: %s [%s] %s\n", 
                   matches, entries[i].timestamp, 
                   level_to_string(entries[i].level), 
                   entries[i].message);
        }
    }
    
    printf("Total %s entries: %d\n", level_to_string(filter_level), matches);
}

void count_log_levels(struct LogEntry* entries, int count) {
    int counts[5] = {0};
    
    for (int i = 0; i < count; i++) {
        if (entries[i].level >= LOG_DEBUG && entries[i].level <= LOG_CRITICAL) {
            counts[entries[i].level]++;
        }
    }
    
    printf("Log level summary:\n");
    for (enum LogLevel level = LOG_DEBUG; level <= LOG_CRITICAL; level++) {
        printf("%s: %d\n", level_to_string(level), counts[level]);
    }
}

int main(void) {
    struct LogEntry entries[100];
    int entry_count = 0;
    char input[400];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL: MESSAGE)\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Enter 'END' on a separate line to finish input\n");
    
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
    
    printf("\nAnalyzing %d log entries...\n\n", entry_count);
    
    count_log_levels(entries, entry_count);
    
    printf("\nEnter log level to filter (DEBUG/INFO/WARNING/ERROR/CRITICAL): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    input[strcspn(input, "\n")] = 0;
    
    int filter_level = parse_log_level(input);
    if (filter_level == -1) {
        printf("Invalid log level specified.\n");
        return 1;
    }
    
    printf("\nFiltered entries for %s:\n", level_to_string(filter_level));
    analyze_log_level(entries, entry_count, filter_level);
    
    return 0;
}