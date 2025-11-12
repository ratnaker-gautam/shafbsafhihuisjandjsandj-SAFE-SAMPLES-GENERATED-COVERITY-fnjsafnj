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
    while (temp[i] != '\0' && i < 19) {
        if (temp[i] == ' ') break;
        entry->timestamp[i] = temp[i];
        i++;
    }
    if (i == 0 || i >= 19) return 0;
    entry->timestamp[i] = '\0';
    
    while (temp[i] != '\0' && isspace(temp[i])) i++;
    
    int j = 0;
    while (temp[i] != '\0' && !isspace(temp[i]) && j < 9) {
        entry->level[j] = temp[i];
        i++;
        j++;
    }
    if (j == 0) return 0;
    entry->level[j] = '\0';
    
    while (temp[i] != '\0' && isspace(temp[i])) i++;
    
    j = 0;
    while (temp[i] != '\0' && j < MAX_LINE_LENGTH - 31) {
        entry->message[j] = temp[i];
        i++;
        j++;
    }
    if (j == 0) return 0;
    entry->message[j] = '\0';
    
    return 1;
}

int main(void) {
    struct LogEntry entries[MAX_ENTRIES];
    int entry_count = 0;
    char line[MAX_LINE_LENGTH];
    
    printf("Enter log entries (empty line to finish):\n");
    
    int line_num = 0;
    while (line_num < MAX_ENTRIES) {
        if (fgets(line, MAX_LINE_LENGTH, stdin) == NULL) break;
        
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        if (line[0] == '\0') break;
        
        if (parse_log_line(line, &entries[entry_count])) {
            entry_count++;
        } else {
            printf("Invalid log format on line %d\n", line_num + 1);
        }
        
        line_num++;
    }
    
    if (entry_count == 0) {
        printf("No valid log entries found.\n");
        return 0;
    }
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    int k = 0;
    do {
        if (strcmp(entries[k].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(entries[k].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(entries[k].level, "INFO") == 0) {
            info_count++;
        }
        k++;
    } while (k < entry_count);
    
    printf("\nLog Analysis Summary:\n");
    printf("Total entries: %d\n", entry_count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nRecent entries:\n");
    int start = (entry_count > 5) ? entry_count - 5 : 0;
    for (int idx = start; idx < entry_count; idx++) {
        printf("%s [%s] %s\n", entries[idx].timestamp, entries[idx].level, entries[idx].message);
    }
    
    return 0;
}