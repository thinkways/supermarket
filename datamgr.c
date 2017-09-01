#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "filemgr.h"
#include "market.h"
#include "datamgr.h"
#include "textmgr.h"

struct goods *hashtable[hashlen];
char *istrdup(const char *s){
	char *t;
	t=(char *)malloc(strlen(s)+1);
	if(t==NULL)
		return NULL;
	return strcpy(t,s);
}


unsigned makehash(char *s ){
	unsigned int t;
	for(t = 0;*s;s++)
		t = *s +31*t;
	return t % hashlen;
}

struct goods *lookup(char *id){
	struct goods *p;
	for(p=hashtable[makehash(id)];p!=NULL;p=p->next)
		if(strcmp(id,p->id)==0)
			return p;
	return NULL;
}


struct goods *install(struct goods *p){
	unsigned hashval;
	struct goods *pn;

	if((pn=lookup(p->id))==NULL){
		if((pn=(struct goods *)malloc(sizeof(*p)))!=NULL){
			hashval=makehash(p->id);
			pn->next=hashtable[hashval];
			hashtable[hashval]=pn;
			p->next=pn->next;
			*pn=*p;
		}else 
			return NULL;
	}else{//overwrite the old data
		pn->name=p->name;
		free((void *)pn->name);
		pn->category=p->category;
		free((void *)pn->category);
		pn->amount = p->amount;
		pn->price = p->price;
		pn->cost = p->cost;
	}
	return pn;
}

void printinfo(struct goods *p){
	if(p==NULL)
		return;
	//printf("%s#%s#%s#%fl#%fl#%d\n",p->id,p->name,p->category,p->cost,p->price,p->amount);
	printf("编号 %-8s品名 %-8s类别 %-8s进价 %-8.3f售价 %-8.3f库存 %-8d\n",p->id,\
		p->name,p->category,p->cost,p->price,p->amount);
}

int checkprice(struct goods *p){
	int error=0;
	char tmp[wdlimit];

	if(p->cost > p->price)
		error = 1;
	while(error){
		printf("the item: %s went wrong",p->id);
		puts("\n\nyour cost price is bigger than sold price,now modify");
		puts("Please input the cost price");
		while(!isdigit(igetword(stdin,tmp)))
			puts("not a number! try again");
		p->cost=atof(tmp);

		puts("\nPlease input the price");
		while(!isdigit(igetword(stdin,tmp)))
			puts("not a number! try again");
		p->price=atof(tmp);
		if(p->cost < p->price)
			error = 0;
	}
	return error;
}

int checkinventory(struct goods *p){
	int ret=1;
	if(p->amount<=0){
		ret=0;
	}
	return ret;
}

int collectinfo(struct goods *p){//初始化商品信息，成功返回1，否则返回0
	char tmp[wdlimit]={0};

	puts("\nPlease input the name");
	//while(!isalpha(igetword(stdin,tmp)))
		//puts("not a name! try again");
	fflush(stdin);
	//igetword(stdin,tmp);
	scanf("%s",tmp);
	p->name=istrdup(tmp);

	puts("\nPlease input the cost price");
	fflush(stdin);
	while(!isdigit(igetword(stdin,tmp))){
		fflush(stdin);
		puts("not a number! try again");
	}
	p->cost=atof(tmp);

	puts("\nPlease input the price");
	fflush(stdin);
	while(!isdigit(igetword(stdin,tmp))){
		fflush(stdin);
		puts("not a number! try again");
	}

	p->price=atof(tmp);

	checkprice(p);

	puts("\nPlease input the category");
	//while(!isalpha(igetword(stdin,tmp)))
		//puts("not a category! try again");
	fflush(stdin);
	//igetword(stdin,tmp);
	scanf("%s",tmp);
	p->category=istrdup(tmp);

	puts("\nPlease input the amount");
	fflush(stdin);
	while(!isdigit(igetword(stdin,tmp))){
		fflush(stdin);
		puts("not a number! try again");
	}
	p->amount=atoi(tmp);

	p->reserve=0;

	if(p->id==NULL||p->name==NULL||p->category==NULL){
		puts("lack of memory\n");
		return 0;
	}
	return 1;
}

