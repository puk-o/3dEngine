
#include "olcConsoleGameEngine.h"
using namespace std;

struct vec3d
{
	float x, y, z;
};

struct triangle 
{
	vec3d p[3];
};

struct mesh {
	vector<triangle> tris;
};

struct mat4x4 {
	float m[4][4] = { 0 };
};

class olcEngine3D : public olcConsoleGameEngine
{
public:
	olcEngine3D()
	{
		m_sAppName = L"3D Demo";
	}

private:
	mesh meshCube;
	mat4x4 matProj;
	float fTheta;

	void MultiplayMatrixVector(vec3d& i, vec3d& o, mat4x4 &m) {
		o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
		o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
		o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
		float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

		if (w != 0.0f) {
			o.x /= w; o.y /= w; o.z /= w;
		};
	};

public:
	bool OnUserCreate() override
	{
		meshCube.tris = {
			//SOUTH-c
			{0.0f, 0.0f, 0.0f,	     0.0f, 1.0f, 0.0f,		1.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 0.0f,	     1.0f, 1.0f, 0.0f,		1.0f, 0.0f, 0.0f},
			//EAST-c
			{1.0f, 0.0f, 0.0f,	     1.0f, 1.0f, 0.0f,		1.0f, 1.0f, 1.0f},
			{1.0f, 0.0f, 0.0f,	     1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f},
			//NORTH-c
			{1.0f, 0.0f, 1.0f,	     1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f},
			{1.0f, 0.0f, 1.0f,	     0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f},
			//WEST-c
			{0.0f, 0.0f, 1.0f,	     0.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 1.0f,	     0.0f, 1.0f, 0.0f,		0.0f, 0.0f, 0.0f},
			//TOP-c
			{0.0f, 1.0f, 0.0f,	     0.0f, 1.0f, 1.0f,		1.0f, 1.0f, 1.0f},
			{0.0f, 1.0f, 0.0f,	     1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 0.0f},
			//BOTTOM
			{1.0f, 0.0f, 1.0f,	     0.0f, 0.0f, 1.0f,		0.0f, 0.0f, 0.0f},
			{1.0f, 0.0f, 1.0f,	     0.0f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f}
		};

		//Projection Matrix siehe Blatt
		float fNear = 0.1f;
		float fFar = 1000;
		float fFov = 90.0;
		float fAspectRation = (float)ScreenHeight() / (float)ScreenWidth();
		float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

		matProj.m[0][0] = fAspectRation * fFovRad;
		matProj.m[1][1] = fFovRad;
		matProj.m[2][2] = fFar / (fFar - fNear);
		matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
		matProj.m[2][3] = 1.0f;
		matProj.m[3][3] = 0.0;

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_RED);

		mat4x4 matRotZ, matRotX;
		fTheta += 1.0f * fElapsedTime;

		//Rotation Z
		matRotZ.m[0][0] = cosf(fTheta);
		matRotZ.m[0][1] = sinf(fTheta);
		matRotZ.m[1][0] = -sinf(fTheta);
		matRotZ.m[1][1] = cosf(fTheta);
		matRotZ.m[2][2] = 1;
		matRotZ.m[3][3] = 1;

		//Rotation X
		matRotX.m[0][0] = 1;
		matRotX.m[1][1] = cosf(fTheta * 0.5f);
		matRotX.m[1][2] = sinf(fTheta * 0.5);
		matRotX.m[2][1] = -sinf(fTheta * 0.5);
		matRotX.m[2][2] = cosf(fTheta * 0.5);
		matRotX.m[3][3] = 1;
	
		//Draw Triangles

		for (auto tri : meshCube.tris)
		{
			triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;


			MultiplayMatrixVector(tri.p[0], triRotatedZ.p[0], matRotZ);
			MultiplayMatrixVector(tri.p[1], triRotatedZ.p[1], matRotZ);
			MultiplayMatrixVector(tri.p[2], triRotatedZ.p[2], matRotZ);

			MultiplayMatrixVector(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
			MultiplayMatrixVector(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
			MultiplayMatrixVector(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);
		
			
			triTranslated = triRotatedZX;
			triTranslated.p[0].z = triRotatedZX.p[0].z + 3.0f;
			triTranslated.p[1].z = triRotatedZX.p[1].z + 3.0f;
			triTranslated.p[2].z = triRotatedZX.p[2].z + 3.0f;


			MultiplayMatrixVector(triTranslated.p[0], triProjected.p[0], matProj);
			MultiplayMatrixVector(triTranslated.p[1], triProjected.p[1], matProj);
			MultiplayMatrixVector(triTranslated.p[2], triProjected.p[2], matProj);


			//Scale into view
			triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
			triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
			triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;

			triProjected.p[0].x *= 0.5f * (float)ScreenWidth();
			triProjected.p[0].y *= 0.5f * (float)ScreenHeight();
			triProjected.p[1].x *= 0.5f * (float)ScreenWidth();
			triProjected.p[1].y *= 0.5f * (float)ScreenHeight();
			triProjected.p[2].x *= 0.5f * (float)ScreenWidth();
			triProjected.p[2].y *= 0.5f * (float)ScreenHeight();


			DrawTriangle(triProjected.p[0].x, triProjected.p[0].y, 
						 triProjected.p[1].x, triProjected.p[1].y,
						 triProjected.p[2].x, triProjected.p[2].y, 
					   	 PIXEL_SOLID, FG_WHITE);
			
		}

		return true;
	}
};


int main()
{
	olcEngine3D demo;
	if (demo.ConstructConsole(256, 240, 4, 4))
		demo.Start();
};

