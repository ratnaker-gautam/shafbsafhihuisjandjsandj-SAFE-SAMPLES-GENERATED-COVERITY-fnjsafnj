//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: log_analyzer
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

int read_log_file(const char* filename, LogEntry* entries, int max_entries) {
    if (!filename || !entries) return 0;
    
    FILE* file = fopen(filename, "r");
    if (!file) return 0;
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, sizeof(line), file) && count < max_entries) {
        if (strlen(line) > 0 && line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }
        
        if (parse_log_line(line, &entries[count])) {
            count++;
        }
    }
    
    fclose(file);
    return count;
}

void print_statistics(int* counts, int total_entries) {
    if (!counts || total_entries <= 0) return;
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", total_entries);
    printf("ERROR: %d (%.1f%%)\n", counts[0], total_entries > 0 ? (counts[0] * 100.0 / total_entries) : 0.0);
    printf("WARN:  %d (%.1f%%)\n", counts[1], total_entries > 0 ? (counts[1] * 100.0 / total_entries) : 0.0);
    printf("INFO:  %d (%.1f%%)\n", counts[2], total_entries > 0 ? (counts[2] * 100.0 / total_entries) : 0.0);
    printf("DEBUG: %d (%.1f%%)\n", counts[3], total_entries > 0 ? (counts[3] * 100.0 / total_entries) : 0.0);
    printf("OTHER: %d (%.1f%%)\n", counts[4], total_entries > 0 ? (counts[4] * 100.0 / total_entries) : 0.0);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    LogEntry entries[MAX_ENTRIES];
    int counts[5] = {0};
    
    int entry_count = read_log_file(argv[1], entries, MAX_ENTRIES);
    if (entry_count <= 0) {
        printf("Error: Could not read log file or no valid entries found.\n");
        return 1;
    }
    
    count_log_levels_recursive(entries, 0, entry_count, counts);
    print_statistics(counts, entry_count);
    
    return 0;
}