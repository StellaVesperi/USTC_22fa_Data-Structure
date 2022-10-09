#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#define MaxLen 1000             //最大长度限制
 
typedef struct                  //结构体，存放单项ax^n的信息
{
    double coef;                //ax^n的系数a
    int index;                  //ax^n的指数n
}term;
 
typedef struct poly             //用链表存储多项式
{
    term data;                  //多项式的单项信息
    poly* next;
}poly_list;
 
/*函数说明：
    比较两个多项式单项元素ax^n_1与bx^n_2的指数大小*/
int cmp (term a, term b)                    
{
    return  a.index - b.index; 
}
 
/*函数说明：
    删除链表指定位置的结点q*/
void Delete_list (poly_list* head, poly_list* q)                 
{
    poly_list* p;
    p = head;
    while (p->next != q)
    	p = p->next;
    p->next = q->next;
}
 
/*Insert_list函数说明:
    将新的多项式元素ax^n插入多项式链表，并将链表按指数降序排列*/
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
        else                                             //大于则直接在当前位置插入链表
        {
            prior->next = q;
            q->next = p;
            return;
        } 
    }
    if (p == NULL)	                                     //空链表直接插入
    {
    	prior->next = q;
    	q->next = NULL;
    	if (q->data.coef == 0)                          
    		Delete_list (head, q);
	}
}
 
/*函数说明：
    判断一个浮点数是否是整数*/
int Control_float (double a)                    
{
    if (fabs (a - (int)a) < 0.01)               //近似认为等于0，a = (int)a
        return 0;
    else return 1;
}
 
/*函数说明：
    在输入的字符串中提取每一项的系数*/
double Get_coef(char *str)  					
{
	double s = 0.0;
	double d = 10.0;							//处理大于10的系数
	bool flag = false;							//flag记录正负，false为正数, true为负数
	while (*str == ' ')                         //如遇空格则继续往后读
		str++;
	if (*str == '-')
	{
		flag = true;  
		str++;					
		if (*str == 'x' || *str == 'X') 		
			return -1.0;                        //加减符号后没遇到系数，系数为-1则直接返回-1
	}
	else if ((*str == '+' && (*(str + 1) == 'x' || *(str + 1) == 'X')) || (*str == 'x'))
		return 1.0;		                        //加减符号后没遇到系数，系数为1则直接返回1
	if (*str == '+' && (*(str + 1) >= '0' && *(str + 1) <= '9'))
		str++;
	if (!(*str >= '0' && *str <= '9'))     		//如果一开始非数字则退出，返回0.0 
		return s;     
	while (*str >= '0' && *str <= '9' && *str != '.')				  
	{                                           //计算小数点前整数部分
		s = s * 10.0 + *str - '0';
		str++;
	}           
	if (*str == '.')                            //以后为小数部分
        str++;         			      
	while (*str >= '0' && *str <= '9')  			//计算小数部分  
	{
		s = s + (*str - '0') / d;
		d *= 10.0;
		str++;
	}
	return s * (flag ? -1.0 : 1.0);             //根据flag值返回相应的正负数
}
 
/*GetNums函数说明：
    将输入的字符串中的系数和指数提取存储到数组里*/
