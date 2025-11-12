//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
    int count;
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp_line[MAX_LINE_LENGTH];
    strncpy(temp_line, line, MAX_LINE_LENGTH - 1);
    temp_line[MAX_LINE_LENGTH - 1] = '\0';
    
    char* parts[3];
    char* token = strtok(temp_line, "|");
    int part_count = 0;
    
    while (token != NULL && part_count < 3) {
        parts[part_count] = token;
        part_count++;
        token = strtok(NULL, "|");
    }
    
    if (part_count != 3) return 0;
    
    strncpy(entry->timestamp, parts[0], 31);
    entry->timestamp[31] = '\0';
    strncpy(entry->level, parts[1], 15);
    entry->level[15] = '\0';
    strncpy(entry->message, parts[2], 255);
    entry->message[255] = '\0';
    entry->count = 1;
    
    return 1;
}

int find_duplicate_entry(struct LogEntry* entries, int count, struct LogEntry* new_entry) {
    int i = 0;
    while (i < count) {
        if (strcmp(entries[i].timestamp, new_entry->timestamp) == 0 &&
            strcmp(entries[i].level, new_entry->level) == 0 &&
            strcmp(entries[i].message, new_entry->message) == 0) {
            entries[i].count++;
            return 1;
        }
        i++;
    }
    return 0;
}

void print_summary(struct LogEntry* entries, int count) {
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    int i = 0;
    
    do {
        if (strcmp(entries[i].level, "ERROR") == 0) error_count += entries[i].count;
        else if (strcmp(entries[i].level, "WARNING") == 0) warning_count += entries[i].count;
        else if (strcmp(entries[i].level, "INFO") == 0) info_count += entries[i].count;
        i++;
    } while (i < count);
    
    printf("Log Summary:\n");
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    printf("Total unique entries: %d\n", count);
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: timestamp|level|message). Enter 'END' to finish:\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, MAX_LINE_LENGTH, stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (strcmp(line, "END") == 0) break;
        
        if (strlen(line) == 0) continue;
        
        struct LogEntry new_entry;
        if (parse_log_line(line, &new_entry)) {
            if (!find_duplicate_entry(entries, entry_count, &new_entry)) {
                if (entry_count < MAX_ENTRIES) {
                    entries[entry_count] = new_entry;
                    entry_count++;
                }
            }
        } else {
            printf("Invalid format. Use: timestamp|level|message\n");
        }
    }
    
    printf("\nProcessing complete.\n\n");
    
    int i;
    for (i = 0; i < entry_count; i++) {
        printf("Entry %d: [%s] %s - %s (count: %d)\n", 
               i + 1, entries[i].timestamp, entries[i].level, 
               entries[i].message, entries[i].count);
    }
    
    printf("\n");
    print_summary(entries, entry_count);
    
    return 0;
}