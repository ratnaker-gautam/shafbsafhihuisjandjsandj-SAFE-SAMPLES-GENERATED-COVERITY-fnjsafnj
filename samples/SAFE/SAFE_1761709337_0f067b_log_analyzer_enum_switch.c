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
    
    if (sscanf(line, "%19s %15s %255[^\n]", 
               entry->timestamp, level_str, entry->message) != 3) {
        return -1;
    }
    
    entry->level = parse_log_level(level_str);
    return 1;
}

void process_log_entry(const struct LogEntry* entry, int* counts) {
    if (entry->level >= LOG_INFO && entry->level <= LOG_DEBUG) {
        counts[entry->level]++;
    } else {
        counts[LOG_UNKNOWN]++;
    }
    
    switch (entry->level) {
        case LOG_ERROR:
            printf("CRITICAL: %s - %s\n", entry->timestamp, entry->message);
            break;
        case LOG_WARNING:
            printf("Attention: %s - %s\n", entry->timestamp, entry->message);
            break;
        case LOG_INFO:
            printf("Normal: %s - %s\n", entry->timestamp, entry->message);
            break;
        case LOG_DEBUG:
            break;
        default:
            printf("Unrecognized: %s - %s\n", entry->timestamp, entry->message);
            break;
    }
}

int main(void) {
    FILE* file;
    struct LogEntry entry;
    int level_counts[5] = {0};
    int total_entries = 0;
    char filename[256];
    
    printf("Enter log file name: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        fprintf(stderr, "Error reading filename\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        fprintf(stderr, "Invalid filename\n");
        return 1;
    }
    
    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return 1;
    }
    
    printf("\nProcessing log file: %s\n\n", filename);
    
    int result;
    while ((result = read_log_entry(file, &entry)) > 0) {
        process_log_entry(&entry, level_counts);
        total_entries++;
    }
    
    if (result < 0) {
        fprintf(stderr, "Warning: Some log entries may be malformed\n");
    }
    
    fclose(file);
    
    printf("\nLog Analysis Summary:\n");
    printf("Total entries processed: %d\n", total_entries);
    printf("INFO: %d\n", level_counts[LOG_INFO]);
    printf("WARNING: %d\n", level_counts[LOG_WARNING]);
    printf("ERROR: %d\n", level_counts[LOG_ERROR]);
    printf("DEBUG: %d\n", level_counts[LOG_DEBUG]);
    printf("UNKNOWN: %d\n", level_counts[LOG_UNKNOWN]);
    
    return 0;
}