//18级信息安全2班

//陈雪莹
/*编译和运行环境：
* 1 Visual C++ 6.0，Visual C++.NET and Visual C++.NET 2003
*  WinNT,Win2000,WinXP and Win2003
* 2 gcc version 3.3.2 20031022(Red Hat Linux 3.3.2-1)
*  Redhat Fedora core 1
*  Intel 32 platform
* 使用方法：
* 运行后输入PL/0源程序文件名
* 回答是否输出虚拟机代码
* 回答是否输出名字表
* fa.tmp输出虚拟机代码
* fa1.tmp输出源文件及其各行对应的首地址
* fa2.tep输出结果
* fas.tmp输出名字表
*/


#pragma warning(disable : 4996)//这个是额外添加的，关闭VS2019中的安全警告，不用VS2019可以删掉。

#include<stdio.h>
#include "pl0.h"
#include "string.h"
#include <stdlib.h>
/*解释执行时使用的栈*/
#define stacksize 500


//黄圆圆
int main()
{
	bool nxtlev[symnum];
	printf("Input pl/0 file?");
	scanf("%s", fname);                                                    /*输入文件名*/
	fin = fopen(fname, "r");
	if (fin)
	{
		printf("List object code? (Y/N)");                                 /*是否输出虚拟机代码*/
		scanf("%s", fname);
		listswitch = (fname[0] == 'y' || fname[0] == 'Y');
		printf("List symbol table?(Y/N)");                                 /*是否输出名字表*/
		scanf("%s", fname);
		tableswitch = (fname[0] == 'y' || fname[0] == 'Y');
		fa1 = fopen("fa1.tmp", "w");
		fprintf(fa1, "Input pl/0 file?");
		fprintf(fa1, "%s\n", fname);
		init();                                                            /*初始化*/
		err = 0;
		cc = cx = ll = 0;
		ch = ' ';
		if (-1 != getsym())
		{
			fa = fopen("fa.tmp", "w");
			fas = fopen("fas.tmp", "w");
			addset(nxtlev, declbegsys, statbegsys, symnum);
			nxtlev[period] = true;

			table[0].type = none;	/*主过程无返回值。*/
			table[0].argc = 0;	/*主过程参数数量为0*/
			// 李纪然
			if (-1 == block(0, 0, nxtlev)) {								/*调用编译程序*/
				fclose(fa);
				fclose(fa1);
				fclose(fas);
				fclose(fin);
				printf("\n");
				return 0;
			}
			fclose(fa);
			fclose(fa1);
			fclose(fas);
			if (sym != period) {
				error(9);
			}
			if (err == 0) {
				fa2 = fopen("fa2.tmp", "w");
				interpret();												/*调用解释程序*/
				fclose(fa2);
			}
			else {
				printf("Errors in pl/0 program");
			}
		}
		fclose(fin);
	}
	else {
		printf("Can't open file!\n");
	}
	printf("\n");
	system("pause");
	return 0;
}



//李文静
/*
*初始化
*/
void init() {
	int i;
	/*设置单字符符号*/
	for (i = 0; i <= 255; i++) {
		ssym[i] = nul;
	}
//	ssym['+'] = plus;	//新增了+=，-=，++，--。
//	ssym['-'] = minus;	//不再是单字符了。
	ssym['*'] = times;
	ssym['/'] = slash;
	ssym['('] = lparen;
	ssym[')'] = rparen;
	ssym['='] = eql;
	ssym[','] = comma;
//	ssym['.'] = period;
//	ssym['#'] = neq;	//替换成<>,不再是单字符了
	ssym[';'] = semicolon;
	ssym['['] = lsparen;
	ssym[']'] = rsparen;
	/* 设置保留字名字，按照字母顺序，便于折半查找；*/
	strcpy(&(word[0][0]), "begin");
	strcpy(&(word[1][0]), "call");
	strcpy(&(word[2][0]), "char");
	strcpy(&(word[3][0]), "const");
	strcpy(&(word[4][0]), "do");
	strcpy(&(word[5][0]), "downto");
	strcpy(&(word[6][0]), "else");
	strcpy(&(word[7][0]), "end");
	strcpy(&(word[8][0]), "for");
	strcpy(&(word[9][0]), "if");
	strcpy(&(word[10][0]), "int");
	strcpy(&(word[11][0]), "odd");
	strcpy(&(word[12][0]), "procedure");
	strcpy(&(word[13][0]), "read");
	strcpy(&(word[14][0]), "real");
	strcpy(&(word[15][0]), "return");
	strcpy(&(word[16][0]), "then");
	strcpy(&(word[17][0]), "to");
	strcpy(&(word[18][0]), "var");
	strcpy(&(word[19][0]), "while");
	strcpy(&(word[20][0]), "write");

	// 左梓仪
	/*设置保留字符号*/
	wsym[0] = beginsym;
	wsym[1] = callsym;
	wsym[2] = charsym;
	wsym[3] = constsym;
	wsym[4] = dosym;
	wsym[5] = downsym;
	wsym[6] = elsesym;
	wsym[7] = endsym;
	wsym[8] = forsym;
	wsym[9] = ifsym;
	wsym[10] = intsym;
	wsym[11] = oddsym;
	wsym[12] = procsym;
	wsym[13] = readsym;
	wsym[14] = realsym;
	wsym[15] = retnsym;
	wsym[16] = thensym;
	wsym[17] = tosym;
	wsym[18] = varsym;
	wsym[19] = whilesym;
	wsym[20] = writesym;
	/*设置指令名称*/
	strcpy(&(mnemonic[lit][0]), "lit");
	strcpy(&(mnemonic[opr][0]), "opr");
	strcpy(&(mnemonic[lod][0]), "lod");
	strcpy(&(mnemonic[sto][0]), "sto");
	strcpy(&(mnemonic[cal][0]), "cal");
	strcpy(&(mnemonic[inte][0]), "int");
	strcpy(&(mnemonic[jmp][0]), "jmp");
	strcpy(&(mnemonic[jpc][0]), "jpc");
	strcpy(&(mnemonic[rlod][0]), "rlod");
	strcpy(&(mnemonic[rsto][0]), "rsto");
	strcpy(&(mnemonic[plod][0]), "plod");
	strcpy(&(mnemonic[psto][0]), "psto");
	/*设置符号集*/
	for (i = 0; i < symnum; i++)
	{
		declbegsys[i] = false;
		statbegsys[i] = false;
		facbegsys[i] = false;
	}
	/*
	周赛星-3218005445
	*/
	/*设置声明开始符号集*/
	declbegsys[constsym] = true;
	declbegsys[varsym] = true;
	declbegsys[procsym] = true;
	declbegsys[intsym] = true;
	declbegsys[charsym] = true;
	declbegsys[realsym] = true;
	/*设置语句开始符号集*/
	statbegsys[beginsym] = true;
	statbegsys[callsym] = true;
	statbegsys[ifsym] = true;
	statbegsys[whilesym] = true;
	statbegsys[forsym] = true;
	/*设置因子开始符号集*/
	facbegsys[ident] = true;
	facbegsys[number] = true;
	facbegsys[lparen] = true;
	facbegsys[pplus] = true;
	facbegsys[mminus] = true;
	facbegsys[realnum] = true;
	facbegsys[charlex] = true;
	rt = 0;
}
/*
*用数组实现集合的集合运算
*/
int inset(int e, bool* s)
{
	return s[e];
}
int addset(bool* sr, bool* s1, bool* s2, int n)
{
	int i;
	for (i = 0; i < n; i++)
	{
		sr[i] = s1[i] || s2[i];
	}
	return 0;
}
int subset(bool* sr, bool* s1, bool* s2, int n)
{
	int i;
	for (i = 0; i < n; i++)
	{
		sr[i] = s1[i] && (!s2[i]);
	}
	return 0;
}

