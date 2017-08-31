#include <stdio.h>
#include <ctype.h>
#include "market.h"
#include "datamgr.h"
#include "buy.h"
#include "filemgr.h"
#include "textmgr.h"

int goback(void){
	int i;

	puts("\npress r/R to main menu or any key to exit");
	fflush(stdin);
	i=getchar();
	if(i=='r'||i=='R')
		return 1;
	else {
		puts("goodbye!");
		fflush(stdin);
		getchar();
		return 0;
	}
}

int main(void){
	int option=0;
	int on=1;
	int back=1;
	char id[wdlimit];
	struct goods *p;

	load(FilePath);

	if(showgoods(checkinventory))
		puts("\nATTENTION! All the item above is out of stock.\n");
	puts("-----------------------------------Welcome--------------------------------------");
	while(back){
		puts("\n\t\tPlease select an option with the number 1~6.\n\t\t\tPress q/Q to exit\n");
		puts("1 add goods\n2 Show inventory\n3 Buy goods\n4 modify inventory\n5 lookup item\n6 delete item\n");
		do 
		{
			fflush(stdin);
			option=getchar();
			if(option>='1'&&option<='6')
				break;
			else if(option=='q'||option=='Q')
				break;
			else
				puts("\noption error,please input number 1~6");
		} while (option!='q'&& option!='Q');

		switch(option){		
		case '1':					
			while(on){
				addgoods();	
				puts("\npress q/Q to exit or any key to continue");						
				fflush(stdin);
				on=getchar();
				if(on=='q'||on=='Q')
					on=0;
			}
			back=goback();
			break;
		case '2':
			if(!showgoods((int (*)(struct goods *))0))
				puts("stock is empty.");
			back=goback();
			break;
		case '3':
			showgoods((int (*)(struct goods *))0);
			buygoods();
			back=goback();
			break;
		case '4':
			puts("please input id");
			while(!isdigit(igetword(stdin,id)))
				puts("not a number! try again");
			p=lookup(id);
			if(p==NULL)
				puts("not found");
			else{
				modifygoods(p);
				printinfo(p);
			}
			
			back=goback();
			break;
		case '5':
			puts("please input id");
			while(!isdigit(igetword(stdin,id)))
				puts("not a number! try again");
			if(p==NULL)
				puts("not found");
			else{
				p=lookup(id);
				printinfo(p);
			}
			
			back=goback();
			break;
		case '6':
			puts("please input id");
			while(!isdigit(igetword(stdin,id)))
				puts("not a number! try again");
			if(p==NULL)
				puts("not found");
			else{
				delgoods(id);
			}
			back=goback();
			break;
		case 'q':
		case 'Q':
			puts("goodbye!\n");
			fflush(stdin);
			getchar();
			return 0;
		default:
			puts("Something went wrong!\n");
			fflush(stdin);
			getchar();
			return 0;
		}
	}
	return 0;
}