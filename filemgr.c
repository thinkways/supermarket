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
	int notes=0; //��¼��ȡ������,ÿ��ȡ��һ�У�notes����+1��
	/********���ļ� ***********/
	file=fopen(filename,"r");
	if(NULL==file) {
		//printf("û�и��ļ������������ļ�");
		file=fopen(filename,"w+");
	} else {
		//do nothing;
	}
	/********��ȡ�ļ���Buff ***********/
	
	while(fgets(Buff,sizeof(Buff),file)!=NULL) {

		/*	2���Ѷ�ȡ���е����ݣ��ֱ𸳸��ṹ�������Ա��*/
		char TempBuff[20]= {0};
		int cur=0; //cur��ǰ���λ��
		int i=1;
		for(; i<7; i++) { //i��#�ĸ�����#����6����
			int k=0; //k���㣬TempBuff�ظ�ʹ�ã�
			int j=cur;
			for(; j<100; j++) {
				if(Buff[j]!='#') {
					TempBuff[k++]=Buff[j];
				} else {
					//����#��־�� break,
					TempBuff[k]='\0';//�ַ���������־
					cur=j+1;//���õ�ǰ���+1  ָ����һ����Ʒ��Ϣ��
					break;//�������ѭ����i++�Լ�
				}
			}
			//Temp�����˵�ǰ��������ݣ� ��Ҫ���ݶ��������ݳ��ȿ��ٿռ�
			//���ݵ�ǰi����ֵ�ж� �����Ǹ����
			if(1==i) {
				//i=1;��ǰ���Ϊ��ƷID
				char *tempid=(char*)calloc(1,strlen(TempBuff)+1);
				strcpy(tempid,TempBuff);
				//				goods_chain[notes].id=tempid;
				goods_chain.id=tempid;

			} else if(2==i) {
				//i=2;��ǰ���Ϊ��Ʒ����
				char *tempname=(char*)calloc(1,strlen(TempBuff)+1);
				strcpy(tempname,TempBuff);
				//				goods_chain[notes].name=tempname;
				goods_chain.name=tempname;

			} else if(3==i) {
				//i=3;��ǰ���Ϊ��Ʒ���
				char *tempcategory=(char*)calloc(1,strlen(TempBuff)+1);
				if(NULL==tempcategory) {
					printf("�ڴ����ʧ��\n");
				}
				//				printf("tempcategory�ĵ�ַ=%p\n",tempcategory);
				strcpy(tempcategory,TempBuff);
				//				goods_chain[notes].category=tempcategory;
				goods_chain.category=tempcategory;
			} else if(4==i) {
				//i=4;��ǰ���Ϊ��Ʒ�ۼ�cost
				//					p.price=atof(TempBuff);
				//				goods_chain[notes].cost=atof(TempBuff);
				goods_chain.cost=atof(TempBuff);
			} else if(5==i) {
				//i=5;��ǰ���Ϊ��Ʒ����price
				//				p.amount=atoi(TempBuff);
				//				goods_chain[notes].price =atof(TempBuff);
				goods_chain.price =atof(TempBuff);
			} else if(6==i) {
				//i=6;��ǰΪ��Ʒ����
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