/*王欢-3218005443
*/
int mulset(bool* sr, bool* s1, bool* s2, int n)
{
	int i;
	for (i = 0; i < n; i++)
	{
		sr[i] = s1[i] && s2[i];
	}
	return 0;
}
/*
*出错处理，打印出错位置和错误编码
*/
void error(int n)
{
	char space[81];
	memset(space, 32, 81);
	space[cc - 1] = 0;//出错时当前符号已经读完，所以cc-1
	printf("****%s!%d\n", space, n);
	fprintf(fa1, "****%s!%d\n", space, n);
	err++;
}
/*
*漏掉空格，读取一个字符。
*
*每次读一行，存入line缓冲区，line被getsym取空后再读一行
*
*被函数getsym 调用
*/
int getch()
{
	if (cc == ll)
	{
		if (feof(fin))//feof(File)检测流上的文件结束符，如果文件结束，则返回非0值，否则返回0
		{
			printf("program incomplete");
			return -1;
		}
		ll = 0;
		cc = 0;
		printf("%d", cx);
		fprintf(fa1, "%d", cx);//位于头文件<cstdio>中，其作用是格式化输出到一个流文件中。
		ch = ' ';
		/*
		* 袁丽玲-3218005444
		*/
		//读取一行字符
		while (ch != 10) {//'\LF',换行健
			//fscanf(fin,"%c",&ch)
			if (EOF == fscanf(fin, "%c", &ch))
			{
				line[ll] = 0;//行终止标记
				break;
			}
			printf("%c", ch);
			fprintf(fa1, "%c", ch);
			line[ll] = ch;
			ll++;
		}
		printf("\n");
		fprintf(fa1, "\n");
	}
	ch = line[cc];
	cc++;
	return 0;
}

/*
* 词法分析，获取一个符号
*/
int getsym()
{
	int i, j, k;
	while (ch == ' ' || ch == 10 || ch == 9)    /*忽略空格、换行和TAB*/
	{
		getchdo;
	}
	if (ch >= 'a' && ch <= 'z')
	{             /*名字或保留字以a~z开头*/
		k = 0;
		do {
			if (k < al)
			{
				a[k] = ch;
				k++;
			}
			getchdo;
		} while (ch >= 'a' && ch <= 'z' || ch >= '0' && ch <= '9');
		/*
		蔡晓芬-3218005438
		*/
		a[k] = 0;
		strcpy(id, a);
		i = 0;
		j = norw - 1;
		do {               /* 二分搜索当前符号是否为保留字 */
			k = (i + j) / 2;
			if (strcmp(id, word[k]) <= 0)
			{
				j = k - 1;
			}
			if (strcmp(id, word[k]) >= 0)
			{
				i = k + 1;
			}
		} while (i <= j);
		if (i - 1 > j)
		{
			sym = wsym[k];
		}
		else
		{
			sym = ident;    /* 搜索失败，则是名字 */
		}
	}
	else if (ch == '\'') {
		getchdo;
		sym = charlex;
		if (ch == '\\') {
			getchdo;
			if ('0' <= ch && ch <= '9') {
				k = 0;
				num = 0;
				sym = number;
				do {
					// 陳家健
					num = 10 * num + ch - '0';
					k++;
					getchdo;
				} while (ch >= '0' && ch <= '9');  /*获取数字的值*/

				k--;
				if (k > nmax) {
					error(30);
				}
			}
			else {
				switch (ch)
				{
				case 'a':num = '\a'; break;
				case 'b':num = '\b'; break;
				case 'f':num = '\f'; break;
				case 'n':num = '\n'; break;
				case 'r':num = '\r'; break;
				case 't':num = '\t'; break;
				case 'v':num = '\v'; break;
				case '\'':num = '\''; break;
				case '\"':num = '\"'; break;
				case '\\':num = '\\'; break;
				default: num = ch;break;
				}
				getchdo;
			}
		}
		else {
			num = ch;
			getchdo;}

		if (ch == '\'') { getchdo; }
		else { error(33); }
	}
	else if (ch == '.') {
		getchdo;
		if (ch >= '0' && ch <= '9') {
			k = 0;
			num = 0;
			sym = realnum;
			int d = 1;
			do {
				num = 10 * num + ch - '0';
				k++;
				d *= 10;
				do {
					getchdo;
				} while (ch >= '0' && ch <= '9' && d == real_div);/*消耗多余的数字。*/
			} while (ch >= '0' && ch <= '9');  /*获取数字的值*/
			k--;
			while (d < real_div) {
				k++;
				d *= 10;
				num *= 10;
			}
			if (k > nmax) {
				error(30);}
		}
		else {
			sym = period;
		}
	}
	else
	{
		if (ch >= '0' && ch <= '9')    /* 检测是否为数字：以0~9开头 */
		{
			k = 0;
			num = 0;
			sym = number;
			do {
				// 陳家健
				num = 10 * num + ch - '0';
				k++;
				getchdo;
			} while (ch >= '0' && ch <= '9');  /*获取数字的值*/

			if (ch == '.') {
				getchdo;
				sym = realnum;
				int d = 1;
				while (ch >= '0' && ch <= '9') {/*获取数字的值*/
					num = 10 * num + ch - '0';
					k++;
					d *= 10;
					do {
						getchdo;
					} while (ch >= '0' && ch <= '9' && d == real_div);/*消耗多余的数字。*/
				}
				while (d < real_div) {
					k++;
					d *= 10;
					num *= 10;
				}
				if (k > nmax) error(30);
			}

			k--;
			if (k > nmax) {
				error(30);
			}
		}
		else {
			if (ch == ':') {   /*检测赋值符号*/
				getchdo;
				if (ch == '=') {
					sym = becomes;
					getchdo;
				}
				else {
					sym = nul;   /*不能识别的符号*/
				}
			}
			//陳乙鑫
			else {
				if (ch == '<') /*检测小于或小于等于符号*/
				{
					getchdo;
					if (ch == '=') {
						sym = leq;
						getchdo;
					}
					else if (ch == '>') {
						sym = neq;
						getchdo;
					}
					else {
						sym = lss;
					}
				}
				else {
					if (ch == '>') /*检测大于或大于等于符号*/
					{
						getchdo;
						if (ch == '=') {
							sym = geq;
							getchdo;
						}
						else
						{

							//陈健
							sym = gtr;
						}
					}
					else {
						if (ch == '+') {
							getchdo;
							if (ch == '=') {
								sym = pluseql;
								getchdo;
							}
							else if (ch == '+') {
								sym = pplus;
								getchdo;
							}
							else {
								sym = plus;
							}
						}
						else {
							if (ch == '-') {
								getchdo;
								if (ch == '=') {
									sym = minuseql;
									getchdo;
								}
								else if (ch == '-') {
									sym = mminus;
									getchdo;
								}
								else {
									sym = minus;
								}
							}
							else {
								sym = ssym[ch]; /*当符号不满足上述条件时，全部按照单字符符号处理*/
								//getchdo;
								//richard
								if (sym != period) {
									getchdo;
								}
								//end richard
							}
						}
					}
				}
			}
		}
	}
	return 0;
}


