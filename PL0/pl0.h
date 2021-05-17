/*PL/0编译系统C版本头文件pl0.h*/

#include <stdio.h>

typedef enum
{
	false,
	true
} bool;

#define norw 18	  /* 关键字个数 */
#define txmax 100 /* 名字表容量 */
#define nmax 14	  /* number 的最大位数 */
#define al 10	  /* 符号的最大长度 */
#define amax 2047 /* 地址上界 */
#define levmax 3  /* 最大允许过程嵌套声明层数 [0, levmax] */
#define cxmax 200 /* 最多的虚拟机代码数 */

// 钟景文
/* 符号 */
enum symbol
{
	nul,		//null
	ident,		//标识符
	number,		//数字
	plus,		//'+'
	minus,		//'-'
	times,		//'*'
	slash,		//'/'
	oddsym,		//'odd'
	eql,		//'='
	neq,		//'<>'
	lss,		//'<'
	leq,		//'<='
	gtr,		//'>'
	geq,		//'>='
	lparen,		//'('
	rparen,		//')'
	comma,		//','
	semicolon,	//';'
	period,		//'.'
	becomes,	//':='
	beginsym,	//'begin'
	endsym,		//'end'
	ifsym,		//'if'
	thensym,	//'then'
	whilesym,	//'while'
	writesym,	//'write'
	readsym,	//'read'
	dosym,		//'do'
	callsym,	//'call'
	constsym,	//'const'
	varsym,		//'var'
	procsym,	//'procedure'
	forsym,		//'for'
	tosym,		//'to'
	downsym,	//'downto'
	pluseql,	//'+='
	minuseql,	//'-='
	pplus,		//'++'
	mminus,		//'--'
	retnsym,	//'return'
	elsesym,	//'else'
};

#define symnum 41

/* 名字表中的类型 */
enum object
{
	constant,
	variable,
	procedur,
};

/* 虚拟机代码 */
enum fct
{
	lit,	
	opr,
	lod,
	sto,
	cal,
	inte,
	jmp,
	jpc,
};

#define fctnum 8

// 钟文磊
/* 虚拟机代码结构 */
struct instruction
{
	enum fct f; /* 虚拟机代码指令 */
	int l;		/* 引用层与声明层的层次差 */
	int a;		/* 根据 f 的不同而不同 */
};

FILE* fas; /* 输出名字表 */
FILE* fa;  /* 输出虚拟机代码 */
FILE* fa1; /* 输出源文件及其各行对应的首地址 */
FILE* fa2; /* 输出结果 */

bool listswitch;  /* 显示虚拟机代码与否 */
bool tableswitch; /* 显示名字表与否 */

char ch;		 /* 读取字符的缓冲区，getch 使用 */
enum symbol sym; /* 当前符号 */
char id[al + 1]; /* 当前 ident ，多出的一个字节用于存放 0 */
int num;		 /* 当前 number */

int cc, ll; /* getch 使用的计数器，cc 表示当前字符 ch 的位置 */
int cx;		/* 虚拟机代码指针，取值范围为 [0, cxmax - 1] */

char line[81];	/* 读取行缓冲区 */
char a[al + 1]; /* 临时保存读取的单词，多出的一个字节用于存放 0 */

// 朱景钊
struct instruction code[cxmax]; /* 存放虚拟机代码的数组 */
char word[norw][al];			/* 保留字 */
enum symbol wsym[norw];			/* 保留字对应的符号值 */
enum symbol ssym[256];			/* 单字符的符号值 */
char mnemonic[fctnum][5];		/* 虚拟机代码指令名称 */

bool declbegsys[symnum]; /* 表明声明开始的符号集合 */
bool statbegsys[symnum]; /* 表明语句开始的符号集合 */
bool facbegsys[symnum];	 /* 表明因子开始的符号集合*/

/* 名字表结构 */
struct tablestruct
{
	char name[al];	  /* 名字 */
	enum object kind; /* 类型： const/var/array/procedure */
	int val;		  /* 常量标识符所代表的数值，仅 const 使用 */
	int level;		  /* 标识符所处层(作用域号)，仅 const 不使用 */
	int adr;		  /* 变量标识符的偏移地址(相对于过程活动记录)，仅 const 不使用 */
	int size;		  /* 过程活动记录的初始数据区大小，仅 procedure 使用 */
};

struct tablestruct table[txmax]; /* 名字表 */

FILE* fin;
FILE* fout;
char fname[al];
int err; /* 错误计数器 */

// 王楷楠
/* 当函数中会发生 fatal error 时，返回 -1 告知调用它的函数，最终退出程序 */
#define getsymdo      if (-1 == getsym()) return -1
#define getchdo       if (-1 == getch()) return -1
#define testdo(a, b, c)   if (-1 ==  test(a, b, c)) return -1
#define gendo(a, b, c)    if (-1 == gen(a, b, c)) return -1
#define expressiondo(a, b, c)   if (-1 == expression(a, b, c)) return -1
#define factordo(a, b, c)       if (-1 == factor(a, b, c)) return -1
#define termdo(a, b, c)        if (-1 == term(a, b, c)) return -1
#define conditiondo(a, b, c)        if (-1 == condition(a, b, c)) return -1
#define statementdo(a, b, c)        if (-1 ==  statement(a, b, c)) return -1
#define constdeclarationdo(a, b, c)        if (-1 == constdeclaration(a, b, c)) return -1
#define vardeclarationdo(a, b, c)        if (-1 ==  vardeclaration(a, b, c)) return -1

void error(int n);
int getsym();
int getch();
void init();
int gen(enum fct x, int y, int z);
int test(bool* s1, bool* s2, int n);
int inset(int e, bool* s);
int addset(bool* sr, bool* s1, bool* s2, int n);
int subset(bool* sr, bool* s1, bool* s2, int n);
int mulset(bool* sr, bool* s1, bool* s2, int n);
int block(int lev, int tx, bool* fsys);
void interpret();
int factor(bool* fsys, int* ptx, int lev);
int term(bool* fsys, int* ptx, int lev);
int condition(bool* fsys, int* ptx, int lev);
int expression(bool* fsys, int* ptx, int lev);
int statement(bool* fsys, int* ptx, int lev);
void listcode(int cx0);
int vardeclaration(int* ptx, int lev, int* pdx);
int constdeclaration(int* ptx, int lev, int* pdx);
int position(char* idt, int tx);
void enter(enum object k, int* ptx, int lev, int* pdx);
int base(int l, int* s, int b);
