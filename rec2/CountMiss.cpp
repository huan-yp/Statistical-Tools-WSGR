#include<bits/stdc++.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#define y1 y3647
#define earse erase
#define INF 1000000000
#define LL long long
#define pii pair<int,int>
using namespace std;
inline void read(int &x)
{
	x=0;int f=1;char ch=getchar();
	while(ch!=45&&(ch>'9'||ch<'0'))ch=getchar();
	if(ch==45){f=-1,ch=getchar();}
	while(ch<='9'&&ch>='0'){x=x*10+ch-48;ch=getchar();}x*=f;
}
void cmin(int &a,int b){a=min(a,b);}
void cmax(int &a,int b){a=max(a,b);}
const int N=1600,M=900;
int i,j,k,n,s,t,m,bright;
const int dx[]={1,1,0,0},dy[]={0,1,0,1};
int fa[N*M],maxn[N*M][2],minu[N*M][2];
int col[N][M],rk[N][M],size[N*M],vis[N*M];
struct point{
	int x,y;
};
struct MyImage{
	short rgb[N][M][3],g[N][M],n,m;
};
struct Matri{
	int l,r,b,t;
	point cen;
	void GetCenter(){cen=(point){t+b>>1,l+r>>1};}
}a[N];
int find(int x)
{
	return fa[x]==x?x:fa[x]=find(fa[x]);
}
void merge(int u,int v)
{
	u=find(u),v=find(v);
	if(u==v)return ;
	fa[u]=v;size[v]+=size[u];
}
signed main()
{
 	freopen("data.in","r",stdin);
	freopen("data.out","w",stdout);
	for(i=0;i<=1.5e6;i++)
	read(s);
	
	for(i=0;i<n*m;i++)fa[i]=i,size[i]=1,minu[i][0]=minu[i][1]=INF;
	for(i=0;i<n;i++)
	for(j=0;j<m;j++)
	{
		rk[i][j]=++s;
		read(col[i][j]);
	}
	for(i=0;i<n;i++)
	for(j=0;j<m;j++)
	{
		for(k=0;k<4;k++)
		{
			int tox=i+dx[k],toy=j+dy[k];
			if(tox>=n||toy>=m)continue;
			if(col[i][j]==col[tox][toy])
			merge(rk[i][j],rk[tox][toy]);
		}
	}
	s=0;
	for(i=0;i<n;i++)
	for(j=0;j<m;j++)
	{
		int u=find(rk[i][j]);
		cmax(maxn[u][0],i),cmin(minu[u][0],i);
		cmax(maxn[u][1],j),cmin(minu[u][1],j);
	}
	for(i=0;i<n*m;i++)
	{
		if(find(i)!=i)continue;
		int height=maxn[i][0]-minu[i][0],weight=maxn[i][1]-minu[i][1];
		if(size[i]<=50||height<=15||weight<=15)continue;
		vis[i]=1;s++;
	}
	printf("%d %d\n",n,m);
	for(i=0;i<n;i++,printf("\n"))
	for(j=0;j<m;j++)
	printf("%d ",col[i][j]*vis[find(rk[i][j])]);
	 
	fclose(stdin);
	fclose(stdout);
	
	freopen("res.out","w",stdout);
	printf("%d\n",s);
	fclose(stdout);
	return 0;
}

