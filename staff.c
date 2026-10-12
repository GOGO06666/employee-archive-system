#include "staff.h"

Staff* hash_id[HASH_SIZE];
Staff* hash_name[HASH_SIZE];

int HashFuncId(int id)
{
    return id % HASH_SIZE;
}

int HashFuncName(char* name)
{
    int sum = 0;
    int len = strlen(name);
    int i;
    for (i = 0; i < len; i++)
        sum += name[i];
    return sum % HASH_SIZE;
}

Staff* CreateStaff(int id, char* name, char* dept, char* date, double sal, char* post)
{
    Staff* p = (Staff*)malloc(sizeof(Staff));
    p->id = id;
    strcpy(p->name, name);
    strcpy(p->dept, dept);
    strcpy(p->entryDate, date);
    p->salary = sal;
    strcpy(p->post, post);
    p->next = NULL;
    return p;
}

void FreeLink(Staff* head)
{
    Staff* cur = head, *del;
    while (cur != NULL)
    {
        del = cur;
        cur = cur->next;
        free(del);
    }
}

void InitHashTable()
{
    int i;
    for (i = 0; i < HASH_SIZE; i++)
    {
        hash_id[i] = NULL;
        hash_name[i] = NULL;
    }
}

int IsIdExist(int id)
{
    int idx = HashFuncId(id);
    Staff* cur = hash_id[idx];
    while (cur != NULL)
    {
        if (cur->id == id)
            return 1;
        cur = cur->next;
    }
    return 0;
}

int AddStaff(int id, char* name, char* dept, char* date, double sal, char* post)
{
    if (IsIdExist(id))
    {
        printf("错误：该工号已存在，无法新增！\n");
        return 0;
    }
    Staff* newNode = CreateStaff(id, name, dept, date, sal, post);
    int idxId = HashFuncId(id);
    newNode->next = hash_id[idxId];
    hash_id[idxId] = newNode;

    Staff* nameNode = CreateStaff(id, name, dept, date, sal, post);
    int idxName = HashFuncName(name);
    nameNode->next = hash_name[idxName];
    hash_name[idxName] = nameNode;

    printf("员工信息录入成功！\n");
    return 1;
}

int ModifyStaff(int targetId, double newSal, char* newPost)
{
    int idx = HashFuncId(targetId);
    Staff* cur = hash_id[idx];
    while (cur != NULL)
    {
        if (cur->id == targetId)
        {
            cur->salary = newSal;
            strcpy(cur->post, newPost);
            break;
        }
        cur = cur->next;
    }
    if (cur == NULL)
    {
        printf("错误：未查询到该工号员工！\n");
        return 0;
    }
    char tmpName[50];
    strcpy(tmpName, cur->name);
    int nameIdx = HashFuncName(tmpName);
    Staff* nameCur = hash_name[nameIdx];
    while (nameCur != NULL)
    {
        if (nameCur->id == targetId)
        {
            nameCur->salary = newSal;
            strcpy(nameCur->post, newPost);
            break;
        }
        nameCur = nameCur->next;
    }
    printf("员工信息修改完成！\n");
    return 1;
}

int DelStaff(int targetId)
{
    if (!IsIdExist(targetId))
    {
        printf("错误：不存在该工号员工！\n");
        return 0;
    }
    char delName[50];
    int idxId = HashFuncId(targetId);
    Staff* pre = NULL, *cur = hash_id[idxId];
    while (cur != NULL && cur->id != targetId)
    {
        pre = cur;
        cur = cur->next;
    }
    strcpy(delName, cur->name);
    if (pre == NULL)
        hash_id[idxId] = cur->next;
    else
        pre->next = cur->next;
    free(cur);

    int idxName = HashFuncName(delName);
    pre = NULL;
    cur = hash_name[idxName];
    while (cur != NULL && cur->id != targetId)
    {
        pre = cur;
        cur = cur->next;
    }
    if (pre == NULL)
        hash_name[idxName] = cur->next;
    else
        pre->next = cur->next;
    free(cur);

    printf("员工离职档案删除成功！\n");
    return 1;
}

void InputNewStaff()
{
    int id;
    char name[50], dept[50], date[20], post[50];
    double sal;
    printf("===== 新员工入职录入 =====\n");
    printf("请输入工号：");
    if (scanf("%d", &id) != 1)
    {
        getchar();
        printf("输入非法数字，录入失败！\n");
        return;
    }
    getchar();
    printf("请输入姓名：");
    gets(name);
    printf("请输入部门：");
    gets(dept);
    printf("请输入入职日期：");
    gets(date);
    printf("请输入薪资：");
    if (scanf("%lf", &sal) != 1)
    {
        getchar();
        printf("薪资输入非法，录入失败！\n");
        return;
    }
    getchar();
    printf("请输入岗位：");
    gets(post);
    AddStaff(id, name, dept, date, sal, post);
}

