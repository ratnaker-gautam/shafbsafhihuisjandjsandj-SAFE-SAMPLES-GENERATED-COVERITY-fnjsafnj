//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LENGTH];
    strncpy(temp, line, MAX_LINE_LENGTH - 1);
    temp[MAX_LINE_LENGTH - 1] = '\0';
    
    char* parts[3];
    parts[0] = temp;
    int part_count = 1;
    
    for (int i = 0; temp[i] != '\0' && part_count < 3; i++) {
        if (temp[i] == '|') {
            temp[i] = '\0';
            parts[part_count] = &temp[i + 1];
            part_count++;
        }
    }
    
    if (part_count != 3) return 0;
    
    if (strlen(parts[0]) >= 19) return 0;
    if (strlen(parts[1]) >= 9) return 0;
    if (strlen(parts[2]) >= MAX_LINE_LENGTH) return 0;
    
    strncpy(entry->timestamp, parts[0], 19);
    entry->timestamp[19] = '\0';
    strncpy(entry->level, parts[1], 9);
    entry->level[9] = '\0';
    strncpy(entry->message, parts[2], MAX_LINE_LENGTH - 1);
    entry->message[MAX_LINE_LENGTH - 1] = '\0';
    
    return 1;
}

void count_log_levels_recursive(struct LogEntry* entries, int start, int end, 
                               int* error_count, int* warning_count, int* info_count) {
    if (start > end) return;
    
    if (strcmp(entries[start].level, "ERROR") == 0) {
        (*error_count)++;
    } else if (strcmp(entries[start].level, "WARNING") == 0) {
        (*warning_count)++;
    } else if (strcmp(entries[start].level, "INFO") == 0) {
        (*info_count)++;
    }
    
    count_log_levels_recursive(entries, start + 1, end, error_count, warning_count, info_count);
}

void print_entries_recursive(struct LogEntry* entries, int start, int end, const char* filter) {
    if (start > end) return;
    
    if (!filter || strcmp(entries[start].level, filter) == 0) {
        printf("%s | %s | %s\n", entries[start].timestamp, 
               entries[start].level, entries[start].message);
    }
    
    print_entries_recursive(entries, start + 1, end, filter);
}

int read_log_file(const char* filename, struct LogEntry* entries, int max_entries) {
    FILE* file = fopen(filename, "r");
    if (!file) return -1;
    
    char line[MAX_LINE_LENGTH];
    int count = 0;
    
    while (fgets(line, MAX_LINE_LENGTH, file) && count < max_entries) {
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
    
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = read_log_file(argv[1], entries, MAX_ENTRIES);
    
    if (entry_count < 0) {
        printf("Error: Could not open file %s\n", argv[1]);
        return 1;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 0;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", entry_count);
    
    int error_count = 0, warning_count = 0, info_count = 0;
    count_log_levels_recursive(entries, 0, entry_count - 1, 
                              &error_count, &warning_count, &info_count);
    
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nAll ERROR entries:\n");
    print_entries_recursive(entries, 0, entry_count - 1, "ERROR");
    
    return 0;
}