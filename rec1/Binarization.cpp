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
const int w=960;
const int N=1600,M=900;
int i,j,k,n,s,t,m,bright;
const int dx[]={1,1,0,0},dy[]={0,1,0,1};
int fa[N*M],mp[N][M],maxn[N*M][2],minu[N*M][2];
int col[N][M],rk[N][M],a[N*M],vis[N*M],size[N*M];
int acc[N][M];
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
void dfs(int x,int y)
{
	if(col[x][y]<=bright)
	acc[x][y]=0;
	if(acc[x][y]!=-1)return ;
	acc[x][y]=0;
	int dx[]={1,-1,0,0},dy[]={0,0,1,-1};
	for(int k=0;k<4;k++)
	{
		int tox=x+dx[k],toy=y+dy[k];
		if(tox>=n||toy>=m||tox<0||toy<0)
		{
			acc[x][y]=1;
			return;
		}
		dfs(tox,toy);
		if(acc[tox][toy]==1)acc[x][y]=1;
	}
}
signed main()
{
 	freopen("data.in","r",stdin);
	freopen("data.out","w",stdout);
	read(n),read(m);s=0,t=0;
	for(i=0;i<n*m;i++)fa[i]=i,size[i]=1,minu[i][0]=minu[i][1]=INF;
	for(i=0;i<n;i++)
	for(j=0;j<m;j++)
	{
		acc[i][j]=-1;
		rk[i][j]=++s;
		read(col[i][j]);
		bright+=col[i][j];
		if(col[i][j]>=168) 
		col[i][j]=255;
	}
	bright=18*1600.0/w; 
	for(i=0;i<n;i++)
	for(j=0;j<m;j++)
	{
		dfs(i,j);
		if(acc[i][j])col[i][j]=0;
	}	
	for(i=0;i<n;i++)
	for(j=0;j<m;j++)
	{
		dfs(i,j);
		if(acc[i][j])col[i][j]=0;
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
	
	for(i=0;i<n;i++)
	for(j=0;j<m;j++)
	{
		int u=find(rk[i][j]);
		if(u==rk[i][j]&&size[u]>=50)
			a[u]=0;
		if(size[u]<=150||col[i][j]!=255||size[u]>500||maxn[u][0]-minu[u][0]>=40||maxn[u][1]-minu[u][1]>=30||maxn[u][0]-minu[u][0]<=20)
		col[i][j]=0;
	}
	printf("%d %d\n",n,m);
	for(i=0;i<n;i++,printf("\n"))
	for(j=0;j<m;j++)
	printf("%d ",col[i][j]);
	
	fclose(stdin);
	fclose(stdout);
	return 0;
}

