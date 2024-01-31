#ifndef CHARMAPPING_H
#define CHARMAPPING_H

#include <LedMatrixChars/SmallCyrillic.h>
#include <LedMatrixChars/BigLatin.h>
#include <LedMatrixChars/SmallLatin.h>
#include <LedMatrixChars/OtherSymbols.h>
#include <unordered_map>
#include <optional>
#include <cmath>

struct DotsChar {
	unsigned short charWidth;
	const unsigned char *dots;

	DotsChar(const unsigned char *dots)
		: dots(dots)
	{
		int maxWidth = 1;

		for (int i = 0; i < 8; ++i) {
			maxWidth = std::max(maxWidth, (int)floor(log2(dots[i])) + 1);
		}

		this->charWidth = maxWidth;
	}

	DotsChar() : charWidth(), dots() { }
};

class CharMapping {
	std::unordered_map<int, DotsChar> items;

public:
	CharMapping() {
		registerChars();
	}

	std::optional<DotsChar> getDotsByChar(int symbol) const {
		auto it = items.find(symbol);

		if (it != items.end()) {
			return it->second;
		}

		return std::nullopt;
	}

	void registerChars() {
		using namespace ledmatrix;

		// small latin chars

		items['a'] = DotsChar(a);
		items['b'] = DotsChar(b);
		items['c'] = DotsChar(c);
		items['d'] = DotsChar(d);
		items['e'] = DotsChar(e);
		items['f'] = DotsChar(f);
		items['g'] = DotsChar(g);
		items['h'] = DotsChar(h);
		items['i'] = DotsChar(i);
		items['j'] = DotsChar(j);
		items['k'] = DotsChar(k);
		items['l'] = DotsChar(l);
		items['m'] = DotsChar(m);
		items['n'] = DotsChar(n);
		items['o'] = DotsChar(o);
		items['p'] = DotsChar(p);
		items['q'] = DotsChar(q);
		items['r'] = DotsChar(r);
		items['s'] = DotsChar(s);
		items['t'] = DotsChar(t);
		items['u'] = DotsChar(u);
		items['v'] = DotsChar(v);
		items['w'] = DotsChar(w);
		items['x'] = DotsChar(x);
		items['y'] = DotsChar(y);
		items['z'] = DotsChar(z);
		items['.'] = DotsChar(DOT);

		// big latin chars

		items['A'] = DotsChar(A);
		items['B'] = DotsChar(B);
		items['C'] = DotsChar(C);
		items['D'] = DotsChar(D);
		items['E'] = DotsChar(E);
		items['F'] = DotsChar(F);
		items['G'] = DotsChar(G);
		items['H'] = DotsChar(H);
		items['I'] = DotsChar(I);
		items['J'] = DotsChar(J);
		items['K'] = DotsChar(K);
		items['L'] = DotsChar(L);
		items['M'] = DotsChar(M);
		items['N'] = DotsChar(N);
		items['O'] = DotsChar(O);
		items['P'] = DotsChar(P);
		items['Q'] = DotsChar(Q);
		items['R'] = DotsChar(R);
		items['S'] = DotsChar(S);
		items['T'] = DotsChar(T);
		items['U'] = DotsChar(U);
		items['V'] = DotsChar(V);
		items['W'] = DotsChar(W);
		items['X'] = DotsChar(X);
		items['Y'] = DotsChar(Y);
		items['Z'] = DotsChar(Z);

		// Special symbols

		items['0'] = DotsChar(_0);
		items['1'] = DotsChar(_1);
		items['2'] = DotsChar(_2);
		items['3'] = DotsChar(_3);
		items['4'] = DotsChar(_4);
		items['5'] = DotsChar(_5);
		items['6'] = DotsChar(_6);
		items['7'] = DotsChar(_7);
		items['8'] = DotsChar(_8);
		items['9'] = DotsChar(_9);
		items[' '] = DotsChar(SPACE);
		items['°'] = DotsChar(DEGREE);
		items['%'] = DotsChar(PERCENT);
		items['/'] = DotsChar(SLASH);
		items['+'] = DotsChar(PLUS);
		items['-'] = DotsChar(MINUS);
		items['_'] = DotsChar(UNDERSCORE);
		items['↑'] = DotsChar(ARROW_UP);
		items['↓'] = DotsChar(ARROW_DOWN);

		// Cyrillic chars

		items['а'] = DotsChar(a);
		items['б'] = DotsChar(cyr_b);
		items['в'] = DotsChar(cyr_v);
		items['г'] = DotsChar(cyr_g);
		items['д'] = DotsChar(cyr_d);
		items['е'] = DotsChar(e);
		items['ё'] = DotsChar(cyr_yo);
		items['ж'] = DotsChar(cyr_zh);
		items['з'] = DotsChar(cyr_z);
		items['и'] = DotsChar(cyr_i);
		items['й'] = DotsChar(cyr_iy);
		items['к'] = DotsChar(cyr_k);
		items['л'] = DotsChar(cyr_l);
		items['м'] = DotsChar(cyr_m);
		items['н'] = DotsChar(cyr_n);
		items['о'] = DotsChar(o);
		items['п'] = DotsChar(cyr_p);
		items['р'] = DotsChar(p);
		items['с'] = DotsChar(c);
		items['т'] = DotsChar(cyr_t);
		items['у'] = DotsChar(y);
		items['ф'] = DotsChar(cyr_f);
		items['х'] = DotsChar(x);
		items['ц'] = DotsChar(cyr_ts);
		items['ч'] = DotsChar(cyr_ch);
		items['ш'] = DotsChar(cyr_sh);
		items['щ'] = DotsChar(cyr_sch);
		items['ъ'] = DotsChar(cyr_hardsign);
		items['ы'] = DotsChar(cyr_yi);
		items['ь'] = DotsChar(cyr_softsign);
		items['э'] = DotsChar(cyr_ye);
		items['ю'] = DotsChar(cyr_yu);
		items['я'] = DotsChar(cyr_ya);
	}
};

#endif