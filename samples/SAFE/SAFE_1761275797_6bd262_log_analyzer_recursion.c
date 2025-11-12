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
    
    if (strlen(level_buf) >= 16 || strlen(message_buf) >= MAX_LINE_LENGTH) {
        return 0;
    }
    
    strncpy(entry->level, level_buf, 15);
    entry->level[15] = '\0';
    strncpy(entry->message, message_buf, MAX_LINE_LENGTH - 1);
    entry->message[MAX_LINE_LENGTH - 1] = '\0';
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
    
    line[MAX_LINE_LENGTH - 1] = '\0';
    
    struct LogEntry entry;
    if (parse_log_line(line, &entry)) {
        if (strcmp(entry.level, "ERROR") == 0) {
            (*error_count)++;
        } else if (strcmp(entry.level, "WARNING") == 0) {
            (*warning_count)++;
        } else if (strcmp(entry.level, "INFO") == 0) {
            (*info_count)++;
        }
        
        printf("Depth %d: %s - %s\n", depth, entry.level, entry.message);
    }
    
    analyze_log_recursive(file, depth + 1, error_count, warning_count, info_count);
}

int main() {
    printf("Log Analyzer - Enter log entries (format: timestamp LEVEL message)\n");
    printf("Supported levels: ERROR, WARNING, INFO\n");
    printf("Enter 'END' on a separate line to finish input\n\n");
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    printf("Processing log entries recursively:\n");
    
    char input_line[MAX_LINE_LENGTH];
    while (1) {
        if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
            break;
        }
        
        input_line[MAX_LINE_LENGTH - 1] = '\0';
        
        if (strncmp(input_line, "END", 3) == 0 && (input_line[3] == '\n' || input_line[3] == '\0')) {
            break;
        }
        
        struct LogEntry entry;
        if (parse_log_line(input_line, &entry)) {
            if (strcmp(entry.level, "ERROR") == 0) {
                error_count++;
            } else if (strcmp(entry.level, "WARNING") == 0) {
                warning_count++;
            } else if (strcmp(entry.level, "INFO") == 0) {
                info_count++;
            }
            
            printf("Parsed: %s - %s\n", entry.level, entry.message);
        } else {
            printf("Invalid log format: %s", input_line);
        }
    }
    
    printf("\nLog Analysis Summary:\n");
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    printf("Total entries: %d\n", error_count + warning_count + info_count);
    
    return 0;
}