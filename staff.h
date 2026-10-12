#ifndef STAFF_H
#define STAFF_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HASH_SIZE 100
#define MAX_STAFF 1000

typedef struct Staff
{
    int id;
    char name[50];
    char dept[50];
    char entryDate[20];
    double salary;
    char post[50];
    struct Staff *next;
} Staff;

extern Staff* hash_id[HASH_SIZE];
extern Staff* hash_name[HASH_SIZE];

int HashFuncId(int id);
int HashFuncName(char* name);
Staff* CreateStaff(int id, char* name, char* dept, char* date, double sal, char* post);
void FreeLink(Staff* head);
void InitHashTable();

int IsIdExist(int id);
int AddStaff(int id, char* name, char* dept, char* date, double sal, char* post);
int ModifyStaff(int targetId, double newSal, char* newPost);
int DelStaff(int targetId);
void InputNewStaff();

typedef struct TempStaff
{
    int id;
    char name[50];
    char dept[50];
    double salary;
} TempStaff;
int GetAllStaff(TempStaff arr[]);
void StatDeptSalary();
void SortStaffSalary();

void SaveDataToFile();
void LoadDataFromFile();

#endif