void GetNums(int &cnt, double* coefs, int* indexs) 					
{
	int i = 0;
	cnt = 0;
	double coef;
	int expn = 0;
	char str[80];
	gets (str);									//存放输入的字符串
	while (*(str + i))
	{
		coef = Get_coef(str + i);
		if (*(str + i) != 'x' && *(str + i ) != 'X') 
			i++;
		while ((*(str + i) >= '0' && *(str + i) <= '9') || (*(str + i) == '.') || (*(str + i) == '-'))  	
			i++;
		if (*(str + i) == '+' || *(str +i) == '-' || *(str + i) == '\0')      //如果没有x则直接将指数赋值为零      
			expn = 0;
		else if (*(str + i) == 'x' || *(str + i) == 'X') 
		{
			i++;
			if (*(str + i) == '+' || *(str + i) == '\0' || *(str + i) == '-') 	//只有x没有数字说明指数为1			
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
		coefs[cnt] = coef;                      //存入数组，元素序号由cnt确定
		indexs[cnt] = expn;
		cnt++;
	}
}
 
/*Create_poly函数说明：
    用通过GetNums函数得到的系数和指数生成多项式链表*/
poly_list* Create_poly()                             
{
	double coefs[80];  								//数组，存各项系数 
	int indexs[80];     							//数组，存各项指数 
	int count;
	GetNums(count, coefs, indexs);                  //通过GetNums把各项信息导入两个数组
    poly_list* head;
    head = (poly_list*)malloc(sizeof(poly_list));
    head->next = NULL;
    poly_list* term_list;                           //多项式链表元素
    for (int i = 0; i < count; i++)
    {
    	term_list = (poly_list*)malloc(sizeof(poly_list));
    	term_list->next = NULL;
    	term_list->data.coef = coefs[i];
    	term_list->data.index = indexs[i];
    	Insert_list (head, term_list);              //插入多项式链表
	}
    return head;
}
 
/*函数说明：
    将计算结果用链表输出*/
void Print_poly (poly_list* head)                     
{
    poly_list* p = head;
    p = p->next;
    if (p == NULL)
    	printf ("0");
    /*各种极端情况的判断*/
    while (p != NULL)
    {
        if (p == head->next)                    //第一个节点输出和后续节点输出方式不同,第一个节点不含加减号
        {
        	if (p->data.coef == 1 && p->data.index ==0)
        		printf ("%.0f", p->data.coef);          //另外当指数为0时不需要输出x
        	if (p->data.coef != 1)
            {
            	if (Control_float (p->data.coef) == 0)  //判断系数是否为整数，整数则输出整数，浮点数则输出浮点数
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

/*Calculate_poly函数说明：
    计算两个多项式的和或差，通过flag的值来控制加法和减法来得到对应的结果*/
poly_list* Calculate_poly (poly_list* p, poly_list* q, int flag)                 
{
    poly_list* result;      //存储结构的链表
    result = (poly_list*)malloc(sizeof(poly_list));
    result->next = NULL;
    poly_list* term_list;
    poly_list* index1 = p->next;                      //两个指针分别指向加数和被加数
    poly_list* index2 = q->next;
    while (index1 != NULL && index2 != NULL)          //若当前两链表节点指数相同则直接相加
    {
        /*用flag来控制加减法，仅用一个函数即可实现多项式加法和减法*/  
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
 
/*函数说明：
    设置光标的位置*/
void Goto_xy (int x,int y)    
{
    COORD c;
    c.X=x-1;
    c.Y=y-1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

/*函数说明：
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
	Goto_xy (35, 22);  printf ("     *{polymials caculator}*  by minervaz         \n");
}

int main()
{
    menu ();
    COORD A, B, Result1, Result2;               //记录对应计算器界面多项式输出处光标的位置
    A.X = 44, A.Y =9;
    B.X = 44, B.Y = 11;
    Result1.X = 49, Result1.Y = 13;
    Result2.X = 49, Result2.Y = 14;
    int flag = 0;                               
    while (scanf ("%d", &flag) && (flag != 3))  //记录键盘输入指令，3为退出      
    {
        system ("cls");                         //清屏
        menu ();
        getchar ();
        Goto_xy (A.X, A.Y);
        poly_list* p1, *p2, *result;            //构造存储两个输入的多项式及计算结果多项式的链表
        p1 = Create_poly (); 
        Goto_xy (B.X, B.Y);
        p2 = Create_poly ();
        result = Calculate_poly (p1, p2, flag);
        Goto_xy (Result1.X, Result1.Y);         //将光标转移到输出位置
        Print_poly (result);
        Goto_xy (Result2.X, Result2.Y);
        menu ();
    }
}