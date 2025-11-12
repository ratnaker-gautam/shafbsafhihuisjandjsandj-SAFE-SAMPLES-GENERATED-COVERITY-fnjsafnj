//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LEVELS 100

struct LogEntry {
    char line[MAX_LINE_LENGTH];
    int level;
};

int parse_log_level(const char* line) {
    if (strstr(line, "[ERROR]")) return 3;
    if (strstr(line, "[WARN]")) return 2;
    if (strstr(line, "[INFO]")) return 1;
    if (strstr(line, "[DEBUG]")) return 0;
    return -1;
}

void process_log_recursive(struct LogEntry* entries, int start, int end, int current_level) {
    if (start >= end || current_level >= MAX_LEVELS) {
        return;
    }

    int count = 0;
    for (int i = start; i < end; i++) {
        if (entries[i].level == current_level) {
            count++;
        }
    }

    if (count > 0) {
        printf("Level %d entries: %d\n", current_level, count);
        for (int i = start; i < end; i++) {
            if (entries[i].level == current_level) {
                printf("  %s", entries[i].line);
            }
        }
        printf("\n");
    }

    process_log_recursive(entries, start, end, current_level + 1);
}

int read_log_file(struct LogEntry* entries, int max_entries) {
    char buffer[MAX_LINE_LENGTH];
    int count = 0;

    printf("Enter log lines (empty line to finish):\n");
    
    while (count < max_entries) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        if (buffer[0] == '\n' || buffer[0] == '\0') {
            break;
        }

        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }

        if (len >= MAX_LINE_LENGTH - 1) {
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
    struct LogEntry entries[1000];
    int entry_count;

    entry_count = read_log_file(entries, 1000);
    
    if (entry_count == 0) {
        printf("No log entries to process.\n");
        return 1;
    }

    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    
    process_log_recursive(entries, 0, entry_count, 0);
    
    int total_entries = 0;
    for (int i = 0; i < 4; i++) {
        int level_count = 0;
        for (int j = 0; j < entry_count; j++) {
            if (entries[j].level == i) {
                level_count++;
            }
        }
        total_entries += level_count;
    }
    
    printf("Total processed entries: %d\n", total_entries);
    
    return 0;
}