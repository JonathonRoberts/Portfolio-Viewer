#include <stdio.h>
#include <stdlib.h> /* atoi */
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "ezXPath.c"

#define MAX_CHAR 300

float stockquote(char tick[]);
float btcquote(char *currency);
void deletespaces(char src[],char dst[]);
float lsequote(char tick[]);

#define LENGTH 300

int main(){
	float wealth = 0;
	char string[MAX_CHAR];
	char ticker[MAX_CHAR];
	char tmp[MAX_CHAR];
	char class[MAX_CHAR];
	float amount=0;
	float price=0;
	FILE *fp;
	lsequote("PURP");
	//fp=fopen("../config", "r");
	//printf("Asset\t|\tPrice\t|\tAmount\t|\tWorth\n");
        //while(fgets(string, MAX_CHAR, fp)!=NULL) {
	//	deletespaces(string,string);
	//	strlcpy(class , strtok(string, ":"),MAX_CHAR);
	//	strlcpy(ticker , strtok(NULL, ":"),MAX_CHAR);
	//	strlcpy(tmp , strtok(NULL, ":"),MAX_CHAR);
	//	amount = atof(tmp);
	//	if(strcmp(class,"stock")==0){
	//		price = stockquote(ticker);
	//	}
	//	else if(strcmp(class,"crypto")==0){
	//		price = btcquote("GBP");
	//	}
	//	printf(" %s\t |\t%.2f\t |\t%.2f\t |\t%.2f\n",ticker,price,amount,price*amount);
	//	wealth +=price*amount;
        //}
	//printf("Net = %.2f\n",wealth);
	return 0;

}


float stockquote(char tick[]){

	//Create API Link
	char apilink[LENGTH] = "/usr/local/bin/curl -s 'http://download.finance.yahoo.com/d/quotes.csv?s=";
	strlcat( apilink, tick, LENGTH);
	strlcat( apilink, "&f=l1c'", LENGTH);

	//get quote
	char price[100];
	FILE *fp;
	fp = popen(apilink, "r");
	fscanf(fp, "%s",price);
	pclose(fp);

	return atoi(price);

}
float lsequote(char tick[])
{
	char *output[10];
	int i;
	int size;
	char website[LENGTH] = "http://www.lse.co.uk/SharePrice.asp?shareprice=";
	char *xpath = "/html/body/div/div/div/div/div/div/div/div/div";
	strlcat(website, tick, LENGTH);
	size = ezXPathHTML(website,xpath,output);
	if(size>0)
		for(i = 0;i<size;i++)
		{
			printf("%s\n",output[i]);
			//fscanf(fp, "%s",price);
			free(output[i]);
		}
	return 0.0;
}
float btcquote(char *currency){

	//Create API Link
	char apilink[LENGTH] = "/usr/local/bin/curl -s 'https://blockchain.info/tobtc?currency=";
	strlcat( apilink, currency, LENGTH);
	strlcat( apilink, "&value=1'", LENGTH);

	//get quote
	char price[100];
	FILE *fp;
	fp = popen(apilink, "r");
	fscanf(fp, "%s",price);
	pclose(fp);

	return 1/atof(price);
}

void deletespaces(char src[], char dst[]){
   // src is supposed to be zero ended
   // dst is supposed to be large enough to hold src
  int s, d=0;
  for (s=0; src[s] != 0; s++)
    if (src[s] != ' ') {
       dst[d] = src[s];
       d++;
    }
  dst[d] = 0;
}
