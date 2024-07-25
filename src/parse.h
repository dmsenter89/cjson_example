#ifndef PARSE_H
#define PARSE_H

typedef struct {
    char* userID;
    char* name;
    int department;
    int* sales;
    size_t sales_count;
} Employee;

typedef struct {
    Employee* member;
    ptrdiff_t len;
    ptrdiff_t cap;
} Staff;

Employee* Employee_new(char* userID);
void Employee_free(Employee* in);

Employee* parse_employee_json(cJSON* json, char* userID);

Staff* Staff_init(ptrdiff_t capacity);

#endif