#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#define MaxLen 1000             //��󳤶�����
 
typedef struct                  //�ṹ�壬��ŵ���ax^n����Ϣ
{
    double coef;                //ax^n��ϵ��a
    int index;                  //ax^n��ָ��n
}term;
 
typedef struct poly             //������洢����ʽ
{
    term data;                  //����ʽ�ĵ�����Ϣ
    poly* next;
}poly_list;
 
/*����˵����
    �Ƚ���������ʽ����Ԫ��ax^n_1��bx^n_2��ָ����С*/
int cmp (term a, term b)                    
{
    return  a.index - b.index; 
}
 
/*����˵����
    ɾ������ָ��λ�õĽ��q*/
void Delete_list (poly_list* head, poly_list* q)                 
{
    poly_list* p;
    p = head;
    while (p->next != q)
    	p = p->next;
    p->next = q->next;
}
 
/*Insert_list����˵��:
    ���µĶ���ʽԪ��ax^n�������ʽ������������ָ����������*/
void Insert_list (poly_list* head, poly_list* q) 
{        
    poly_list* p = head;
    poly_list* prior = head;
    p = p->next;
    while (p != NULL)
    {
        if (cmp (p->data, q->data) == 0)                 //�������д��ڵ�ָ����ͬ�����ֱ�����
        {
            p->data.coef += q->data.coef;
            if (p->data.coef == 0)                       //����ϵ��Ϊ0�ļ������ɾ���ýڵ�
                Delete_list (head, p);
            return;
        }
        else if (cmp (p->data, q->data) > 0)             //ָ��С�ڵ�ǰ�ڵ��������
        {
            p = p->next;
            prior = prior->next;
        }
        else                                             //������ֱ���ڵ�ǰλ�ò�������
        {
            prior->next = q;
            q->next = p;
            return;
        } 
    }
    if (p == NULL)	                                     //������ֱ�Ӳ���
    {
    	prior->next = q;
    	q->next = NULL;
    	if (q->data.coef == 0)                          
    		Delete_list (head, q);
	}
}
 
/*����˵����
    �ж�һ���������Ƿ�������*/
int Control_float (double a)                    
{
    if (fabs (a - (int)a) < 0.01)               //������Ϊ����0��a = (int)a
        return 0;
    else return 1;
}
 
/*����˵����
    ��������ַ�������ȡÿһ���ϵ��*/
double Get_coef(char *str)  					
{
	double s = 0.0;
	double d = 10.0;							//�������10��ϵ��
	bool flag = false;							//flag��¼������falseΪ����, trueΪ����
	while (*str == ' ')                         //�����ո�����������
		str++;
	if (*str == '-')
	{
		flag = true;  
		str++;					
		if (*str == 'x' || *str == 'X') 		
			return -1.0;                        //�Ӽ����ź�û����ϵ����ϵ��Ϊ-1��ֱ�ӷ���-1
	}
	else if ((*str == '+' && (*(str + 1) == 'x' || *(str + 1) == 'X')) || (*str == 'x'))
		return 1.0;		                        //�Ӽ����ź�û����ϵ����ϵ��Ϊ1��ֱ�ӷ���1
	if (*str == '+' && (*(str + 1) >= '0' && *(str + 1) <= '9'))
		str++;
	if (!(*str >= '0' && *str <= '9'))     		//���һ��ʼ���������˳�������0.0 
		return s;     
	while (*str >= '0' && *str <= '9' && *str != '.')				  
	{                                           //����С����ǰ��������
		s = s * 10.0 + *str - '0';
		str++;
	}           
	if (*str == '.')                            //�Ժ�ΪС������
        str++;         			      
	while (*str >= '0' && *str <= '9')  			//����С������  
	{
		s = s + (*str - '0') / d;
		d *= 10.0;
		str++;
	}
	return s * (flag ? -1.0 : 1.0);             //����flagֵ������Ӧ��������
}
 
/*GetNums����˵����
    ��������ַ����е�ϵ����ָ����ȡ�洢��������*/
