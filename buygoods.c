#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "buygoods.h"
#include "textmgr.h"
#include "market.h"
#include "datamgr.h"
#include "filemgr.h"


struct cart item[wdlimit]={0};

int item_exist_num(struct cart *p){
	int i=0;
	int ret=0;
	for(;i<wdlimit;i++)
		if(item[i].id != NULL)
			ret++;
	return ret;
}

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

void set_cart_empty(struct cart *p){
	int i=0;
	for(;i<wdlimit;i++){
		if(item[i].id!=NULL){
			free((void *)item[i].id);
		}
		item[i].amount=0;//set to zero
	}
}

void buygoods(void){
	int i=0;
	char buff[wdlimit];
	int flag=1;
	int getitem=0;
	int go_on=1;
	int option=1;
	int exist_num=0;
	int amount=0;
	int not_enough=0;
	struct goods *p;

	i=item_exist_num(item);
	while(go_on){
		flag=1;
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

			//check go_on,if 0 then no need to continue
			if(go_on==0)
				break;

			p=lookup(buff);
			if(go_on&&(p==NULL))
				puts("item not found,try again");
			else if(go_on&&(p->amount <= (exist_num=Added_quantity(buff)))){
				puts("Insufficient stock,try again");
			}
			else if(go_on){
				//store id
				if(-1==lookup_item(buff)){//不存在此id
					i=item_exist_num(item);
					item[i].id=istrdup(buff);
					flag=0;
				}
				else{
					i=lookup_item(buff);
					flag=0;
				}	
			}
		}

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
			p=lookup(item[i].id);
			not_enough=(p->amount < amount+exist_num);
			if(!not_enough)
				item[i].amount=amount+exist_num;
			else{
				printf("\nyou want too much, only %d remaining. ",p->amount);				
				puts("try again");
			}
		} while (not_enough);

		getitem=1;	
	}

	while(getitem){
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
}

void checkout(struct cart *item){
	int i=0;
	double sum=0;
	double profits=0;
	double one_piece_sum=0;
	double one_piece_profits=0;
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
			one_piece_sum = p->price *item[i].amount;
			one_piece_profits = (p->price - p->cost) * item[i].amount;
			p->amount -= item[i].amount;
		}
		else{
			one_piece_sum=0;
			one_piece_profits=0;
		}
		sum += one_piece_sum;
		profits += one_piece_profits;
	}
	sum += one_piece_sum;
	profits += one_piece_profits;
	puts("\n\n\n");	
	printf("the Total price is %f,the profits is %f.\n",sum,profits);
	backup();
}