/*
*生成虚拟机代码
*x:instruction.f;
*y: instruction.l;
*z:instruction.a;
*/
int gen(enum fct x, int y, int z) {
	if (cx >= cxmax) {
		printf("Program too long");/*程序过长*/
		return -1;
	}
	code[cx].f = x;
	code[cx].l = y;
	code[cx].a = z;
	cx++;
	return 0;
}
/*
*测试当前符号是否合法
*
*在某一部分（如一条语句，一个表达式）将要结束时我们希望下一个符号属于某集合
*（该部分的后跟符号），test负责这项检测，并且负责当检测不通过时的补救措施
*程序在需要检测时指定当前需要的符号集合和补救用的集合（如之前未完成部分的后跟
*符号），以及检测不通过时的错误号
*
*s1:我们需要的符号
*s2:如果不是我们需要的，则需要一个补救用的集合
*n:错误号
*/

//陈蜀毅
/*
* sym必须是sl的符号，
* 否则继续获取符号，知道sym是s1或s2的符号。
*/
int test(bool* sl, bool* s2, int n) {
	if (!inset(sym, sl)) {
		error(n);
		/*当检测不通过时，不停获取符号，直到他属于需要的集合或补救的集合*/
		while ((!inset(sym, sl)) && (!inset(sym, s2))) {
			getsymdo;
		}
	}
	return 0;
}
/*
*编译程序主体
*
*lev：当前分程序所在层
*tx:名字表当前尾指针
*fsys:当前模块后跟符号集合
*/

/*曾广宁*/
int block(int lev, int tx, bool* fsys) {
	int i;
	int dx;/*名字分配到的相对地址*/
	int tx0;/*保留初始tx*/
	int cx0;/*保留初始cx*/
	bool nxtlev[symnum];
	/*在下级函数的参数中，符号集合均为值参，但由于使用数组实现，传递进来的是指针，为防止下级函数改变上级函数的集合，开辟新的空间传递给下级函数*/
	dx = 3 + table[tx].argc;	/*加上参数变量的空间。*/
	tx0 = tx;/*记录本层名字的初始位置*/
	table[tx].adr = cx;
	tx = tx + table[tx].argc;	/*加上参数变量的名字表长度。*/
	gendo(jmp, 0, 0);/**跳转语句，以便跳过子过程代码，由于过程代码地址未定，故暂为0。**/
	if (lev > levmax) {
		error(32);
	}
	do {
		if (sym == constsym) /*收到常量声明符号，开始处理常量声明*/
		{
			getsymdo;
			do {
				constdeclarationdo(&tx, lev, &dx);/*dx的值会被constdeclaration改变，使用指针*/
				while (sym == comma) {
					getsymdo;
					constdeclarationdo(&tx, lev, &dx);
				}
				if (sym == semicolon)
					/* 3118005406 杜维佳 Begin */
				{
					getsymdo;
				}
				else
				{
					error(5);/*漏掉了逗号或者分号*/
				}
			} while (sym == ident);
		}
		if (sym == varsym)/*收到变量声明符号，开始处理变量声明*/
		{
			getsymdo;
			do {
				vardeclarationdo(&tx, lev, &dx);
				while(sym == comma)
				{
					getsymdo;
					vardeclarationdo(&tx, lev, &dx);
				}
				if (sym == semicolon)
				{
					getsymdo;
				}
				else
				{
					error(5);
				}
			} while (sym == ident);
		}
		while (sym == procsym || sym == intsym || sym == charsym || sym == realsym)/*收到过程声明符号，开始处理过程声明*/
		{
			enum data type = none;	/*记录过程返回类型。*/
			if (sym == intsym) {
				type = integer;
				getsymdo;}
			else if (sym == charsym) {
				type = character;
				getsymdo;}
			else if (sym == realsym) {
				type = real;
				getsymdo;}
			if (sym != procsym) {
				error(33);
			}
			else {
				getsymdo;
				if (sym == ident)
				{
					enter(procedur, type, &tx, lev, &dx);/*记录过程名字*/
					getsymdo;
					int ptx = tx;
					int pdx = 3;
					if (sym == lparen) 	/*处理参数变量。*/
					{getsymdo;
						do {
							if (sym == ident || sym == intsym || sym == charsym || sym == realsym) {
								vardeclarationdo(&ptx, lev + 1, &pdx);
								while (sym == comma)
								{getsymdo;
									vardeclarationdo(&ptx, lev + 1, &pdx);
								}
							}
							if (sym == rparen) { getsymdo; }
							else { error(5); }
						} while (sym == ident);
					}
					table[tx].argc = ptx - tx;
					for (int argi = 0; argi < table[tx].argc; argi++) {
						table[tx].argv[argi] = table[tx + argi + 1].type;
					}
					if (table[tx].argc > argc_max) {
						error(33);
						table[tx].argc = argc_max;
					}
				}
				else
				{
					error(4);/*procedure后应为标识符*/
				}
				if (sym == semicolon)
				{
					getsymdo;
				}
				else
				{
					error(5);/*漏掉了分号*/
				}
				/* 3118005406 杜维佳 End */

			   //杜仲谋
				memcpy(nxtlev, fsys, sizeof(bool) * symnum);
				nxtlev[semicolon] = true;
				if (-1 == block(lev + 1, tx, nxtlev))
				{
					return -1;           /*递归调用*/
				}
				if (sym == semicolon)
				{
					getsymdo;
					memcpy(nxtlev, statbegsys, sizeof(bool) * symnum);
					nxtlev[ident] = true;
					nxtlev[procsym] = true;
					nxtlev[intsym] = true;
					nxtlev[charsym] = true;
					nxtlev[realsym] = true;
					testdo(nxtlev, fsys, 6);
				}
				else
				{
					error(5);                    /*漏掉了分号*/
				}
			}
		}
		memcpy(nxtlev, statbegsys, sizeof(bool) * symnum);
		nxtlev[ident] = true;
		nxtlev[period] = true;
		testdo(nxtlev, declbegsys, 7);
	} while (inset(sym, declbegsys));  /*直到没有声明符号*/

	ptype = table[tx0].type;
	/*开始生成当前过程代码*/
	code[table[tx0].adr].a = cx;     /*设置首部跳转语句地址*/
	table[tx0].adr = cx;             /*设置当前过程代码地址*/
	table[tx0].size = dx;            /*设置当前过程栈帧大小，声明部分中每增加一条声明都会给dx增加1，声明部分
										已经结束，dx就是当前过程数据的size*/
	cx0 = cx;
	gendo(inte, 0, dx);              /*生成分配内存代码*/
	if (tableswitch)                 /*输出名字表*/
	{
		printf("TABLE:\n");
		if (tx0 + 1 > tx)
		{
			printf("NULL\n");
		}

		// begin 方俊涛 3118005408
		for (i = tx0 + 1; i <= tx; i++)
		{
			switch (table[i].kind)
			{

			case constant:
				printf("%d const %s ", i, table[i].name);
				printf("val = %d\n", table[i].val);
				fprintf(fas, "%d const %s", i, table[i].name);
				fprintf(fas, "val = %d\n", table[i].val);
				break;

			case variable:
				printf("%d var%s", i, table[i].name);
				printf("lev= %d addr=%d\n", table[i].level, table[i].adr);
				fprintf(fas, "%d var%s", i, table[i].name);
				fprintf(fas, "lev= %d addr= %d\n", table[i].level, table[i].adr);
				break;

			case array:
				printf("%d var%s", i, table[i].name);
				printf("lev= %d addr=%d length=%d\n", table[i].level, table[i].adr, table[i].length);
				fprintf(fas, "%d var%s", i, table[i].name);
				fprintf(fas, "lev= %d addr= %d\n", table[i].level, table[i].adr);
				break;

			case procedur:
				printf("%d proc%s ", i, table[i].name);
				printf("lev = %d addr= %d size = %d\n", table[i].level, table[i].adr, table[i].size);
				fprintf(fas, "%d proc%s ", i, table[i].name);
				fprintf(fas, "lev= %d addr= %d size= %d\n", table[i].level, table[i].adr, table[i].size);
				break;
			}
		}
		printf("\n");
	}
	/*语句后跟符号为分号或end */
	memcpy(nxtlev, fsys, sizeof(bool) * symnum);/*每个后跟符号集和都包含上层后跟符号集合，以便补救*/
	nxtlev[semicolon] = true;
	nxtlev[endsym] = true;
	ptype = table[tx0].type;
	statementdo(nxtlev, &tx, lev);	/*生成过程代码*/
	if (ptype == none) {
		gendo(opr, 0, 0);/*每个过程出口都要使用的释放数据段指令*/
	}
	else {
		if (havret == false) {	/*需得有返回值。*/
			error(33);
		}
	}
	memset(nxtlev, 0, sizeof(bool) * symnum);/*分程序没有补救集合*/
	testdo(fsys, nxtlev, 8);/*检测后跟符号的正确性*/
	listcode(cx0);/*输出代码*/
	return 0;
}
// end 方俊涛 3118005408


