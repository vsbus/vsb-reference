int pred[N] = {-1, -1, -1, ..., -1};
void dfs1(int u) {
	color[u] = 1;
	low[u] = d[u] = ++dfs_time;
	art[u] = false;
	int i, children = 0;
	FOR(i,0,e[u].size()) {
		int v = e[u][i];
		if(color[v] == 0) {
			pred[v] = u;
			dfs1(v);
			if(pred[u] != -1 && low[v] >= d[u])
				art[u] = true;
			children++;
			low[u] = min(low[u], low[v]);
		}
		else if(/*color[u] == 1 &&*/ v != pred[u])
			low[u] = min(low[u], d[v]);
	}
	if(pred[u] == -1)
	art[u] = children > 1;
	f[u] = ++dfs_time;
	color[u] = 2;
}
void dfs2(int u) {
	int i;
	FOR(i,0,e[u].size()){
		if (pred[e[u][i]] == u) {
			bcc[e[u][i]] = 
				(low[e[u][i]] > d[u]) ? -1 :
				(low[e[u][i]] < d[u] ? bcc[u] : comps_count++);
			dfs2(e[u][i]);
		}
	}
}
int getComp(int u, int v) {
	return d[u] < d[v] ? bcc[v] : bcc[u];
}
