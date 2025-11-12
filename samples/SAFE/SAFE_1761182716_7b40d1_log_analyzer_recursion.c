//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

typedef struct {
    char line[MAX_LINE_LENGTH];
    int level;
} LogEntry;

int parse_log_level(const char* line) {
    if (strstr(line, "[ERROR]")) return 3;
    if (strstr(line, "[WARN]")) return 2;
    if (strstr(line, "[INFO]")) return 1;
    if (strstr(line, "[DEBUG]")) return 0;
    return -1;
}

void analyze_log_recursive(LogEntry* entries, int start, int end, int current_level) {
    if (start >= end || current_level >= MAX_LEVELS) return;
    
    int error_count = 0;
    int warn_count = 0;
    int info_count = 0;
    int debug_count = 0;
    
    for (int i = start; i < end; i++) {
        if (entries[i].level == current_level) {
            switch (entries[i].level) {
                case 3: error_count++; break;
                case 2: warn_count++; break;
                case 1: info_count++; break;
                case 0: debug_count++; break;
            }
        }
    }
    
    if (error_count > 0 || warn_count > 0 || info_count > 0 || debug_count > 0) {
        printf("Level %d: Errors=%d, Warnings=%d, Info=%d, Debug=%d\n",
               current_level, error_count, warn_count, info_count, debug_count);
    }
    
    analyze_log_recursive(entries, start, end, current_level + 1);
}

int read_log_file(LogEntry* entries, int max_entries) {
    char buffer[MAX_LINE_LENGTH];
    int count = 0;
    
    printf("Enter log entries (empty line to finish):\n");
    
    while (count < max_entries) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }
        
        if (buffer[0] == '\0') break;
        
        if (len >= MAX_LINE_LENGTH) {
            printf("Line too long, skipping.\n");
            continue;
        }
        
        strncpy(entries[count].line, buffer, MAX_LINE_LENGTH - 1);
        entries[count].line[MAX_LINE_LENGTH - 1] = '\0';
        entries[count].level = parse_log_level(buffer);
        count++;
    }
    
    return count;
}

int main() {
    LogEntry entries[1000];
    int entry_count;
    
    entry_count = read_log_file(entries, 1000);
    
    if (entry_count == 0) {
        printf("No log entries to analyze.\n");
        return 1;
    }
    
    printf("\nLog Analysis Summary:\n");
    analyze_log_recursive(entries, 0, entry_count, 0);
    
    return 0;
}