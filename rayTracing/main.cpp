#include <iostream>

#define math3D_DeclrationOnly 1

#include "win32WindowingSystem.h"
#include "rayTrace.h"


namespace diffuseL {
#include "testCase0.h"
}
namespace reflection{
#include "testCase1.h"
}
namespace refraction {
#include "testCase2.h"
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {
	enableConsole();
	int x = 800, y = 600;
	window w1(hInstance, nCmdShow, L"ray tracing project", x, y);
	for (int i = 0; i < x * y; ++i) {
		w1.data[i * 3 + 2] = 255;
	}
	w1.draw();
	camera c;
	c.xRes = x;
	c.yRes = y;
	c.vertex = vec3d(0, -1, 0);
	c.topLeftCorner = vec3d(-1, 0, double(y)/x);
	c.down = vec3d(0, 0, -2 * double(y) / x);
	c.right = vec3d(2, 0, 0);

	std::cout << "Enter the scene to be rendered: 'd'(diffuse lighting example) , 'r'(reflection example) , 'R'(refraction)  :";
	char input;
	std::cin >> input;
	
	triangle* Mesh = nullptr;
	vec3d* pts = nullptr;
	collTriangle* cTrs = nullptr;
	switch (input){
	case 'd':
		std::cout << "\n\n rendering\n";
		diffuseL::initLights();
		Mesh = new triangle[diffuseL::getNoFaces()];
		pts = new vec3d[diffuseL::getNoVertices()];
		diffuseL::generateScene(Mesh, pts);
		cTrs = initMesh(Mesh, diffuseL::getNoFaces());
		render(c, Mesh, cTrs, diffuseL::getNoFaces(), diffuseL::pL, diffuseL::dL, lightBounces, w1.data);
		break;
	case 'r':
		std::cout << "\n\n rendering\n";
		reflection::initLights();
		Mesh = new triangle[reflection::getNoFaces()];
		pts = new vec3d[reflection::getNoVertices()];
		reflection::generateScene(Mesh, pts);
		cTrs = initMesh(Mesh, reflection::getNoFaces());
		render(c, Mesh, cTrs, reflection::getNoFaces(), reflection::pL, reflection::dL, lightBounces, w1.data);
		break;
	case 'R':
		std::cout << "\n\n rendering\n";
		refraction::initLights();
		Mesh = new triangle[refraction::getNoFaces()];
		pts = new vec3d[refraction::getNoVertices()];
		refraction::generateScene(Mesh, pts);
		cTrs = initMesh(Mesh, refraction::getNoFaces());
		render(c, Mesh, cTrs, refraction::getNoFaces(), refraction::pL, refraction::dL, lightBounces, w1.data);
		break;
	default:
		std::cout << "\n\n ERR wrong key entered\n";
		system("pause");
		return 1;
	}
	
	
	w1.draw();
	
	delete[] Mesh;
	delete[] pts;
	delete[] cTrs;
	
	system("pause");
	
	return 0;
}