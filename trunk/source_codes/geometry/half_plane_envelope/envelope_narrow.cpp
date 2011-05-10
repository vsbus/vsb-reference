struct HPlane {
	Point	point, dir;
	double	startArg, endArg, dirAngle;
	HPlane(Point point, Point dir) : point(point), dir(dir), startArg(-inf), endArg(inf) {
		dirAngle = fmod(atan2(dir.y, dir.x) + Pi2, Pi2);
	}
	Point getParamPoint(double par) { 
		return point + dir * par;
	}
	bool operator < (const HPlane &op) const {
		if (fabs(dirAngle - op.dirAngle) > eps) {
			return dirAngle < op.dirAngle;
		}
		return (op.point - point) % op.dir > 0;
	}
};

void getIntArgs(HPlane &plane1, HPlane &plane2, double &p1, double &p2) {
	Point s1 = plane1.point;  Point e1 = plane1.point + plane1.dir;
	Point s2 = plane2.point;  Point e2 = plane2.point + plane2.dir;
	p1 = - ((s1 - s2) % (e2 - s2)) / ((e1 - s1) % (e2 - s2));
	p2 = - ((s2 - s1) % (e1 - s1)) / ((e2 - s2) % (e1 - s1));
}

bool isInside(Point p, HPlane hPlane) {
	return sign(hPlane.dir % (p - hPlane.point)) == 1;
}

bool isOutside(HPlane toCheck, HPlane hPlane) {
	if (fabs(toCheck.dir % hPlane.dir) <= eps) {
		return !isInside(toCheck.point, hPlane);
	}
	double par1, par2;
	getIntArgs(toCheck, hPlane, par1, par2);
	int z = sign(toCheck.dir % hPlane.dir);
	return sign(toCheck.startArg - par1) * z >= 0
		&& sign(toCheck.endArg - par1) * z >= 0;
}

vector<HPlane> getEnvelope(vector<HPlane> L) {
	sort(L.begin(), L.end());
	for (int i = 0; i < L.size(); ++i) {
		int j = (i + 1) % L.size();
		if (fmod(L[j].dirAngle - L[i].dirAngle + 2 * Pi, 2 * Pi) >= Pi - eps) {
			rotate(L.begin(), L.begin() + j, L.end());
			break;
		}
	}
	deque<HPlane> res;
	res.push_back(L[0]);
	for (int i = 1; i < L.size(); ++i) {
		if (res.back().endArg < inf 
				&& isInside(res.back().getParamPoint(res.back().endArg), L[i])
				|| fmod(L[i].dirAngle - res.back().dirAngle + 2 * Pi, 2 * Pi) <= eps) { 
			continue;  // hPlane contain all polygon
		}
		while (res.size() > 0 && isOutside(res.back(), L[i])) { // pop all outside segments
			res.pop_back();
		}
		if (res.size() == 0) { // result is empty
			return vector<HPlane>(res.begin(), res.end());
		}
		if (fabs(L[i].dir % res.back().dir) > eps) { // intersection point exists
			getIntArgs(res.back(), L[i], res.back().endArg, L[i].startArg);
		}
		while (res.size() > 0 && isOutside(res.front(), L[i])) { // pop all outside segments
			res.pop_front();
		}
		if (L[i].dir % res.front().dir > eps) { // if region closed
			getIntArgs(res.front(), L[i], res.front().startArg, L[i].endArg);
		}
		res.push_back(L[i]);
	}
	return vector<HPlane>(res.begin(), res.end());
}
