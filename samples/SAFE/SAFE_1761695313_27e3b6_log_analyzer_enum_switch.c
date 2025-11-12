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

void print_summary(int* counts, int total) {
    printf("\nLog Analysis Summary:\n");
    printf("====================\n");
    printf("Total entries: %d\n", total);
    printf("DEBUG:    %d\n", counts[0]);
    printf("INFO:     %d\n", counts[1]);
    printf("WARNING:  %d\n", counts[2]);
    printf("ERROR:    %d\n", counts[3]);
    printf("CRITICAL: %d\n", counts[4]);
}

int main(void) {
    struct LogEntry entry;
    int counts[5] = {0};
    int total_entries = 0;
    int valid_entries = 0;
    
    printf("Log Analyzer - Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Enter 'END' on a new line to finish input\n");
    printf("Example: 2024-01-15T10:30:00 INFO User logged in\n\n");
    
    while (1) {
        printf("Log entry %d: ", total_entries + 1);
        
        int result = read_log_entry(stdin, &entry);
        if (result == 0) {
            break;
        }
        
        total_entries++;
        
        if (result == -1) {
            printf("Invalid format. Skipping entry.\n");
            char buffer[512];
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
            continue;
        }
        
        if (strcmp(entry.timestamp, "END") == 0) {
            break;
        }
        
        valid_entries++;
        analyze_log_level(entry.level, counts);
        
        printf("Processed: [%s] %s - %s\n", 
               entry.timestamp, level_to_string(entry.level), entry.message);
    }
    
    if (total_entries > 0) {
        print_summary(counts, valid_entries);
        
        if (valid_entries < total_entries) {
            printf("\nNote: %d entries were invalid and skipped.\n", 
                   total_entries - valid_entries);
        }
    } else {
        printf("No log entries were processed.\n");
    }
    
    return 0;
}