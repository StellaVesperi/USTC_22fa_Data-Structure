/*
typedef struct 
{
    double coef;                //多项式系数
    int index;                  //多项式指数
}term;
 
typedef struct poly             //多项式链表存储结构,稀疏多项式所以用链表存储结构
{
    term data;
    struct poly* next;
}poly_list;
*/

/*
#include <stdio.h>
#include <stdlib.h>
struct person {
    int num;
    float salary;
    struct person *next;
};
int main(){
    struct person *head = NULL;
    struct person *p;
    int no;
    //头插
    head = NULL;
    printf ("Please enter employee number(enter 0 to terminate): ");
    scanf("%d", &no);
    while (no != 0){
        p = (struct person *) malloc(sizeof(struct person));
        p->number = no;
        printf ("Please enter employee salary: ");
        scanf ("%f", & p->salary);
        p->next = head;
        head = p;
        printf ("Please enter employee number(enter 0 to terminate): ");
        scanf ("%d", &no);
    }
    //traverse
    p = head;
    while (p != NULL){
        printf (" %d %f\n", p->num, p->salary);
        p = p->next;
    }
}
*/

/*
struct no *CreateListF() { 
    struct no *p; //用来为新结点申请动态内存的指针
    int n; 
    struct no *head = NULL; //空链表头指针先指向空地址
    scanf("%d", &n); //输入数据
    while(n > 0) { //数据为正数时循环
        p=(struct no *) malloc(sizeof(struct no));//申请内存
        p->num = n; //将输入的数据存入新申请结点的成员num
        p->next = head; //将新结点的尾指针指向链表的头指针
        head = p; //将链表的头指针指向新结点(空间)的起始地址
        scanf("%d", &n); //继续输入数据
    } 
    return head; //将头指针（链表起始地址）传出函数
} //为什么传出这个地址有效？因为是malloc产生的堆地址，不释放
*/

/*函数说明：
    将链表逆序输出，采用堆栈的方式进行*/
/*void reverse (poly_list* head)
{
    float a[100];               //使用堆栈操作进行逆序输出
    int b[100];
    int i = 0;
    poly_list* p = head->next;
    while (p != NULL)           //压栈
    {
        a[i] = p->data.coef;
        b[i] = p->data.index;
        i++;
        p = p->next;
    }
    /*控制输出格式, 两个二元组之间用“ ”隔开，二元组内部用“，”隔开*/    
/*    printf ("%d", i);
    printf (" ");
    for (int j = i - 1; j >= 0; j--)
    {
        if (j == i - 1)                         //第一个节点输出和后续节点输出方式不同,第一个节点不含加减号
        {
        	if (a[j] == 1 && b[j] ==0)
        	{
                printf ("%.0f", a[j]);
                printf (",");
                printf ("%d", b[j]);
            }
            else if (a[j] == 1)
                printf ("1");
        	if (a[j] != 1)
            {
            	if (Control_float (a[j]) == 0)
                    printf ("%.0f", a[j]);
                    else printf ("%.1f", a[j]);
            }
            if (b[j] != 0)
            {
                printf (",");
        	    printf ("%d", b[j]);
                printf (" ");
        	}
        }
        else
        {
             if (a[j] < 0)
                printf ("-");
            if (fabs (a[j]) != 1)
            {
            	if (Control_float (a[j]) == 0)
                    printf ("%.0f", fabs(a[j]));
                else printf ("%.1f", fabs(a[j]));
            }
            else if (a[j] == 1)
                printf ("1");
            if (b[j] != 0)
            {
                printf (",");
        	    printf ("%d", b[j]);
                printf (" ");
        	}
        }
    }
}
*/