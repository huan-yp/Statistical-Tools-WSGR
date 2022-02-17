#include <bits/stdc++.h>
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
int i,j,k,n,s,t,m;
const int dx[]={1,1,0,0},dy[]={0,1,0,1};
int fa[N*M],mp[N][M],maxn[N*M][2],minu[N*M][2];
int col[N][M],rk[N][M],a[N*M],vis[N*M],size[N*M];
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
struct Matri{
	int l,r,t,b;	
	friend bool operator <(const Matri &a,const Matri &b)
	{
		return a.l<b.l;
	}
}Mat[100];
signed main()
{
 	freopen("data.in","r",stdin);
	freopen("data.out","w",stdout);
	read(n),read(m);s=0,t=0;
	for(i=0;i<n*m;i++)fa[i]=i,size[i]=1,minu[i][0]=minu[i][1]=INF;
	for(i=0;i<n;i++)		
	for(j=0;j<m;j++)
	{
		rk[i][j]=++s;
		read(col[i][j]);
		if(col[i][j]<=192) 
		col[i][j]=0;
		else 
		col[i][j]=1;
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
		if(u==rk[i][j]&&col[i][j]==1)
		{
			Mat[++s]=(Matri){max(minu[u][1]-2,0),max(minu[u][0]-2,0)
		,min(maxn[u][1]+2,m-1),min(maxn[u][0]+2,n-1)};
		}
	}
	sort(Mat+1,Mat+s+1);
	printf("%d\n",s);
	for(i=1;i<=s;i++)
	printf("%d %d %d %d\n",Mat[i].l,Mat[i].r,Mat[i].t,Mat[i].b);
	fclose(stdin);
	fclose(stdout);
	return 0;
}