/*
 * 冯宇航
 * 3118005409
 */
 /*
  * 在名字表中加入一项
  *
  * k：名字种类 const、var 或 procedure
  * ptx：名字表尾指针的指针，为了可以改变名字表尾指针的值
  * lev：名字所在的层次，以后所有的 lev 都是这样
  * pdx：dx 为当前应分配的变量的相对地址，分配后要增加1
  */
void enter(enum object k, enum data t, int* ptx, int lev, int* pdx)
{
	(*ptx)++;
	strcpy(table[(*ptx)].name, id);		/*全局变量id中已存有当前名字的名字*/
	table[(*ptx)].kind = k;
	table[(*ptx)].type = t;
	switch (k)
	{
	case constant:						/*常量名字*/
		if (((t == integer || t == character) && num > amax) || (t == real && num > amax * real_div))
		{
			error(31);					/*数越界*/
			num = 0;
		}
		table[(*ptx)].val = num;
		break;
	case variable:						/*变量名字*/
		table[(*ptx)].level = lev;
		table[(*ptx)].adr = (*pdx);
		(*pdx)++;
		break;
	case array:
		table[(*ptx)].level = lev;
		table[(*ptx)].adr = (*pdx);
		table[(*ptx)].length = length;
		(*pdx)+=length;
	case procedur:						/*过程名字*/
		table[(*ptx)].level = lev;
		/*
		*何悦
		*/

		break;
	}
}

/*
 * 查找名字的位置
 * 找到则返回在名字表中的位置,否则返回0
 *
 *  idt:要查找的名字
 *  tx:当前名字表尾指针
 */

int position(char* idt, int tx)
{
	int i;
	strcpy(table[0].name, idt);
	i = tx;
	while (strcmp(table[i].name, idt) != 0)
	{
		i--;
	}
	return i;
}
/*
  *常量声明处理
  */
int constdeclaration(int* ptx, int lev, int* pdx)
{
	if (sym == ident)
	{
		getsymdo;
		if (sym == eql || sym == becomes)
		{
			if (sym == becomes)
			{
				error(1); /*把=写成了:=*/
			}
			getsymdo;
			if (sym == number)
			{
				enter(constant, integer, ptx, lev, pdx);
				getsymdo;
			}
			else if (sym == realnum) {
				enter(constant, real, ptx, lev, pdx);
				getsymdo;}
			else if (sym == charlex) {
				enter(constant, character, ptx, lev, pdx);
				getsymdo;}
			else
			{
				error(2); /*常量说明-后应是数字*/
			}
		}
		else
		{
			error(3); /* 常量说明标识后应是= */
		}
	}
	else
		/*
	  *晓煜
	  */
	{
		error(4); /*const 后应是标识 */
	}
	return 0;
}

/*
*变量声明处理
*/
int vardeclaration(int* ptx, int lev, int* pdx)
{
	enum data t = integer;
	if (sym == realsym) {
		t = real;
		getsymdo;
	}
	else if (sym == charsym) {
		t = character;
		getsymdo;
	}
	else if (sym == intsym) {
		t = integer;
		getsymdo;
	}

	enum object kind = variable;
	length = 1;
	if (sym == lsparen) {
		kind = array;
		getsymdo;
		if (sym == number || sym == charlex) {
			length = num;
			getsymdo;
			if (sym == rsparen) {
				getsymdo;
			}
		}
	}

	if (sym == ident)
	{
		enter(kind, t, ptx, lev, pdx); //填写名字表
		getsymdo;
	}
	else
	{
		error(4); /*var 后应是标识*/
	}
	return 0;
}

/*
*输出目标代码清单
*/
void listcode(int cx0)
{
	int i;
	if (listswitch)
	{
		for (i = cx0; i < cx; i++)
		{
			printf("%d %s %d %d\n", i, mnemonic[code[i].f], code[i].l, code[i].a);
			fprintf(fa, "%d %s %d %d\n", i, mnemonic[code[i].f], code[i].l, code[i].a);
		}
	}
}

