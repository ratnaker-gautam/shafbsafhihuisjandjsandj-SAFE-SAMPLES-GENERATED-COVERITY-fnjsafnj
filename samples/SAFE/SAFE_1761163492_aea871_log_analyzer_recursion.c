//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH];
} LogEntry;

int read_log_file(const char* filename, LogEntry entries[], int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return -1;
    }
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) && count < max_entries) {
        if (strlen(line) > 0) {
            line[strcspn(line, "\n")] = 0;
            
            char* timestamp = strtok(line, " ");
            char* level = strtok(NULL, " ");
            char* message = strtok(NULL, "");
            
            if (timestamp && level && message) {
                strncpy(entries[count].timestamp, timestamp, sizeof(entries[count].timestamp) - 1);
                entries[count].timestamp[sizeof(entries[count].timestamp) - 1] = '\0';
                
                strncpy(entries[count].level, level, sizeof(entries[count].level) - 1);
                entries[count].level[sizeof(entries[count].level) - 1] = '\0';
                
                strncpy(entries[count].message, message, sizeof(entries[count].message) - 1);
                entries[count].message[sizeof(entries[count].message) - 1] = '\0';
                
                count++;
            }
        }
    }
    
    fclose(file);
    return count;
}

void count_log_levels_recursive(LogEntry entries[], int index, int total, int counts[]) {
    if (index >= total) {
        return;
    }
    
    if (strcmp(entries[index].level, "ERROR") == 0) {
        counts[0]++;
    } else if (strcmp(entries[index].level, "WARN") == 0) {
        counts[1]++;
    } else if (strcmp(entries[index].level, "INFO") == 0) {
        counts[2]++;
    } else if (strcmp(entries[index].level, "DEBUG") == 0) {
        counts[3]++;
    } else {
        counts[4]++;
    }
    
    count_log_levels_recursive(entries, index + 1, total, counts);
}

void print_analysis(int counts[]) {
    printf("Log Level Analysis:\n");
    printf("ERROR: %d\n", counts[0]);
    printf("WARN:  %d\n", counts[1]);
    printf("INFO:  %d\n", counts[2]);
    printf("DEBUG: %d\n", counts[3]);
    printf("OTHER: %d\n", counts[4]);
    
    int total = counts[0] + counts[1] + counts[2] + counts[3] + counts[4];
    if (total > 0) {
        printf("\nTotal entries: %d\n", total);
        printf("ERROR percentage: %.1f%%\n", (counts[0] * 100.0) / total);
    }
}

int main() {
    char filename[256];
    printf("Enter log file name: ");
    
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    filename[strcspn(filename, "\n")] = 0;
    
    if (strlen(filename) == 0) {
        printf("Invalid filename\n");
        return 1;
    }
    
    LogEntry entries[MAX_ENTRIES];
    int entry_count = read_log_file(filename, entries, MAX_ENTRIES);
    
    if (entry_count < 0) {
        printf("Error: Could not open file '%s'\n", filename);
        return 1;
    }
    
    if (entry_count == 0) {
        printf("No log entries found in file '%s'\n", filename);
        return 0;
    }
    
    int level_counts[5] = {0};
    count_log_levels_recursive(entries, 0, entry_count, level_counts);
    print_analysis(level_counts);
    
    return 0;
}