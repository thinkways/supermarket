#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "market.h"
#include "filemgr.h"
#include "datamgr.h"

extern struct goods *hashtable[hashlen];
int backup(void){
	int i=0;
	FILE *fp;
	struct goods *p=NULL;

	if((fp=fopen(FilePath,"w"))==NULL){
		puts("open file failed!");
		return 0;
	}
	while(i<hashlen){
		p=hashtable[i];
		if(p==NULL){
			i++;
			continue;
		}else{
			for(;p!=NULL;p=p->next)
				fprintf(fp,"%s#%s#%s#%f#%f#%d#\n",p->id,p->name,p->category,\
				p->cost,p->price,p->amount);
		}
		i++;
	}
	fclose(fp);
	return 1;
}

int load(const char *filename) {
	FILE *file;
	char Buff[100]= {0};
	struct goods goods_chain;
	int notes=0; //记录读取到行数,每读取到一行，notes就自+1；
	/********打开文件 ***********/
	file=fopen(filename,"r");
	if(NULL==file) {
		//printf("没有该文件，即将创建文件");
		file=fopen(filename,"w+");
	} else {
		//do nothing;
	}
	/********读取文件到Buff ***********/
	
	while(fgets(Buff,sizeof(Buff),file)!=NULL) {

		/*	2、把读取这行的数据，分别赋给结构体变量成员；*/
		char TempBuff[20]= {0};
		int cur=0; //cur当前光标位置
		int i=1;
		for(; i<7; i++) { //i是#的个数，#共有6个；
			int k=0; //k置零，TempBuff重复使用；
			int j=cur;
			for(; j<100; j++) {
				if(Buff[j]!='#') {
					TempBuff[k++]=Buff[j];
				} else {
					//读到#标志， break,
					TempBuff[k]='\0';//字符串结束标志
					cur=j+1;//设置当前光标+1  指向下一个商品信息；
					break;//跳出这层循环；i++自加
				}
			}
			//Temp保存了当前的类别数据； 需要根据读到的数据长度开辟空间
			//根据当前i的数值判断 到了那个类别；
			if(1==i) {
				//i=1;当前类别为商品ID
				char *tempid=(char*)calloc(1,strlen(TempBuff)+1);
				strcpy(tempid,TempBuff);
				//				goods_chain[notes].id=tempid;
				goods_chain.id=tempid;

			} else if(2==i) {
				//i=2;当前类别为商品名称
				char *tempname=(char*)calloc(1,strlen(TempBuff)+1);
				strcpy(tempname,TempBuff);
				//				goods_chain[notes].name=tempname;
				goods_chain.name=tempname;

			} else if(3==i) {
				//i=3;当前类别为商品类别
				char *tempcategory=(char*)calloc(1,strlen(TempBuff)+1);
				if(NULL==tempcategory) {
					printf("内存分配失败\n");
				}
				//				printf("tempcategory的地址=%p\n",tempcategory);
				strcpy(tempcategory,TempBuff);
				//				goods_chain[notes].category=tempcategory;
				goods_chain.category=tempcategory;
			} else if(4==i) {
				//i=4;当前类别为商品售价cost
				//					p.price=atof(TempBuff);
				//				goods_chain[notes].cost=atof(TempBuff);
				goods_chain.cost=atof(TempBuff);
			} else if(5==i) {
				//i=5;当前类别为商品进价price
				//				p.amount=atoi(TempBuff);
				//				goods_chain[notes].price =atof(TempBuff);
				goods_chain.price =atof(TempBuff);
			} else if(6==i) {
				//i=6;当前为商品数量
				//				goods_chain[notes].amount =atoi(TempBuff);
				goods_chain.amount =atoi(TempBuff);
			}
		}
		notes++;
		checkprice(&goods_chain);
		goods_chain.reserve=0;
		install(&goods_chain);
	}
	fclose(file);
	backup();
	return notes;
}
