//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[MAX_LINE_LENGTH];
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LENGTH];
    strncpy(temp, line, MAX_LINE_LENGTH - 1);
    temp[MAX_LINE_LENGTH - 1] = '\0';
    
    char* timestamp = strtok(temp, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    
    strncpy(entry->timestamp, timestamp, sizeof(entry->timestamp) - 1);
    entry->timestamp[sizeof(entry->timestamp) - 1] = '\0';
    
    strncpy(entry->level, level, sizeof(entry->level) - 1);
    entry->level[sizeof(entry->level) - 1] = '\0';
    
    strncpy(entry->message, message, sizeof(entry->message) - 1);
    entry->message[sizeof(entry->message) - 1] = '\0';
    
    return 1;
}

void count_log_levels_recursive(LogEntry* entries, int index, int total, int* counts) {
    if (index >= total) return;
    
    if (strcmp(entries[index].level, "INFO") == 0) {
        counts[0]++;
    } else if (strcmp(entries[index].level, "WARNING") == 0) {
        counts[1]++;
    } else if (strcmp(entries[index].level, "ERROR") == 0) {
        counts[2]++;
    } else {
        counts[3]++;
    }
    
    count_log_levels_recursive(entries, index + 1, total, counts);
}

int read_log_file(const char* filename, LogEntry* entries, int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) return -1;
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) && count < max_entries) {
        if (strlen(line) > 0) {
            line[strcspn(line, "\n")] = '\0';
            if (parse_log_line(line, &entries[count])) {
                count++;
            }
        }
    }
    
    fclose(file);
    return count;
}

void print_summary(int* counts) {
    printf("Log Level Summary:\n");
    printf("INFO: %d\n", counts[0]);
    printf("WARNING: %d\n", counts[1]);
    printf("ERROR: %d\n", counts[2]);
    printf("OTHER: %d\n", counts[3]);
    printf("TOTAL: %d\n", counts[0] + counts[1] + counts[2] + counts[3]);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    LogEntry entries[MAX_ENTRIES];
    int log_count = read_log_file(argv[1], entries, MAX_ENTRIES);
    
    if (log_count < 0) {
        printf("Error: Could not open file '%s'\n", argv[1]);
        return 1;
    }
    
    if (log_count == 0) {
        printf("No valid log entries found in '%s'\n", argv[1]);
        return 0;
    }
    
    int level_counts[4] = {0};
    count_log_levels_recursive(entries, 0, log_count, level_counts);
    
    print_summary(level_counts);
    
    return 0;
}