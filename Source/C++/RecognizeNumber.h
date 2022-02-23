void cmax(int& x, int y) {
	x = max(x, y);
}
void cmin(int& x, int y) {
	x = min(x, y);
}
struct NumberMatri {
	/*
	����㷨��
	ֱ������ƥ��+��ͨ����ƥ��
	��Ŀ�꽨������ֱ�ߣ����չؼ�����ٶ�ֱ�߷��࣬���� 01 ����������ƥ�䡣
	*/
	int a[50][50], n, m, cover, all = 0, block = 0;
	int rk[50][50], fa[100 * 25], size[25 * 100], bl[100];
	int maxn[100 * 25][2], minu[100 * 25][2];
	int line[100];
	int find(int x) {
		return fa[x] == x ? x : fa[x] = find(fa[x]);
	}
	void merge(int u, int v) {
		u = find(u), v = find(v);
		if (u == v)return;
		size[v] += size[u];
		cmax(maxn[v][0], maxn[u][0]), cmax(maxn[v][1], maxn[u][1]);
		cmin(minu[v][0], minu[u][0]), cmin(minu[v][1], minu[u][1]);
		fa[u] = v;
	}
	void init(ImageMatri Img, int t, int b, int l, int r, int c) {
		const int dx[] = { 1,-1,0,0 }, dy[] = { 0,0,1,-1 };

		memset(a, 0, sizeof(a));
		memset(line, 0, sizeof(line));
		block = 0;
		n = b - t + 3, m = r - l + 3;
		int s = 0;
		for (int i = t;i <= b;i++)
			for (int j = l;j <= r;j++)
				a[i - t + 1][j - l + 1] = Img.g[i][j] / 255;
		for (int i = 0;i < n;i++)
			for (int j = 0;j < m;j++) {
				all += a[i][j];
				rk[i][j] = ++s;
				fa[s] = s, size[s] = 1;
				maxn[s][0] = minu[s][0] = i;
				maxn[s][1] = minu[s][1] = j;
			}
		//���ν���
		for (int times = 1;times <= 5;times++)
			for (int i = 0;i < n;i++)
				for (int j = 0;j < m;j++) {
					if (i == 0 || j == 0 || i == n - 1 || j == m - 1)continue;
					int cnt = 0;
					for (int k = 0;k < 4;k++) {
						int tox = i + dx[k], toy = j + dy[k];
						cnt += a[tox][toy];
					}
					if (cnt > 2)a[i][j] = 1;
					if (cnt < 2)a[i][j] = 0;
				}
		//��ȡ������
		for (int i = 0;i < n;i++)
			for (int j = 0;j < m;j++) {
				line[i] += a[i][j];

				for (int k = 0;k < 4;k++) {
					int tox = i + dx[k], toy = j + dy[k];
					if (tox >= n || toy >= m || tox < 0 || toy < 0)continue;
					if (a[i][j] == a[tox][toy])
						merge(rk[i][j], rk[tox][toy]);
				}
			}
		for (int i = 1;i <= n * m;i++) {
			if (find(i) == i && size[i] >= 10)
				bl[++block] = i;
		}
		cover = c;
		if (cover == 2 && n > 33)cover = 0;
	}
	int TestNumber() {
		//��ȥ�� 1
		//��������
		if (m <= 18) {
			int u = find(1), Target = bl[1] == u ? 2 : 1;Target = bl[Target];
			int height = maxn[Target][0] - minu[Target][0], width = maxn[Target][1] - minu[Target][1];
			if (1.0 * height / width <= 2)
				return 7;
			return 1;
		}
		if (cover) {
			//���ڵ���ƥ�䷽ʽ
			//������ͨ���С�ж� 7
			if (all <= 200)return 7;
			//��ͨ�����ɸ�� 6,8
			if (block == 3) {
				//����ϲ�����ֱ���ж� 6,8
				int SwitchLine = 0;
				for (int i = 2;i <= n / 2 - 4;i++) {
					int SwitchTimes = 0;
					for (int j = 0;j < m;j++) {
						if (j + 1 == m || a[i][j] == a[i][j + 1])continue;
						SwitchTimes++;
					}
					if (SwitchTimes == 4)SwitchLine++;
				}
				if (SwitchLine > 2)return 8;
				return 6;
			}
			int SwitchLine = 0;
			for (int i = 0;i <= n / 2 - 2;i++) {
				int SwitchTimes = 0;
				for (int j = 0;j < m;j++) {
					if (j + 1 == m || a[i][j] == a[i][j + 1])continue;
					SwitchTimes++;
				}
				if (SwitchTimes == 4)SwitchLine++;
			}
			int u = find(1), Target = bl[1] == u ? 2 : 1;Target = bl[Target];
			if (SwitchLine >= 3) {
				//�����и����� 0 
				int SwitchLine = 0;
				for (int i = 0;i <= n / 2 - 2;i++) {
					int SwitchTimes = 0;
					for (int j = 0;j < m;j++) {
						if (j + 1 == m || a[i][j] == a[i][j + 1])continue;
						SwitchTimes++;
					}
					if (SwitchTimes == 4)SwitchLine++;
				}
				if (SwitchLine > 10)return 0;
				//����ֱ���и����� 4,9
				int maxpos = 0;
				for (int i = 0;i < n;i++)
					for (int j = 0;j <= n;j++)
						if (a[i][j] && j == minu[Target][1])
							cmax(maxpos, i);
				if (maxpos >= n / 2 + 2)return 4;
				return 9;
			}
			//�����������ֱ������ 2,3,5
			int SwitchTime = 0;
			for (int i = 0;i < n;i++) {
				if (i + 1 == n - 1 || (line[i] <= 15) == (line[i + 1] <= 15))continue;
				SwitchTime++;
			}
			if (SwitchTime < 4)return 2;
			//��������������ֱ������ 3,5;
			int first = m, last = 0;
			if (line[minu[Target][0]] <= 3) minu[Target][0]++;
			for (int i = 0;i <= n;i++) {
				for (int j = 0;j < m;j++)
					if (i == minu[Target][0] && a[i][j])
						cmin(first, j), cmax(last, j);
			}
			if (first <= m * 2 / 5.5)return 5;
			if (last >= m * 3.5 / 5.5)return 3;
			printf("Boom!");
			return -1;
		}
		else {
			//û���ڵ���ƥ�䷽ʽ
			//�����ͨ�飬�ж� 8��6��9��0
			if (block == 4)return 8;

			if (block == 3) {
				//4 ���ϲ��������ģ�������Ϊһ����ͨ��
				//ע��������ܰ� 4 �жϽ�ȥ
				int siz = min(size[bl[1]], min(size[bl[2]], size[bl[3]])), rk = 0;
				//�����Ĵ�С���ж� 0 
				if (siz > 80)return 0;
				for (int i = 1;i <= 3;i++)if (size[bl[i]] == siz)rk = bl[i];
				//�������ֱ���ж� 4
				if (maxn[rk][0] - minu[rk][0] > 8)return 4;
				//����ϲ�����ֱ���ų� 8 
				int SwitchLine = 0;
				for (int i = 2;i <= n / 2 - 4;i++) {
					int SwitchTimes = 0;
					for (int j = 0;j < m;j++) {
						if (j + 1 == m || a[i][j] == a[i][j + 1])continue;
						SwitchTimes++;
					}
					if (SwitchTimes == 4)SwitchLine++;
				}
				int pos = maxn[rk][0] + minu[rk][0] >> 1;
				if (pos < n / 2)return 9;
				if (SwitchLine > 2)return 8;
				//�����ͨ��λ���ж� 6,9
				return 6;
				//9,0,6
			}
			//�����������ֱ�ߣ��ж� 2,4
			int SwitchLine = 0;
			for (int i = 0;i < n / 2;i++) {
				int SwitchTimes = 0;
				for (int j = 0;j < m;j++) {
					if (a[i][j] == a[i][j + 1] || j + 1 == m)continue;
					SwitchTimes++;
				}
				if (SwitchTimes == 4)SwitchLine++;
			}
			if (SwitchLine) {
				if (SwitchLine > 4)return 4;
				return 2;
			}
			int SwitchTimes = 0;
			int pos = n / 2 - 4, i;
			for (i = n - 1;i >= n / 2 - 3;i--) {
				if (line[i] >= 15)break;
			}
			//�����²�����ֱ���ж� 7
			if (i < n / 2 - 3) return 7;

			//�����ϲ�����ֱ���ж� 3,5
			for (i = 5;i < n;i++) {
				int first = m, last = 0;
				if (line[i] < 10 && line[i] >= 3) {
					for (int j = 0;j <= m;j++)
						if (a[i][j])
							cmin(first, j), cmax(last, j);
					if (first <= m * 2 / 7)return 5;
					if (last >= m * 5 / 7)return 3;
				}
			}
		}
		printf("Boom!!!\n");
		return -1;
	}
};
