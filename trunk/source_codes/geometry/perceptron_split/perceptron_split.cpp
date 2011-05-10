struct Obj {
	Point point;
	int expectedSign;
};
bool getSplitLine(vector<Obj> p, double &A, double &B, double &C) {
	A = B = C = 1;
	for (int it = 0; it < 1000000; ++it) { // !!! constant as bigger then better
		bool ok = true;
		for (int i = 0; i < p.size(); ++i) {
			double x = p[i].point.x;
			double y = p[i].point.y;
			int s = p[i].expectedSign;
			int ns = sign(A * x + B * y + C * 1);
			if (s != ns) {
				A += s * x;
				B += s * y;
				C += s * 1;
				ok = false;
			}
		}
		if (ok) {
			return true;
		}
	}
	return false;
}
