#include <stdio.h>
int a[100], b[100], c[100];

int extrage(int m, int n)
{
	int k=0;

	for (int i=0; i<n; i++)
		for (int j=0; j<m; j++)
		{
			if (a[i]==b[j])
			{
				c[k] = a[i]; //c[k] gen calvin kelin hehe ce gluma buna
				k++;
			}
		}

	return k;
}

int main()
{
int n,m,k;

scanf("%d", &n);
for (int i=0; i<n; i++)
	scanf("%d", &a[i]);

scanf("%d", &m);
for (int i=0; i<m; i++)
	scanf("%d", &b[i]);

k = extrage(m, n);

printf("Am gasit %d element comune\n", k);
for (int i=0; i<k; i++) printf("%d ", c[i]);


}