/*
*语句处理
* fsys：语句可能后跟符号的集合。
* ptx: 名字表指针的pointer。
* lev: 语句的层次。
*/
int statement(bool* fsys, int* ptx, int lev)
{
	int i, cx1, cx2;//i是被查询的符号在符号表的指针。
	bool nxtlev[symnum];
	enum bool lasthavret = havret;/*保存上一条语句的返回状态。*/
	havret = false;	/*先假设该语句没有返回。*/
		if (sym == ident) /*准备按照赋值语句处理*/
	{
		i = position(id, *ptx);
		if (i == 0)
		{
			error(11); /*变量未找到*/
		}
		else
		{
			/*
			*科宪
			*/
			if (table[i].kind != variable && table[i].kind != array)
			{
				error(12); /*赋值语句格式错误*/
				i = 0;
			}
			else
			{
				getsymdo;
				if (table[i].kind == array) {
					if (sym == lsparen) {
						getsymdo;
					}
					memset(nxtlev, false, sizeof(bool) * symnum);
					nxtlev[rsparen] = true;
					expressiondo(nxtlev, ptx, lev);
					gendo(rsto, 0, rt++);
					if (sym == rsparen) {
						getsymdo;
					}
				}
				if (sym == pplus || sym == mminus) {
					if (table[i].kind == array) {
						gendo(rlod, 0, rt);
						gendo(plod, lev - table[i].level, table[i].adr);
					}
					else {
						gendo(lod, lev - table[i].level, table[i].adr);
					}
					if (table[i].type != real) {
						gendo(lit, 0, 1);
					}
					else{
						gendo(lit, 0, real_div);
					}
					if (sym == pplus) {
						gendo(opr, 0, 2);
					}
					else {
						gendo(opr, 0, 3);
					}
					if (table[i].kind == array) {
						gendo(rlod, 0, rt);
						gendo(psto, lev - table[i].level, table[i].adr);
					}
					else {
						gendo(sto, lev - table[i].level, table[i].adr);
					}
					
					getsymdo;	
				}
				else {
					enum symbol type = sym;
					if (sym == becomes || sym == pluseql || sym == minuseql)
					{
						getsymdo;
					}
					else
					{
						error(13); /*没有检测到赋值符号 */
					}
					if(sym != callsym) {
						if (type != becomes) {
							if (table[i].kind == array) {
								gendo(rlod, 0, rt);
								gendo(plod, lev - table[i].level, table[i].adr);
							}
							else {
								gendo(lod, lev - table[i].level, table[i].adr);
							}
							if (table[i].type != real) {	/*转换为实数格式以便计算。*/
								gendo(lit, 0, real_div);
								gendo(opr, 0, 4);
							}
						}
						extpe = integer;
						memcpy(nxtlev, fsys, sizeof(bool) * symnum);
						expressiondo(nxtlev, ptx, lev); /*处理赋值符号右侧表达式*/
						if (type == pluseql) {
							gen(opr, 0, 2);	/*id + expression*/
						}
						else if (type == minuseql) {
							gen(opr, 0, 3);	/*id - expression*/
						}
						if (i != 0)
						{
							if (table[i].type != real) {	/*转换为integer.*/
								gendo(lit, 0, real_div);
								gendo(opr, 0, 5);
							}
							/*expression将执行一系列指令，但最终结果将会保存在栈顶，执行 sto命令完成赋值 */
							if (table[i].kind == array) {
								gendo(rlod, 0, rt);
								gendo(psto, lev - table[i].level, table[i].adr);
							}
							else {
								gendo(sto, lev - table[i].level, table[i].adr);
							}
						}
					}else {/*调用过程。*/
						statementdo(fsys, ptx, lev);
						if (ctype != none) {
							gendo(inte, 0, 1);	/*升高栈顶以获取返回值。*/
							if (table[i].type != real && ctype == real) {/*转换返回类型为变量类型。*/
								gendo(lit, 0, real_div);
								gendo(opr, 0, 5);}

							if (type != becomes) {
								if (table[i].kind == array) {
									gendo(rlod, 0, rt);
									gendo(plod, lev - table[i].level, table[i].adr);
								}
								else {
									gendo(lod, lev - table[i].level, table[i].adr);
								}
								if (type == pluseql) { gendo(opr, 0, 2); }	/*expression + id*/
								else if (type == minuseql) {
									gendo(opr, 0, 3); /*expression - id*/
									gendo(opr, 0, 1); /*id - expression*/
								}	
							}
							if (table[i].kind == array) {
								gendo(rlod, 0, rt);
								gendo(psto, lev - table[i].level, table[i].adr);
							}
							else {
								gendo(sto, lev - table[i].level, table[i].adr);
							}
						}}
				}
				if (table[i].kind == array) {
					rt--;
				}
			}
		}//if(i == 0)
	}
	else if (sym == pplus || sym == mminus) {
		int a = 0;
		if (sym == pplus)		a = 2;
		else if (sym == mminus)	a = 3;

		getsymdo;

		if (sym != ident) {
			error(33);/*格式错误。*/
		}
		else {
			i = position(id, *ptx);
			if (i == 0){
				error(11); /*变量未找到*/
			}
			else if (table[i].kind != variable && table[i].kind != array){
					error(12); /*赋值语句格式错误*/
					i = 0;
			}
			else {
				getsymdo;
				if (table[i].kind == array) {
					if (sym == lsparen) {
						getsymdo;
					}
					memset(nxtlev, false, sizeof(bool)* symnum);
					expressiondo(nxtlev, ptx, lev);
					gendo(rsto, 0, rt++);
					if (sym == rsparen) {
						getsymdo;
					}
				}
				
				if (table[i].kind == array) {
					gendo(rlod, 0, rt);
					gendo(plod, lev - table[i].level, table[i].adr);
				}
				else {
					gendo(lod, lev - table[i].level, table[i].adr);
				}
				if (table[i].type != real) {
					gendo(lit, 0, 1);
				}
				else{
					gendo(lit, 0, real_div);
				}
				gendo(opr, 0, a);
				if (table[i].kind == array) {
					gendo(rlod, 0, rt);
					gendo(psto, lev - table[i].level, table[i].adr);
				}
				else {
					gendo(sto, lev - table[i].level, table[i].adr);
				}
				
				if (sym == pplus || sym == mminus) {
					if (table[i].kind == array) {
						gendo(rlod, 0, rt);
						gendo(plod, lev - table[i].level, table[i].adr);
					}
					else {
						gendo(lod, lev - table[i].level, table[i].adr);
					}
					if (table[i].type != real) {
						gendo(lit, 0, 1);
					}
					else {
						gendo(lit, 0, real_div);
					}
					if (sym == pplus) {
						gendo(opr, 0, 2);
					}
					else {
						gendo(opr, 0, 3);
					}
					if (table[i].kind == array) {
						gendo(rlod, 0, rt);
						gendo(psto, lev - table[i].level, table[i].adr);
					}
					else {
						gendo(sto, lev - table[i].level, table[i].adr);
					}
					getsymdo;}
				if (table[i].kind == array) {
					rt--;
				}
			}
		}
	}
	else
	{
		if (sym == readsym) /*准备按照 read语句处理*/
		{
			getsymdo;
			if (sym != lparen)
			{
				error(34); /*格式错误，应是左括号 */
			}
			else
			{
				do
				{
					getsymdo;
					if (sym == ident)
					{
						i = position(id, *ptx); /*查找要读的变量 */
					}
					else
					{
						i = 0;
					}
					if (i == 0)
					{
						error(35); /*read() 中应是声明过的变量名 */
					}
					/*
						* 孔止
						* begin
					*/

					else
					{
						gendo(opr, 0, 16);  //生成输入指令，读取值到栈顶
						if (table[i].type != real) {/*将real转换成integer。*/
							gendo(lit, 0, real_div);
							gendo(opr, 0, 5);
						}
						if (table[i].kind == array) {
							getsymdo;
							if (sym == lsparen) {
								getsymdo;
							}
							memset(nxtlev, false, sizeof(bool)* symnum);
							nxtlev[rsparen] = true;
							expressiondo(nxtlev, ptx, lev);
							gendo(rsto, 0, rt++);
							gendo(rlod, 0, rt);
							gendo(psto, lev - table[i].level, table[i].adr);
							rt--;
						}
						else{
							gendo(sto, lev - table[i].level, table[i].adr);  //储存到变量
						}
					}
					getsymdo;
				} while (sym == comma);  //一条read语句可读多个变量
			}
			if (sym != rparen)
			{
				error(33);                  //格式错误，应是右括号
				while (!inset(sym, fsys))    //出错补救，直到收到上层函数的后跟符号
				{
					getsymdo;
				}
			}
			else
			{
				getsymdo;
			}
		}
		else
		{
			if (sym == writesym)   //准备按照write语句处理，与read类似
			{
				getsymdo;
				if (sym == lparen)
				{
					do {
						getsymdo;
						memcpy(nxtlev, fsys, sizeof(bool) * symnum);
						nxtlev[rparen] = true;
						nxtlev[comma] = true;  //write的后跟符号为 ) or ,
						cx1 = cx;
						expressiondo(nxtlev, ptx, lev);   /*
															* 调用表达式处理，此处与read不同，
															* read为给变量赋值
															*/
						gendo(opr, 0, 14);     //生成输出指令，输出栈顶的值
					} while (sym == comma);
					if (sym != rparen)
					{
						error(33);    //write()中应为完整表达式
					}
					else
					{
						getsymdo;
					}
				}
				gendo(opr, 0, 15);      //输出换行
			}
			else
			{
				if (sym == callsym)      //准备按照call语句处理
				{
					ctype = none;
					getsymdo;
					if (sym != ident)
					{
						error(14);      //call后应为标识符
					}
					else
					{
						i = position(id, *ptx);
						if (i == 0)
						{
							error(11);  //过程未找到
							getsymdo;
						}
						else
						{
							ctype = table[i].type;	/*被调用过程的返回类型。*/
							gendo(cal, 1, cx + 2);	/*把参数传递过程当作一个过程来调用，以便传参。*/
							int ipbcx = cx;
							gendo(jmp, 0, 0);	/*跳出无名过程代码。*/
							gendo(inte, 0, 3);
							getsymdo;
							if (sym == lparen) {/*传递参数过程。*/
								getsymdo;
								for (int argi = 0; argi < table[i].argc; argi++) {
									if (argi != 0) {
										while (sym != comma)
										{
											getsymdo;
										}
										getsymdo;
									}
									extpe = integer;
									memset(nxtlev, false, sizeof(bool)* symnum);
									nxtlev[comma] = true;
									nxtlev[rparen] = true;
									expressiondo(nxtlev, ptx, lev+1); /* 处理参数表达式。*/

									if (table[i].argv[argi] != real) {/*转换为参数类型。*/
										gendo(lit, 0, real_div);
										gendo(opr, 0, 5);
									}
								}
								if (sym == rparen) {
									getsymdo;
								}
								else {
									error(33);
								}
							}
							gendo(opr, 0, 0);	/*结束参数传递过程。*/
							code[ipbcx].a = cx; /*跳出无名过程代码。*/
							if (table[i].kind == procedur)
							{
								gendo(cal, lev - table[i].level, table[i].adr);
							}
							else
							{
								error(15);
							}
						}
					}
				}
				else if (sym == retnsym) {
					havret = true;	/*该语句有返回。*/
					getsymdo;
					if (ptype != none) {
						extpe = integer;
						memcpy(nxtlev, fsys, sizeof(bool) * symnum);
						expressiondo(nxtlev, ptx, lev); /*处理赋值符号右侧表达式*/
						if (ptype != real && extpe == real) {
							gendo(lit, 0, real_div);
							gendo(opr, 0, 5);
						}
						gendo(sto, 0, 0);
						gendo(opr, 0, 0);
					}
				}

				/*
				* 卢柏铖
				* begin
				*/
				else
				{
					if (sym == ifsym)
					{
						enum bool allhavret = true;
						getsymdo;
						memcpy(nxtlev, fsys, sizeof(bool) * symnum);
						nxtlev[thensym] = true;
						nxtlev[dosym] = true;
						conditiondo(nxtlev, ptx, lev);
						if (sym == thensym)
						{
							getsymdo;
						}
						else
						{
							error(16);
						}
						cx1 = cx;
						gendo(jpc, 0, 0);
						statementdo(fsys, ptx, lev);
						if (havret != true) {/*记录if子句是否有返回。*/
							allhavret = false;
						}
						if (sym == elsesym) {
							gendo(jmp, 0, 0);	/*跳过接下来的else子句。*/
						}
						code[cx1].a = cx;
						if (sym == elsesym) {	/*处理else子句。*/
							getsymdo;
							cx1 = cx - 1;					/*记录待gendo(jmp, 0, 0)代码的地址。*/
							statementdo(fsys, ptx, lev);	/*分析else子句。*/
							code[cx1].a = cx;				/*反填跳转语句跳转地址。*/
							if (havret != true) {	/*记录if子句和else子句是否都返回。*/
								allhavret = false;
							}
						}
						havret = allhavret;	/*记录if语句是否有返回。*/

					}
					else
					{
						/*
						* end
						*/

						/*
						* 李兆海
						*/
						if (sym == beginsym)		/*准备按照复合语句处理*/
						{
							getsymdo;
							memcpy(nxtlev, fsys, sizeof(bool) * symnum);
							nxtlev[semicolon] = true;
							nxtlev[endsym] = true;		/*后跟符号为分号或end*/
														/*循环调用语句处理函数，直到下一个符号不是语句开始符号或收到end*/
							statementdo(nxtlev, ptx, lev);
							while (inset(sym, statbegsys) || sym == semicolon)
							{
								if (sym == semicolon)
								{
									getsymdo;
								}
								else
								{
									error(10);		/*缺少分号*/
								}
								statementdo(nxtlev, ptx, lev);
							}
							if (sym == endsym)
							{
								getsymdo;
							}
							else
							{
								error(17);		/*缺少end或分号*/
							}
						}
						/*
						* end
						*/

						/*
						* 刘芊羿
						* begin
						*/

						else
						{
							if (sym == whilesym)
							{
								cx1 = cx;
								getsymdo;
								memcpy(nxtlev, fsys, sizeof(bool) * symnum);
								nxtlev[dosym] = true;
								conditiondo(nxtlev, ptx, lev);
								cx2 = cx;
								gendo(jpc, 0, 0);
								if (sym == dosym)
								{
									getsymdo;
								}
								else {
									error(18);
								}
								statementdo(fsys, ptx, lev);
								gendo(jmp, 0, cx1);
								/*
									* end
									*/
									//3118005419彭凯金 P388

								code[cx2].a = cx;  /* 反填跳出循环的地址，与if类似 */
							}
							else if (sym == forsym) {
								getsymdo;
								if (sym != ident) {
									error(14);
								}
								else {
									i = position(id, *ptx);
									if (i == 0) {
										error(11);
									}
									else{
										memset(nxtlev, 0, sizeof(bool)* symnum);
										nxtlev[tosym] = true;
										nxtlev[downsym] = true;
										statementdo(nxtlev, ptx, lev);				/*处理赋值语句。*/
										rt++;

										if (sym != tosym && sym != downsym) {		/*处理TO或DOWNTO关键字*/
											error(33);
										}
										else {
											enum symbol flag = sym;
											getsymdo;

											cx1 = cx;								/*循环入口*/
											if (table[i].kind == array) {
												gendo(rlod, 0, rt);
												gendo(plod, lev - table[i].level, table[i].adr);	/*加载变量值*/
											}
											else {
												gendo(lod, lev - table[i].level, table[i].adr);	/*加载变量值*/
											}
											if (table[i].type != real) {	/*一律使用实数格式比较。*/
												gendo(lit, 0, real_div);
												gendo(opr, 0, 4);
											}
											memset(nxtlev, 0, sizeof(bool) * symnum);
											nxtlev[dosym] = true;
											expressiondo(nxtlev, ptx, lev);			/*处理第2个表达式。*/

											if (flag == tosym) {							/*循环判断*/
												gendo(opr, 0, 13);								/*id >= 结束值*/
											}
											else {
												gendo(opr, 0, 11);								/*id <= 结束值 */
											}
											cx2 = cx;
											gendo(jpc, 0, 0);									/*条件转移语句*/

											if (sym != dosym) {								/*处理DO*/
												error(33);										/*格式错误，缺少do*/
											}
											getsymdo;

											statementdo(fsys, ptx, lev);					/*处理语句。*/
																							/*计数处理。*/
											if (table[i].kind == array) {
												gendo(rlod, 0, rt);
												gendo(plod, lev - table[i].level, table[i].adr);
											}
											else {
												gendo(lod, lev - table[i].level, table[i].adr);		/*加载迭代变量。*/
											}
											if (table[i].type != real) {
												gendo(lit, 0, 1);									/*加载自增或自减值。*/
											}
											else {
												gendo(lit, 0, real_div);
											}
											if (flag == tosym) {								/*加法或减法*/
												gendo(opr, 0, 2);									/*加上自增值。*/
											}
											else {
												gendo(opr, 0, 3);									/*减去自减值。*/
											}
											if (table[i].kind == array) {
												gendo(rlod, 0, rt);
												gendo(psto, lev - table[i].level, table[i].adr);
											}
											else {
												gendo(sto, lev - table[i].level, table[i].adr);		/*更新迭代变量。*/
											}
											gendo(jmp, 0, cx1);									/*跳转到循环入口。*/

											code[cx2].a = cx;								/*反填条件转移语句。*/
										}
										rt--;
									}
								}
							}
							else
							{
								memset(nxtlev, 0, sizeof(bool) * symnum); /* 语句结束无补救集合*/

								testdo(fsys, nxtlev, 19); /* 检测语句结束的正确性 */

								havret = lasthavret;/*无效语句不考虑。*/
							}
						}
					}

				}
			}
		}
	}
	return 0;
}

