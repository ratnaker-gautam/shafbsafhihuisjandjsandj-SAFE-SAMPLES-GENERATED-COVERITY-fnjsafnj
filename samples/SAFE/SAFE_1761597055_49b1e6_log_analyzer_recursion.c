//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
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
    if (strstr(line, "ERROR")) return 3;
    if (strstr(line, "WARN")) return 2;
    if (strstr(line, "INFO")) return 1;
    if (strstr(line, "DEBUG")) return 0;
    return -1;
}

void analyze_log_recursive(struct LogEntry* entries, int start, int end, int current_level, int* level_counts) {
    if (start > end || current_level >= MAX_LEVELS) {
        return;
    }
    
    if (start == end) {
        if (entries[start].level == current_level) {
            level_counts[current_level]++;
        }
        return;
    }
    
    int mid = start + (end - start) / 2;
    
    if (entries[mid].level == current_level) {
        level_counts[current_level]++;
    }
    
    analyze_log_recursive(entries, start, mid - 1, current_level, level_counts);
    analyze_log_recursive(entries, mid + 1, end, current_level, level_counts);
}

int read_log_file(const char* filename, struct LogEntry* entries, int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return -1;
    }
    
    int count = 0;
    char buffer[MAX_LINE_LENGTH];
    
    while (count < max_entries && fgets(buffer, sizeof(buffer), file)) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (len > 1) {
            strncpy(entries[count].line, buffer, MAX_LINE_LENGTH - 1);
            entries[count].line[MAX_LINE_LENGTH - 1] = '\0';
            entries[count].level = parse_log_level(buffer);
            count++;
        }
    }
    
    fclose(file);
    return count;
}

int main(void) {
    char filename[256];
    printf("Enter log file name: ");
    
    if (!fgets(filename, sizeof(filename), stdin)) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    
    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }
    
    struct LogEntry entries[1000];
    int entry_count = read_log_file(filename, entries, 1000);
    
    if (entry_count <= 0) {
        printf("Could not read log file or file is empty\n");
        return 1;
    }
    
    int level_counts[4] = {0};
    
    for (int level = 0; level < 4; level++) {
        analyze_log_recursive(entries, 0, entry_count - 1, level, level_counts);
    }
    
    printf("Log Analysis Results:\n");
    printf("DEBUG entries: %d\n", level_counts[0]);
    printf("INFO entries: %d\n", level_counts[1]);
    printf("WARN entries: %d\n", level_counts[2]);
    printf("ERROR entries: %d\n", level_counts[3]);
    
    int total = level_counts[0] + level_counts[1] + level_counts[2] + level_counts[3];
    printf("Total analyzed entries: %d\n", total);
    
    return 0;
}