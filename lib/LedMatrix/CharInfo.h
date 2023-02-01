#include <LedMatrixChars.h>

struct CharInfo {
	int charWidth;
	const unsigned char * dots;

	CharInfo() : charWidth(5), dots(0) { }

	static CharInfo byChar(int c) {
		using namespace ledmatrix;
		CharInfo info;
		switch (c) {
		case '0': info.dots = _0; break;
		case '1': info.dots = _1; break;
		case '2': info.dots = _2; break;
		case '3': info.dots = _3; break;
		case '4': info.dots = _4; break;
		case '5': info.dots = _5; break;
		case '6': info.dots = _6; break;
		case '7': info.dots = _7; break;
		case '8': info.dots = _8; break;
		case '9': info.dots = _9; break;
		case ' ': info.charWidth = 3; info.dots = SPACE; break;
		case '+': info.dots = PLUS; break;
		case '-': info.dots = MINUS; break;
		case 'a': info.charWidth = 4; info.dots = A; break;
		case 'b': info.charWidth = 4; info.dots = B; break;
		case 'c': info.charWidth = 4; info.dots = C; break;
		case 'd': info.charWidth = 4; info.dots = D; break;
		case 'e': info.charWidth = 4; info.dots = E; break;
		case 'f': info.dots = F; break;
		case 'g': info.charWidth = 4; info.dots = G; break;
		case 'h': info.charWidth = 4; info.dots = H; break;
		case 'i': info.charWidth = 1; info.dots = I; break;
		case 'j': info.charWidth = 3; info.dots = J; break;
		case 'k': info.charWidth = 4; info.dots = K; break;
		case 'l': info.charWidth = 4; info.dots = L; break;
		case 'm': info.dots = M; break;
		case 'n': info.charWidth = 4; info.dots = N; break;
		case 'o': info.charWidth = 4; info.dots = O; break;
		case 'p': info.charWidth = 4; info.dots = P; break;
		case 'q': info.dots = Q; break;
		case 'r': info.dots = R; break;
		case 's': info.dots = S; break;
		case 't': info.charWidth = 4; info.dots = T; break;
		case 'u': info.charWidth = 4; info.dots = U; break;
		case 'v': info.dots = V; break;
		case 'w': info.dots = W; break;
		case 'x': info.dots = X; break;
		case 'y': info.charWidth = 4; info.dots = Y; break;
		case 'z': info.dots = Z; break;

		case 'а': info.charWidth = 4; info.dots = A; break;
		case 'б': info.charWidth = 4; info.dots = RUS_B; break;
		case 'в': info.charWidth = 4; info.dots = RUS_V; break;
		case 'г': info.charWidth = 4; info.dots = RUS_G; break;
		case 'д': info.dots = RUS_D; break;
		case 'е': info.charWidth = 4; info.dots = E; break;
		case 'ё': info.charWidth = 4; info.dots = E; break;
		case 'ж': info.dots = ZH; break;
		case 'з': info.charWidth = 4; info.dots = RUS_Z; break;
		case 'и': info.dots = RUS_I; break;
		case 'й': info.dots = IY; break;
		case 'к': info.charWidth = 4; info.dots = RUS_K; break;
		case 'л': info.dots = RUS_L; break;
		case 'м': info.dots = RUS_M; break;
		case 'н': info.charWidth = 4; info.dots = RUS_N; break;
		case 'о': info.charWidth = 4; info.dots = O; break;
		case 'п': info.charWidth = 4; info.dots = RUS_P; break;
		case 'р': info.charWidth = 4; info.dots = P; break;
		case 'с': info.charWidth = 4; info.dots = C; break;
		case 'т': info.dots = RUS_T; break;
		case 'у': info.charWidth = 4; info.dots = Y; break;
		case 'ф': info.dots = RUS_F; break;
		case 'х': info.dots = X; break;
		case 'ц': info.dots = TS; break;
		case 'ч': info.charWidth = 4; info.dots = CH; break;
		case 'ш': info.dots = SH; break;
		case 'щ': info.charWidth = 6; info.dots = SCH; break;
		case 'ъ': info.dots = HARDSIGN; break;
		case 'ы': info.dots = YI; break;
		case 'ь': info.charWidth = 4; info.dots = SOFTSIGN; break;
		case 'э': info.charWidth = 4; info.dots = YE; break;
		case 'ю': info.dots = YU; break;
		case 'я': info.charWidth = 4; info.dots = YA; break;
		case '°': info.dots = DEGREE; break;
		case '.': info.charWidth = 3; info.dots = DOT; break;
		}
		return info;
	}
};