void GetNums(int &cnt, double* coefs, int* indexs) 					
{
	int i = 0;
	cnt = 0;
	double coef;
	int expn = 0;
	char str[80];
	gets (str);									//���������ַ���
	while (*(str + i))
	{
		coef = Get_coef(str + i);
		if (*(str + i) != 'x' && *(str + i ) != 'X') 
			i++;
		while ((*(str + i) >= '0' && *(str + i) <= '9') || (*(str + i) == '.') || (*(str + i) == '-'))  	
			i++;
		if (*(str + i) == '+' || *(str +i) == '-' || *(str + i) == '\0')      //���û��x��ֱ�ӽ�ָ����ֵΪ��      
			expn = 0;
		else if (*(str + i) == 'x' || *(str + i) == 'X') 
		{
			i++;
			if (*(str + i) == '+' || *(str + i) == '\0' || *(str + i) == '-') 	//ֻ��xû������˵��ָ��Ϊ1			
				expn = 1;							
			else if (*(str + i) == '^')
			{
				i++;
				expn = (int)Get_coef(str + i);
				if (*(str + i) == '-')
					i++;
				while ((*(str + i) >= '0'&&*(str + i) <= '9') || (*(str + i) == '.'))
					i++;
			}
		}
		coefs[cnt] = coef;                      //�������飬Ԫ�������cntȷ��
		indexs[cnt] = expn;
		cnt++;
	}
}
 
/*Create_poly����˵����
    ��ͨ��GetNums�����õ���ϵ����ָ�����ɶ���ʽ����*/
poly_list* Create_poly()                             
{
	double coefs[80];  								//���飬�����ϵ�� 
	int indexs[80];     							//���飬�����ָ�� 
	int count;
	GetNums(count, coefs, indexs);                  //ͨ��GetNums�Ѹ�����Ϣ������������
    poly_list* head;
    head = (poly_list*)malloc(sizeof(poly_list));
    head->next = NULL;
    poly_list* term_list;                           //����ʽ����Ԫ��
    for (int i = 0; i < count; i++)
    {
    	term_list = (poly_list*)malloc(sizeof(poly_list));
    	term_list->next = NULL;
    	term_list->data.coef = coefs[i];
    	term_list->data.index = indexs[i];
    	Insert_list (head, term_list);              //�������ʽ����
	}
    return head;
}
 
/*����˵����
    �����������������*/
void Print_poly (poly_list* head)                     
{
    poly_list* p = head;
    p = p->next;
    if (p == NULL)
    	printf ("0");
    /*���ּ���������ж�*/
    while (p != NULL)
    {
        if (p == head->next)                    //��һ���ڵ�����ͺ����ڵ������ʽ��ͬ,��һ���ڵ㲻���Ӽ���
        {
        	if (p->data.coef == 1 && p->data.index ==0)
        		printf ("%.0f", p->data.coef);          //���⵱ָ��Ϊ0ʱ����Ҫ���x
        	if (p->data.coef != 1)
            {
            	if (Control_float (p->data.coef) == 0)  //�ж�ϵ���Ƿ�Ϊ��������������������������������������
                    printf ("%.0f", p->data.coef);
                    else printf ("%.1f", p->data.coef);
            }
            if (p->data.index != 0)
            {
		        printf ("X^");
        	    printf ("%d", p->data.index);
        	}
        }
        else
        {
            if (p->data.coef > 0)           //�����ڵ���Ҫ����Ӽ���
                printf ("+");
            else if (p->data.coef < 0)
                printf ("-");
            if (fabs (p->data.coef) != 1)
            {
            	if (Control_float (p->data.coef) == 0)
                    printf ("%.0f", fabs(p->data.coef));
                else printf ("%.1f", fabs(p->data.coef));
            }
            if (p->data.index != 0)
            {
		        printf ("X^");
        	    printf ("%d", p->data.index);
        	}
        }
        p = p->next;
    }
    printf ("\n\n\n");
}

/*Calculate_poly����˵����
    ������������ʽ�ĺͻ�ͨ��flag��ֵ�����Ƽӷ��ͼ������õ���Ӧ�Ľ��*/
