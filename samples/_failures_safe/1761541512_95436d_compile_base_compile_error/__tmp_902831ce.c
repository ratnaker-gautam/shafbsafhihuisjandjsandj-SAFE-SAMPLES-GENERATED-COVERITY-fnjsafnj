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

int is_valid_timestamp(const char* timestamp) {
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
    
    if (fgets(line, sizeof(line), file) == NULL) {
        return 0;
    }
    
    line[strcspn(line, "\n")] = 0;
    
    if (strcmp(line, "END") == 0) {
        return 0;
    }
    
    if (sscanf(line, "%19s %15s %255[^\n]", 
               entry->timestamp, level_str, entry->message) != 3) {
        return -1;
    }
    
    if (!is_valid_timestamp(entry->timestamp)) {
        return -1;
    }
    
    int level = parse_log_level(level_str);
    if (level == -1) {
        return -1;
    }
    entry->level = level;
    
    return 1;
}

void analyze_log_levels(int counts[5]) {
    printf("\nLog Level Analysis:\n");
    printf("===================\n");
    
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

void display_entries_by_level(struct LogEntry* entries, int count, enum LogLevel filter_level) {
    printf("\nEntries with level %s:\n", level_to_string(filter_level));
    printf("=======================\n");
    
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (entries[i].level == filter_level) {
            printf("%s [%s] %s\n", 
                   entries[i].timestamp, 
                   level_to_string(entries[i].level),
                   entries[i].message);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No entries found for this level.\n");
    }
}

int main(void) {
    struct LogEntry entries[1000];
    int entry_count = 0;
    int level_counts[5] = {0};
    
    printf("Log Analyzer\n");
    printf("============\n");
    printf("Enter log entries in format: YYYY-MM-DD HH:MM:SS LEVEL message\n");
    printf("Supported levels: DEBUG, INFO, WARNING, ERROR, CRITICAL\n");
    printf("Enter 'END' on a separate line to finish input.\n\n");
    
    while (entry_count < 1000) {
        printf("Enter log entry %d: ", entry_count + 1);
        
        struct LogEntry entry;
        int result = read_log_entry(stdin, &entry);
        
        if (result == 0) {
            break;
        }
        
        if (result == -1) {
            printf("Invalid log entry format. Please try again.\n");
            continue;
        }
        
        entries[entry_count] = entry;
        level_counts[entry.level]++;
        entry_count++;
    }
    
    if (entry_count == 0) {