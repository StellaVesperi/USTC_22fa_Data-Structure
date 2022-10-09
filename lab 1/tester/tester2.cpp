#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#define MaxLen 1000             //最大长度限制
 
typedef struct 
{
    double coef;                //多项式系数
    int index;                  //多项式指数
}term;
 
typedef struct poly             //多项式链表存储结构,稀疏多项式所以用链表存储结构
{
    term data;
    poly* next;
}poly_list;
 
int cmp (term a, term b)                    //比较两个多项式元素的指数大小
{
    return  a.index - b.index; 
}
 
void Delete_list (poly_list* head, poly_list* q)                 //删除链表指定位置节点
{
    poly_list* p;
    p = head;
    while (p->next != q)
    	p = p->next;
    p->next = q->next;
}
 
/*Insert_list函数说明:
    该函数实现将新的多项式元素插入多项式链表，并将链表按降序排列*/
void Insert_list (poly_list* head, poly_list* q) 
{        
    poly_list* p = head;
    poly_list* prior = head;
    p = p->next;
    while (p != NULL)
    {
        if (cmp (p->data, q->data) == 0)                 //若链表中存在的指数相同的项，则直接相加
        {
            p->data.coef += q->data.coef;
            if (p->data.coef == 0)                       //遇到系数为0的极端情况删除该节点
                Delete_list (head, p);
            return;
        }
        else if (cmp (p->data, q->data) > 0)             //指数小于当前节点则向后移
        {
            p = p->next;
            prior = prior->next;
        }
        else                                            //大于则插入链表
        {
            prior->next = q;
            q->next = p;
            return;
        } 
    }
    if (p == NULL)	                                    //空链表直接插入
    {
    	prior->next = q;
    	q->next = NULL;
    	if (q->data.coef == 0)                          
    		Delete_list (head, q);
	}
}
 
int Control_float (double a)                      //判断该浮点数是否是整数
{
    if (fabs (a - (int)a) < 0.01)
        return 0;
    else return 1;
}
 
double Get_coef(char *str)  					//在输入的字符串中提取系数
{
	double s = 0.0;
	double d = 10.0;							//处理大于10的系数
	bool flag = false;							//flag为false即为正数
	while (*str == ' ')  
		str++;
	if (*str == '-')							//记录数字正负  
	{
		flag = true;  
		str++;					
		if (*str == 'x' || *str == 'X') 		//如果系数为-1则直接返回
			return -1.0;
	}
	else if ((*str == '+'&& (*(str + 1) == 'x' || *(str + 1) == 'X')) || (*str == 'x')) 		//系数为1则直接返回
		return 1.0;		
	if (*str == '+' && (*(str + 1) >= '0' && *(str + 1) <= '9'))
		str++;
	if (!(*str >= '0' && *str <= '9'))     		//如果一开始非数字则退出，返回0.0 
		return s;     
	while (*str >= '0' && *str <= '9' && *str != '.')				//计算小数点前整数部分  
	{
		s = s * 10.0 + *str - '0';
		str++;
	}           
	if (*str == '.')                            //以后为小数部分
        str++;         			      
	while (*str >= '0'&&*str <= '9')  			//计算小数部分  
	{
		s = s + (*str - '0') / d;
		d *= 10.0;
		str++;
	}
	return s * (flag ? -1.0 : 1.0);             //根据flag值返回相应的正负数
}
 
/*GetNums函数说明：
    该函数实现将输入的字符串中的系数和指数提取存储到数组里面*/
