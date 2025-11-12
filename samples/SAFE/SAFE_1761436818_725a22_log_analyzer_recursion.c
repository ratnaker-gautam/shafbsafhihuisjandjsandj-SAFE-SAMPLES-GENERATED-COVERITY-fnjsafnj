//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    char level[16];
    char message[256];
    int timestamp;
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char level_buf[16];
    char message_buf[256];
    int timestamp;
    
    int result = sscanf(line, "%d %15s %255[^\n]", &timestamp, level_buf, message_buf);
    
    if (result == 3) {
        entry->timestamp = timestamp;
        strncpy(entry->level, level_buf, sizeof(entry->level) - 1);
        entry->level[sizeof(entry->level) - 1] = '\0';
        strncpy(entry->message, message_buf, sizeof(entry->message) - 1);
        entry->message[sizeof(entry->message) - 1] = '\0';
        return 1;
    }
    
    return 0;
}

void analyze_log_recursive(FILE* file, int depth, int* error_count, int* warning_count, int* info_count) {
    if (depth >= MAX_LEVELS || file == NULL) return;
    
    char line[MAX_LINE_LENGTH];
    if (fgets(line, sizeof(line), file) != NULL) {
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            if (strcmp(entry.level, "ERROR") == 0) {
                (*error_count)++;
            } else if (strcmp(entry.level, "WARNING") == 0) {
                (*warning_count)++;
            } else if (strcmp(entry.level, "INFO") == 0) {
                (*info_count)++;
            }
        }
        
        analyze_log_recursive(file, depth + 1, error_count, warning_count, info_count);
    }
}

void print_analysis(int error_count, int warning_count, int info_count) {
    printf("Log Analysis Results:\n");
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    printf("Total entries: %d\n", error_count + warning_count + info_count);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error: Cannot open file '%s'\n", argv[1]);
        return 1;
    }
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    analyze_log_recursive(file, 0, &error_count, &warning_count, &info_count);
    
    fclose(file);
    
    print_analysis(error_count, warning_count, info_count);
    
    return 0;
}