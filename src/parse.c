#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cjson/cJSON.h>

#ifdef TESTING
#include <acutest/acutest.h>
#endif

#include "parse.h"

static int get_cjson_int(cJSON* json, char* key);

Employee* Employee_new(char* userID)
{
    Employee* out = malloc(sizeof(Employee));
    out->userID = malloc(sizeof(userID));
    strcpy(out->userID, userID);
    out->name = 0;
    out->department = 0;
    out->sales = 0;
    out->sales_count = 0;
    return out;
}

Employee* parse_employee_json(cJSON* json, char* userID)
{
    if (!cJSON_IsObject(json)) {
        return 0;
    }

    cJSON* scratch = 0;

    Employee* out = Employee_new(userID);

    // get Name
    scratch = cJSON_GetObjectItemCaseSensitive(json, "name");
    if (cJSON_IsString(scratch)) {
        out->name = malloc(strlen(scratch->valuestring)+1);
        strcpy(out->name, scratch->valuestring);
    }

    // use helper function for department
    out->department = get_cjson_int(json, "department");

    // check if sales array exists, handle if necessary
    scratch = cJSON_GetObjectItemCaseSensitive(json, "sales");

    if (scratch && cJSON_IsArray(scratch)) {
        out->sales_count = cJSON_GetArraySize(scratch);
        out->sales = malloc(sizeof(int)*out->sales_count);
        size_t i = 0;
        cJSON* elem = 0;

        cJSON_ArrayForEach(elem, scratch) {
            if (cJSON_IsNumber(elem)) {
                out->sales[i] = elem->valueint;
            }
            ++i;
        }
    }

    return out;
}

static int get_cjson_int(cJSON* json, char* key)
{
    cJSON* entry = cJSON_GetObjectItemCaseSensitive(json, key);
    if (cJSON_IsNumber(entry)) {
        return entry->valueint;
    } else {
        return -999;
    }
}

Staff* Staff_init(ptrdiff_t capacity)
{
    Staff* out = malloc(sizeof(Staff));
    out->member = malloc(sizeof(Employee)*capacity);
    out->len = 0;
    out->cap = capacity;
    
    return out;
}

Staff* parse_staff_json(cJSON* root)
{
    Staff* out = Staff_init(10);
    cJSON* user = 0;
    cJSON_ArrayForEach(user, root) {
        char* userID = user->string;
        if (userID) {
            Employee* emp = parse_employee_json(user, userID);
            out->member[out->len++] = *emp;
        }
    }

    return out;
}

#ifndef TESTING
int main(int argc,  char* argv[argc+1])
{
    return 0;
}
#else
void test_parse_employee_json_sale(void)
{
    const char* json_string =
        "{"
        "    \"name\": \"Daniel Green\","
        "    \"department\": 4,"
        "    \"sales\": [4, 3, 7]"
        "}";

    cJSON* json = cJSON_Parse(json_string);
    char* userID = "dangre";
    Employee* emp = parse_employee_json(json, userID);

    TEST_CHECK(strcmp(emp->userID, userID) == 0);
    TEST_MSG("Expected: '%s'", userID);
    TEST_MSG("Produced: '%s'", emp->userID);

    TEST_CHECK(strcmp(emp->name, "Daniel Green") == 0);
    TEST_MSG("Expected: 'Daniel Green'");
    TEST_MSG("Produced: '%s'", emp->name);

    TEST_CHECK(emp->department == 4);
    TEST_MSG("Expected: '%d'", 4);
    TEST_MSG("Produced: '%d'", emp->department);

    int sales[] = {4, 3, 7};
    for (size_t i=0; i<3; ++i) {
        TEST_CHECK(emp->sales[i] == sales[i]);
    }

    cJSON_Delete(json);
}

void test_parse_employee_json_nosale(void)
{
    const char* json_string =
        "{"
        "   \"name\": \"Jennifer Barnes\","
        "   \"department\": 1"
        "}";
    cJSON* json = cJSON_Parse(json_string);
    char* userID = "jenbar";
    Employee* emp = parse_employee_json(json, userID);

    TEST_CHECK(strcmp(emp->name, "Jennifer Barnes") == 0);
    TEST_CHECK(emp->sales_count == 0);
    TEST_CHECK(emp->sales == 0);

    cJSON_Delete(json);
}

void test_iterate_employee_recs(void)
{
    const char *json_string =
        "{"
        "   \"dangre\" : {"
        "      \"name\" : \"Daniel Green\","
        "      \"department\" : 4,"
        "      \"sales\" : [ 4, 3, 7 ]"
        "   },"
        "   \"jenbar\" : {"
        "       \"name\" : \"Jennifer Barnes\","
        "       \"department\" : 1"
        "}}";
    cJSON* json = cJSON_Parse(json_string);
    TEST_CHECK(!cJSON_IsInvalid(json));

    Staff* staff = parse_staff_json(json);

    TEST_CHECK(staff->len == 2);

    TEST_CHECK(strcmp(staff->member[0].userID, "dangre") == 0);
    TEST_MSG("Expected: '%s'", "dangre");
    TEST_MSG("Produced: '%s'", staff->member[0].userID);

    TEST_CHECK(strcmp(staff->member[1].userID, "jenbar") == 0);
    TEST_MSG("Expected: '%s'", "jenbar");
    TEST_MSG("Produced: '%s'", staff->member[1].userID);

    cJSON_Delete(json);
}

TEST_LIST = {
   { "employee parsing - with sale", test_parse_employee_json_sale},
   { "employee parsing - no sales", test_parse_employee_json_nosale},
   { "iterate over multiple emp recs", test_iterate_employee_recs},
   { NULL, NULL }     /* zeroed record marking the end of the list */
};
#endif