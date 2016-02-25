#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define N 10
//elegxoys malloc init mutex
//na kanoume destroy


int un_count_b,un_count_r;   // o arithmos twn amaksiwn kathe xrwmatos pou pernaei th gefyra
int i_blue, i_red;          //o arithmos twn amaksiwn pou dinei kathe fora o xrhsths  

pthread_mutex_t m_ex_blue, m_ex_red;  //dwsimo seiras ap t main sta threads
pthread_mutex_t red5;                //enallagh seiras blue-red
pthread_mutex_t mtx;                //gia amoivaio apokleismo sta threads
pthread_mutex_t safe_mtx;          //gia na perimenoume na mas eidopoihsei to amaksi oti mphke sth gefyra

void *red(void *num_r);
void *blue(void *num_b);

int main(int argc, char *argv[]){
	char c;
	int i;
	int *array_r, *array_b;  //arithmos amaksiou twn red kai blue
	pthread_t *p_red, *p_blue;
	
	/*arxikopoioume ta mutexes*/
	pthread_mutex_init(&m_ex_red, NULL);   
	pthread_mutex_init(&m_ex_blue, NULL);  
	pthread_mutex_init(&red5, NULL);
	pthread_mutex_init(&safe_mtx, NULL);
	pthread_mutex_init(&mtx, NULL);
	
	/*epanalhyh gia na dwsei o xrhsths oses fores thelei autokinhta*/
	while(1) {
		/*ebodizoume ta threads na ksekinhsoun*/
		pthread_mutex_lock(&m_ex_red);  
		pthread_mutex_lock(&m_ex_blue);  
		pthread_mutex_lock(&safe_mtx);
		
		/*mhdenizoume tous metrhtes gia kathe kainouria mera*/
		i_blue = 0;
		i_red = 0;
		un_count_b=0;
		un_count_r=0;
		
		printf("Press 0 to insert a red car, 1 for blue car or 'q' to exit\n");
		do{
			c = getchar();
			if(c=='0'){
				//red
				i_red++;
			}
			else if(c=='1'){
				//blue
				i_blue++;
			}
			else if(c=='q') {
				return(0);
			}
			
		}while(c!='\n');
		
		/*elegxos sthn periptosh opou den dhlothei kanena amaksi eksodos apo to programma */
		if(i_red==0 && i_blue==0){
			return(0);
		}
		printf("Α new day dawned\n");
		
		/*anoigoume theseis gia ola ta oxhmata/nhmata*/
		p_red = (pthread_t*)malloc(sizeof(pthread_t)*i_red);
		p_blue = (pthread_t*)malloc(sizeof(pthread_t)*i_blue);
		array_r = (int*)malloc(sizeof(int)*i_red);
		array_b = (int*)malloc(sizeof(int)*i_blue);
		
		/*dhmioyrgoyme ta kokkina oxhmata*/
		for(i=0; i<i_red; i++){
			array_r[i] = i;
			pthread_create(&p_red[i], NULL, &red, &array_r[i]);
		}
		
		/*dhmioyrgoyme ta mple oxhmata*/
		for(i=0; i<i_blue; i++){
			array_b[i] = i;
			pthread_create(&p_blue[i], NULL, &blue, &array_b[i]);
		}
		
		printf("red %d, blue %d\n",i_red, i_blue);
		/*epanalhpsh opou eisagoume sth gefyra kathe fora N oxhmata kathe xromatos mexri na eksantlithoun osa perimenoyne*/
		while(1) {
			/*kleidonoyme thn kateythinsh oste na mhn epitrapei sth main meta na ksypnisei oxhmata antitheths kateythinshs*/
			pthread_mutex_lock(&red5);
			if(i_red == un_count_r) {		//elegxos ean teleiosan ta kokkina oxhmata
				pthread_mutex_unlock(&red5);  //otan perasoun ola red dwse prosvash sta blue
			}
			for(i=0; i<N && i<i_red; i++ ) {
				pthread_mutex_unlock(&m_ex_red); //energopoioume ena amaksi na mpei sth gefyra
				pthread_mutex_lock(&safe_mtx);	//perimenoume na mas eidopoihsei to amaksi oti mphke sth gefyra
			}
			
			/*kleidonoyme thn kateythinsh gia na mhn epitrapei sth main na ksypnisei mple oxhmata mexri na bgei kai to teleytaio kokkino amaksi poy mphke sth gefyra*/
			pthread_mutex_lock(&red5);
			
			if(i_blue == un_count_b) {		//elegxos ean teleiosan ta mple oxhmata
				pthread_mutex_unlock(&red5); //otan perasoun ola blue dwse prosvash sta red
			}
			for(i=0; i<N && i<i_blue; i++ ) {
				pthread_mutex_unlock(&m_ex_blue);  //energopoioume ena amaksi na mpei sth gefyra
				pthread_mutex_lock(&safe_mtx);	//perimenoume na mas eidopoihsei to amaksi oti mphke sth gefyra
				
			}
			
			/*kleidonoyme thn kateythinsh oste na mhn epitrapei sth main na eleksei thn synthiki termatismoy ths while prin teleiosei to teleytaio mple amaksi*/
			pthread_mutex_lock(&red5);
			/*elegxos ean teleiosan ola ta oxhmata tote teleionei h mera*/
			if(un_count_r == i_red && un_count_b == i_blue){	
				pthread_mutex_unlock(&red5);
				pthread_mutex_unlock(&m_ex_blue);
				pthread_mutex_unlock(&m_ex_red);
				pthread_mutex_unlock(&safe_mtx);
				printf("End of the day\n");
				break;
			}
			pthread_mutex_unlock(&red5);
		}
		free(p_red);
		free(p_blue);
		free(array_r);
		free(array_b);
	}
	
	/*katastrefoyme ta mutexes*/
	pthread_mutex_destroy(&m_ex_red);
	pthread_mutex_destroy(&m_ex_blue);
	pthread_mutex_destroy(&red5);
	pthread_mutex_destroy(&safe_mtx);
	pthread_mutex_destroy(&mtx);
	
	return(0);
}

