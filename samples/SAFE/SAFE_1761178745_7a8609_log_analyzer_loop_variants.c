//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define MAX_ENTRIES 1000

struct LogEntry {
    char timestamp[32];
    char level[16];
    char message[256];
};

int parse_log_line(const char* line, struct LogEntry* entry) {
    if (!line || !entry) return 0;
    
    char temp[MAX_LINE_LEN];
    if (strlen(line) >= MAX_LINE_LEN) return 0;
    strcpy(temp, line);
    
    char* parts[3];
    char* token = strtok(temp, "|");
    int count = 0;
    
    while (token != NULL && count < 3) {
        parts[count] = token;
        count++;
        token = strtok(NULL, "|");
    }
    
    if (count != 3) return 0;
    
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

void analyze_logs(struct LogEntry* logs, int count) {
    if (!logs || count <= 0) return;
    
    int error_count = 0;
    int warning_count = 0;
    int info_count = 0;
    
    int i = 0;
    while (i < count) {
        if (strcmp(logs[i].level, "ERROR") == 0) {
            error_count++;
        } else if (strcmp(logs[i].level, "WARNING") == 0) {
            warning_count++;
        } else if (strcmp(logs[i].level, "INFO") == 0) {
            info_count++;
        }
        i++;
    }
    
    printf("Log Analysis Results:\n");
    printf("Total entries: %d\n", count);
    printf("ERROR entries: %d\n", error_count);
    printf("WARNING entries: %d\n", warning_count);
    printf("INFO entries: %d\n", info_count);
    
    printf("\nRecent ERROR entries:\n");
    int found = 0;
    for (int j = count - 1; j >= 0 && found < 3; j--) {
        if (strcmp(logs[j].level, "ERROR") == 0) {
            printf("- %s: %s\n", logs[j].timestamp, logs[j].message);
            found++;
        }
    }
    
    if (found == 0) {
        printf("No ERROR entries found.\n");
    }
}

int main() {
    struct LogEntry logs[MAX_ENTRIES];
    int entry_count = 0;
    
    printf("Log Analyzer\n");
    printf("Enter log entries in format: TIMESTAMP|LEVEL|MESSAGE\n");
    printf("Enter 'END' on a separate line to finish input.\n");
    printf("Maximum %d entries allowed.\n\n", MAX_ENTRIES);
    
    char line[MAX_LINE_LEN];
    
    while (entry_count < MAX_ENTRIES) {
        printf("Log entry %d: ", entry_count + 1);
        
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = 0;
        
        if (strcmp(line, "END") == 0) {
            break;
        }
        
        if (strlen(line) == 0) {
            continue;
        }
        
        struct LogEntry entry;
        if (parse_log_line(line, &entry)) {
            logs[entry_count] = entry;
            entry_count++;
        } else {
            printf("Invalid format. Use: TIMESTAMP|LEVEL|MESSAGE\n");
        }
    }
    
    if (entry_count > 0) {
        analyze_logs(logs, entry_count);
    } else {
        printf("No valid log entries provided.\n");
    }
    
    return 0;
}