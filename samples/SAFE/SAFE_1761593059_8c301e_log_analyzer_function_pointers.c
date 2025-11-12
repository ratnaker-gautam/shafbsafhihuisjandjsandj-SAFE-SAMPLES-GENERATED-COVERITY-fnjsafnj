//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: log_analyzer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[20];
    char level[10];
    char message[MAX_LINE_LENGTH];
} LogEntry;

typedef void (*LogProcessor)(LogEntry*);

void count_entries(LogEntry* entry) {
    static int count = 0;
    count++;
    printf("Processed %d entries\n", count);
}

void show_errors(LogEntry* entry) {
    if (strcmp(entry->level, "ERROR") == 0) {
        printf("ERROR: %s - %s\n", entry->timestamp, entry->message);
    }
}

void show_warnings(LogEntry* entry) {
    if (strcmp(entry->level, "WARN") == 0) {
        printf("WARNING: %s - %s\n", entry->timestamp, entry->message);
    }
}

void process_log_file(FILE* file, LogProcessor processor) {
    char line[MAX_LINE_LENGTH];
    LogEntry entry;
    int line_num = 0;
    
    while (fgets(line, sizeof(line), file) != NULL) {
        line_num++;
        if (line_num > MAX_ENTRIES) {
            printf("Maximum entries reached\n");
            break;
        }
        
        if (strlen(line) >= MAX_LINE_LENGTH - 1) {
            printf("Line too long at line %d\n", line_num);
            continue;
        }
        
        memset(&entry, 0, sizeof(entry));
        
        int fields = sscanf(line, "%19s %9s %1023[^\n]", 
                           entry.timestamp, entry.level, entry.message);
        
        if (fields == 3) {
            for (int i = 0; entry.level[i]; i++) {
                entry.level[i] = toupper(entry.level[i]);
            }
            processor(&entry);
        } else {
            printf("Invalid log format at line %d\n", line_num);
        }
    }
}

int main() {
    printf("Log Analyzer\n");
    printf("Available processors:\n");
    printf("1. Count entries\n");
    printf("2. Show errors only\n");
    printf("3. Show warnings only\n");
    printf("Enter choice (1-3): ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice < 1 || choice > 3) {
        printf("Invalid choice\n");
        return 1;
    }
    
    printf("Enter log data (end with empty line):\n");
    
    FILE* temp_file = tmpfile();
    if (temp_file == NULL) {
        printf("Failed to create temporary file\n");
        return 1;
    }
    
    char buffer[MAX_LINE_LENGTH];
    getchar();
    
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (buffer[0] == '\n') {
            break;
        }
        if (fputs(buffer, temp_file) == EOF) {
            printf("Error writing to temporary file\n");
            fclose(temp_file);
            return 1;
        }
    }
    
    rewind(temp_file);
    
    LogProcessor processors[] = {count_entries, show_errors, show_warnings};
    process_log_file(temp_file, processors[choice - 1]);
    
    fclose(temp_file);
    return 0;
}