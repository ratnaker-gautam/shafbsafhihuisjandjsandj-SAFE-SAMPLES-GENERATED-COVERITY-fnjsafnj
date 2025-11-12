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

void print_analysis(int* counts, int total) {
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    
    for (enum LogLevel level = LOG_DEBUG; level <= LOG_CRITICAL; level++) {
        float percentage = total > 0 ? (counts[level] * 100.0f) / total : 0.0f;
        printf("%-8s: %3d entries (%5.1f%%)\n", 
               level_to_string(level), counts[level], percentage);
    }
    
    printf("Total   : %3d entries\n", total);
}

int main(void) {
    struct LogEntry entries[1000];
    int counts[5] = {0};
    int entry_count = 0;
    
    printf("Log Analyzer\n");
    printf("============\n");
    printf("Enter log entries in format: TIMESTAMP LEVEL MESSAGE\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Example: 2024-01-15T10:30:00 INFO User logged in\n");
    printf("Enter 'END' on a blank line to finish input.\n\n");
    
    while (entry_count < 1000) {
        printf("Log entry %d: ", entry_count + 1);
        
        char input[512];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if (strlen(input) == 1 && input[0] == '\n') {
            continue;
        }
        
        if (strncmp(input, "END", 3) == 0 && (input[3] == '\n' || input[3] == '\0')) {
            break;
        }
        
        struct LogEntry entry;
        char level_str[16];
        int result = sscanf(input, "%19s %15s %255[^\n]", 
                           entry.timestamp, level_str, entry.message);
        
        if (result != 3) {
            printf("Invalid format. Please use: TIMESTAMP LEVEL MESSAGE\n");
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (level == -1) {
            printf("Invalid log level. Use: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
            continue;
        }
        
        entry.level = level;
        entries[entry_count] = entry;
        analyze_log_level(entry.level, counts);
        entry_count++;
    }
    
    if (entry_count > 0) {
        print_analysis(counts, entry_count);
        
        printf("\nFilter by level (or 'ALL'): ");
        char filter[16];
        if (fgets(filter, sizeof(filter), stdin) != NULL) {
            filter[strcspn(filter, "\n")] = '\0';
            
            if (strcmp(filter, "ALL") == 0) {
                printf("\nAll log entries:\n");
                for (int i = 0; i < entry_count; i++) {
                    printf("%s %-8s %s\n", 
                           entries[i].timestamp, 
                           level_to_string(entries[i].level), 
                           entries[i].message);
                }
            } else {
                int filter_level = parse_log_level(filter);
                if (filter_level != -1) {
                    printf("\n%s entries:\n", level_to