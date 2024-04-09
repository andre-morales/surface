#include "Client.h"
#include "Glow/Window.h"
#include "Glass/Elements/Element.h"
#include "Glass/Elements/Button.h"
#include "Glass/Reflection.h"
#include "Math/Matrices.h"
#include "Strings.h"
#include "Exception.h"
#include "Pointers.h"
#include "Loggy.h"
#include <iostream>
#include <cmath>

static Loggy::Logger print{"Client"};

void pause();

int main() {
	bool error = true;
	print("Welcome!");
	 
	try {
		// Instantiate client structure on stack.
		Client client{};
		client.init();

		// run() only returns when execution is finished.
		client.run();
		client.dispose();

		print("Cleaning up resources...");
		error = false;
	} catch (const Exception& ex) {
		print("*** ERROR: [Exception]: ", ex.what());
	} catch(std::bad_cast ex) {
		print("*** ERROR: [std::bad_cast]: ", ex.what());
	} catch(const std::exception& ex) {
		print("*** ERROR: [std::exception]: ", ex.what());
	} catch (...) {
		print("*** ERROR: Unknown exception thrown!");
	}

	if (error) {
		pause();
	}

	print("Goodbye!");

	pause();
}

void pause() {
	char c = getchar();
}