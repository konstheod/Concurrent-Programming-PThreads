#include <stdio.h>
#include <ctype.h>

#define N 100000

//in out end_flag einai global

int main(int argc, char *argv[]){
	char l1,l2,array[N];
	int in,out,end_flag1;
	
	in = 0;
	out = N;
	end_flag1 = 0;
	
	do{//bgazoume ta perita kena sthn arxh
		l1=getchar();
		if(l1==EOF){
			end_flag1 = 1;
			return(0);
		}
	}
	while(l1=='\n' || l1==' ');
	
	
	while(end_flag1!=1){
		l2=getchar();
		if(l2==EOF){
			end_flag1 = 1;
			
		}
		
		while(in == out){}//synthiki anamonhs
		
		if(l1!='\n' && l1!=' '){
			array[in] = l1;
			in = (in+1)%N;
			l1 = l2;
			continue;
		}
		else{
					
			while(l2=='\n' || l2==' '){
				l2 = getchar();
				if(l2==EOF){
					end_flag1 = 1;
					break;
				}
			}
			if(end_flag1 != 1) {
				array[in] = ' ';
				in = (in+1)%N;
			}
			l1 = l2;
		}
	}
	
	array[in] = '\0';
	printf(" %s",array);
	return(0);
}