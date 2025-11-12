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

int parse_log_entry(const char* line, struct LogEntry* entry) {
    char level_str[16];
    int parsed = sscanf(line, "%19s %15s %255[^\n]", 
                       entry->timestamp, level_str, entry->message);
    if (parsed != 3) {
        return 0;
    }
    entry->level = parse_log_level(level_str);
    return 1;
}

void analyze_log_level(enum LogLevel level, int* counts) {
    switch (level) {
        case LOG_INFO:
            counts[0]++;
            break;
        case LOG_WARNING:
            counts[1]++;
            break;
        case LOG_ERROR:
            counts[2]++;
            break;
        case LOG_DEBUG:
            counts[3]++;
            break;
        default:
            counts[4]++;
            break;
    }
}

void print_analysis(int* counts) {
    printf("Log Analysis Results:\n");
    printf("INFO: %d\n", counts[0]);
    printf("WARNING: %d\n", counts[1]);
    printf("ERROR: %d\n", counts[2]);
    printf("DEBUG: %d\n", counts[3]);
    printf("UNKNOWN: %d\n", counts[4]);
    
    int total = counts[0] + counts[1] + counts[2] + counts[3] + counts[4];
    if (total > 0) {
        printf("Total entries: %d\n", total);
        if (counts[2] > 0) {
            printf("ALERT: %d error(s) found!\n", counts[2]);
        }
    }
}

int main() {
    struct LogEntry entries[100];
    int entry_count = 0;
    int level_counts[5] = {0};
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Example: 2024-01-15T10:30:00 INFO System started\n");
    printf("Enter 'END' on a new line to finish input.\n");
    
    char buffer[512];
    while (entry_count < 100) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        buffer[strcspn(buffer, "\n")] = 0;
        
        if (strcmp(buffer, "END") == 0) {
            break;
        }
        
        if (strlen(buffer) == 0) {
            continue;
        }
        
        if (parse_log_entry(buffer, &entries[entry_count])) {
            analyze_log_level(entries[entry_count].level, level_counts);
            entry_count++;
        } else {
            printf("Invalid format: %s\n", buffer);
        }
    }
    
    if (entry_count == 0) {
        printf("No valid log entries to analyze.\n");
        return 1;
    }
    
    printf("\n");
    print_analysis(level_counts);
    
    printf("\nDetailed breakdown:\n");
    for (int i = 0; i < entry_count; i++) {
        printf("%s [%s] %s\n", 
               entries[i].timestamp, 
               level_to_string(entries[i].level),
               entries[i].message);
    }
    
    return 0;
}