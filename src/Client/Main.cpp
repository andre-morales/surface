#include "Pointers.h"
#include "Loggy.h"
#include "Client.h"
#include "Glow/Window.h"
#include "Exception.h"
#include <iostream>
#include "Glass\Elements\Element.h"
#include "Glass\Elements\Button.h"
#include "Glass\Reflection.h"
#include "Math/Matrices.h"
#include "Strings.h"

static Loggy::Logger print{"Client"};

void pause();

int main() {
	bool error = true;
	print("Welcome!");
	 
	try {

		Math::Mat4f mat;
		mat.m03 = 3;
		mat.m30 = 1;
		mat.m21 = 4;
		mat = mat.inverse();
		print(mat.toString());

		Client client{};
		client.init();
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