//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    char level[16];
    char message[MAX_LINE_LENGTH];
    int timestamp;
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char level_buf[16];
    int timestamp;
    char message_buf[MAX_LINE_LENGTH];
    
    if (sscanf(line, "%d %15s %1023[^\n]", &timestamp, level_buf, message_buf) != 3) {
        return 0;
    }
    
    if (timestamp < 0) return 0;
    if (strlen(level_buf) >= 16) return 0;
    if (strlen(message_buf) >= MAX_LINE_LENGTH) return 0;
    
    strcpy(entry->level, level_buf);
    strcpy(entry->message, message_buf);
    entry->timestamp = timestamp;
    
    return 1;
}

void analyze_log_recursive(FILE* file, int depth, int* error_count, int* warning_count, int* info_count) {
    if (depth >= MAX_LEVELS || !file || !error_count || !warning_count || !info_count) {
        return;
    }
    
    char line[MAX_LINE_LENGTH];
    if (fgets(line, sizeof(line), file) == NULL) {
        return;
    }
    
    line[strcspn(line, "\n")] = '\0';
    
    if (strlen(line) == 0) {
        analyze_log_recursive(file, depth + 1, error_count, warning_count, info_count);
        return;
    }
    
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

int main(void) {
    FILE* file = fopen("sample.log", "r");
    if (!file) {
        printf("Error: Could not open log file\n");
        return 1;
    }
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    analyze_log_recursive(file, 0, &error_count, &warning_count, &info_count);
    
    fclose(file);
    
    printf("Log Analysis Results:\n");
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    printf("Total entries: %d\n", error_count + warning_count + info_count);
    
    return 0;
}