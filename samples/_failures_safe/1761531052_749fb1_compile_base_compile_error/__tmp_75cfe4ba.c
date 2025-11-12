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
    printf("\nLog Level Analysis:\n");
    printf("==================\n");
    
    for (enum LogLevel level = LOG_DEBUG; level <= LOG_CRITICAL; level++) {
        printf("%-8s: %d entries\n", level_to_string(level), counts[level]);
    }
    
    int total = 0;
    for (int i = 0; i <= LOG_CRITICAL; i++) {
        total += counts[i];
    }
    printf("Total   : %d entries\n", total);
}

int main(void) {
    struct LogEntry entries[1000];
    int counts[LOG_CRITICAL + 1] = {0};
    int entry_count = 0;
    
    printf("Log Analyzer\n");
    printf("============\n");
    printf("Enter log entries in format: TIMESTAMP LEVEL MESSAGE\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Enter 'END' to finish input\n\n");
    
    char input[512];
    while (entry_count < 1000) {
        printf("Log entry %d: ", entry_count + 1);
        
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
        
        char timestamp[20];
        char level_str[16];
        char message[256];
        
        if (sscanf(input, "%19s %15s %255[^\n]", 
                   timestamp, level_str, message) != 3) {
            printf("Invalid format. Use: TIMESTAMP LEVEL MESSAGE\n");
            continue;
        }
        
        int level = parse_log_level(level_str);
        if (level == -1) {
            printf("Invalid log level. Use: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
            continue;
        }
        
        if (strlen(timestamp) != 19 || timestamp[4] != '-' || 
            timestamp[7] != '-' || timestamp[10] != ' ' || 
            timestamp[13] != ':' || timestamp[16] != ':') {
            printf("Invalid timestamp format. Use: YYYY-MM-DD HH:MM:SS\n");
            continue;
        }
        
        strcpy(entries[entry_count].timestamp, timestamp);
        entries[entry_count].level = level;
        strcpy(entries[entry_count].message, message);
        
        analyze_log_level(level, counts);
        entry_count++;
    }
    
    if (entry_count > 0) {
        print_analysis(counts);
        
        printf("\nSample entries by level:\n");
        for (enum LogLevel level = LOG_DEBUG; level <= LOG_CRITICAL; level++) {
            if (counts[level] > 0) {
                printf("\n%s entries:\n", level_to_string(level));
                int printed = 0;
                for (int i = 0; i < entry_count && printed < 3; i++) {
                    if (entries[i].level == level) {
                        printf("  %s: %s\n", entries[i].timestamp,