//括号对应有问题,需解决？？，已经解决，原780行少了个括号



/*
* 表达式处理
* 将结果保存到栈顶。
*/
int expression(bool* fsys, int* ptx, int lev)
{
	enum symbol addop;   /* 用于保存正负号 */
	bool nxtlev[symnum];
	//3118005424谭宗麟
	if (sym == plus || sym == minus)          /*开头的正负号，此时当前表达式被看作一个正的或负的项*/
	{
		addop = sym;                     /*保存开头的正负号*/
		getsymdo;
		memcpy(nxtlev, fsys, sizeof(bool) * symnum);
		nxtlev[plus] = true;
		nxtlev[minus] = true;
		termdo(nxtlev, ptx, lev);       /*处理项*/
		if (addop == minus)
		{
			gendo(opr, 0, 1);           /*如果开头为负号，生成取负指令*/
		}
	}
	else                              /*此时表达式被看作项的加减*/
	{
		memcpy(nxtlev, fsys, sizeof(bool) * symnum);
		nxtlev[plus] = true;
		nxtlev[minus] = true;
		termdo(nxtlev, ptx, lev);      /*处理项*/
	}

	//3118005418 卢敏聪 P389
	while (sym == plus || sym == minus)
	{
		addop = sym;
		getsymdo;
		memcpy(nxtlev, fsys, sizeof(bool) * symnum);
		nxtlev[plus] = true;
		nxtlev[minus] = true;
		termdo(nxtlev, ptx, lev); /*处理项*/
		if (addop == plus)
		{
			gendo(opr, 0, 2);//生成加法指令
		}
		else
		{
			gendo(opr,0,3);//生成减法指令
		}
	}
	return 0;
}


