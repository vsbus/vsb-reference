double getMaxRadius(Poly p) {
	int n = p.size();
	vector<Line> lines(n);
	vector<int> next(n), prev(n);
	for (int j = n - 1, i = 0; i < n; j = i++) {
		lines[i] = Line(p[j], p[i]);
		next[j] = i;
		prev[i] = j;
	}
	vector<double> deathTime(n);
	set<pair<double, int> > s;
	for (int i = 0; i < n; ++i) {
		deathTime[i] = getDeathTime(i, lines, prev, next);
		s.insert(makePair(deathTime, i));
	}
	while (s.size() > 3) {
		int cur = s.begin()->second;
		s.erase(makePair(deathTime, cur));
		s.erase(makePair(deathTime, prev[cur]));
		s.erase(makePair(deathTime, next[cur]));
		next[prev[cur]] = next[cur];
		prev[next[cur]] = prev[cur];
		deathTime[prev[cur]] = getDeathTime(prev[cur], lines, prev, next);
		deathTime[next[cur]] = getDeathTime(next[cur], lines, prev, next);
		s.insert(makePair(deathTime, prev[cur]));
		s.insert(makePair(deathTime, next[cur]));
	}
	return deathTime[s.begin()->second];
}
pair<double, int> makePair(vector<double> &d, int i) {
	return make_pair(d[i], i);
}
double getDeathTime(int i, vector<Line> &lines, vector<int> &prev, vector<int> &next) {
	double alp = getPositiveAng(lines[i].getDir(), -lines[prev[i]].getDir()) / 2;
	double beta = getPositiveAng(lines[next[i]].getDir(), -lines[i].getDir()) / 2;
	if (alp <= eps || beta <= eps) {
		return 1e100;
	}
	Point p1 = getIntPoint(lines[prev[i]].from, lines[prev[i]].to, lines[i].from, lines[i].to);
	Point p2 = getIntPoint(lines[next[i]].from, lines[next[i]].to, lines[i].from, lines[i].to);
	double A = (p1 - p2).length();
	double H = A / (1 / tan(alp) + 1 / tan(beta));
	return H;
}
