//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

typedef struct {
    char timestamp[32];
    char level[16];
    char message[MAX_LINE_LENGTH];
} LogEntry;

int parse_log_level(const char* level_str) {
    if (strcmp(level_str, "DEBUG") == 0) return 0;
    if (strcmp(level_str, "INFO") == 0) return 1;
    if (strcmp(level_str, "WARNING") == 0) return 2;
    if (strcmp(level_str, "ERROR") == 0) return 3;
    if (strcmp(level_str, "CRITICAL") == 0) return 4;
    return -1;
}

void print_log_entry(const LogEntry* entry, int indent_level) {
    if (entry == NULL || indent_level < 0 || indent_level >= MAX_LEVELS) return;
    
    for (int i = 0; i < indent_level; i++) {
        printf("  ");
    }
    
    printf("[%s] %s: %s\n", entry->timestamp, entry->level, entry->message);
}

int process_log_recursive(FILE* file, int current_depth, int max_depth) {
    if (file == NULL || current_depth < 0 || current_depth >= max_depth) return 0;
    
    char line[MAX_LINE_LENGTH];
    if (fgets(line, sizeof(line), file) == NULL) return 0;
    
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0';
    }
    
    if (strlen(line) == 0) return process_log_recursive(file, current_depth, max_depth);
    
    LogEntry entry;
    memset(&entry, 0, sizeof(entry));
    
    char* timestamp = strtok(line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "");
    
    if (timestamp != NULL && level != NULL && message != NULL) {
        strncpy(entry.timestamp, timestamp, sizeof(entry.timestamp) - 1);
        strncpy(entry.level, level, sizeof(entry.level) - 1);
        strncpy(entry.message, message, sizeof(entry.message) - 1);
        
        int level_value = parse_log_level(entry.level);
        if (level_value >= 2) {
            print_log_entry(&entry, current_depth);
        }
        
        if (level_value == 3 || level_value == 4) {
            int processed = process_log_recursive(file, current_depth + 1, max_depth);
            if (processed > 0) {
                return processed + 1;
            }
        }
    }
    
    return 1;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    const char* filename = argv[1];
    if (filename == NULL || strlen(filename) == 0) {
        fprintf(stderr, "Invalid filename\n");
        return 1;
    }
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return 1;
    }
    
    printf("Processing log file: %s\n", filename);
    printf("Showing WARNING and higher level messages:\n");
    printf("==========================================\n");
    
    int total_processed = 0;
    while (!feof(file)) {
        total_processed += process_log_recursive(file, 0, MAX_LEVELS);
    }
    
    printf("==========================================\n");
    printf("Total log entries processed: %d\n", total_processed);
    
    fclose(file);
    return 0;
}