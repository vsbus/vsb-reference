double minDistPoints(Points p) {
	int n = p.size();
	VInt X(n), Y(n);
	for (int i = 0; i < n; ++i) {
		X[i] = Y[i] = i;
	}
	sort(X.begin(), X.end(), LessIndXY(p));
	if (isTwoSamePointsExist(p, X)) {
		return 0;
	}
	sort(Y.begin(), Y.end(), LessIndYX(p));
	return sqrt(delta(p, X, Y));
}
double delta(Points &p, VInt &X, VInt &Y) {
	int n = X.size();
	if (n == 1) {
		return 1e100;
	}
	if (n == 2) {
		return (p[X[0]] - p[X[1]]).length2();
	}
	VInt XL, XR, YL, YR, YMid;
	int mid = n / 2;
	for (int i = 0; i < mid; ++i) {
		XL.push_back(X[i]);
	}
	for (int i = mid; i < n; ++i) {
		XR.push_back(X[i]);
	}
	for (int i = 0; i < n; ++i) {
		(lessXY(p[Y[i]], p[X[mid]]) ? YL : YR).push_back(Y[i]);
	}
	double d = min(delta(p, XL, YL), delta(p, XR, YR));
	double res = d;
	for (int i = 0; i < n; ++i) {
		if (sqr(p[Y[i]].x - p[X[mid]].x) <= d + eps) {
			YMid.push_back(Y[i]);
		}
	}
	for (int i = 0; i < n; ++i) {
		for (int j = i + 1; j < i + 8 && j < YMid.size(); ++j) {
			res = min(res, (p[YMid[i]] - p[YMid[j]]).length2());
		}
	}
	return res;
}
