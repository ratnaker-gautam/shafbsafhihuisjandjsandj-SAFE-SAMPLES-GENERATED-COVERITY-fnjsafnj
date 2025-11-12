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

int validate_timestamp(const char* timestamp) {
    if (strlen(timestamp) != 19) return 0;
    for (int i = 0; i < 19; i++) {
        if (i == 4 || i == 7) {
            if (timestamp[i] != '-') return 0;
        } else if (i == 10) {
            if (timestamp[i] != ' ') return 0;
        } else if (i == 13 || i == 16) {
            if (timestamp[i] != ':') return 0;
        } else {
            if (!isdigit(timestamp[i])) return 0;
        }
    }
    return 1;
}

int read_log_entry(struct LogEntry* entry) {
    char line[512];
    char level_str[16];
    
    if (fgets(line, sizeof(line), stdin) == NULL) return 0;
    
    if (sscanf(line, "%19s %15s %255[^\n]", 
               entry->timestamp, level_str, entry->message) != 3) {
        return 0;
    }
    
    if (!validate_timestamp(entry->timestamp)) return 0;
    
    int level = parse_log_level(level_str);
    if (level == -1) return 0;
    
    entry->level = level;
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

void print_analysis(int* counts) {
    printf("\nLog Analysis Summary:\n");
    printf("====================\n");
    printf("DEBUG:    %d\n", counts[0]);
    printf("INFO:     %d\n", counts[1]);
    printf("WARNING:  %d\n", counts[2]);
    printf("ERROR:    %d\n", counts[3]);
    printf("CRITICAL: %d\n", counts[4]);
    printf("TOTAL:    %d\n", 
           counts[0] + counts[1] + counts[2] + counts[3] + counts[4]);
}

int main(void) {
    struct LogEntry entry;
    int level_counts[5] = {0};
    int total_entries = 0;
    int valid_entries = 0;
    
    printf("Enter log entries (format: YYYY-MM-DD HH:MM:SS LEVEL message)\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Enter empty line to finish.\n\n");
    
    while (1) {
        printf("Log entry %d: ", total_entries + 1);
        
        if (!read_log_entry(&entry)) {
            char line[512];
            if (fgets(line, sizeof(line), stdin) != NULL) {
                if (strlen(line) <= 1) break;
            }
            printf("Invalid log format. Please try again.\n");
            continue;
        }
        
        analyze_log_level(entry.level, level_counts);
        valid_entries++;
        total_entries++;
    }
    
    if (valid_entries > 0) {
        print_analysis(level_counts);
        printf("\nValid entries processed: %d\n", valid_entries);
    } else {
        printf("No valid log entries were processed.\n");
    }
    
    return 0;
}