import java.util.*;

public class SuffixArray {
	static public int[] nlog(String s) {
		s += (char) 0;
		int n = s.length();
		int[] p = new int[n];
		int[] backet = new int[n];
		int[] temp = new int[n];
		int[] count = new int[n];
		sortByFirst(p, backet, s);
		for (int half = 1; backet[p[n - 1]] != n - 1; half *= 2) {
			Arrays.fill(count, 0);
			for (int i = 0; i < n; ++i) {
				++count[backet[p[i]]];
			}
			for (int i = 1; i < count.length; ++i) {
				count[i] += count[i - 1];
			}
			for (int i = 0; i < n; ++i) {
				temp[i] = (p[i] - half + n) % n;
			}
			for (int i = p.length - 1; i >= 0; --i) {
				p[--count[backet[temp[i]]]] = temp[i];
			}
			temp[p[0]] = 0;
			for (int i = 1; i < n; ++i) {
				if (backet[p[i]] != backet[p[i - 1]] || backet[(p[i] + half) % n] != backet[(p[i - 1] + half) % n]) {
					temp[p[i]] = temp[p[i - 1]] + 1;
				} else {
					temp[p[i]] = temp[p[i - 1]];
				}
			}
			System.arraycopy(temp, 0, backet, 0, n);
		}
		int[] res = new int[n - 1];
		for (int i = 0; i < res.length; ++i) {
			res[i] = p[i + 1];
		}
		return res;
	}

	static private void sortByFirst(int[] p, int[] backet, String s) {
		int[] count = new int[256];
		for (int i = 0; i < s.length(); ++i) {
			++count[s.charAt(i)];
		}
		for (int i = 1; i < count.length; ++i) {
			count[i] += count[i - 1];
		}
		for (int i = 0; i < s.length(); ++i) {
			p[--count[s.charAt(i)]] = i;
		}
		backet[p[0]] = 0;
		for (int i = 1; i < s.length(); ++i) {
			if (s.charAt(p[i]) != s.charAt(p[i - 1])) {
				backet[p[i]] = backet[p[i - 1]] + 1;
			} else {
				backet[p[i]] = backet[p[i - 1]];
			}
		}
	}

	static public int[] lcp(int[] p, String s) {
		int[] res = new int[p.length];
		int[] where = new int[p.length];
		for (int i = 0; i < p.length; ++i) {
			where[p[i]] = i;
		}
		int common = 0;
		for (int i = 0; i < p.length; ++i) {
			if (common > 0) {
				--common;
			}
			int x = where[i];
			if (x != p.length - 1) {
				while (common + p[x] < s.length() && common + p[x + 1] < s.length()) {
					if (s.charAt(common + p[x]) != s.charAt(common + p[x + 1])) {
						break;
					}
					++common;
				}
				res[x] = common;
			}
		}
		return res;
	}
}
