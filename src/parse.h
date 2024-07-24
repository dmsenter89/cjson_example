#ifndef PARSE_H
#define PARSE_H

typedef struct {
    char* userID;
    char* name;
    int department;
    int* sales;
    size_t sales_count;
} Employee;

Employee* Employee_new(char* userID);
void Employee_free(Employee* in);

Employee* parse_employee_json(cJSON* json, char* userID);

#endif