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
    if (strstr(line, "[ERROR]")) return 3;
    if (strstr(line, "[WARN]")) return 2;
    if (strstr(line, "[INFO]")) return 1;
    if (strstr(line, "[DEBUG]")) return 0;
    return -1;
}

void analyze_log_recursive(struct LogEntry* entries, int start, int end, int current_level, int* counts) {
    if (start > end || current_level >= MAX_LEVELS) return;
    
    for (int i = start; i <= end; i++) {
        if (entries[i].level == current_level) {
            counts[current_level]++;
            analyze_log_recursive(entries, start, i - 1, current_level + 1, counts);
            analyze_log_recursive(entries, i + 1, end, current_level + 1, counts);
            break;
        }
    }
}

int read_log_file(const char* filename, struct LogEntry* entries, int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) return -1;
    
    int count = 0;
    char buffer[MAX_LINE_LENGTH];
    
    while (count < max_entries && fgets(buffer, sizeof(buffer), file)) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (len >= sizeof(entries[count].line)) {
            fclose(file);
            return -1;
        }
        
        strncpy(entries[count].line, buffer, sizeof(entries[count].line) - 1);
        entries[count].line[sizeof(entries[count].line) - 1] = '\0';
        entries[count].level = parse_log_level(buffer);
        count++;
    }
    
    fclose(file);
    return count;
}

int main(void) {
    struct LogEntry entries[1000];
    int level_counts[MAX_LEVELS] = {0};
    
    printf("Enter log filename: ");
    char filename[256];
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
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
    
    int entry_count = read_log_file(filename, entries, 1000);
    if (entry_count <= 0) {
        printf("Error reading log file or file empty\n");
        return 1;
    }
    
    analyze_log_recursive(entries, 0, entry_count - 1, 0, level_counts);
    
    printf("\nLog Analysis Results:\n");
    printf("DEBUG entries: %d\n", level_counts[0]);
    printf("INFO entries: %d\n", level_counts[1]);
    printf("WARN entries: %d\n", level_counts[2]);
    printf("ERROR entries: %d\n", level_counts[3]);
    
    int total = level_counts[0] + level_counts[1] + level_counts[2] + level_counts[3];
    printf("Total analyzed entries: %d\n", total);
    
    return 0;
}