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

int parse_log_level(const char* level_str) {
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

int read_log_entry(FILE* file, struct LogEntry* entry) {
    char line[512];
    char level_str[16];
    
    if (fgets(line, sizeof(line), file) == NULL) {
        return 0;
    }
    
    line[strcspn(line, "\n")] = '\0';
    
    if (sscanf(line, "%19s %15s %255[^\n]", 
               entry->timestamp, level_str, entry->message) != 3) {
        return 0;
    }
    
    entry->level = parse_log_level(level_str);
    return 1;
}

void analyze_log_entry(const struct LogEntry* entry) {
    printf("Timestamp: %s\n", entry->timestamp);
    printf("Level: %s\n", level_to_string(entry->level));
    
    switch (entry->level) {
        case LOG_INFO:
            printf("Action: Process informational message\n");
            break;
        case LOG_WARNING:
            printf("Action: Investigate potential issue\n");
            break;
        case LOG_ERROR:
            printf("Action: Immediate attention required\n");
            break;
        case LOG_DEBUG:
            printf("Action: Debug information - monitor\n");
            break;
        case LOG_UNKNOWN:
            printf("Action: Unknown level - review manually\n");
            break;
    }
    
    printf("Message: %s\n", entry->message);
    printf("---\n");
}

int main(void) {
    FILE* file;
    char filename[256];
    struct LogEntry entry;
    int entry_count = 0;
    int level_counts[5] = {0};
    
    printf("Enter log filename: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        fprintf(stderr, "Error reading filename\n");
        return 1;
    }
    
    filename[strcspn(filename, "\n")] = '\0';
    
    if (strlen(filename) == 0) {
        fprintf(stderr, "Empty filename\n");
        return 1;
    }
    
    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return 1;
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n\n");
    
    while (read_log_entry(file, &entry)) {
        entry_count++;
        if (entry.level >= 0 && entry.level < 5) {
            level_counts[entry.level]++;
        }
        analyze_log_entry(&entry);
    }
    
    fclose(file);
    
    printf("Summary:\n");
    printf("Total entries: %d\n", entry_count);
    printf("INFO: %d\n", level_counts[LOG_INFO]);
    printf("WARNING: %d\n", level_counts[LOG_WARNING]);
    printf("ERROR: %d\n", level_counts[LOG_ERROR]);
    printf("DEBUG: %d\n", level_counts[LOG_DEBUG]);
    printf("UNKNOWN: %d\n", level_counts[LOG_UNKNOWN]);
    
    return 0;
}