void modifygoods(struct goods *p){//修改的就是此ID
	int option=0;
	char buff[wdlimit]={0};
	puts("\nyou can now modify the info of this goods\n");
	puts("select one option(1~5) to continue,press q/Q to exit");
	puts("1 name\n2 category\n3 cost price\n4 sell price\n5 amount \n");

	do 
	{
		option=getchar();
		fflush(stdin);
		if(option>='1'&&option<='5')
			break;
		else if(option != 'q'&& option != 'Q')
			puts("\nplease input number 1~4");
	} while (option!='q'&& option!='Q');

	switch(option){
		case '1':
			puts("\nnow you will change the name");
			puts("\nPlease input the name");
			//while(!isalpha(igetword(stdin,buff)))
				//puts("not a name! try again");
			fflush(stdin);
			//igetword(stdin,buff);
			scanf("%s",buff);
			free((void *)p->name);
			p->name=istrdup(buff);
			break;
		case '2':
			puts("\nnow you will change the category");
			puts("\nPlease input the category");
			//while(!isalpha(igetword(stdin,buff)))
				//puts("not a category! try again");
			fflush(stdin);
			//igetword(stdin,buff);
			scanf("%s",buff);
			free((void *)p->category);
			p->category=istrdup(buff);
			break;
		case '3':
			puts("\nPlease input the cost price");
			fflush(stdin);
			while(!isdigit(igetword(stdin,buff))){
				fflush(stdin);
				puts("not a number! try again");
			}
			p->cost=atof(buff);
			break;
		case '4':
			puts("\nPlease input the price");
			fflush(stdin);
			while(!isdigit(igetword(stdin,buff))){
				fflush(stdin);
				puts("not a number! try again");
			}
			p->price=atof(buff);
			break;
		case '5':
			puts("\nPlease input the amount");
			fflush(stdin);
			while(!isdigit(igetword(stdin,buff))){
				fflush(stdin);
				puts("not a number! try again");
			}
			p->amount=atoi(buff);
			break;
		case 'q':
		case 'Q':
			puts("\nDone! goodbye!\n");
			break;
		default:
			puts("Something went wrong!\n");
			break;
	}

	checkprice(p);
	backup();
}

struct goods* addgoods(void){
	struct goods ptmp,*p;
	unsigned hashval;
	char buff[wdlimit]={0};

	puts("\nPlease input the ID");
	fflush(stdin);
	while(!isdigit(igetword(stdin,buff))){
		fflush(stdin);
		puts("not a number! try again");
	}
	ptmp.id=istrdup(buff);

	p=lookup(ptmp.id);
	if(p==NULL){
		if(collectinfo(&ptmp)==0)
			return NULL;
		if(p=(struct goods *)malloc(sizeof(*p))){
			hashval=makehash(ptmp.id);
			p->next=hashtable[hashval];
			hashtable[hashval]=p;
			ptmp.next=p->next;
			*p=ptmp;
		}
		else 
			return NULL;
	}
	else{//存在
		puts("this id has already existed\n");
		printinfo(p);
		modifygoods(p);
		printinfo(p);
	}
	backup();
	return p;
}

int delgoods(char *id){
	struct goods *p,*pnext,*prev;

	p=hashtable[makehash(id)];

	//一开始就找到
	if(strcmp(id,p->id)==0){
		free((void *)p->id);
		free((void *)p->name);
		free((void *)p->category);
		free((void *)p);
		hashtable[makehash(id)]=NULL;
		backup();
		return 1;
	}

	for(;p!=NULL;prev=p,p=p->next,pnext=p->next){
		if(strcmp(id,p->id)==0){
			prev->next=pnext;
			free((void *)p->id);
			free((void *)p->name);
			free((void *)p->category);
			free((void *)p);
			backup();
			return 1;
		}
	}
	return 0;
}

int showgoods(int (*func)(struct goods *)){
	int i=0;
	int flag=0;
	struct goods *p=NULL;
	while(i<hashlen){
		p=hashtable[i];
		if(p==NULL){
			i++;
			continue;
		}else{
			for(;p!=NULL;p=p->next){
				if(func==NULL){
					printinfo(p);
					flag=1;
				}
				else if(!(*func)(p)){
					printinfo(p);
					flag=1;
				}
			}
		}
		i++;
	}
	return flag;
}