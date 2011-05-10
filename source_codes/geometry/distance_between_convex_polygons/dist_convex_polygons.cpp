double distPointsSegs(vector<HPlane> &A, vector<HPlane> &B) {
	double res = inf;
	int j = 0;
	int nb = B.size();
	for (int i = 0; i < A.size(); ++i) {
		Point p = A[i].getStartPoint();
		findBestSeg(p, B, j, next);
		findBestSeg(p, B, j, prev);
		res = min(res, getDist2(p, B[j]));
	}
	return res;
}
void findBestSeg(Point &p, vector<HPlane> &B, int &j, int (*next)(int, int)) {
	int n = B.size();
	int nj = next(j, n);
	double curDist = getDist2(p, B[j]);
	double newDist = getDist2(p, B[nj]);
	while (curDist > newDist + eps) {
		curDist = newDist;
		j = nj;
		nj = next(j, n);
		newDist = getDist2(p, B[nj]);
	}
}
int next(int i, int n) {
	if (++i == n) i = 0;
	return i;
}
int prev(int i, int n) {
	if (--i == -1) i = n - 1;
	return i;
}
double getDist2(Point &p, HPlane &seg) {
	++cnt;
	Point s = seg.point;
	Point e = s + seg.dir;
	double par = -((s - p) * (e - s)) / ((e - s) * (e - s));
	if (par < seg.startArg) return (p - getStartPoint(seg)).length2();
	if (par > seg.endArg) return (p - getEndPoint(seg)).length2();
	return (p - (s + (e - s) * par)).length2();
}
