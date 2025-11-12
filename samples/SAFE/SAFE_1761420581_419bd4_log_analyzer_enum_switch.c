//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum LogLevel {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_DEBUG,
    LOG_UNKNOWN
};

struct LogEntry {
    char timestamp[20];
    enum LogLevel level;
    char message[256];
};

enum LogLevel parse_log_level(const char* level_str) {
    if (strcmp(level_str, "INFO") == 0) return LOG_INFO;
    if (strcmp(level_str, "WARNING") == 0) return LOG_WARNING;
    if (strcmp(level_str, "ERROR") == 0) return LOG_ERROR;
    if (strcmp(level_str, "DEBUG") == 0) return LOG_DEBUG;
    return LOG_UNKNOWN;
}

const char* level_to_string(enum LogLevel level) {
    switch (level) {
        case LOG_INFO: return "INFO";
        case LOG_WARNING: return "WARNING";
        case LOG_ERROR: return "ERROR";
        case LOG_DEBUG: return "DEBUG";
        default: return "UNKNOWN";
    }
}

void analyze_log_entry(const struct LogEntry* entry) {
    printf("Timestamp: %s\n", entry->timestamp);
    printf("Level: %s\n", level_to_string(entry->level));
    printf("Message: %s\n", entry->message);
    
    switch (entry->level) {
        case LOG_INFO:
            printf("Analysis: Normal operation\n");
            break;
        case LOG_WARNING:
            printf("Analysis: Potential issue detected\n");
            break;
        case LOG_ERROR:
            printf("Analysis: Error condition requiring attention\n");
            break;
        case LOG_DEBUG:
            printf("Analysis: Debug information for developers\n");
            break;
        case LOG_UNKNOWN:
            printf("Analysis: Unrecognized log level\n");
            break;
    }
    printf("---\n");
}

int parse_log_line(const char* line, struct LogEntry* entry) {
    char level_str[16];
    char timestamp[20];
    char message[256];
    
    if (strlen(line) >= 300) return 0;
    
    int parsed = sscanf(line, "%19s %15s %255[^\n]", timestamp, level_str, message);
    if (parsed != 3) return 0;
    
    for (int i = 0; timestamp[i]; i++) {
        if (!isdigit(timestamp[i]) && timestamp[i] != '-' && timestamp[i] != ':' && timestamp[i] != '.') {
            return 0;
        }
    }
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    entry->level = parse_log_level(level_str);
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

int main(void) {
    struct LogEntry entries[10];
    int entry_count = 0;
    char input[512];
    
    printf("Log Analyzer - Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: INFO, WARNING, ERROR, DEBUG\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    printf("Enter 'quit' to finish input\n\n");
    
    while (entry_count < 10) {
        printf("Log entry %d: ", entry_count + 1);
        
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "quit") == 0) break;
        
        if (strlen(input) == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(input, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
        } else {
            printf("Invalid log format. Please use: TIMESTAMP LEVEL MESSAGE\n");
        }
    }
    
    printf("\n=== LOG ANALYSIS REPORT ===\n\n");
    
    int level_counts[5] = {0};
    
    for (int i = 0; i < entry_count; i++) {
        analyze_log_entry(&entries[i]);
        level_counts[entries[i].level]++;
    }
    
    printf("=== SUMMARY ===\n");
    printf("Total entries: %d\n", entry_count);
    printf("INFO: %d\n", level_counts[LOG_INFO]);
    printf("WARNING: %d\n", level_counts[LOG_WARNING]);
    printf("ERROR: %d\n", level_counts[LOG_ERROR]);
    printf("DEBUG: %d\n", level_counts[LOG_DEBUG]);
    printf("UNKNOWN: %d\n", level_counts[LOG_UNKNOWN]);
    
    return 0;
}