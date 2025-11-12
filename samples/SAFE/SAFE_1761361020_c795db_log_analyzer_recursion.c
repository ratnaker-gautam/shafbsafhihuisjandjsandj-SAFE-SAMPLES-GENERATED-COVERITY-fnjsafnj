//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[32];
    char level[16];
    char message[MAX_LINE_LEN];
} LogEntry;

int parse_log_line(const char* line, LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LEN];
    strncpy(temp, line, MAX_LINE_LEN - 1);
    temp[MAX_LINE_LEN - 1] = '\0';
    
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

void count_log_levels_recursive(LogEntry* entries, int start, int end, int* counts) {
    if (start > end) return;
    
    if (strcmp(entries[start].level, "ERROR") == 0) {
        counts[0]++;
    } else if (strcmp(entries[start].level, "WARN") == 0) {
        counts[1]++;
    } else if (strcmp(entries[start].level, "INFO") == 0) {
        counts[2]++;
    } else if (strcmp(entries[start].level, "DEBUG") == 0) {
        counts[3]++;
    }
    
    count_log_levels_recursive(entries, start + 1, end, counts);
}

void display_logs_recursive(LogEntry* entries, int start, int end, const char* filter_level) {
    if (start > end) return;
    
    if (filter_level == NULL || strcmp(entries[start].level, filter_level) == 0) {
        printf("%s %s %s\n", entries[start].timestamp, entries[start].level, entries[start].message);
    }
    
    display_logs_recursive(entries, start + 1, end, filter_level);
}

int read_log_file(const char* filename, LogEntry* entries, int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        return 0;
    }
    
    char line[MAX_LINE_LEN];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) && count < max_entries) {
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) > 0) {
            if (parse_log_line(line, &entries[count])) {
                count++;
            }
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
    
    LogEntry entries[MAX_ENTRIES];
    int entry_count = read_log_file(argv[1], entries, MAX_ENTRIES);
    
    if (entry_count == 0) {
        printf("No valid log entries found or file is empty.\n");
        return 1;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    
    int level_counts[4] = {0};
    count_log_levels_recursive(entries, 0, entry_count - 1, level_counts);
    
    printf("ERROR: %d\n", level_counts[0]);
    printf("WARN: %d\n", level_counts[1]);
    printf("INFO: %d\n", level_counts[2]);
    printf("DEBUG: %d\n", level_counts[3]);
    
    printf("\nRecent log entries:\n");
    int display_count = (entry_count > 5) ? 5 : entry_count;
    display_logs_recursive(entries, entry_count - display_count, entry_count - 1, NULL);
    
    return 0;
}