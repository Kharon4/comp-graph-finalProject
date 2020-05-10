#include <iostream>

#define math3D_DeclrationOnly 1

#include "win32WindowingSystem.h"
#include "testCase1.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {
	enableConsole();
	int x = 800, y = 600;
	window w1(hInstance, nCmdShow, L"hello world", x, y);
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
	initLights();
	triangle * Mesh = new triangle[getNoFaces()];
	vec3d *pts = new vec3d[getNoVertices()];
	generateScene(Mesh, pts);
	collTriangle* cTrs = initMesh(Mesh, getNoFaces());
	
	render(c, Mesh, cTrs, getNoFaces(),pL,dL,lightBounces, w1.data);
	w1.draw();
	
	delete[] Mesh;
	delete[] pts;
	delete[] cTrs;
	
	system("pause");
	
	
	
	return 0;
}