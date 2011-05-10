struct HP {
	Point p, dir; double lo, hi, ang;
	HP(Point point, Point dir) : p(point), dir(dir), lo(-inf), hi(inf) {
		ang = fmod(atan2(dir.y, dir.x) + Pi2, Pi2); }
	Point getParamPoint(double par) { return p + dir * par; }
	bool operator < (const HP &op) const {
		if (fabs(ang - op.ang) > eps) return ang < op.ang;
		return (op.p - p) % op.dir > 0;
	}
};
void getIntArgs(HP &plane1, HP &plane2, double &p1, double &p2) {
	Point s1 = plane1.p;  Point e1 = plane1.p + plane1.dir;
	Point s2 = plane2.p;  Point e2 = plane2.p + plane2.dir;
	p1 = - ((s1 - s2) % (e2 - s2)) / ((e1 - s1) % (e2 - s2));
	p2 = - ((s2 - s1) % (e1 - s1)) / ((e2 - s2) % (e1 - s1));
}
bool isInside(Point p, HP hPlane) { return sign(hPlane.dir % (p - hPlane.p)) == 1; }
bool isOutside(HP toCheck, HP hPlane) {
	if (fabs(toCheck.dir % hPlane.dir) <= eps) return !isInside(toCheck.p, hPlane);
	double par1, par2;
	getIntArgs(toCheck, hPlane, par1, par2);
	int z = sign(toCheck.dir % hPlane.dir);
	return sign(toCheck.lo - par1) * z >= 0 && sign(toCheck.hi - par1) * z >= 0;
}
vector<HP> getEnvelope(vector<HP> L) {
	sort(L.begin(), L.end());
	for (int i = 0; i < L.size(); ++i) {
		int j = (i + 1) % L.size();
		if (fmod(L[j].ang - L[i].ang + 2 * Pi, 2 * Pi) >= Pi - eps) {
			rotate(L.begin(), L.begin() + j, L.end());
			break;
		}
	}
	deque<HP> R;
	R.push_back(L[0]);
	for (int i = 1; i < L.size(); ++i) {
		if (R.back().hi < inf && isInside(R.back().getParamPoint(R.back().hi), L[i])
				|| fmod(L[i].ang - R.back().ang + 2 * Pi, 2 * Pi) <= eps) { 
			continue; }  // hPlane contain all polygon 
		while (R.size() > 0 && isOutside(R.back(), L[i])) { 
			R.pop_back(); } // pop all outside segments
		if (R.size() == 0) { // result is empty
			return vector<HP>(R.begin(), R.end()); }
		if (fabs(L[i].dir % R.back().dir) > eps) { // intersection point exists
			getIntArgs(R.back(), L[i], R.back().hi, L[i].lo); }
		while (R.size() > 0 && isOutside(R.front(), L[i])) { 
			R.pop_front(); } // pop all outside segments
		if (L[i].dir % R.front().dir > eps) { // if region closed
			getIntArgs(R.front(), L[i], R.front().lo, L[i].hi); }
		R.push_back(L[i]);
	}
	return vector<HP>(R.begin(), R.end());
}
