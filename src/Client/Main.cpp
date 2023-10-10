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
#include <cmath>
#include "Graphics/MeshGeneratorTables.h"
#include "Physics\Collisions.h"

static Loggy::Logger print{"Client"};

void pause();

int main() {
	bool error = true;
	print("Welcome!");
	 
	try {
		/*AABB boxes[]{
			{ {2, 4, -1}, {0.5, 0.5, 0.5} },
			{ {-1.0f, 5.0f, 0.0f}, {0.5f, 0.5f, 0.5f}},
			{ {0.0f, 0.0f, 0.0f }, {5, 7, 5      }},
			{ {2.0f, 3.0f, -3.0f}, {3, 0.5f, 2    }},

			{ {2, 2, 2       }, {1.0f, 1.0f, 1.0f}},
			{ {-2, -2, -2    }, {1.0f, 1.0f, 1.0f}},
			{ {10, 10, 10    }, {11.0f, 11.0f, 11.0f}}
		};
		Vector3f triangle[]{
			{-1, 5, 0},
			{2, 2, -3},
			{5, 5, 0}
		};
		bool v = Collisions::IsIntersecting(boxes[0], triangle);
		
		print(Collisions::IsIntersecting(boxes[0], triangle));
		print(Collisions::IsIntersecting(boxes[1], triangle));
		print(Collisions::IsIntersecting(boxes[2], triangle));
		print(Collisions::IsIntersecting(boxes[3], triangle));
		print(Collisions::IsIntersecting(boxes[4], triangle));
		print(Collisions::IsIntersecting(boxes[5], triangle));
		print(Collisions::IsIntersecting(boxes[6], triangle));*/
		//auto& table = Tables::closestEdgeOfCases;
		//;
		//print(table[0][0]);

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