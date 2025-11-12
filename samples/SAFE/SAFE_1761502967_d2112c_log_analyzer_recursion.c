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

void analyze_log_recursive(struct LogEntry* entries, int start, int end, int* counts) {
    if (start > end) return;
    
    int level = entries[start].level;
    if (level >= 0 && level <= 3) {
        counts[level]++;
    }
    
    analyze_log_recursive(entries, start + 1, end, counts);
}

int read_log_file(const char* filename, struct LogEntry* entries, int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) return -1;
    
    char buffer[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(buffer, sizeof(buffer), file) && count < max_entries) {
        if (strlen(buffer) > 0) {
            buffer[strcspn(buffer, "\n")] = 0;
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
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    struct LogEntry entries[1000];
    int counts[4] = {0};
    const char* level_names[] = {"DEBUG", "INFO", "WARN", "ERROR"};
    
    int entry_count = read_log_file(argv[1], entries, 1000);
    if (entry_count < 0) {
        printf("Error: Could not open file '%s'\n", argv[1]);
        return 1;
    }
    
    if (entry_count == 0) {
        printf("No log entries found.\n");
        return 0;
    }
    
    analyze_log_recursive(entries, 0, entry_count - 1, counts);
    
    printf("Log Analysis Results:\n");
    printf("====================\n");
    printf("Total entries: %d\n", entry_count);
    printf("\nBreakdown by level:\n");
    
    for (int i = 0; i < 4; i++) {
        printf("%s: %d\n", level_names[i], counts[i]);
    }
    
    int total_counted = counts[0] + counts[1] + counts[2] + counts[3];
    if (total_counted < entry_count) {
        printf("Unclassified: %d\n", entry_count - total_counted);
    }
    
    return 0;
}