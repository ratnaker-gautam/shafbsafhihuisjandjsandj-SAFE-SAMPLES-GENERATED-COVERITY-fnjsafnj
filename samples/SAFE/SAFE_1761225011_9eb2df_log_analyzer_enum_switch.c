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

int read_log_entry(FILE* file, struct LogEntry* entry) {
    char line[512];
    char level_str[16];
    
    if (fgets(line, sizeof(line), file) == NULL) return 0;
    
    line[strcspn(line, "\n")] = 0;
    
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

void analyze_log_levels(int counts[5]) {
    printf("\nLog Level Analysis:\n");
    printf("==================\n");
    
    for (int i = 0; i < 5; i++) {
        switch (i) {
            case LOG_DEBUG:
                printf("DEBUG:    %d entries\n", counts[i]);
                break;
            case LOG_INFO:
                printf("INFO:     %d entries\n", counts[i]);
                break;
            case LOG_WARNING:
                printf("WARNING:  %d entries\n", counts[i]);
                break;
            case LOG_ERROR:
                printf("ERROR:    %d entries\n", counts[i]);
                break;
            case LOG_CRITICAL:
                printf("CRITICAL: %d entries\n", counts[i]);
                break;
        }
    }
}

void process_log_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file '%s'\n", filename);
        return;
    }
    
    struct LogEntry entry;
    int level_counts[5] = {0};
    int total_entries = 0;
    int valid_entries = 0;
    
    printf("Processing log file: %s\n", filename);
    printf("====================\n");
    
    while (read_log_entry(file, &entry)) {
        total_entries++;
        
        switch (entry.level) {
            case LOG_CRITICAL:
            case LOG_ERROR:
                printf("ALERT: %s - %s: %s\n", 
                       entry.timestamp, level_to_string(entry.level), entry.message);
                level_counts[entry.level]++;
                valid_entries++;
                break;
                
            case LOG_WARNING:
                printf("NOTICE: %s - %s: %s\n",
                       entry.timestamp, level_to_string(entry.level), entry.message);
                level_counts[entry.level]++;
                valid_entries++;
                break;
                
            case LOG_INFO:
            case LOG_DEBUG:
                level_counts[entry.level]++;
                valid_entries++;
                break;
        }
    }
    
    fclose(file);
    
    printf("\nSummary:\n");
    printf("========\n");
    printf("Total lines processed: %d\n", total_entries);
    printf("Valid log entries: %d\n", valid_entries);
    
    analyze_log_levels(level_counts);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    process_log_file(argv[1]);
    return 0;
}