/* 3118005421 邱泽滨 P389
	* 项处理
	*/
int term(bool* fsys, int* ptx, int lev)
{
	enum symbol mulop;              /*保存乘除法符号*/
	bool nxtlev[symnum];
	memcpy(nxtlev, fsys, sizeof(bool) * symnum);
	nxtlev[times] = true;
	nxtlev[slash] = true;
	factordo(nxtlev, ptx, lev);
	while (sym == times || sym == slash)
	{
		if (sym == slash) {
			gendo(lit, 0, real_div);/*方便实数除法。*/
			gendo(opr, 0, 4);
		}
		mulop = sym;
		getsymdo;
		factordo(nxtlev, ptx, lev);   /*处理因子*/
		if (mulop == times)
		{
			gendo(opr, 0, 4);         /*生成乘法指令*/
			gendo(lit, 0, real_div);  /*恢复实数格式。*/
			gendo(opr, 0, 5);		  
		}
		else
		{
			gendo(opr, 0, 5);         /*生成除法指令*/
		}
	}
	return 0;
}



// 沈权斌
/*
	因子处理
*/
int factor(bool* fsys, int* ptx, int lev) {
	int i;
	bool nxtlev[symnum];
	testdo(facbegsys, fsys, 24);   //检测因子的开始符号
	while (inset(sym, facbegsys)) {  //循环直到不是因子开始符号
		int a = 0;
		if (sym == pplus ){		/*处理先自增自减。*/
			a = 2;
			getsymdo;
		}else if(sym == mminus) {
			a = 3;
			getsymdo;
		}
		if (sym == ident) {     //因子为常量或变量或数组
			i = position(id, *ptx);  //查找名字
			if (i == 0) {
				error(11);      //标识符未声明
			}
			else {
				getsymdo;
				if (table[i].type == real) {
					extpe = real;
				}
				switch (table[i].kind) {
				case constant:      //名字为常量
					gendo(lit, 0, table[i].val);    // 直接把常量的值入栈
					if (table[i].type != real) {
						gendo(lit, 0, real_div);
						gendo(opr, 0, 4);
					}
					a = 0;
					break;
				case variable:      //名字为变量
					if (0 != a) {				/*处理先自增自减再取值。*/
						gendo(lod, lev - table[i].level, table[i].adr);
						if (table[i].type != real) {
							gendo(lit, 0, 1);
						}
						else {
							gendo(lit, 0, real_div);
						}
						gendo(opr, 0, a);
						gendo(sto, lev - table[i].level, table[i].adr);
					}
					gendo(lod, lev - table[i].level, table[i].adr);    //找到变量地址并将其入栈
					if (table[i].type != real) {
						gendo(lit, 0, real_div);
						gendo(opr, 0, 4);
					}
					a = 1;
					break;
				case array:
					if (sym == lsparen) {
						getsymdo;
					}
					memset(nxtlev, false, sizeof(bool) * symnum);
					nxtlev[rsparen] = true;
					expressiondo(nxtlev, ptx, lev);
					gendo(rsto, 0, rt++);	/*保存偏移量，以便后续计算。*/
					if (sym == rsparen) {
						getsymdo;
					}
					
					if (0 != a) {				/*处理先自增自减再取值。*/
						gendo(rlod, 0, rt);		/*取偏移量。*/
						gendo(plod, lev - table[i].level, table[i].adr);
						if (table[i].type != real) {
							gendo(lit, 0, 1);
						}
						else {
							gendo(lit, 0, real_div);
						}
						gendo(opr, 0, a);
						gendo(rlod, 0, rt);		/*取偏移量*/
						gendo(psto, lev - table[i].level, table[i].adr);
					}
					gendo(rlod, 0, rt);	/*取偏移量*/
					gendo(plod, lev - table[i].level, table[i].adr);    //找到变量地址并将其入栈
					if (table[i].type != real) {
						gendo(lit, 0, real_div);
						gendo(opr, 0, 4);
					}
					a = 1;
					break;
				case procedur:      //名字为过程
					error(21);          //不能为过程
					a = 0;
					break;
				}
			}
			
			if (1 == a && (sym == pplus || sym == mminus)) {	/*处理后自增自减。*/
				if (sym == pplus)		a = 2;
				else if (sym == mminus)	a = 3;

				if (table[i].kind == array) {
					gendo(rlod, 0, rt);
					gendo(plod, lev - table[i].level, table[i].adr);
				}
				else {
					gendo(lod, lev - table[i].level, table[i].adr);
				}
				if (table[i].type != real) {
					gendo(lit, 0, 1);
				}
				else {
					gendo(lit, 0, real_div);
				}
				gendo(opr, 0, a);
				if (table[i].kind == array) {
					gendo(rlod, 0, rt);
					gendo(psto, lev - table[i].level, table[i].adr);
				}
				else {
					gendo(sto, lev - table[i].level, table[i].adr);
				}
				getsymdo;}
			if (table[i].kind == array) {
				rt--;
			}
		}
		else {
			if (sym == number || sym == charlex) {     //因子为数
				if (num > amax) {
					error(31);
					num = 0;
				}
				gendo(lit, 0, num * real_div);
				getsymdo;
			}
			else if (sym == realnum) {
				extpe = real;
				if (num > amax * real_div) {
					error(31);
					num = 0;
				}
				gendo(lit, 0, num);
				getsymdo;}
			else {
				if (sym == lparen) {     //因子为表达式
					getsymdo;
					memcpy(nxtlev, fsys, sizeof(bool) * symnum);
					nxtlev[rparen] = true;
					expressiondo(nxtlev, ptx, lev);
					if (sym == rparen) {
						getsymdo;
					}
					else {
						error(22);      //缺少右括号
					}
				}
				testdo(fsys, facbegsys, 23);      //因子后有非法符号
			}
		}
	}
	return 0;
}

