//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: log_analyzer
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
    int count;
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strncpy(temp, line, MAX_LINE_LENGTH - 1);
    temp[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    if (strlen(timestamp) >= 19) return 0;
    strncpy(entry->timestamp, timestamp, 19);
    entry->timestamp[19] = '\0';
    
    if (strlen(level) >= 9) return 0;
    strncpy(entry->level, level, 9);
    entry->level[9] = '\0';
    
    if (strlen(message) >= MAX_LINE_LENGTH - 1) return 0;
    strncpy(entry->message, message, MAX_LINE_LENGTH - 1);
    entry->message[MAX_LINE_LENGTH - 1] = '\0';
    
    entry->count = 1;
    
    return 1;
}

void count_log_levels_recursive(FILE* file, int level_counts[], const char* levels[], int num_levels, int depth) {
    if (!file || depth >= MAX_LEVELS) return;
    
    char line[MAX_LINE_LENGTH];
    if (fgets(line, sizeof(line), file)) {
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            for (int i = 0; i < num_levels; i++) {
                if (strcmp(entry.level, levels[i]) == 0) {
                    level_counts[i]++;
                    break;
                }
            }
        }
        count_log_levels_recursive(file, level_counts, levels, num_levels, depth + 1);
    }
}

void analyze_log_file(const char* filename) {
    if (!filename) return;
    
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        return;
    }
    
    const char* levels[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    int num_levels = sizeof(levels) / sizeof(levels[0]);
    int level_counts[sizeof(levels) / sizeof(levels[0])] = {0};
    
    count_log_levels_recursive(file, level_counts, levels, num_levels, 0);
    
    fclose(file);
    
    printf("Log level analysis for %s:\n", filename);
    for (int i = 0; i < num_levels; i++) {
        printf("%s: %d\n", levels[i], level_counts[i]);
    }
}

int main() {
    char filename[256];
    printf("Enter log file name: ");
    
    if (fgets(filename, sizeof(filename), stdin)) {
        size_t len = strlen(filename);
        if (len > 0 && filename[len - 1] == '\n') {
            filename[len - 1] = '\0';
        }
        
        if (strlen(filename) > 0) {
            analyze_log_file(filename);
        } else {
            printf("Error: Empty filename\n");
        }
    } else {
        printf("Error: Failed to read input\n");
    }
    
    return 0;
}