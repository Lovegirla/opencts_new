#include"ds.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAXCHAR 500
#define WORDLEN 10
extern double DbVs ;
extern double DbVh;

extern WIRELIB wirelib;
extern BUFLIB	buflib ;

int crt_process(int flag, double * rval);
int read_clr(FILE *, double *c);


double getfloat(char *bf){
double d = 0.0;
double di = 10.0;
	while(*bf == ' ' || *bf == '\t') bf ++;
	while(*bf != '.' && *bf != ')' && * bf != '(' &&  *bf != ' ' && *bf != '\t' && *bf != '\0' ){
		if ( isdigit(*bf)){
			d = (d * di + (double) (*bf)-'0');
			bf ++;
		}
	}
	if ( *bf == '\0' || *bf == ')' || *bf == '(' || *bf == ' ' || *bf == '\t')
		return d;
	(bf)++;
	while(*bf != '\0'){
		if ( isdigit(*bf)){
			d += (double)((*bf)-'0')/di;
			di *= 10.0;
			(bf)++;
		}

	}
	return d ;

}

char * getword(char ** bf){
char * wbuf = (char * ) malloc(sizeof(char)*WORDLEN);
char * ibuf = wbuf;
//	printf(" %s\n",*bf);
	while(**bf== ' ' || **bf == '\t') (*bf) ++;
	while(**bf != ' ' && **bf != '\t' && **bf != '\n'){
		(*(ibuf++)) = (*((*bf)++));
	}
	*ibuf = '\0';
	return wbuf ;

}
/*
double cal_buf_d(FILE * fp){
char * ib, * buf, * iword ;
int  i,j;
double sum = 0.0, di = 0.0 ;
double d1, d2;
double buf_d1[4];
double buf_d2[4];
buf = (char *) malloc(sizeof(char)*MAXCHAR);
if ( buf == NULL){
	printf(" buf allocation error \n");
	free(buf);
	return 0.0 ;
}
i=j=0;
while(fgets(buf,MAXCHAR,fp)!= NULL ){
	ib = buf;
	//while((*ib) == ' ' || (*ib) == '\t') ib++;
	iword = getword(&ib);
	if ( !strcmp("===",iword)) {
		free(iword);
		continue;
	}
	//iword = getword(&ib);
	printf("%s\n",iword);
	if ( !strcmp("sink",iword))
		free(iword);
	else {
		free(iword);
		continue;
	}
	iword = getword(&ib);
	printf("%s\n",iword);
	if ( *iword == '2'){
		free(iword);
        getword(&ib);
        iword = getword(&ib);

        if ( !strcmp("latency",iword)){
	        free(iword);
	    }
		else {
			free(iword);
			continue;
		}
		iword = getword(&ib);
		d1 = getfloat(iword);
		buf_d1[i++] = d1 ;
		printf("d1 is %f\n",d1);
		if ( d1 <= -1.0) {
			free(iword);
			free(buf);
			return -1.0 ;
		}
		else {
			free(iword);
			continue;
		}
	}

	else if ( *iword == '3'){

		free(iword);
        getword(&ib);
        iword = getword(&ib);

        if ( !strcmp("latency",iword)){
	        free(iword);
	    }
		else {
			free(iword);
			continue;
		}
		iword = getword(&ib);
		d2 = getfloat(iword);
		printf("d2 is %f\n",d2);
		buf_d2[j++] = d2;
		if ( d2 <= -1.0) {
			free(iword);
			free(buf);
			return -1.0 ;
		}
		else {
			free(iword);
			continue;
		}
	}
	else {
		free(iword);
		continue;
	}
}

sum = (buf_d2[0]-buf_d1[0]+buf_d2[1]-buf_d1[1])/ 2.0 ;
printf(" v1.0 is %f\n",sum);
DbVs = sum ;
sum = (buf_d2[2]-buf_d1[2]+buf_d2[3]-buf_d1[3])/ 2.0 ;
printf(" v1.2 is %f\n",sum);
DbVh = sum ;
free(buf);
return sum;

}
*/

