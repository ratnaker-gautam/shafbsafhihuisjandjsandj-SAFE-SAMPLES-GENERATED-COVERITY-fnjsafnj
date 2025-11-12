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
    char message[MAX_LINE_LENGTH - 30];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LENGTH];
    strncpy(temp, line, MAX_LINE_LENGTH - 1);
    temp[MAX_LINE_LENGTH - 1] = '\0';
    
    if (strlen(temp) < 30) return 0;
    
    char* parts[3];
    parts[0] = temp;
    
    int part_count = 0;
    for (int i = 0; i < (int)strlen(temp) && part_count < 3; i++) {
        if (temp[i] == ' ' && part_count < 2) {
            temp[i] = '\0';
            parts[++part_count] = &temp[i + 1];
        }
    }
    
    if (part_count != 2) return 0;
    
    if (strlen(parts[0]) >= sizeof(entry->timestamp) ||
        strlen(parts[1]) >= sizeof(entry->level) ||
        strlen(parts[2]) >= sizeof(entry->message)) {
        return 0;
    }
    
    strcpy(entry->timestamp, parts[0]);
    strcpy(entry->level, parts[1]);
    strcpy(entry->message, parts[2]);
    
    return 1;
}

void count_log_levels_recursive(struct LogEntry* entries, int count, int index, 
                               int* error_count, int* warning_count, int* info_count) {
    if (index >= count) return;
    
    if (strcmp(entries[index].level, "ERROR") == 0) {
        (*error_count)++;
    } else if (strcmp(entries[index].level, "WARNING") == 0) {
        (*warning_count)++;
    } else if (strcmp(entries[index].level, "INFO") == 0) {
        (*info_count)++;
    }
    
    count_log_levels_recursive(entries, count, index + 1, error_count, warning_count, info_count);
}

void display_entries_recursive(struct LogEntry* entries, int count, int index) {
    if (index >= count) return;
    
    printf("Entry %d: [%s] %s - %s\n", 
           index + 1, 
           entries[index].timestamp, 
           entries[index].level, 
           entries[index].message);
    
    display_entries_recursive(entries, count, index + 1);
}

int read_log_file(const char* filename, struct LogEntry* entries, int max_entries) {
    if (!filename || !entries || max_entries <= 0) return 0;
    
    FILE* file = fopen(filename, "r");
    if (!file) return 0;
    
    char line[MAX_LINE_LENGTH];
    int entry_count = 0;
    
    while (fgets(line, sizeof(line), file) && entry_count < max_entries) {
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) > 0) {
            struct LogEntry entry;
            if (parse_log_line(line, &entry)) {
                entries[entry_count++] = entry;
            }
        }
    }
    
    fclose(file);
    return entry_count;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }
    
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = read_log_file(argv[1], entries, MAX_ENTRIES);
    
    if (entry_count == 0) {
        printf("No valid log entries found or file cannot be read.\n");
        return 1;
    }
    
    printf("Found %d log entries:\n\n", entry_count);
    
    int error_count = 0, warning_count = 0, info_count = 0;
    count_log_levels_recursive(entries, entry_count, 0, &error_count, &warning_count, &info_count);
    
    printf("Log Level Summary:\n");
    printf("ERROR: %d\n", error_count);
    printf("WARNING: %d\n", warning_count);
    printf("INFO: %d\n", info_count);
    printf("\n");
    
    printf("Log Entries:\n");
    display_entries_recursive(entries, entry_count, 0);
    
    return 0;
}