poly_list* Calculate_poly (poly_list* p, poly_list* q, int flag)                 
{
    poly_list* result;      //�洢�ṹ������
    result = (poly_list*)malloc(sizeof(poly_list));
    result->next = NULL;
    poly_list* term_list;
    poly_list* index1 = p->next;                      //����ָ��ֱ�ָ������ͱ�����
    poly_list* index2 = q->next;
    while (index1 != NULL && index2 != NULL)          //����ǰ������ڵ�ָ����ͬ��ֱ�����
    {
        /*��flag�����ƼӼ���������һ����������ʵ�ֶ���ʽ�ӷ��ͼ���*/  
		term_list = (poly_list*)malloc(sizeof(poly_list));
    	term_list->next = NULL;
        if (cmp (index1->data, index2->data) == 0 && flag == 1)
        {
            term_list->data.coef = index1->data.coef + index2->data.coef;
            term_list->data.index = index1->data.index;
            Insert_list (result,  term_list);
            index1 = index1->next;
            index2 = index2->next;
        }
        else if (cmp (index1->data, index2->data) == 0 && flag == 2)
        {
            term_list->data.coef = index1->data.coef - index2->data.coef;
            term_list->data.index = index1->data.index;
            Insert_list (result,  term_list);
            index1 = index1->next;
            index2 = index2->next;
        }
        else if (cmp(index1->data, index2->data) > 0)               //����ǰ�ڵ�ָ����Ĳ���result����
        {
            term_list->data.coef = index1->data.coef;
            term_list->data.index = index1->data.index;
            Insert_list (result, term_list);
            index1 = index1->next;
        }
        else
        {
            term_list->data.coef = index2->data.coef;
            term_list->data.index = index2->data.index;
            Insert_list (result,  term_list);
            index2 = index2->next;
        }
    }
    /*�������ж��������Ȳ���ͬ�����*/
    if (index1 != NULL)             //index1�������
        while (index1 != NULL)
        {
        	term_list = (poly_list*)malloc(sizeof(poly_list));
    		term_list->next = NULL;
            term_list->data.coef = index1->data.coef;
            term_list->data.index = index1->data.index;
            Insert_list (result, term_list);
            index1 = index1->next;
        }
    else if (index2 != NULL)            //index2�������
        while (index2 != NULL)
        {
        	term_list = (poly_list*)malloc(sizeof(poly_list));
    		term_list->next = NULL;
            term_list->data.coef = index2->data.coef;
            term_list->data.index = index2->data.index;
            Insert_list(result, term_list);
            index2 = index2->next;
        }
    return result;
}
 
/*����˵����
    ���ù���λ��*/
void Goto_xy (int x,int y)    
{
    COORD c;
    c.X=x-1;
    c.Y=y-1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

/*����˵����
  ��������ʾ���棬����windows.hͷ�ļ����ƹ��λ��ʵ�ּ���������*/
void menu()                 
{   
    Goto_xy (35, 2);   printf ("**************************************************\n");
	Goto_xy (35, 3);   printf ("*            polynomials caculators              *\n");
	Goto_xy (35, 4);   printf ("**************************************************");
	Goto_xy (35, 5);   printf ("*     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"); Goto_xy(84, 5); printf("*\n");
	Goto_xy (35, 6);   printf ("*     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"); Goto_xy(84, 6); printf("*\n");
	Goto_xy (35, 7);   printf ("*     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"); Goto_xy(84, 7); printf("*\n");
	Goto_xy (35, 8);   printf ("**************************************************");
	Goto_xy (35, 9);   printf ("*PolyA :");              Goto_xy(84, 9); printf("*");
	Goto_xy (35, 10);  printf ("**************************************************");
	Goto_xy (35, 11);  printf ("*PolyB :");             Goto_xy(84, 11); printf("*");
	Goto_xy (35, 12);  printf ("**************************************************");
	Goto_xy (35, 13);  printf ("*PolyResult :");         Goto_xy(84, 13); printf("*");
    Goto_xy (35, 14);  printf ("*             ");         Goto_xy(84, 14); printf("*");
	Goto_xy (35, 15);  printf ("**************************************************");
	Goto_xy (35, 16);  printf ("*   Put 1 to add      |    put 2 to decrease     *\n");
	Goto_xy (35, 17);  printf ("**************************************************");
	Goto_xy (35, 18);  printf ("*   Put 3 to exit     |    put Enter to perform  *\n");
	Goto_xy (35, 19);  printf ("**************************************************");
	Goto_xy (35, 20);  printf ("*                                                *\n");
	Goto_xy (35, 21);  printf ("**************************************************");
	Goto_xy (35, 22);  printf ("     *{polymials caculator}*  by minervaz         \n");
}

int main()
{
    menu ();
    COORD A, B, Result1, Result2;               //��¼��Ӧ�������������ʽ���������λ��
    A.X = 44, A.Y =9;
    B.X = 44, B.Y = 11;
    Result1.X = 49, Result1.Y = 13;
    Result2.X = 49, Result2.Y = 14;
    int flag = 0;                               
    while (scanf ("%d", &flag) && (flag != 3))  //��¼��������ָ�3Ϊ�˳�      
    {
        system ("cls");                         //����
        menu ();
        getchar ();
        Goto_xy (A.X, A.Y);
        poly_list* p1, *p2, *result;            //����洢��������Ķ���ʽ������������ʽ������
        p1 = Create_poly (); 
        Goto_xy (B.X, B.Y);
        p2 = Create_poly ();
        result = Calculate_poly (p1, p2, flag);
        Goto_xy (Result1.X, Result1.Y);         //�����ת�Ƶ����λ��
        Print_poly (result);
        Goto_xy (Result2.X, Result2.Y);
        menu ();
    }
}