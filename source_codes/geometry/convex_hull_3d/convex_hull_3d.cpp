typedef vector<int> Side;
vector<int> rnd; // random order on vertices

Side getFirstSide(vector<Point> &p) {
	int i1 = 0;
	for (int i = 0; i < p.size(); ++i) {
		if (p[i].z < p[i1].z
				|| p[i].z == p[i1].z && p[i].x < p[i1].x
				|| p[i].z == p[i1].z && p[i].x == p[i1].x && p[i].y < p[i1].y) {
			i1 = i;
		}
	}
	int i2 = i1 == 0 ? 1 : 0;
	for (int i = 0; i < p.size(); ++i) {
		if (i != i1) {
			Point zDir(0, 0, 1);
			double curCos = (p[i] - p[i1]) * zDir / (p[i] - p[i1]).length();
			double bestCos = (p[i2] - p[i1]) * zDir / (p[i2] - p[i1]).length();
			if (curCos < bestCos) {
				i2 = i;
			}
		}
	}
	int i3 = -1;
	int n = p.size();
	for (int ri = 0; ri < n; ++ri) {
		int i = rnd[ri];
		if (i != i1 && i != i2) {
			Point norm = (p[i1] - p[i]) % (p[i2] - p[i]);
			bool sg[] = {0, 0, 0};
			for (int t = 0; t < n; ++t) {
				int j = rnd[t];
				sg[1 + sign((p[j] - p[i]) * norm)] = true;
				if (sg[0] && sg[2]) {
					break;
				}
			}
			if (sg[0] ^ sg[2]) {
				i3 = i;
				if (sg[0] == false) {
					swap(i3, i2);
				}
				break;
			}
		}
	}
	vector<int> res;
	res.push_back(i1);
	res.push_back(i2);
	res.push_back(i3);
	return res;
}

double getAngle(const Point &n1, const Point &n2) {
	return atan2((n1 % n2).length(), n1 * n2);
}

double getNormsAngle(int i, int j, int k, int t, vector<Point> &p) {
	Point n1 = (p[j] - p[i]) % (p[k] - p[i]);
	Point n2 = (p[t] - p[i]) % (p[j] - p[i]);
	return getAngle(n1, n2);
}

void dfs(int i, int j, int k, vector<Point> &p, vector<Side> &sides) {
	if (i < j && i < k) {
		vector<int> side(3);
		side[0] = i;
		side[1] = j;
		side[2] = k;
		sides.push_back(side);
	}
	int key = getSideKey(i, j, k);
	setUsed(key);
	int n = p.size();
	if (!checkUsed(j, k, i)) {
		dfs(j, k, i, p, sides);
	}
	if (!checkUsed(k, i, j)) {
		dfs(k, i, j, p, sides);
	}
	int bestT = -1;
	double bestAngle = 1e20;
	Point curNorm = (p[j] - p[i]) % (p[k] - p[i]);
	Point dir = p[j] - p[i];
	for (int t = 0; t < n; ++t) {
		if (t != i && t != j && t != k) {
			Point newNorm = (p[t] - p[i]) % dir;
			double curAng = curNorm * newNorm / newNorm.length();
			if (bestT == -1 || curAng > bestAngle) {
				bestT = t;
				bestAngle = curAng;
			}
		}
	}
	if (!checkUsed(i, bestT, j)) {
		dfs(i, bestT, j, p, sides);
	}
}

vector<Side> solve(vector<Point> p) {
	int i, j, k;
	Side side0 = getFirstSide(p);
	vector<Side> sides;
	used.clear();
	dfs(side0[0], side0[1], side0[2], p, sides);
	return sides;

}
