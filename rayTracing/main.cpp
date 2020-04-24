#include <iostream>

#define math3D_DeclrationOnly 1

#include "win32WindowingSystem.h"

#include "testCase0.h"


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
	c.topLeftCorner = vec3d(-1, 0, 1);
	c.down = vec3d(0, 0, -2);
	c.right = vec3d(2, 0, 0);
	initLights();
	triangle* trs = generateTriangles();
	triangle sphereMesh[triangleNo(10, 10)];
	vec3d pts[vertexNo(10, 10)];
	triangle colorVal;
	colorVal.diffuseRefelctivity = vec3f(255, 0, 0);
	generateSphere(vec3d(0, 2, -1), 1, 10, 10, pts, sphereMesh,colorVal);
	collTriangle* cTrs = initMesh(sphereMesh, triangleNo(10, 10));
	render(c, sphereMesh, cTrs, triangleNo(10, 10),pL,dL, w1.data);
	w1.draw();
	deleteTrs(trs, cTrs, noTrs);
	system("pause");
	
	
	
	return 0;
}