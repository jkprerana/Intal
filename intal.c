#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "intal.h"

static char* remove_leading_zero(char * str);
static char* revstr(char *str);
static char* copy(char *a);

void* intal_create(const char* str){
	if(strcmp(str,"") == 0 || strlen(str) == 0)
		return NULL;
	int i;
	for(i=0; str[i]!='\0'; i++){
		if(str[i] < '0' || str[i] > '9')
			return NULL;
	}
	char *x=(char *)malloc(sizeof(char) * (1 + strlen(str)));
	for(i=0; str[i] != '\0'; i++)
		x[i] = str[i];
	x[i] = '\0';
	x = remove_leading_zero(x);
	return x;
}

char* intal2str(void* intal){
	if(intal == NULL){
		char *x = (char *)malloc(sizeof(char)*4);
		x[0]='N';x[1]='a'; x[2]='N'; x[3]='\0';
		return x;
	}
	char *a = copy(intal);
	return a;
}

void intal_destroy(void* intal){
	if(intal == NULL)
		return;
	else
		free(intal);
}

void* intal_increment(void* intal){
	if(intal == NULL)
		return NULL;
	return intal_add(intal, "1");
}

void* intal_decrement(void* intal){
	if(intal == NULL)
		return NULL;
	if(strcmp(intal, "0") == 0 || strcmp(intal, "1") ==0)
		return intal_create("0");
	return intal_diff(intal, "1");
}

void* intal_add(void* intal1, void* intal2){
	if(intal1 == NULL || intal2 == NULL)
		return NULL;
	char *a = copy(intal1);
	char *b = copy(intal2);
	a = remove_leading_zero(a);
	b = remove_leading_zero(b);
	if(strcmp(a,"0")== 0){
		intal_destroy(a);
		return b;
	}
	if(strcmp(b,"0")== 0){
		intal_destroy(b);
		return a;
	}
	if(strlen(a) > strlen(b)){
		a = copy(intal2);
		b = copy(intal1);
		a = remove_leading_zero(a);
		b = remove_leading_zero(b);
	}
	int n = strlen(a), m = strlen(b);
	char * str = (char *)malloc((m + 2) *sizeof(char));
	int diff = m - n;
	int i,carry = 0, k = 0;
	for(i= n - 1; i>=0; i--){
		int sum = ((a[i] - '0') + (b[i+diff] - '0') + carry);
		str[k++] = sum%10 + '0';
		carry = sum/10;
	}
	for(i= m - n - 1 ; i>=0; i--){
		int sum = ((b[i] - '0') + carry);
		str[k++] = sum%10 + '0';
		carry = sum/10;
	}
	if (carry)
		str[k++] = carry + '0';
	str[k] = '\0';
	str = revstr(str);
	str = remove_leading_zero(str);
	intal_destroy(a);
	intal_destroy(b);
	return str;
}

void* intal_diff(void* intal1, void* intal2){
	if(intal1 == NULL || intal2 == NULL)
		return NULL;
	char *a = copy(intal1);
	char *b = copy(intal2);
	a = remove_leading_zero(a);
	b = remove_leading_zero(b);
	if(strcmp(a,"0") == 0){
		intal_destroy(a);
		return b;
	}
	if(strcmp(b,"0") == 0){
		intal_destroy(b);
		return a;
	}
	if(intal_compare(intal1, intal2) == -1){
		a = copy(intal2);
		b = copy(intal1);
		a = remove_leading_zero(a);
		b = remove_leading_zero(b);
	}
	int n = strlen(a), m = strlen(b);
	char * str = (char *)malloc((n+1) * sizeof(char));
	int diff = n - m;
	int i, carry =  0, k = 0;
	for(i=m - 1; i>=0; i--){
		int sub = ((a[i+diff] - '0') - (b[i] - '0') - carry);
		if(sub < 0){
			sub = sub + 10;
			carry = 1;
		}
		else{
			carry = 0;
		}
		str[k++] = sub + '0';
	}
	for(i=(diff - 1); i>=0; i--){
		if(a[i] == '0' && carry == 1){
			str[k++] = '9';
			continue;
		}
		int sub = ((a[i] - '0') - carry);
		if(i>0 || sub>0)
			str[k++] = sub + '0';
		carry = 0;
	}
	str[k] = '\0';
	char * difference = revstr(str);
	difference = remove_leading_zero(difference);
	intal_destroy(a);
	intal_destroy(b);
	return difference;
}

void* intal_multiply(void* intal1, void* intal2){
	if(intal1 == NULL || intal2 == NULL)
		return NULL;
	char *a = copy(intal1);
	char *b = copy(intal2);
	a = remove_leading_zero(a);
	b = remove_leading_zero(b);
	if(strcmp(a, "0" ) == 0 || strcmp(b, "0" ) == 0){
		intal_destroy(a);
		intal_destroy(b);
		return "0";
	}
	if(strcmp(b, "1" ) == 0){
		intal_destroy(b);
		return a;
	}
	if(strcmp(a, "1" ) == 0){
		intal_destroy(a);
		return b;
	}
	int n = strlen(a), m = strlen(b);
	int x = n + m ;
	int r = x - 1;
	char * c = (char *)malloc(sizeof(char) * (x + 1));
	int i, j, tmp;
	for(i=0; i< x; i++){
		c[i] = '0';
	}
	c[i] = '\0';
	for(i= m - 1 ; i>=0; i--){
		int offset = 0;
		int carry = 0;
		for(j= n-1; j>=0; j--){
			tmp =(( a[j] - '0') * ( b[i] - '0') + carry + (c[r-offset]-'0'));
			carry = tmp/10;
			c[r-offset] = tmp%10 + '0';
			offset++;
		}
		if(carry != 0){
			c[r - offset] = carry + '0';
		}
		r--;
	}
	c = remove_leading_zero(c);
	intal_destroy(a);
	intal_destroy(b);
	return c;
}

