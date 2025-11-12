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
    
    if (matches == 0) {
        printf("No entries found.\n");
    } else {
        printf("Total: %d entries\n", matches);
    }
}

int main(void) {
    struct LogEntry entries[100];
    int entry_count = 0;
    char buffer[512];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    
    while (entry_count < 100) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        buffer[strcspn(buffer, "\n")] = '\0';
        
        if (strcmp(buffer, "END") == 0) {
            break;
        }
        
        if (strlen(buffer) == 0) {
            continue;
        }
        
        if (parse_log_entry(buffer, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log entry format: %s\n", buffer);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries provided.\n");
        return 1;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    
    enum LogLevel levels[] = {LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERROR, LOG_CRITICAL};
    for (int i = 0; i < 5; i++) {
        analyze_log_level(entries, entry_count, levels[i]);
        printf("\n");
    }
    
    int level_counts[5] = {0};
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].level >= LOG_DEBUG && entries[i].level <= LOG_CRITICAL) {
            level_counts[entries[i].level]++;
        }
    }
    
    printf("Summary by level:\n");
    for (int i = 0; i < 5; i++) {
        printf("%s: %d entries\n", level_to_string(levels[i]), level_counts[i]);
    }
    
    return 0;
}