// 带工号去重，保证数组内同一工号仅一条记录
int GetAllStaff(TempStaff arr[])
{
    int cnt = 0;
    int i, j;
    Staff* cur;
    for (i = 0; i < HASH_SIZE; i++)
    {
        cur = hash_id[i];
        while (cur != NULL)
        {
            int repeat = 0;
            for (j = 0; j < cnt; j++)
            {
                if (arr[j].id == cur->id)
                {
                    repeat = 1;
                    break;
                }
            }
            if (repeat == 0)
            {
                arr[cnt].id = cur->id;
                strcpy(arr[cnt].name, cur->name);
                strcpy(arr[cnt].dept, cur->dept);
                arr[cnt].salary = cur->salary;
                cnt++;
            }
            cur = cur->next;
        }
    }
    return cnt;
}

void StatDeptSalary()
{
    TempStaff all[MAX_STAFF];
    int total = GetAllStaff(all);
    if (total == 0)
    {
        printf("暂无员工数据，无法统计！\n");
        return;
    }
    char deptList[100][50];
    double sumSal[100] = {0};
    int deptCnt[100] = {0};
    int deptNum = 0;
    int i, j;
    for (i = 0; i < total; i++)
    {
        int exist = 0;
        for (j = 0; j < deptNum; j++)
        {
            if (strcmp(all[i].dept, deptList[j]) == 0)
            {
                sumSal[j] += all[i].salary;
                deptCnt[j]++;
                exist = 1;
                break;
            }
        }
        if (!exist)
        {
            strcpy(deptList[deptNum], all[i].dept);
            sumSal[deptNum] = all[i].salary;
            deptCnt[deptNum] = 1;
            deptNum++;
        }
    }
    printf("========== 部门薪资统计报表 ==========\n");
    for (i = 0; i < deptNum; i++)
    {
        double avg = sumSal[i] / deptCnt[i];
        printf("部门：%s 人数：%d 总薪资：%.2lf 平均薪资：%.2lf\n",
               deptList[i], deptCnt[i], sumSal[i], avg);
    }
}

void SortStaffSalary()
{
    TempStaff all[MAX_STAFF];
    int total = GetAllStaff(all);
    if (total == 0)
    {
        printf("暂无员工数据，无法排序！\n");
        return;
    }
    int i, j;
    for (i = 0; i < total - 1; i++)
    {
        for (j = 0; j < total - i - 1; j++)
        {
            if (all[j].salary < all[j + 1].salary)
            {
                TempStaff t = all[j];
                all[j] = all[j + 1];
                all[j + 1] = t;
            }
        }
    }
    printf("========== 全公司薪资排行榜 ==========\n");
    int rank = 1;
    for (i = 0; i < total; i++)
    {
        if (i > 0 && all[i].salary != all[i - 1].salary)
            rank = i + 1;
        printf("第%d名 | 工号：%d | 姓名：%s | 部门：%s | 薪资：%.2lf\n",
               rank, all[i].id, all[i].name, all[i].dept, all[i].salary);
    }
}

void SaveDataToFile()
{
    FILE* fp = fopen("staff.txt", "w");
    if (fp == NULL)
    {
        printf("文件打开失败，无法保存数据！\n");
        return;
    }
    int i;
    Staff* cur;
    for (i = 0; i < HASH_SIZE; i++)
    {
        cur = hash_id[i];
        while (cur != NULL)
        {
            fprintf(fp, "%d %s %s %s %.2lf %s\n",
                    cur->id, cur->name, cur->dept, cur->entryDate, cur->salary, cur->post);
            cur = cur->next;
        }
    }
    fclose(fp);
    printf("所有员工数据已保存至 staff.txt\n");
}

void LoadDataFromFile()
{
    FILE* fp = fopen("staff.txt", "r");
    if (fp == NULL)
    {
        printf("档案文件不存在，初始化空系统\n");
        return;
    }
    int id;
    char name[50], dept[50], date[20], post[50];
    double sal;
    while (fscanf(fp, "%d %s %s %s %lf %s", &id, name, dept, date, &sal, post) != EOF)
    {
        AddStaff(id, name, dept, date, sal, post);
    }
    fclose(fp);
    printf("成功加载本地人事档案数据\n");
}
