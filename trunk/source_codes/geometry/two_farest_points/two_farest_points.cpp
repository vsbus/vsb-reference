double maxConvPolyDist(Poly p) {
	int n = p.size();
	double res = 0;
	if (n == 2) {
		return sqrt((p[0] - p[1]).length2());
	}
	if (n > 2) {
		for (int j = n - 1, i = 0, cur = 1, next = (cur + 1) % n; i < n; j = i++) {
			Point v = p[i] - p[j];
			while(v % (p[cur] - p[j]) <= v % (p[next] - p[j])) {
				cur = next;
				if (++next == n) {
					next = 0;
				}
			}
			res = max(res, max((p[i] - p[cur]).length2(), (p[j] - p[cur]).length2()));
		}
	}
	return sqrt(res);
}
