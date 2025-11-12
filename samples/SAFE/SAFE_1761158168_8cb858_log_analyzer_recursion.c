//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH];
    int severity;
};

int parse_log_level(const char* level) {
    if (strcmp(level, "ERROR") == 0) return 3;
    if (strcmp(level, "WARN") == 0) return 2;
    if (strcmp(level, "INFO") == 0) return 1;
    if (strcmp(level, "DEBUG") == 0) return 0;
    return -1;
}

int read_log_entry(FILE* file, struct LogEntry* entry) {
    char line[MAX_LINE_LENGTH];
    if (fgets(line, sizeof(line), file) == NULL) return 0;
    
    line[strcspn(line, "\n")] = 0;
    
    char* timestamp = strtok(line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "");
    
    if (timestamp == NULL || level == NULL || message == NULL) return 0;
    
    if (strlen(timestamp) >= sizeof(entry->timestamp) ||
        strlen(level) >= sizeof(entry->level) ||
        strlen(message) >= sizeof(entry->message)) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->severity = parse_log_level(level);
    
    return 1;
}

void analyze_log_recursive(FILE* file, int depth, int* error_count, int* warn_count, int* info_count, int* debug_count) {
    if (depth >= MAX_LEVELS) return;
    
    struct LogEntry entry;
    if (!read_log_entry(file, &entry)) return;
    
    switch (entry.severity) {
        case 3: (*error_count)++; break;
        case 2: (*warn_count)++; break;
        case 1: (*info_count)++; break;
        case 0: (*debug_count)++; break;
    }
    
    analyze_log_recursive(file, depth + 1, error_count, warn_count, info_count, debug_count);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Cannot open file %s\n", argv[1]);
        return 1;
    }
    
    int error_count = 0, warn_count = 0, info_count = 0, debug_count = 0;
    
    analyze_log_recursive(file, 0, &error_count, &warn_count, &info_count, &debug_count);
    
    fclose(file);
    
    printf("Log Analysis Results:\n");
    printf("ERROR: %d\n", error_count);
    printf("WARN:  %d\n", warn_count);
    printf("INFO:  %d\n", info_count);
    printf("DEBUG: %d\n", debug_count);
    printf("Total: %d\n", error_count + warn_count + info_count + debug_count);
    
    return 0;
}