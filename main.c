#include "staff.h"

void ShowProjectMenu()
{
    printf("\n==================== 项目管理栏 ====================\n");
    printf("1. 新员工入职录入（档案基础管理）\n");
    printf("2. 修改员工岗位薪资（档案基础管理）\n");
    printf("3. 离职员工档案删除（档案基础管理）\n");
    printf("4. 部门薪资人数统计（薪资统计排行）\n");
    printf("5. 全公司薪资排行榜（薪资统计排行）\n");
    printf("6. 手动保存人事档案（文件持久化）\n");
    printf("0. 退出系统（自动保存数据）\n");
    printf("====================================================\n");
    printf("请输入功能编号：");
}

int main()
{
    InitHashTable();
    LoadDataFromFile();
    int op;
    while (1)
    {
        ShowProjectMenu();
        if (scanf("%d", &op) != 1)
        {
            getchar();
            printf("输入错误，请输入数字0-6！\n");
            continue;
        }
        getchar();
        switch (op)
        {
            case 1: InputNewStaff(); break;
            case 2:
            {
                int id; double sal; char post[50];
                printf("请输入要修改的员工工号：");
                scanf("%d", &id);
                getchar();
                printf("输入新岗位："); gets(post);
                printf("输入新薪资："); scanf("%lf", &sal);
                ModifyStaff(id, sal, post);
                break;
            }
            case 3:
            {
                int id;
                printf("请输入离职员工工号：");
                scanf("%d", &id);
                DelStaff(id);
                break;
            }
            case 4: StatDeptSalary(); break;
            case 5: SortStaffSalary(); break;
            case 6: SaveDataToFile(); break;
            case 0:
                SaveDataToFile();
                printf("数据已保存，系统正常退出！\n");
                int i;
                for (i = 0; i < HASH_SIZE; i++)
                {
                    FreeLink(hash_id[i]);
                    FreeLink(hash_name[i]);
                }
                return 0;
            default:
                printf("输入编号超出范围，请重新选择！\n");
        }
    }
    return 0;
}