void GetNums(int &cnt, double* coefs, int* indexs) 					
{
	int i = 0;
	cnt = 0;
	double coef;
	int expn = 0;
	char str[80];
	gets (str);														//存放输入的字符串
	while (*(str + i))
	{
		coef = Get_coef(str + i);
		if (*(str + i) != 'x' && *(str + i ) != 'X') 
			i++;
		while ((*(str + i) >= '0' && *(str + i) <= '9') || (*(str + i) == '.') || (*(str + i) == '-'))  	
			i++;
		if (*(str + i) == '+' || *(str + i) == '\0')      									//如果没有X则直接将指数赋值为零      
			expn = 0;
		else if (*(str + i) == 'x' || *(str + i) == 'X') 
		{
			i++;
			if (*(str + i) == '+' || *(str + i) == '\0' || *(str + i) == '-') 				//只有x没有数字说明指数为1			
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
		coefs[cnt] = coef;
		indexs[cnt] = expn;
		cnt++;
	}
}
 
/*Create_poly函数说明：
    通过GetNums函数得到的系数和指数生成多项式链表*/
poly_list* Create_poly()                             
{
	double coefs[80];  								//存系数 
	int indexs[80];     							//存指数 
	int count;
	GetNums(count, coefs, indexs);
    poly_list* head;
    head = (poly_list*)malloc(sizeof(poly_list));
    head->next = NULL;
    poly_list* term_list;
    for (int i = 0; i < count; i++)
    {
    	term_list = (poly_list*)malloc(sizeof(poly_list));
    	term_list->next = NULL;
    	term_list->data.coef = coefs[i];
    	term_list->data.index = indexs[i];
    	Insert_list (head, term_list);
	}
    return head;
}
 
void Show_poly (poly_list* head)                     //将计算结果多项式链表输出
{
    poly_list* p = head;
    p = p->next;
    if (p == NULL)
    	printf ("0");
    /*各种极端情况的判断*/
    while (p != NULL)
    {
        if (p == head->next)                         //第一个节点输出和后续节点输出方式不同,第一个节点不含加减号
        {
        	if (p->data.coef == 1 && p->data.index ==0)
        		printf ("%.0f", p->data.coef);          //另外当指数为0时不需要输出x
        	if (p->data.coef != 1)
            {
            	if (Control_float (p->data.coef) == 0)          //判断系数是否为整数，整数则输出整数，浮点数则输出浮点数
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
            if (p->data.coef > 0)           //后续节点需要输出加减号
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
 
/*AddDecrease_poly函数说明：
    该函数用于计算两个多项式的和或差，通过flag的值来控制加法和减法来得到对应的结果*/
poly_list* AddorDecrease_poly (poly_list* p, poly_list* q, int flag)                 //多项式加法运算函数
{
    poly_list* result;      //存储结构的链表
    result = (poly_list*)malloc(sizeof(poly_list));
    result->next = NULL;
    poly_list* term_list;
    poly_list* index1 = p->next;                      //两个指针分别指向加数和被加数
    poly_list* index2 = q->next;
    while (index1 != NULL && index2 != NULL)            //若当前两链表节点指数相同则直接相加
    {
        /*巧妙运用flag来控制加减法，仅用一个函数即可实现多项式加法和减法*/  
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
        else if (cmp(index1->data, index2->data) > 0)               //将当前节点指数大的插入result链表
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
    /*接下来判断两链表长度不相同的情况*/
    if (index1 != NULL)             //index1长的情况
        while (index1 != NULL)
        {
        	term_list = (poly_list*)malloc(sizeof(poly_list));
    		term_list->next = NULL;
            term_list->data.coef = index1->data.coef;
            term_list->data.index = index1->data.index;
            Insert_list (result, term_list);
            index1 = index1->next;
        }
    else if (index2 != NULL)            //index2长的情况
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
 
void Goto_xy (int x,int y)            //设置光标的位置
{
    COORD c;
    c.X=x-1;
    c.Y=y-1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
 
/*将链表逆序输出，并采用堆栈的方式进行*/
void reverse (poly_list* head)
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
    printf ("%d", i);
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
 
/*menu函数说明：
  计算器显示界面，运用windows.h头文件控制光标位置实现计算器界面*/
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
	Goto_xy (35, 22);  printf ("     	*{polymials caculator}*  by htc            \n");
}
 
/*main函数说明：
            循环执行功能*/
int main()
{
    menu ();
    COORD A, B, Result1, Result2;                     //记录对应计算器界面多项式输出处光标的位置
    A.X = 44, A.Y =9;
    B.X = 44, B.Y = 11;
    Result1.X = 49, Result1.Y = 13;
    Result2.X = 49, Result2.Y = 14;
    int flag = 0;
    while (scanf ("%d", &flag) && (flag != 3))          
    {
        system ("cls");                     //清屏
        menu ();
        getchar ();
        Goto_xy (A.X, A.Y);
        poly_list* p1, *p2, *result;            //两个输入的多项式及存储计算结果的多项式链表
        p1 = Create_poly (); 
        Goto_xy (B.X, B.Y);
        p2 = Create_poly ();
        result = AddorDecrease_poly (p1, p2, flag);
        Goto_xy (Result1.X, Result1.Y);         //将光标转移到输出位置
        Show_poly (result);
        Goto_xy (Result2.X, Result2.Y);
        reverse (result);
        menu ();
    }
}