double cal_sink_d(FILE  * fp, CUSINK * cu){
char * ib, * buf, * iword ;
int  voltage = -1;
double vtage = -1.0;
int loopi = 0;
buf = (char *) malloc(sizeof(char)*MAXCHAR);
if ( buf == NULL){
	printf(" buf allocation error \n");
	free(buf);
	return 0.0 ;
}
while(fgets(buf,MAXCHAR,fp)!= NULL ){
	ib = buf;
	//while((*ib) == ' ' || (*ib) == '\t') ib++;
	iword = getword(&ib);
	if ( !strcmp("===",iword)) {
		free(iword);
		loopi = 0;
		iword = getword(&ib);
		if (!strcmp("clk",iword)){
			free(iword);
			iword = getword(&ib);
			if ( strcmp("src",iword)){
			    printf("wrong cutest file, src should be here\n");
			    free(iword);
			    exit(0);
			}
			free(iword);
			iword = getword(&ib);
			if (!strcmp("rising",iword))
				free(iword);
			else if (!strcmp("falling",iword)){
				if ( voltage < 0){
					printf(" votage should > 0 here\n");
					free(iword);
					exit(0);
				}
				voltage ++;
				free(iword);
			}
			else {
				printf("wrong word for rising or falling\n");
				free(iword);
				exit(0);
			}

			continue;
		}
		else if(!strcmp("vdd",iword));{
			free(iword);
			iword = getword(&ib);
			vtage = getfloat(iword);
			if (ABS(vtage-1.0)<0.001)
				voltage = 0;
			else if ( ABS(vtage-1.2)<0.001)
				voltage = 2;
			else {
				printf("wrong voltage level %f\n",vtage);
				free(iword);
				exit(0);
			}
			continue;
		}

		continue;
	}
	if ( !strcmp("ERROR",iword)){
		free(iword);
		continue;
	}
	//iword = getword(&ib);
	//printf("%s\n",iword);
	if ( !strcmp("sink",iword))
		free(iword);
	else {
		free(iword);
		continue;
	}
	iword = getword(&ib);
//	printf("here\n");

//	printf("%s\n",iword);
//	printf("isdit %d\n",isdigit(*iword));
	if ( isdigit(*iword) ){
//	printf("%s\n",iword);
	cu[loopi].node_id = (int) getfloat(iword);
 //       printf(" I stack here\n");
	free(iword);
        iword = getword(&ib);

	if ( (*iword) != '(' ){
		printf(" error for reading sink index\n");
		free(iword);
		exit(0);
	}
	else { iword ++;
//		printf(" I am here\n");
	       	cu[loopi].sink_index = (int)getfloat(iword);
		iword--;
		free(iword);
	}

        iword = getword(&ib);

        if ( !strcmp("latency",iword)){
	        free(iword);
	    }
	else {
		free(iword);
		printf(" shuld be latency here\n");
		exit(0);
	}
	iword = getword(&ib);
	cu[loopi++].latency[voltage] = getfloat(iword);
	}
    }
	return 0.0;
}
int check_if_over(FILE *fp){
char * buf = (char *) malloc(sizeof(char)*MAXCHAR);
char * bf ;
	if (fgets(buf,MAXCHAR,fp) == NULL){
		free(buf);
		return -1;
	}

	bf = getword(&buf) ;

	if ( 	strcmp("ERROR",bf)  != 0 ){
		free(bf);
		return 0;
	}
	else {
		free(bf);
		getword(&buf);
		bf = getword(&buf);
		if ( strcmp("slew",bf) != 0){
			free(bf);
			return  -1;
		}
		else return 1;

	}

	return 1;

}


int read_clr(FILE  * fp, double *c){
char * ib, * buf, * iword ;
double d1;
int clrflag = 1;
buf = (char *) malloc(sizeof(char)*MAXCHAR);
if ( buf == NULL){
	printf(" buf allocation error \n");
	free(buf);
	return 0.0 ;
}
while(fgets(buf,MAXCHAR,fp)!= NULL ){
	ib = buf;
	iword = getword(&ib);
	if ( !strcmp(iword,"crtinfile")){
		free(iword);
		iword = getword(&ib);
		if ( !strcmp(iword,"crtoutfile")){
			free(iword);
			iword = getword(&ib);
			free(iword);
			iword = getword(&ib);
			d1 = getfloat(iword);
			*c = d1;
			return clrflag;

		}

		else{
			printf("wrong crt test file\n");
			free(iword);
			exit(0);
		}

	}
	else if(!strcmp(iword,"ERROR" )){
		free(iword);
		iword = getword(&ib);
		if (!strcmp(iword,"slew")){
			clrflag = 0;
			free(iword);
		}
		else {
			free(iword);
			clrflag = -1;
		}

	}
}
	printf(" error read ctr file\n");
	exit(0);
}

int preprocess(int flag,CUSINK * cu, int cunum){
FILE * ifp ;
double sink_delay ;
//     printf(" buf lib num is %d\n",buflib.num);
		if ( flag == 1)
			system("./cu1.pl -s -v1 infile outfile tuned_45nm_HP.pm > cutest");
		else if(flag == 0)
			system("./cu0.pl -s -v1 infile outfile  tuned_45nm_HP.pm > cutest");
		else{
			printf(" error flag for inverter ? \n");
			return 0;
		}



		ifp = (FILE *) fopen("cutest","r");
		if ( ifp == NULL){
			printf("no test file generated\n");
			return -1;
		}

		cal_sink_d(ifp,cu);
		fclose(ifp);


	return (int)sink_delay;

}

int crt_process(int flag, double * rval){
FILE * ifp;
int er;
double crt= 0.0;
		if ( flag == 1){
			system("./cularge_1.pl -s -v1 crtinfile crtoutfile tuned_45nm_HP.pm ");
			system("./cularge_1.pl -s -v1 crtinfile crtoutfile tuned_45nm_HP.pm > crtest");
		}
		else if(flag == 4){
		
			system("./cularge_4.pl -s -v1 crtinfile crtoutfile tuned_45nm_HP.pm ");
			system("./cularge_4.pl -s -v1 crtinfile crtoutfile tuned_45nm_HP.pm > crtest");
		
		
		}
			ifp = (FILE *) fopen("crtest","r");
			if ( ifp == NULL){
				printf("no crt test file\n");
				exit(0);
			}
			er = read_clr(ifp, &crt);
			* rval = crt;
			printf(" real crt is %f\n", crt);
			fclose(ifp);
			return er;


}





