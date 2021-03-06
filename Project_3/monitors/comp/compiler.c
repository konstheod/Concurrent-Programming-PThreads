#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include<unistd.h>
#include <ctype.h>

#define N 100
/*thewroume oti o xrhsths:*/
//den xrhsimopoiei th leksh synch opoydhpote ektos apo to monitor akomh kai se sxolia
//den topothetei kena metaksu parenthesewn otan prokeitai gia sunarthseis tou monitor
//den vazei pollapla orismata stis sunarthseis tou monitor(synch,synch_begin,synch_end)
//meta to kalesma ths sunarthshs den exei grayei allous xarakthres
//den kleinei to idio label se diaforetikh synarthsh apo oti to anoigei

//ean exei ginei synch to flag einai 1, ean exei kanei begin to flag ginetai 2, sto end ksanaginetai 1
struct label_str{
	char name[N];
	int flag;
};

int main(int argc, char *argv[]){
	char c[10]={'\0'}, title[N],temp, label[N]={'\0'}, synch[N]={'\0'};
	int fd_synch, fd_synch_begin, fd_synch_end, fd_synch_wait, fd_synch_notify, fd_synch_notifyall, fd_synch_statement, fd_file, fd_temp, fd_newfile;
	int check,flag,row, i, j, label_count,safe, flag_break, flag_error;
	struct label_str synch_label[N];
	
	/*elegxoume an exei dwsei argument o xrhsths*/
	if(argc<1){
		printf("Put an argument\n");
		return(-1);
	} 
	/*anoigoume ta arxeia pou mas xreiazodai*/
	fd_synch = open("synch.txt", O_RDONLY);
	if(fd_synch<0){
		printf("Error with fd_synch\n");
		return(-1);
	}
	
	fd_synch_begin = open("synch_begin.txt", O_RDONLY);
	if(fd_synch_begin<0){
		printf("Error with fd_synch_begin\n");
		close(fd_synch);
		return(-1);
	}
	
	fd_synch_end = open("synch_end.txt", O_RDONLY);
	if(fd_synch_end<0){
		printf("Error with fd_synch_end\n");
		close(fd_synch);
		close(fd_synch_begin);
		return(-1);
	}
	
	fd_synch_wait = open("synch_wait.txt", O_RDONLY);
	if(fd_synch_wait<0){
		printf("Error with fd_synch_wait\n");
		close(fd_synch);
		close(fd_synch_begin);
		close(fd_synch_end);
		return(-1);
	}
	
	fd_synch_notify = open("synch_notify.txt", O_RDONLY);
	if(fd_synch_notify<0){
		printf("Error with fd_synch_notify\n");
		close(fd_synch);
		close(fd_synch_begin);
		close(fd_synch_end);
		close(fd_synch_wait);
		return(-1);
	}
	
	fd_synch_notifyall = open("synch_notifyall.txt", O_RDONLY);
	if(fd_synch_notifyall<0){
		printf("Error with fd_synch_notifyall\n");
		close(fd_synch);
		close(fd_synch_begin);
		close(fd_synch_end);
		close(fd_synch_wait);
		close(fd_synch_notify);
		return(-1);
	}
	
	fd_synch_statement = open("synch_statement.txt", O_RDONLY);
	if(fd_synch_statement<0){
		printf("Error with fd_synch_statement\n");
		close(fd_synch);
		close(fd_synch_begin);
		close(fd_synch_end);
		close(fd_synch_wait);
		close(fd_synch_notify);
		close(fd_synch_notifyall);
		return(-1);
	}
	
	fd_file = open(argv[1],  O_RDONLY , S_IRUSR | 0777 );
	if(fd_file<0){
		printf("Error with fd_file\n");
		close(fd_synch);
		close(fd_synch_begin);
		close(fd_synch_end);
		close(fd_synch_wait);
		close(fd_synch_notify);
		close(fd_synch_notifyall);
		close(fd_synch_statement);
		
		return(-1);
	}
	
	strcpy(title,argv[1]);
	strcpy(&title[strlen(title)-2],"_new.c");
	
	fd_newfile = open(title, O_RDWR | O_CREAT, S_IRUSR | 0777 );
	if(fd_newfile<0){
		printf("Error with fd_newfile\n");
		close(fd_synch);
		close(fd_synch_begin);
		close(fd_synch_end);
		close(fd_synch_wait);
		close(fd_synch_notify);
		close(fd_synch_notifyall);
		close(fd_synch_statement);
		close(fd_file);
		return(-1);
	}
	fd_temp = open("new.c", O_RDWR | O_CREAT, S_IRUSR | 0777 );
	if(fd_temp<0){
		printf("Error with fd_temp\n");
		close(fd_synch);
		close(fd_synch_begin);
		close(fd_synch_end);
		close(fd_synch_wait);
		close(fd_synch_notify);
		close(fd_synch_notifyall);
		close(fd_synch_statement);
		close(fd_file);
		close(fd_newfile);
		unlink(title);
		return(-1);
	}
	
	temp = '\0';
	flag = 0;
	row = 1;
	label_count = 0;
	flag_break = 0;
	flag_error = 0;
	/*elegxos logikwn kai sudaktikwn lathwn kai dhmiourgia ektelesimou*/
	while(flag_break == 0){
		
		check = read(fd_file, &c[0], sizeof(char));
		/*telos arxeiou*/
		if(check<=0){
			break;
		}
		/*metrhma allaghs grammhs*/
		if(c[0] == '\n'){
			row++;
		}
		/*an o xarakthras pou diavasame einai s*/
		if(c[0] == 's' ) {
			/*diavazoume an eiani h leksh synch*/
			check = read(fd_file, &c[1], sizeof(char)*4);
			/*telos arxeiou an den einai h synch kai eisagwgh twn dedomenwn*/
			if(check<4){
				write(fd_temp, c, (check+1)*sizeof(char));
				break;
			}
			else{
				c[5] = '\0';
				/*an h leksh pou mas exei dwsei o xrhsths einai h synch*/
				if(strcmp(c,"synch")==0 && isspace(temp)!=0){
					do{//gia na elegxoyme ean h synch akoloytheitai apo epithimhtoys xarakthres kena
						check = read(fd_file, &temp, sizeof(char));
						if(check<=0){
							flag_break = 1;
							break;
						}
						flag = 1;
					}while(isspace(temp)!=0);
					/*an o xrhsths mas zhtaei na arxikopoihsoume to "label" pou thelei na xrhsimopoihsei*/
					if(temp=='('){
						i=0;
						do {
							check = read(fd_file, &label[i], sizeof(char));
							if(check<=0){
								flag_break = 1;
								break;
							}
							i++;
						}while(label[i-1]!=';' && i!=(N-2));
						label[i-2] = '\0';
						safe = 0;
						
						/*elegxos an uparxei to idio label kai se allh arxikopoihsh tou monitor mas*/
						for(j=0; j<label_count; j++){
							if(strcmp(synch_label[j].name, label)==0){
								printf("%s:%d: error: you have already used and initialized this label\n", argv[1], row);
								flag_error = 1;
								safe = 1;
							}
						}
						if(safe == 1) {
							continue;
						}
						
						strcpy(synch_label[label_count].name, label);
						synch_label[label_count].flag = 1;
						label_count++;
						
						lseek(fd_synch,0,SEEK_SET);
						while(1){
							check = read(fd_synch, synch, 5*sizeof(char));
							if(check<=0){
								break;
							}
							else if(check<5){
								write(fd_temp, synch, check*sizeof(char));
								break;
							}
							synch[5] = '\0';
							if(strcmp(synch, "label")==0) {
								write(fd_temp, label, strlen(label)*sizeof(char));
							}
							else {
								write(fd_temp, &synch[0], sizeof(char));
								lseek(fd_synch,-(4*sizeof(char)),SEEK_CUR);
							}
						}
					}
					/*an o xrhsths thelei na ulopoihsoume kapoia sunarthsh*/
					else if(temp=='_'){
						i = 0;
						do {//briskoume thn synarthsh
							check = read(fd_file, &c[i], sizeof(char));
							if(check<=0){
								flag_break = 1;
								break;
							}
							i++;
						}while(i<9 && c[i-1]!='(' && isalpha(c[i-1])!=0);
						c[i-1] = '\0';
						
						i = 0;
						/*briskoyme to label stis sunartiseis mas*/
						do {
							check = read(fd_file, &label[i], sizeof(char));
							if(check<=0){
								flag_break = 1;
								break;
							}
							i++;
						}while(label[i-1]!=';' && i!=(N-2));
						/*an einai begin/notify/notifyall den uparxei orisma*/
						if(i==2) {
							label[i-2] = '\0';
							if(strcmp(c,"wait")==0) {
								safe = -1;
								for(j=0; j<label_count; j++){
									if(synch_label[j].flag==2){
										safe = 0;
										strcpy(label,synch_label[j].name);
									}
								}
								if(safe==-1){
									printf("%s:%d: error: you haven't begin a monitor with any label\n", argv[1], row);
									flag_error = 1;
									continue;
								}
								lseek(fd_synch_wait,0,SEEK_SET);
								while(1){
									check = read(fd_synch_wait, synch, 5*sizeof(char));
									if(check<=0){
										break;
									}
									else if(check<5){
										write(fd_temp, synch, check*sizeof(char));
										break;
									}
									synch[5] = '\0';
									if(strcmp(synch, "label")==0) {
										write(fd_temp, label, strlen(label)*sizeof(char));
									}
									else {
										write(fd_temp, &synch[0], sizeof(char));
										lseek(fd_synch_wait,-(4*sizeof(char)),SEEK_CUR);
									}
								}
							}
							
							else if(strcmp(c,"notify")==0) {
								safe = -1;
								for(j=0; j<label_count; j++){
									if(synch_label[j].flag==2){
										safe = 0;
										strcpy(label,synch_label[j].name);
									}
								}
								if(safe==-1){
									printf("%s:%d: error: you haven't begin a monitor with any label\n", argv[1], row);
									flag_error = 1;
									continue;
								}
								lseek(fd_synch_notify,0,SEEK_SET);
								while(1){
									check = read(fd_synch_notify, synch, 5*sizeof(char));
									if(check<=0){
										break;
									}
									else if(check<5){
										write(fd_temp, synch, check*sizeof(char));
										break;
									}
									synch[5] = '\0';
									if(strcmp(synch, "label")==0) {
										write(fd_temp, label, strlen(label)*sizeof(char));
									}
									else {
										write(fd_temp, &synch[0], sizeof(char));
										lseek(fd_synch_notify,-(4*sizeof(char)),SEEK_CUR);
									}
								}
							}
							
							else if(strcmp(c,"notifyall")==0) {
								safe = -1;
								for(j=0; j<label_count; j++){
									if(synch_label[j].flag==2){
										safe = 0;
										strcpy(label,synch_label[j].name);
									}
								}
								if(safe==-1){
									printf("%s:%d: error: you haven't begin a monitor with any label\n", argv[1], row);
									flag_error = 1;
									continue;
								}
								lseek(fd_synch_notifyall,0,SEEK_SET);
								while(1){
									check = read(fd_synch_notifyall, synch, 5*sizeof(char));
									if(check<=0){
										break;
									}
									else if(check<5){
										write(fd_temp, synch, check*sizeof(char));
										break;
									}
									synch[5] = '\0';
									if(strcmp(synch, "label")==0) {
										write(fd_temp, label, strlen(label)*sizeof(char));
									}
									else {
										write(fd_temp, &synch[0], sizeof(char));
										lseek(fd_synch_notifyall,-(4*sizeof(char)),SEEK_CUR);
									}
								}
							}
							else {
								printf("%s:%d: error: no such function\n",argv[1],row);
								flag_error = 1;
							}
						}
						/*an uparxei to label pou exei dwsei o xrhsths*/
						else if(label[i-2]!=')') {
							printf("%s:%d: error: too long label for this function\n", argv[1], row);
							flag_error = 1;
						}
						else{
							label[i-2] = '\0';
							/*an exei ginei begin*/
							if(strcmp(c,"begin")==0) {
								safe = -1;
								for(j=0; j<label_count; j++){
									//elegxos ean yparxei to label dhlomeno
									if(strcmp(label, synch_label[j].name)==0){
										//ean exei ksanakanei begin
										if(synch_label[j].flag==2){
											printf("%s:%d: error: you have already begin the monitor with this label\n", argv[1], row);
											flag_error = 1;
										}
										else{
											safe = j;
										}
									}
									else{
										//ean uparxei kapoio allo anoixto monitor
										if(synch_label[j].flag==2){
											printf("%s:%d: error: you have already begin another monitor with different label\n", argv[1], row);
											flag_error = 1;
										}
										
									}
								}
								/*vazoume th sunarthsh synch_begin sto ektelesimo*/
								if(safe!=-1){
									synch_label[safe].flag = 2;
								}
								else{
									continue;
								}
								lseek(fd_synch_begin,0,SEEK_SET);
								while(1){
									check = read(fd_synch_begin, synch, 5*sizeof(char));
									if(check<=0){
										break;
									}
									else if(check<5){
										write(fd_temp, synch, check*sizeof(char));
										break;
									}
									synch[5] = '\0';
									if(strcmp(synch, "label")==0) {
										write(fd_temp, label, strlen(label)*sizeof(char));
									}
									else {
										write(fd_temp, &synch[0], sizeof(char));
										lseek(fd_synch_begin,-(4*sizeof(char)),SEEK_CUR);
									}
								}
							}
							/*an exei ginei end*/
							else if(strcmp(c,"end")==0) {
								safe = -1;
								for(j=0; j<label_count; j++){
									//elegxos ean yparxei to label dhlomeno
									if(synch_label[j].flag==2){
										//ean exei ksanakanei begin
										if(strcmp(label, synch_label[j].name)==0){
											safe = j;
										}
										else{
											printf("%s:%d: error: you haven't begin the monitor with this label\n", argv[1], row);
											flag_error = 1;
											break;
										}
									}
								}
								
								if(safe==-1){
									continue;
								}
								else{
									synch_label[safe].flag = 1;
								}
								
								lseek(fd_synch_end,0,SEEK_SET);
								while(1){
									check = read(fd_synch_end, synch, 5*sizeof(char));
									if(check<=0){
										break;
									}
									else if(check<5){
										write(fd_temp, synch, check*sizeof(char));
										break;
									}
									synch[5] = '\0';
									if(strcmp(synch, "label")==0) {
										write(fd_temp, label, strlen(label)*sizeof(char));
									}
									else {
										write(fd_temp, &synch[0], sizeof(char));
										lseek(fd_synch_end,-(4*sizeof(char)),SEEK_CUR);
									}
								}
							}
							/*an den valei begin/end*/
							else {
								printf("%s:%d: error: no such function\n",argv[1],row);
								flag_error = 1;
							}
						}
					}
					else{
						if(flag){
							lseek(fd_file,-(2*sizeof(char)),SEEK_CUR);
							printf("%s:%d: error: expected argument after synch\n",argv[1],row);
							flag_error = 1;
						}
						else{
							lseek(fd_file,-(1*sizeof(char)),SEEK_CUR);
							printf("%s:%d: error: no such function\n",argv[1],row);
							flag_error = 1;
						}
						
					}
				}
				/*an einai mia allh leksh apo s*/
				else{
					lseek(fd_file,-(4*sizeof(char)),SEEK_CUR);
					write(fd_temp, &c[0], sizeof(char));
				}
			}
		}
		/*an den einai s o xarakthras pou diavasame*/
		else {
			write(fd_temp, &c[0], sizeof(char));
			temp = c[0];
		}
	}
	
	for(j=0;j<label_count;j++){
		if(synch_label[j].flag!=1){
			printf("%s:%d: error: you have not close the %s label\n",argv[1],row,synch_label[j].name);
			flag_error = 1;
		}
	}
	if(flag_error==0) {
		write(fd_newfile, "#include<pthread.h>\n", 20*sizeof(char));
		
		for(j=0;j<label_count;j++){
			lseek(fd_synch_statement,0,SEEK_SET);
			while(1){
				check = read(fd_synch_statement, synch, 5*sizeof(char));
				if(check<=0){
					break;
				}
				else if(check<5){
					write(fd_newfile, synch, check*sizeof(char));
					break;
				}
				synch[5] = '\0';
				if(strcmp(synch, "label")==0) {
					write(fd_newfile, synch_label[j].name, strlen(synch_label[j].name)*sizeof(char));
				}
				else {
					write(fd_newfile, &synch[0], sizeof(char));
					lseek(fd_synch_statement,-(4*sizeof(char)),SEEK_CUR);
				}
			}
		}
		write(fd_newfile, "\n", sizeof(char));
		
		lseek(fd_temp,0,SEEK_SET);
		while(1) {
			check = read(fd_temp, synch, N*sizeof(char));
			if(check<=0){
				break;
			}
			write(fd_newfile, synch, check*sizeof(char));
		}
	}
	else {
		unlink(title);
	}
	
	
	close(fd_synch);
	close(fd_synch_begin);
	close(fd_synch_end);
	close(fd_synch_wait);
	close(fd_synch_notify);
	close(fd_synch_notifyall);
	close(fd_file);
	close(fd_temp);
	close(fd_newfile);
	
	unlink("new.c");
		
	return(0);
}