void *red(void *num_r){
	int number;
	number = *(int *)(num_r);
	
	pthread_mutex_lock(&m_ex_red); 	//kleidonoyme to amaksi mexri na to energopoihsei h main gia na mpei sth gefyra
	
	pthread_mutex_unlock(&safe_mtx); //leme sth main oti to amaksi mphke sth gefyra
	
	/*apokleismos metaksi ton threads oste na mporei na grapsei kai na afksisei to metrhth to kathena me asfaleia*/
	pthread_mutex_lock(&mtx);
	un_count_r++;
	printf("red %d\n", number);
	if(un_count_r%N == 0 || un_count_r == i_red){
		printf("count_red %d\n", un_count_r);
		pthread_mutex_unlock(&red5);
	}
	pthread_mutex_unlock(&mtx);
	
	return(NULL);
}

void *blue(void *num_b){
	int number;
	number = *(int *)(num_b);
	
	pthread_mutex_lock(&m_ex_blue);  //kleidonoyme to amaksi mexri na to energopoihsei h main gia na mpei sth gefyra
	
	pthread_mutex_unlock(&safe_mtx);	//leme sth main oti to amaksi mphke sth gefyra
	
	/*apokleismos metaksi ton threads oste na mporei na grapsei kai na afksisei to metrhth to kathena me asfaleia*/
	pthread_mutex_lock(&mtx);
	un_count_b++;
	printf("blue %d\n", number);
	if(un_count_b%N==0 || i_blue == un_count_b){
		printf("count_blue %d\n", un_count_b);
		pthread_mutex_unlock(&red5);
	}
	pthread_mutex_unlock(&mtx);
	
	return(NULL);
}