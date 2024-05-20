
#include "olcConsoleGameEngine.h"
using namespace std;


// This project is created following a Tutorial on Youtube: https://www.youtube.com/watch?v=ih20l3pJoeU
// Since this is my first project in C++ ever, this is just for learning the language 
// and understanding the math behind the engine.
// 
// 
// 
// 
// 
// 
// 
// Started on: 19th May ´24
// Finished: tbd

struct vec3d
{
	float x, y, z;
};

struct triangle 
{
	vec3d p[3];

	wchar_t sym;
	short col;
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
	vec3d vCamera;
	
	float fTheta;

	
	//Vectormultiplication

	void MultiplayMatrixVector(vec3d& i, vec3d& o, mat4x4 &m) {
		o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
		o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
		o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
		float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

		if (w != 0.0f) {
			o.x /= w; o.y /= w; o.z /= w;
		};
	};


	CHAR_INFO GetColour(float lum) 
	{
		short bg_col, fg_col;
		wchar_t sym;
		int pixel_bw = (int)(13.0f * lum);
		switch (pixel_bw) {
		case 0: bg_col = BG_BLACK; fg_col = FG_BLACK; sym = PIXEL_SOLID; break;
		
		case 1: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_QUARTER;		 break;
		case 2: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_HALF;			 break;
		case 3: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_THREEQUARTERS; break;
		case 4: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_SOLID;		 break;
		
		case 5: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_QUARTER;		 break;
		case 6: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_HALF;			 break;
		case 7: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_THREEQUARTERS;	 break;
		case 8: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_SOLID;			 break;

		case 9: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_QUARTER;		 break;
		case 10: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_HALF;			 break;
		case 11: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_THREEQUARTERS;	 break;
		case 12: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_SOLID;			 break;

		default:
			bg_col = BG_BLACK; fg_col = FG_BLACK; sym = PIXEL_SOLID;
		};

		CHAR_INFO c;
		c.Attributes = bg_col | fg_col;
		c.Char.UnicodeChar = sym;
		return c;
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
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);

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


			//Rotating by the Z axis
			
			MultiplayMatrixVector(tri.p[0], triRotatedZ.p[0], matRotZ);
			MultiplayMatrixVector(tri.p[1], triRotatedZ.p[1], matRotZ);
			MultiplayMatrixVector(tri.p[2], triRotatedZ.p[2], matRotZ);

			//Rotating by the x Axis

			MultiplayMatrixVector(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
			MultiplayMatrixVector(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
			MultiplayMatrixVector(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);
		
			
			//Offset into screen so we can see things
			
			triTranslated = triRotatedZX;
			triTranslated.p[0].z = triRotatedZX.p[0].z + 3.0f;
			triTranslated.p[1].z = triRotatedZX.p[1].z + 3.0f;
			triTranslated.p[2].z = triRotatedZX.p[2].z + 3.0f;


			//calculate normalvector 

			vec3d normal, line1, line2;
			line1.x = triTranslated.p[1].x - triTranslated.p[0].x;
			line1.y = triTranslated.p[1].y - triTranslated.p[0].y;
			line1.z = triTranslated.p[1].z - triTranslated.p[0].z;

			line2.x = triTranslated.p[2].x - triTranslated.p[0].x;
			line2.y = triTranslated.p[2].y - triTranslated.p[0].y;
			line2.z = triTranslated.p[2].z - triTranslated.p[0].z;

			normal.x = line1.y * line2.z - line1.z * line2.y;
			normal.y = line1.z * line2.x - line1.x * line2.z;
			normal.z = line1.x * line2.y - line1.y * line2.x;


			//length of normal vector and normalizing the originial vector to have length 1 and same direction

			float l = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
			normal.x /= l; normal.y /= l; normal.z /= l;


			//if (normal.z < 0) -> new version makes sure not to see lines we shouldnt be seeing by calculating the angle 
			//with the scalarproduct not 100% understood the math but i think were trying to use the formula a*b = a1*b1+a2*b2+a3*b3 to calculate the angle
			//at wich we can see the cube, if the scalar of the vectors is less than 0 we can see the part of it (understand the math but too tired for logic rn)

			//2min later: undestood the logic: we can only see "light" that has the direction (or normalized vector) of 0 to -1 because the object is in front of us 
			//and we can only see "light" coming towards us, the normalized vector tells us the direction wich the light is traveling to. by logic we can only see triangles
			//with normal vectors facing us so thats why we can ONLY see triangles with negative normalized vectors pointing at us as the viewer its a bit liek a lamp. 
			//but there are normal vectors < 0 that we logically shouldnt be seeing so we we calculate the angle at wich the light is coming at us, if its less than 90 degrees 
			//we can physically see it so we calculate how "similar" the vectors are with the scalar product and if the value is between 0 and -1 the vector is facing us                                              i hate maths -.-
			

			if( normal.x * (triTranslated.p[0].x - vCamera.x) +
				normal.y * (triTranslated.p[0].y - vCamera.y) +
				normal.z * (triTranslated.p[0].z - vCamera.z) < 0.0f)
				
			{

				//we need light! °.°
				vec3d light_direction = { 0.0f, 0.0f, -1.0f };

				float l = sqrtf(light_direction.x * light_direction.x + light_direction.y * light_direction.y + light_direction.z * light_direction.z);
				light_direction.x /= l; light_direction.y /= l; light_direction.z /= l;

				float dp = normal.x * light_direction.x + normal.y * light_direction.y + normal.z * light_direction.z;

				CHAR_INFO c = GetColour(dp);
				triTranslated.col = c.Attributes;
				triTranslated.sym = c.Char.UnicodeChar;


				//project from 3d->2d 

				MultiplayMatrixVector(triTranslated.p[0], triProjected.p[0], matProj);
				MultiplayMatrixVector(triTranslated.p[1], triProjected.p[1], matProj);
				MultiplayMatrixVector(triTranslated.p[2], triProjected.p[2], matProj);
				triProjected.col = triTranslated.col;
				triProjected.sym = triTranslated.sym;


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


				FillTriangle(triProjected.p[0].x, triProjected.p[0].y,
					triProjected.p[1].x, triProjected.p[1].y,
					triProjected.p[2].x, triProjected.p[2].y,
					triProjected.sym, triProjected.col);

				/*DrawTriangle(triProjected.p[0].x, triProjected.p[0].y,
				triProjected.p[1].x, triProjected.p[1].y,
					triProjected.p[2].x, triProjected.p[2].y,
					PIXEL_SOLID, FG_WHITE);*/
			}
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