/*
条件处理。
若测试成功将储存0到栈顶。
*/
int condition(bool* fsys, int* ptx, int lev) {
	enum symbol relop;
	bool nxtlev[symnum];
	if (sym == oddsym) {      //准备按照 odd 运算处理
		getsymdo;
		expressiondo(fsys, ptx, lev);
		gendo(opr, 0, 6);     //生成 odd 指令
	}
	else {
		// 逻辑表达式处理
		memcpy(nxtlev, fsys, sizeof(bool) * symnum);
		nxtlev[eql] = true;
		nxtlev[neq] = true;
		nxtlev[lss] = true;
		nxtlev[leq] = true;


		//肖起鑫
		nxtlev[gtr] = true;
		nxtlev[geq] = true;
		expressiondo(nxtlev, ptx, lev);
		if (sym != eql && sym != neq && sym != lss && sym != leq && sym != gtr && sym != geq)
		{
			error(20);
		}
		else {
			relop = sym;
			getsymdo;
			expressiondo(fsys, ptx, lev);
			switch (relop)
			{
			case eql:
				gendo(opr, 0, 8);
				break;
			case neq:
				gendo(opr, 0, 9);
				break;
				//吴春丰
			case lss:
				gendo(opr, 0, 10);
				break;
			case geq:
				gendo(opr, 0, 11);
				break;
			case gtr:
				gendo(opr, 0, 12);
				break;
			case leq:
				gendo(opr, 0, 13);
				break;
			}
		}
	}
	return 0;
}


/*
	*解释程序
	*/
void interpret()
{
	int p, b, t;			/*指令指针，栈帧基址，栈顶指针，t[b]:访问链父帧基址，t[b+1]:控制链父帧基址，t[b+2]:返回地址。*/
	struct instruction i; 		/*存放当前指令*/
	int s[stacksize];		/*栈*/
	int reg[stacksize/5];		/*寄存器。*/
	printf("start pl0\n");
	//吴润和
	t = 0;
	b = 0;
	p = 0;
	s[0] = s[1] = s[2] = 0;
	do {
		i = code[p];					/*请当前指令*/
		p++;							/*计数下一个指令。*/
		switch (i.f) {
		case lit:
			s[t] = i.a;				/*将。的值取到栈顶*/
			t++;
			break;
		case opr:					/*数学、逻辑运算*/
			switch (i.a)
			{
			case 0:
				t = b;			
				p = s[t + 2];
				b = s[t + 1];	
				break;
			case 1:
				s[t - 1] = -s[t - 1];
				break;
			case 2:
				t--;
				s[t - 1] = s[t - 1] + s[t];
				break;
			case 3:
				t--;
				s[t - 1] = s[t - 1] - s[t];
				break;
			case 4:
				t--;
				s[t - 1] = s[t - 1] * s[t];
				break;
			case 5:
				t--;
				s[t - 1] = s[t - 1] / s[t];
				break;
			case 6:
				s[t - 1] = s[t - 1] % 2;
				break;
			case 8:
				//信安2班 肖裕海，严为炜
				t--;
				s[t - 1] = (s[t - 1] == s[t]);
				break;

			case 9:
				t--;
				s[t - 1] = (s[t - 1] != s[t]);
				break;

			case 10:
				t--;
				s[t - 1] = (s[t - 1] < s[t]);
				break;

			case 11:
				t--;
				s[t - 1] = (s[t - 1] >= s[t]);
				break;

			case 12:
				t--;
				s[t - 1] = (s[t - 1] > s[t]);
				break;

			case 13:
				t--;
				s[t - 1] = (s[t - 1] <= s[t]);
				break;

			case 14:
				printf("%.3lf", ((double)s[t-1]) / real_div);
				fprintf(fa2, "%.3lf", ((double)s[t - 1]) / real_div);
				t--;
				break;

			case 15:
				printf("\n");
				fprintf(fa2, "\n");
				break;

			case 16:
				printf("?");
				fprintf(fa2, "?");
				double input = 0;
				scanf("%lf", &input);
				fprintf(fa2, "%lf\n", input);
				s[t] = (int)(input * real_div);
				t++;
				break;
			}
			break;
		case lod:
			s[t] = s[base(i.l, s, b) + i.a];
			t++;
			break;
			// 杨海滨
		case sto: /*栈顶的值存到相对当前过程的数据基地址为a的内存*/
			t--;
			s[base(i.l, s, b) + i.a] = s[t];
			break;
		case plod:
			s[t - 1] = s[base(i.l, s, b) + i.a + s[t - 1]];
			break;
		case psto:
			t--;
			s[base(i.l, s, b) + i.a + s[t]] = s[t-1]; 
			t--;
			break;
		case rlod:
			s[t] = reg[i.a-1];
			t++;
			break;
		case rsto:
			t--;
			reg[i.a] = s[t]/1000;
			break;
		case cal:  /*调用子过程*/
			s[t] = base(i.l, s, b); /*将父过程基地址人栈*/				/*静态链（访问链），找到最近的层次与被调用过程相同的父过程的父过程，该父过程的父过程也是被调用过程的父过程，故可通过层差获取。*/
			s[t + 1] = b;  /*将本过程基地址人栈，此两项用于base函数*/		/*动态链（控制链）*/
			s[t + 2] = p;    /*将当前指令指针入栈*/						/*返回地址*/
			b = t;  /*改变基地址指针值为新过程的基地址*/	
			p = i.a;  /*跳转*/
			break;

		case inte:   /*分配内存*/
			t += i.a;
			break;

		case jmp:  /* 直接跳转 */
			p = i.a;
			break;

		case jpc:  /*条件跳转*/
			t--;
			if (s[t] == 0) {
				p = i.a;
			}
			break;
		}
	} while (p != 0);

}

/*张家维 */
/* 通过过程基址求上一层过程的基址 */
int base(int l, int* s, int b)
{
	int b1;
	b1 = b;
	while (l > 0)
	{
		b1 = s[b1];
		l--;
	}
	return b1;
}
