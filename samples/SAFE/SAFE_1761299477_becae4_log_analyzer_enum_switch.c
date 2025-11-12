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
    if (line == NULL || entry == NULL) return 0;
    
    char level_str[16];
    int parsed = sscanf(line, "%19s %15s %255[^\n]", 
                       entry->timestamp, level_str, entry->message);
    
    if (parsed != 3) return 0;
    
    entry->level = parse_log_level(level_str);
    return 1;
}

void analyze_log_level(enum LogLevel level, int* counts) {
    if (counts == NULL) return;
    
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
        case LOG_UNKNOWN:
            counts[4]++;
            break;
    }
}

void print_analysis(int* counts) {
    if (counts == NULL) return;
    
    printf("\nLog Analysis Summary:\n");
    printf("====================\n");
    printf("INFO:    %d entries\n", counts[0]);
    printf("WARNING: %d entries\n", counts[1]);
    printf("ERROR:   %d entries\n", counts[2]);
    printf("DEBUG:   %d entries\n", counts[3]);
    printf("UNKNOWN: %d entries\n", counts[4]);
    printf("TOTAL:   %d entries\n", 
           counts[0] + counts[1] + counts[2] + counts[3] + counts[4]);
}

int main(void) {
    struct LogEntry entries[100];
    int entry_count = 0;
    int level_counts[5] = {0};
    
    printf("Log Analyzer - Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: INFO, WARNING, ERROR, DEBUG\n");
    printf("Enter 'END' on a separate line to finish input\n");
    printf("Maximum 100 entries allowed\n\n");
    
    char input[512];
    while (entry_count < 100) {
        printf("Entry %d: ", entry_count + 1);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strcmp(input, "END") == 0) {
            break;
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        struct LogEntry entry;
        if (parse_log_entry(input, &entry)) {
            entries[entry_count] = entry;
            analyze_log_level(entry.level, level_counts);
            entry_count++;
        } else {
            printf("Invalid format. Use: YYYY-MM-DD HH:MM:SS LEVEL message\n");
        }
    }
    
    if (entry_count > 0) {
        printf("\nParsed %d log entries:\n", entry_count);
        for (int i = 0; i < entry_count; i++) {
            printf("%d. %s [%s] %s\n", i + 1, 
                   entries[i].timestamp,
                   level_to_string(entries[i].level),
                   entries[i].message);
        }
        
        print_analysis(level_counts);
    } else {
        printf("No valid log entries were provided.\n");
    }
    
    return 0;
}