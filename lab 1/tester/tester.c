/*
typedef struct 
{
    double coef;                //����ʽϵ��
    int index;                  //����ʽָ��
}term;
 
typedef struct poly             //����ʽ����洢�ṹ,ϡ�����ʽ����������洢�ṹ
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
    //ͷ��
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
    struct no *p; //����Ϊ�½�����붯̬�ڴ��ָ��
    int n; 
    struct no *head = NULL; //������ͷָ����ָ��յ�ַ
    scanf("%d", &n); //��������
    while(n > 0) { //����Ϊ����ʱѭ��
        p=(struct no *) malloc(sizeof(struct no));//�����ڴ�
        p->num = n; //����������ݴ�����������ĳ�Աnum
        p->next = head; //���½���βָ��ָ�������ͷָ��
        head = p; //�������ͷָ��ָ���½��(�ռ�)����ʼ��ַ
        scanf("%d", &n); //������������
    } 
    return head; //��ͷָ�루������ʼ��ַ����������
} //Ϊʲô���������ַ��Ч����Ϊ��malloc�����Ķѵ�ַ�����ͷ�
*/

/*����˵����
    ������������������ö�ջ�ķ�ʽ����*/
/*void reverse (poly_list* head)
{
    float a[100];               //ʹ�ö�ջ���������������
    int b[100];
    int i = 0;
    poly_list* p = head->next;
    while (p != NULL)           //ѹջ
    {
        a[i] = p->data.coef;
        b[i] = p->data.index;
        i++;
        p = p->next;
    }
    /*���������ʽ, ������Ԫ��֮���á� ����������Ԫ���ڲ��á���������*/    
/*    printf ("%d", i);
    printf (" ");
    for (int j = i - 1; j >= 0; j--)
    {
        if (j == i - 1)                         //��һ���ڵ�����ͺ����ڵ������ʽ��ͬ,��һ���ڵ㲻���Ӽ���
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