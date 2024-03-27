#pragma once 

#include <d3d9.h>
#include <d3dx9.h>
#include <D3dx9tex.h>

#include <map>
#include <string>

// 라이브러리
#pragma comment(lib, "d3d9")
#pragma comment(lib, "d3dx9")
#pragma comment(lib, "winmm")

using namespace std;

struct CVERTEX
{
	FLOAT x,y,z;
	FLOAT nx,ny,nz; //nomal
	FLOAT u,v;   //uv좌표
};

#define D3DFVF_CVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 )

struct D3DTEXTURE
{
	float fsizeX;
	float fsizeY;
	LPDIRECT3DTEXTURE9 Texture;
};

struct D3DSPRITE
{
	D3DSPRITE()
		:fStartX(0),fEndX(0),fStartY(0),fEndY(0),
		wCount(0),hCount(0)
	{
	}
	D3DTEXTURE Texture;
	float fStartX;
	float fEndX;
	float fStartY;
	float fEndY;
	int wCount;
	int hCount;
};

class C3DRender
{
public :
	C3DRender();
	~C3DRender();

	int Creat( HWND m_hWnd );
	void Destory();
	
	
	HRESULT BeginScene();
	HRESULT Clear(	D3DCOLOR col= D3DCOLOR_XRGB(0,0,0),
		DWORD flag=(D3DCLEAR_TARGET  ), 
				    float z=1000.0f,
				    DWORD s=0);

	HRESULT EndScene();
	HRESULT Flip();
	
	void LoadTexture(char* filePath, float fsizeX, float fsizeY, D3DTEXTURE* outLoadTexture);

	//max option
	void DrawEx(D3DTEXTURE* d3dtexture ,bool mask , int Sampler,
										float fPosX, float fPosY, float fPosZ,
										float foutWidth, float foutHight, 
										float texture_startX, float texture_endX,  
										float texture_startY, float texture_endY,
										float fRotX, float fRotY, float fRotZ );

	//simple option
	void DrawEx(D3DTEXTURE* d3dtexture ,float fPosX, float fPosY, float fPosZ,
										float foutWidth, float foutHight, 
										float texture_startX, float texture_endX,  
										float texture_startY, float texture_endY,
										float fRotX, float fRotY, float fRotZ );

	void LoadSprite(char* filePath, float fsizeX, float fsizeY, 
									float fstartX, float fendX, 
									float fstartY, float fendY, 
									D3DSPRITE* outLoadSprite);
	
	void LoadSprite(D3DTEXTURE* loadtexture, 
									float fstartX, float fendX, 
									float fstartY, float fendY, 
									D3DSPRITE* outLoadSprite);

	//simple option
	void DrawSprite(D3DSPRITE* d3dsprite, float fPosX, float fPosY, float fPosZ,
										  float foutWidth, float foutHight,
										  int widthCount, int hightCount ,
										  float time,
										  float fRotX, float fRotY, float fRotZ );
	

	//max option
	void DrawSprite(D3DSPRITE* d3dsprite, bool mask,
										  int Sampler,
										  float fPosX, float fPosY, float fPosZ,
										  float foutWidth, float foutHight,
										  int widthCount, int hightCount ,
										  float time,
										  float fRotX, float fRotY, float fRotZ );
	


	void DrawString(char* outchar, float posX, float posY ,DWORD color);

	void ShowFPS(DWORD tick);
	DWORD getTime();
	
private:

	LPDIRECT3D9			m_pD3D;
	LPDIRECT3DDEVICE9	m_pD3dDevice;
	LPD3DXFONT			m_PFont;	
	LPDIRECT3DVERTEXBUFFER9  m_pVB;


	CVERTEX* m_pVertices;

};

