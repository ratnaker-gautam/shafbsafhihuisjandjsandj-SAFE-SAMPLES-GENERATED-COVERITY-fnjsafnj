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

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    struct LogEntry entries[1000];
    int counts[MAX_LEVELS] = {0};
    
    int entry_count = read_log_file(argv[1], entries, 1000);
    if (entry_count < 0) {
        fprintf(stderr, "Error: Cannot open file %s\n", argv[1]);
        return 1;
    }
    
    if (entry_count == 0) {
        printf("No log entries found.\n");
        return 0;
    }
    
    analyze_log_recursive(entries, 0, entry_count - 1, 0, counts);
    
    printf("Log Analysis Results:\n");
    printf("DEBUG level entries: %d\n", counts[0]);
    printf("INFO level entries: %d\n", counts[1]);
    printf("WARN level entries: %d\n", counts[2]);
    printf("ERROR level entries: %d\n", counts[3]);
    
    int total = counts[0] + counts[1] + counts[2] + counts[3];
    int unclassified = entry_count - total;
    if (unclassified > 0) {
        printf("Unclassified entries: %d\n", unclassified);
    }
    
    return 0;
}