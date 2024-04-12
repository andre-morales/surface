#include "Debug.h"
#include <conio.h>

namespace Debug {
	char choice() {
		return _getch();
	}

	void pause() {
		choice();
	}
}