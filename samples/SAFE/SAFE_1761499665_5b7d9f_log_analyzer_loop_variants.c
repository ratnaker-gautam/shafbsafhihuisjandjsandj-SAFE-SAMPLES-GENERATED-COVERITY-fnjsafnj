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
    char temp_line[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strcpy(temp_line, line);
    
    char* timestamp = strtok(temp_line, " ");
    char* level = strtok(NULL, " ");
    char* message = strtok(NULL, "\n");
    
    if (!timestamp || !level || !message) return 0;
    if (strlen(timestamp) >= 32 || strlen(level) >= 16 || strlen(message) >= 256) return 0;
    
    strcpy(entry->timestamp, timestamp);
    strcpy(entry->level, level);
    strcpy(entry->message, message);
    entry->count = 1;
    
    return 1;
}

int main() {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (empty line to finish):\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (!fgets(line, sizeof(line), stdin)) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') line[len - 1] = '\0';
        if (strlen(line) == 0) break;
        
        struct LogEntry new_entry;
        if (!parse_log_line(line, &new_entry)) {
            printf("Invalid log format. Use: TIMESTAMP LEVEL MESSAGE\n");
            continue;
        }
        
        int found = 0;
        int i = 0;
        while (i < entry_count) {
            if (strcmp(entries[i].timestamp, new_entry.timestamp) == 0 &&
                strcmp(entries[i].level, new_entry.level) == 0 &&
                strcmp(entries[i].message, new_entry.message) == 0) {
                entries[i].count++;
                found = 1;
                break;
            }
            i++;
        }
        
        if (!found) {
            if (entry_count < MAX_ENTRIES) {
                entries[entry_count] = new_entry;
                entry_count++;
            }
        }
    }
    
    printf("\nLog Summary:\n");
    printf("============\n");
    
    int total_entries = 0;
    int j = 0;
    do {
        if (j >= entry_count) break;
        printf("Timestamp: %s\n", entries[j].timestamp);
        printf("Level: %s\n", entries[j].level);
        printf("Message: %s\n", entries[j].message);
        printf("Count: %d\n", entries[j].count);
        printf("---\n");
        total_entries += entries[j].count;
        j++;
    } while (j < entry_count);
    
    printf("Total log entries processed: %d\n", total_entries);
    printf("Unique log patterns: %d\n", entry_count);
    
    int k = 0;
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    for (k = 0; k < entry_count; k++) {
        if (strcmp(entries[k].level, "ERROR") == 0) {
            error_count += entries[k].count;
        } else if (strcmp(entries[k].level, "WARNING") == 0) {
            warning_count += entries[k].count;
        } else if (strcmp(entries[k].level, "INFO") == 0) {
            info_count += entries[k].count;
        }
    }
    
    printf("\nLevel Statistics:\n");
    printf("ERROR: %d\n", error_count);
    printf("WARNING: %d\n", warning_count);
    printf("INFO: %d\n", info_count);
    
    return 0;
}