#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "textmgr.h"
#include "market.h"
#include "datamgr.h"
#include "filemgr.h"
#include "buy.h"

struct cart item[wdlimit];

int lookup_item(char *id){
	int i=0;
	for(;i<wdlimit;i++){
		if(item[i].id != NULL)
			if(strcmp(id,item[i].id)==0)
				return i;
	}
	return -1;
}

int Added_quantity(char *id){
	int i=0;
	i=lookup_item(id);
	if(i==-1)
		return 0;
	else
		return item[i].amount;
}

int buygoods(void){
	char buff[wdlimit];
	int flag=1;
	int go_on=1;
	int i=0;
	int option=1;
	int id_num=0;
	int exist_num=0;
	int amount=0;
	int not_enough=0;
	struct goods *p;

	while(go_on){
		id_num=i;
		while(flag){
			puts("\nplease input item id,press q/Q to exit");
			fflush(stdin);
			while(!isdigit(igetword(stdin,buff))){
				if(buff[0]=='q'||buff[0]=='Q'){
					flag=0;
					go_on=0;	
					break;
				}
				puts("not a number,try again");
			}

			p=lookup(buff);
			if(go_on&&(p==NULL))
				puts("item not found,try again");
			else if(go_on&&(p->amount <= (exist_num=Added_quantity(buff)))){
				puts("Insufficient stock,try again");
			}
			else if(go_on){
				//store id
				if(-1==lookup_item(buff)){//不存在此id
					item[i].id=istrdup(buff);
					flag=0;
				}
				else{
					id_num=lookup_item(buff);
					flag=0;
				}	
			}
		}

		//check go_on,if 0 then no need to continue
		if(go_on==0)
			break;
		//get amount 
		puts("\nPlease input amount");
		do 
		{
			fflush(stdin);
			while(!isdigit(igetword(stdin,buff)))
				puts("not a number,try again");
			amount=atoi(buff);
			p=lookup(item[id_num].id);
			not_enough=(p->amount < amount+exist_num);
			if(!not_enough)
				item[id_num].amount=amount+exist_num;
			else{
				printf("\nyou want too much, only %d remaining. ",p->amount);				
				puts("try again");
			}
		} while (not_enough);

		flag=1;
		if(id_num==i)
			i++;	
	}

	while(!flag){
		puts("\npress c/C to check out,press b/B to buy something");
		fflush(stdin);
		option=getchar();
		if(option=='c'||option=='C'){
			checkout(item);
			break;
		}
		else if(option=='b'||option=='B'){
			buygoods();
			break;
		}
	}
	return i;
}

void checkout(struct cart *item){
	int i=0;
	double sum=0;
	double profits=0;
	struct goods *p;
	
	for(i=0;i < wdlimit;i++){
		if(item[i].id!=NULL){
			p=lookup(item[i].id);
			printf("编号 %-10s品名 %-10s总订购数量 %-10d\n",p->id,p->name,item[i].amount);
		}
	}
	if(i>wdlimit)
		puts("\nthe cart is empty");

	for(i=0;i < wdlimit;i++){
		if(item[i].id!=NULL){
			p=lookup(item[i].id);
			sum = p->price *item[i].amount;
			profits = (p->price - p->cost) * item[i].amount;
			p->amount -= item[i].amount;
			item[i].amount=0;//set to zero
			free((void *)item[i].id);
		}
	}
	puts("\n\n\n");
	printf("the Total price is %f,the profits is %f.\n",sum,profits);
	backup();
}