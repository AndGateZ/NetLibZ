#include <stdio.h>
#include <string.h>
#define MAXN 100000
#define MAXL 1000000
int prime[MAXN];
bool check[MAXL]; //check==1 非质数  check == 0 质数 

int main(void)
{
    int n, count;
    scanf("%d",&n);
	memset(check, 0, sizeof(check));
	count = 0;

	for (int i = 2; i <= n; i++)
	{
		if (!check[i])  prime[count++] = i;	//是质数，加入prime[]
		for (int j = 0; j < count; j++)
		{
			if (i*prime[j] > MAXL)  break; 	// 过大的时候跳出
			check[i*prime[j]] = 1;			//设置合数
			if ((i%prime[j]) == 0) break;	// 如果i是一个合数，而且i % prime[j] == 0
		}
	}

	for (int i = 0; i < count; i++) printf("%d\n", prime[i]);
    
    return 0;
}
//给出一个正整数n，打印出所有从1~n的质数