void* intal_divide(void* intal1, void* intal2){
	if(intal1 == NULL || intal2 == NULL)
		return NULL;
	if(strcmp(intal2, "0" ) == 0)
		return NULL;
	if(strcmp(intal2, "1" ) == 0){
		char * a = copy(intal1);
		return a;
	}
	if(intal_compare(intal1, intal2) == 0)
		return "1";
	if(intal_compare(intal1, intal2) == -1)
		return "0";
	char *a = copy(intal1);
	char *b = copy(intal2);
	a = remove_leading_zero(a);
	b = remove_leading_zero(b);
	int n = strlen(a), m = strlen(b);
	char *c = (char *)malloc(sizeof(char) * (n + 1));
	char *dividend = (char *)malloc(sizeof(char) * (m + 2));
	char *divisor = (char *)malloc(sizeof(char) * (m + 1));
	char *res = (char *)malloc(sizeof(char) * 2);
	res[0] = '0';
	res[1] = '\0';
	int i, count = 0;
	int t = m - 1;
	for(i=0; i<m ; i++){
   		dividend[i] = a[i];
   		divisor[i] = b[i];
   	}
   	dividend[m] = '\0';
   	divisor[m] = '\0';
	while(t < n){
   		int flag = 0;
   		dividend = remove_leading_zero(dividend);
	   	if(intal_compare(dividend,divisor) >= 0){
			while(intal_compare(dividend,divisor) >= 0){
				dividend = (char *)intal_diff(dividend, divisor);
				res = (char *)intal_increment(res);
			}
			c[count++] = res[0];
			c[count] = '\0';
			res[0] = '0';
			res[1] = '\0';
			flag = 1;
		}
		else{
			if(t == (n-1))
				break;
			int temp = strlen(dividend);
			temp++;
			dividend[temp - 1] = a[t + 1];
			dividend[temp] = '\0';
			t++;
		}
		if(!flag && intal_compare(dividend,divisor) == -1){
			c[count++] = '0';
			c[count] = '\0';
		}
	}
	c = remove_leading_zero(c);
	intal_destroy(a);
	intal_destroy(b);
	intal_destroy(res);
	intal_destroy(dividend);
	intal_destroy(divisor);
	return c;
}

int intal_compare(void* intal1, void* intal2){
	if(intal1 == NULL || intal2 == NULL)
		return -2;
	char *a = copy(intal1);
	char *b = copy(intal2);
	a = remove_leading_zero(a);
	b = remove_leading_zero(b);
	int n = strlen(a), m = strlen(b);
	if(n < m){
		intal_destroy(a);
		intal_destroy(b);
		return -1;
	}
	if(n > m){
		intal_destroy(a);
		intal_destroy(b);
		return 1;
	}
	for(int i=0; i<n ; i++){
		if(a[i] < b[i]){
			intal_destroy(a);
			intal_destroy(b);
			return -1;
		}
		else if(a[i] > b[i]){
			intal_destroy(a);
			intal_destroy(b);
			return 1;
		}
	}
	intal_destroy(a);
	intal_destroy(b);
	return 0;
}

void* intal_pow(void* intal1, void* intal2){
	if(intal1 == NULL || intal2 == NULL)
		return NULL;
	char *a = copy(intal1);
	char *b = copy(intal2);
	a = remove_leading_zero(a);
	b = remove_leading_zero(b);
	if(strcmp(a, "0" ) == 0 || strcmp(a, "1" ) == 0){
		intal_destroy(b);
		return a;
	}
	if(strcmp(b, "0" ) == 0){
		intal_destroy(a);
		intal_destroy(b);
		return "1";
	}
	if(strcmp(b, "1" ) == 0){
		intal_destroy(b);
		return a;
	}
	char *pow = intal_pow(a, intal_divide(b, "2"));
	if(b[strlen(b) -1 ] % 2 == 0){
		intal_destroy(a);
		intal_destroy(b);
		return intal_multiply(pow, pow);
	}
	else{
		intal_destroy(a);
		intal_destroy(b);
		return intal_multiply(intal1, intal_multiply(pow,pow));
	}
}

static char * revstr(char *str){
	if (str==NULL){
   		return NULL;
    }
    int i,k=0;
    char * a = (char *)malloc(sizeof(char) * (strlen(str)+1));
	for(i = strlen(str)-1; i>=0; i--){
		a[k++] = str[i];
	}
	a[k++] = '\0';
	intal_destroy(str);
  	return a;
}

static char* remove_leading_zero(char * str){
	int i, count = 0;
	for(i=0; str[i]!='\0'; i++){
		if(str[i] == '0'){
			count++;
		}
		else
			break;
	}
	if(i == strlen(str)){
		char * temp = (char *)malloc(sizeof(char) * 2);
		temp[0] = '0';
		temp[1] = '\0';
		intal_destroy(str);
		return temp;
	}
	int j = 0;
	char *temp=(char*)malloc(sizeof(char) * (1 + strlen(str) - count));
	for(i=count; str[i] != '\0'; i++)
		temp[j++] = str[i];
	temp[j] = '\0';
	intal_destroy(str);
	return temp;
}

static char* copy(char *a){
	char * temp = (char *)malloc(sizeof(char) * (strlen(a)+1));
	int i;
	for(i=0; a[i] != '\0'; i++)
		temp[i] = a[i];
	temp[i] = '\0';
	return temp;
}
