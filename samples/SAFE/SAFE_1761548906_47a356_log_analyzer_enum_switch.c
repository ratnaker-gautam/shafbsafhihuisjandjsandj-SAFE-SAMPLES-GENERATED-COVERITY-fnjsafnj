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

int parse_log_line(const char* line, struct LogEntry* entry) {
    char level_str[16];
    
    if (strlen(line) >= sizeof(entry->timestamp) + sizeof(level_str) + sizeof(entry->message)) {
        return 0;
    }
    
    if (sscanf(line, "%19s %15s %255[^\n]", entry->timestamp, level_str, entry->message) != 3) {
        return 0;
    }
    
    for (int i = 0; entry->timestamp[i]; i++) {
        if (!isdigit(entry->timestamp[i]) && entry->timestamp[i] != '-' && entry->timestamp[i] != ':' && entry->timestamp[i] != '.') {
            return 0;
        }
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
        case LOG_UNKNOWN:
            counts[4]++;
            break;
    }
}

void print_analysis(int* counts) {
    printf("\nLog Analysis Summary:\n");
    printf("====================\n");
    printf("INFO:    %d entries\n", counts[0]);
    printf("WARNING: %d entries\n", counts[1]);
    printf("ERROR:   %d entries\n", counts[2]);
    printf("DEBUG:   %d entries\n", counts[3]);
    printf("UNKNOWN: %d entries\n", counts[4]);
    printf("TOTAL:   %d entries\n", counts[0] + counts[1] + counts[2] + counts[3] + counts[4]);
}

int main(void) {
    struct LogEntry entries[100];
    int entry_count = 0;
    int level_counts[5] = {0};
    char input_line[512];
    
    printf("Log Analyzer - Enter log entries (format: TIMESTAMP LEVEL MESSAGE)\n");
    printf("Supported levels: INFO, WARNING, ERROR, DEBUG\n");
    printf("Enter 'END' on a separate line to finish input\n");
    printf("Maximum 100 entries allowed\n\n");
    
    while (entry_count < 100) {
        printf("Entry %d: ", entry_count + 1);
        
        if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
            break;
        }
        
        input_line[strcspn(input_line, "\n")] = '\0';
        
        if (strcmp(input_line, "END") == 0) {
            break;
        }
        
        if (strlen(input_line) == 0) {
            continue;
        }
        
        if (parse_log_line(input_line, &entries[entry_count])) {
            analyze_log_level(entries[entry_count].level, level_counts);
            entry_count++;
        } else {
            printf("Invalid log format. Please use: YYYY-MM-DD HH:MM:SS LEVEL message\n");
        }
    }
    
    if (entry_count > 0) {
        printf("\nParsed %d log entries:\n", entry_count);
        for (int i = 0; i < entry_count; i++) {
            printf("%d. %s [%s] %s\n", i + 1, entries[i].timestamp, 
                   level_to_string(entries[i].level), entries[i].message);
        }
        
        print_analysis(level_counts);
    } else {
        printf("No valid log entries were provided.\n");
    }
    
    return 0;
}