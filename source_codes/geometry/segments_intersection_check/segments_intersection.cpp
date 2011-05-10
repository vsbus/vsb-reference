struct Event {
	Point p;
	int kind; // 0 open, 1 close
	int id;
	Event() {}
	Event(Point p, int kind, int id) : p(p), kind(kind), id(id) {}
	bool operator < (const Event &op) const {
		return p.x < op.p.x
			|| p.x == op.p.x && p.y < op.p.y
			|| p.x == op.p.x && p.y == op.p.y && kind < op.kind
			|| p.x == op.p.x && p.y == op.p.y && kind == op.kind && id < op.id;
	}
};

struct segLess{
	static vector<Seg> *pSegs;
	static Point eventPoint;
	bool operator () (int i, int j) const {
		double y1 = getY((*pSegs)[i]);
		double y2 = getY((*pSegs)[j]);
		return y1 < y2 || y1 == y2 && i < j;
	}
	double getY(Seg &seg) const {
		if (fabs(seg.to.x - seg.from.x) <= eps) {
			return eventPoint.y;
		}
		return seg.from.y + (eventPoint.x - seg.from.x) * (seg.to.y - seg.from.y) / (seg.to.x - seg.from.x);
	}
};
vector<Seg> *segLess::pSegs;
Point segLess::eventPoint;

bool intersect(Point s1, Point e1, Point s2, Point e2) {
	if (max(s1.x, e1.x) < min(s2.x, e2.x) - eps) return false;
	if (max(s1.y, e1.y) < min(s2.y, e2.y) - eps) return false;
	if (max(s2.x, e2.x) < min(s1.x, e1.x) - eps) return false;
	if (max(s2.y, e2.y) < min(s1.y, e1.y) - eps) return false;
	int v1 = sign((s2 - s1) % (e1 - s1)) * sign((e1 - s1) % (e2 - s1));
	int v2 = sign((s1 - s2) % (e2 - s2)) * sign((e2 - s2) % (e1 - s2));
	return v1 >= -eps && v2 >= -eps;
}

bool checkIntersection(vector<Seg> &segs, int i, int j) {
	return intersect(segs[i].from, segs[i].to, segs[j].from, segs[j].to);
}

bool findIntPair(vector<Seg> &segs, int &r1, int &r2) {
	int n = segs.size();
	vector<Event> events(n * 2);
	for (int i = 0; i < segs.size(); ++i) {
		Point from = segs[i].from;
		Point to = segs[i].to;
		if (from.x > to.x || from.x == to.x && from.y > to.y) {
			swap(from, to);
		}
		events[2 * i] = Event(from, 0, i);
		events[2 * i + 1] = Event(to, 1, i);
	}
	sort(events.begin(), events.end());
	segLess::pSegs = &segs;
	set<int, segLess> s;
	set<int, segLess>::iterator it, up, down;
	for (int i = 0; i < events.size(); ++i) {
		segLess::eventPoint = events[i].p;
		if (events[i].kind == 0) {
			it = s.insert(events[i].id).first;
			if (it != s.begin()) {
				up = it;
				r1 = *(--up);
				r2 = *it;
				if (checkIntersection(segs, r1, r2)) {
					return true;
				}
			}
			if (it != --s.end()) {
				down = it;
				r1 = *it;
				r2 = *(++down);
				if (checkIntersection(segs, r1, r2)) {
					return true;
				}
			}			
		} else {
			it = s.find(events[i].id);
			if (it != s.begin() && it != --s.end()) {
				up = down = it;
				r1 = *(--up);
				r2 = *(++down);
				if (checkIntersection(segs, r1, r2)) {
					return true;
				}
			}
			s.erase(it);
		}
	}
	return false;
}
