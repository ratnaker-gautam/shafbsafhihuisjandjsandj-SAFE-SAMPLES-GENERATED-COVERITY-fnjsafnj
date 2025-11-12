//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 256
#define MAX_ENTRIES 100

struct LogEntry {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH - 30];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (line == NULL || entry == NULL) return 0;
    
    char temp[MAX_LINE_LENGTH];
    if (strlen(line) >= MAX_LINE_LENGTH - 1) return 0;
    strncpy(temp, line, MAX_LINE_LENGTH - 1);
    temp[MAX_LINE_LENGTH - 1] = '\0';
    
    int i = 0;
    while (temp[i] != '\0' && isspace(temp[i])) i++;
    
    int field = 0;
    int pos = 0;
    int len = strlen(temp);
    
    for (int j = i; j < len && field < 3; j++) {
        if (temp[j] == ' ' && field < 2) {
            if (field == 0) {
                if (pos >= 19) return 0;
                entry->timestamp[pos] = '\0';
            } else if (field == 1) {
                if (pos >= 9) return 0;
                entry->level[pos] = '\0';
            }
            field++;
            pos = 0;
            while (j < len - 1 && temp[j + 1] == ' ') j++;
        } else {
            if (field == 0) {
                if (pos < 19) entry->timestamp[pos++] = temp[j];
                else return 0;
            } else if (field == 1) {
                if (pos < 9) entry->level[pos++] = temp[j];
                else return 0;
            } else if (field == 2) {
                if (pos < MAX_LINE_LENGTH - 31) entry->message[pos++] = temp[j];
                else return 0;
            }
        }
    }
    
    if (field == 2 && pos > 0) {
        if (pos >= MAX_LINE_LENGTH - 30) return 0;
        entry->message[pos] = '\0';
        return 1;
    }
    
    return 0;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (format: TIMESTAMP LEVEL MESSAGE):\n");
    printf("Enter 'END' on a separate line to finish.\n");
    
    while (entry_count < MAX_ENTRIES) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') line[len - 1] = '\0';
        
        if (strcmp(line, "END") == 0) break;
        
        if (len == 0) continue;
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            entries[entry_count] = entry;
            entry_count++;
        } else {
            printf("Invalid log format: %s\n", line);
        }
    }
    
    printf("\nLog Analysis Results:\n");
    printf("====================\n");
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    int i = 0;
    while (i < entry_count) {
        if (strcmp(entries[i].level, "ERROR") == 0) error_count++;
        else if (strcmp(entries[i].level, "WARNING") == 0) warning_count++;
        else if (strcmp(entries[i].level, "INFO") == 0) info_count++;
        i++;
    }
    
    printf("Total entries: %d\n", entry_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nError messages:\n");
    int j = 0;
    do {
        if (j < entry_count && strcmp(entries[j].level, "ERROR") == 0) {
            printf("- %s: %s\n", entries[j].timestamp, entries[j].message);
        }
        j++;
    } while (j < entry_count);
    
    printf("\nRecent entries (last 5):\n");
    int start = (entry_count > 5) ? entry_count - 5 : 0;
    for (int k = start; k < entry_count; k++) {
        printf("%s [%s] %s\n", entries[k].timestamp, entries[k].level, entries[k].message);
    }
    
    return 0;
}