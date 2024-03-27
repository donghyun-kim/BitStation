#include "D3DRender.h"


C3DRender::C3DRender()
:m_pD3dDevice(0)
{

}

C3DRender::~C3DRender()
{

}


int C3DRender::Creat(HWND m_hWnd)
{


	///////////////////////////////// dx ///////////////////////////////////////

	if(NULL == ( m_pD3D = Direct3DCreate9(D3D_SDK_VERSION) ) )
	{
		return FALSE;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp , sizeof(d3dpp));

	d3dpp.Windowed					= TRUE;					//창모드 생성
	d3dpp.BackBufferWidth = 800;
	d3dpp.BackBufferHeight = 600;
	
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	//가장 효율적인 효과
	d3dpp.BackBufferFormat			= D3DFMT_A8R8G8B8;			//현재 바탕화면모드에 맞추어 후면버퍼생성
	d3dpp.BackBufferCount			= 1;
	d3dpp.MultiSampleType			= D3DMULTISAMPLE_NONE;
	d3dpp.EnableAutoDepthStencil	= TRUE;
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D24S8;
	d3dpp.Flags						= 0;
	d3dpp.hDeviceWindow				= m_hWnd;

	//d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval	     = D3DPRESENT_INTERVAL_IMMEDIATE;

	//d3dpp.EnableAutoDepthStencil	= TRUE;						//복잡한 다각형을 그리기위한
	//d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;

	//d3d 객채 생성
	m_pD3D->CreateDevice(	D3DADAPTER_DEFAULT,
										D3DDEVTYPE_HAL,
										m_hWnd,
										D3DCREATE_SOFTWARE_VERTEXPROCESSING,
										&d3dpp,
										&m_pD3dDevice);

	
	//컬링모드
	m_pD3dDevice->SetRenderState(D3DRS_CULLMODE , D3DCULL_NONE);



	//z버퍼
	m_pD3dDevice->SetRenderState(D3DRS_ZENABLE  , TRUE);




/*
 //---------------------------------------------
 //     2D 셋팅과 렌더링 설정
 //---------------------------------------------
 D3DXMATRIX     matproj,matview;


 //2D 뷰와 렌더링 상태를 설정한다.
 D3DXMatrixIdentity(&matview);
 m_pD3dDevice->SetTransform(D3DTS_VIEW,&matview);

 //직교 교차 렌더링을 설정한다(2D처럼 보이는 3D)
 D3DXMatrixOrthoLH(&matproj,(float)800,
  (float)600, 0, 1);
 
 //투영 행렬을 설정한다.
 m_pD3dDevice->SetTransform(D3DTS_PROJECTION,&matproj);

 //후면 추려내기를 끈다.
 m_pD3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);

 //조명을 끈다.
 m_pD3dDevice->SetRenderState(D3DRS_LIGHTING,FALSE);

 //Z 버퍼를 끈다.
 m_pD3dDevice->SetRenderState(D3DRS_ZENABLE,FALSE);
 m_pD3dDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
*/



/*
	D3DXMATRIXA16 matView;
	D3DXMatrixIdentity(&matView);
	
	D3DXMATRIXA16 matProj;
	D3DXMatrixOrthoLH(&matProj ,800 ,600 ,0,1);
	//월드 행열
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	m_pD3dDevice->SetTransform(D3DTS_WORLD , &matWorld);
*/
	
	//뷰 행열
	D3DXVECTOR3 vEyePt( 0.0f, 0.0f, -300.0);		//눈위치
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f , 0.0f);	//눈이바라보는 위치
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);		//천정 방향
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	m_pD3dDevice->SetTransform(D3DTS_VIEW, &matView);

	//프로젝션 행열
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj,
								D3DX_PI*0.5f,  //45도
								(float)800/ (float)600,		//종횡비
								100.0f,		//near
								1000.0f);	//par
	m_pD3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
	
	//후면 추려내기를 끈다.
	m_pD3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);

	//조명을 끈다.
	m_pD3dDevice->SetRenderState(D3DRS_LIGHTING,FALSE);

	//Z 버퍼를 끈다.
	m_pD3dDevice->SetRenderState(D3DRS_ZENABLE,FALSE);
	m_pD3dDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);


	




	D3DXCreateFont( m_pD3dDevice, 0, 0, 0, 1, FALSE, DEFAULT_CHARSET, 
     OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
     "Fixedsys", &m_PFont );

		m_pD3dDevice->CreateVertexBuffer(4 * sizeof(CVERTEX), 
									 0, 
									 D3DFVF_CVERTEX,D3DPOOL_DEFAULT,
									 &m_pVB,
									 NULL);

	return TRUE;
}

void C3DRender::Destory()
{

	//return (int) m_msg.wParam;
	//(int) m_msg.wParam;
}


HRESULT C3DRender::BeginScene()
{


	return m_pD3dDevice->BeginScene();
}

HRESULT C3DRender::Clear( D3DCOLOR col,
						  DWORD flag, 
						  float z,
						  DWORD s)
{

	return m_pD3dDevice->Clear(0, NULL , flag , col , z ,s ) ;

	 
}

HRESULT C3DRender::EndScene()
{
	return m_pD3dDevice->EndScene();
}

//버퍼 전환
HRESULT C3DRender::Flip()
{
	return m_pD3dDevice->Present( NULL, NULL ,NULL , NULL);
}

void C3DRender::LoadTexture(char* filePath ,float sizeX, float sizeY, D3DTEXTURE* outLoadTexture )
{
	if(m_pD3dDevice == NULL)
		return;

	outLoadTexture->fsizeX = sizeX;
	outLoadTexture->fsizeY = sizeY;

	D3DXCreateTextureFromFileExA(m_pD3dDevice ,  filePath ,
								 0,0,0,0,
								 D3DFMT_A8R8G8B8,  
								 D3DPOOL_MANAGED,D3DX_FILTER_NONE,
								 D3DX_DEFAULT, 
								 D3DCOLOR_RGBA(0,0,0,255),
								 NULL,NULL ,&outLoadTexture->Texture  );	

}


void C3DRender::DrawEx(D3DTEXTURE* d3dtexture ,float fPosX, float fPosY, float fPosZ,
										float foutWidth, float foutHight, 
										float texture_startX, float texture_endX,  
										float texture_startY, float testure_endY,
										float fRotX, float fRotY, float fRotZ )
{
	//sample 0포인트 1리니어 2비등방성
	DrawEx( d3dtexture , false,
						 0,
										 fPosX,  fPosY,  fPosZ,
										 foutWidth,  foutHight, 
										 texture_startX,  texture_endX,  
										 texture_startY,  testure_endY,
										 fRotX,  fRotY,  fRotZ );
}
	

void C3DRender::DrawEx(D3DTEXTURE* d3dtexture , 
										bool mask,
										int Sampler,
										float fPosX, float fPosY, float fPosZ,
										float foutWidth, float foutHight, 
										float texture_startX, float texture_endX,  
										float texture_startY, float texture_endY,
										float fRotX, float fRotY, float fRotZ )
{
	if(d3dtexture == NULL)
	{
		return;
	}

	D3DXMATRIX  matWorld, matRotation;
	D3DXMATRIX  matTranslation, matScale;
	float   fXPos, fYPos;

	//디폴트 위치, 배율, 회전각 등을 설정한다.
	D3DXMatrixIdentity(&matTranslation);

	//스프라이트 크기를 지정한다.
	D3DXMatrixScaling( &matScale, foutWidth, foutHight, 1.0f);
	D3DXMatrixMultiply( &matTranslation, &matTranslation, &matScale);

	//스프라이트를 회전시킨다.
	D3DXMatrixRotationYawPitchRoll(&matRotation, fRotY, fRotX, fRotZ);
	D3DXMatrixMultiply( &matWorld, &matTranslation, &matRotation);

	
	//화면 공간에서 위치를 계산한다.
	fXPos = (float) ( -(800/2) + fPosX + (texture_endX - texture_startX) / (( (texture_endX - texture_startX)/foutWidth)*2)  );
	fYPos = (float) ( -(600/2) + fPosY + (texture_endY - texture_startY) / (( (texture_endY - texture_startY)/foutHight)*2) );

	//스프라이트를 옮긴다.
	matWorld._41 = fXPos; //X
	matWorld._42 = fYPos; //Y

	//행렬을 지정한다.
	m_pD3dDevice->SetTransform(D3DTS_WORLD,&matWorld);


	
	m_pD3dDevice->SetTexture(0, d3dtexture->Texture);
	m_pD3dDevice->SetRenderState (D3DRS_ALPHABLENDENABLE, TRUE );
	m_pD3dDevice->SetRenderState (D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
	// 버텍스들의 알파셋에 대하여 블랜딩 설정& 마스킹 이미지 설정
	if(mask)
	{
		m_pD3dDevice->SetRenderState (D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pD3dDevice->SetRenderState (D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	}
	else
	{
		m_pD3dDevice->SetRenderState (D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
	}

	//셈플링
	switch(Sampler)
	{
		case 0:
		m_pD3dDevice->SetSamplerState(0,D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		m_pD3dDevice->SetSamplerState(0,D3DSAMP_MINFILTER, D3DTEXF_POINT);
		break;

		case 1:
		m_pD3dDevice->SetSamplerState(0,D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pD3dDevice->SetSamplerState(0,D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		break;

		case 2:
		m_pD3dDevice->SetSamplerState(0,D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
		m_pD3dDevice->SetSamplerState(0,D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
		m_pD3dDevice->SetSamplerState(0,D3DSAMP_MAXANISOTROPY, 4);
		break;	
	}

	m_pVB->Lock(0, 0, (void**)&m_pVertices,0);	

	m_pVertices[0].x = - 0.5f;
	m_pVertices[0].y = - 0.5f;
	m_pVertices[0].z =  fPosZ;
	m_pVertices[0].nx =  0.0f;
	m_pVertices[0].ny =  0.0f;
	m_pVertices[0].nz = -1.0f;
	m_pVertices[0].u = (texture_startX / d3dtexture->fsizeX);
	m_pVertices[0].v = (texture_endY   / d3dtexture->fsizeY);

	m_pVertices[1].x = -0.5f;
	m_pVertices[1].y =  0.5f;
	m_pVertices[1].z = fPosZ;
	m_pVertices[1].nx =  0.0f;
	m_pVertices[1].ny =  0.0f;
	m_pVertices[1].nz = -1.0f;
	m_pVertices[1].u = (texture_startX / d3dtexture->fsizeX);
	m_pVertices[1].v = (texture_startY / d3dtexture->fsizeY);
	
	m_pVertices[2].x =  0.5f;
	m_pVertices[2].y = -0.5f;
	m_pVertices[2].z = fPosZ;
	m_pVertices[2].nx =  0.0f;
	m_pVertices[2].ny =  0.0f;
	m_pVertices[2].nz = -1.0f;
	m_pVertices[2].u = (texture_endX   / d3dtexture->fsizeX);
	m_pVertices[2].v = (texture_endY   / d3dtexture->fsizeY);

	m_pVertices[3].x =  0.5f;
	m_pVertices[3].y =  0.5f;
	m_pVertices[3].z = fPosZ;
	m_pVertices[3].nx =  0.0f;
	m_pVertices[3].ny =  0.0f;
	m_pVertices[3].nz = -1.0f;
	m_pVertices[3].u = (texture_endX    / d3dtexture->fsizeX);
	m_pVertices[3].v = (texture_startY  / d3dtexture->fsizeY);

	m_pVB->Unlock();

	// 텍스쳐 버텍스 출력 
	m_pD3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(CVERTEX));
	m_pD3dDevice->SetFVF( D3DFVF_CVERTEX );
	m_pD3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

	//텍스쳐 참조를 해제한다.
	m_pD3dDevice->SetTexture( 0, NULL);
	m_pD3dDevice->SetTexture( 1, NULL);


 
}

void C3DRender::LoadSprite(char* filePath, float fsizeX, float fsizeY, 
										   float fstartX, float fendX, 
										   float fstartY, float fendY, 
										   D3DSPRITE* outLoadSprite)
{
	outLoadSprite->fStartX = fstartX;
	outLoadSprite->fEndX =  fendX;
	outLoadSprite->fStartY = fstartY;
	outLoadSprite->fEndY = fendY;

	outLoadSprite->Texture.fsizeX = fsizeX;
	outLoadSprite->Texture.fsizeY = fsizeY;
	D3DXCreateTextureFromFileA(m_pD3dDevice ,  filePath , &outLoadSprite->Texture.Texture);	
}

void C3DRender::LoadSprite(D3DTEXTURE* loadtexture, 
										   float fstartX, float fendX, 
										   float fstartY, float fendY, 
										   D3DSPRITE* outLoadSprite)
{
	outLoadSprite->fStartX = fstartX;
	outLoadSprite->fEndX =  fendX;
	outLoadSprite->fStartY = fstartY;
	outLoadSprite->fEndY = fendY;

	outLoadSprite->Texture.fsizeX = loadtexture->fsizeX;
	outLoadSprite->Texture.fsizeY = loadtexture->fsizeY;
	outLoadSprite->Texture.Texture = loadtexture->Texture;
}

void C3DRender::DrawSprite(D3DSPRITE* d3dsprite, bool mask,
												 int Sampler,
												 float fPosX, float fPosY, float fPosZ,
												 float foutWidth, float foutHight,
												 int widthCount, int hightCount ,
												 float time,
												 float fRotX, float fRotY, float fRotZ )
{

	float fStartX = d3dsprite->fStartX;
	float fEndX   = d3dsprite->fEndX;
	float fStartY = d3dsprite->fStartY;
	float fEndY   = d3dsprite->fEndY;

	//static int wCount = 0;
	//static int hCount = 0;

	float outStartX = 0;
	float outStartY = 0;
	float outEndX = 0;
	float outEndY = 0;

	int width  = (fEndX- fStartX) / widthCount;
	int hight  = (fEndY - fStartY) / hightCount;
	
	outStartX = fStartX + (width * (d3dsprite->wCount) ) + d3dsprite->wCount+2;
	outEndX = outStartX + width -2;
	outStartY = fStartY + (hight * (d3dsprite->hCount) ) + d3dsprite->hCount+2;
	outEndY = outStartY + hight -2;

	DWORD now = timeGetTime();
	static DWORD old = timeGetTime();
	DWORD tick = now - old;
	static float chk2 =0;

	if(tick + chk2 >= time)
	{
		old = now;
		chk2 = (tick + chk2) - time;
		//wCount < widthCount-1 ? wCount++ : wCount =0;
		if(d3dsprite->wCount < widthCount-1)
		{
			d3dsprite->wCount++;
		}
		else
		{
			d3dsprite->hCount < hightCount-1  ? d3dsprite->hCount++ : d3dsprite->hCount =0;
			d3dsprite->wCount =0;
		}

	}

	DrawEx( &d3dsprite->Texture ,mask , Sampler,fPosX,fPosY ,fPosZ,  foutWidth, foutHight, outStartX, outEndX, outStartY, outEndY ,  fRotX,fRotY,fRotZ );


}

void C3DRender::DrawSprite(D3DSPRITE* d3dsprite, float fPosX, float fPosY, float fPosZ,
												 float foutWidth, float foutHight,
												 int widthCount, int hightCount ,
												 float time,
												 float fRotX, float fRotY, float fRotZ )
{
	DrawSprite(d3dsprite, true,	 0, fPosX, fPosY, fPosZ,foutWidth, foutHight, widthCount,hightCount ,time, fRotX, fRotY, fRotZ );
}

void C3DRender::DrawString(char* outchar, float posX, float posY ,DWORD color)
{
	RECT test = {posX, posY,800,600};
	m_PFont->DrawText(NULL, outchar , -1, &test , DT_LEFT , color);
}


void C3DRender::ShowFPS(DWORD tick)
{
	char outBuffer[255] = {0,}; 	
	//char buffer[255];

	static int FpsCount = 0;
	static int OutFpsCount =0;
	
	DWORD nowTick = tick;
	static DWORD oldTick =0;
	DWORD tempTick =0;

	if(((nowTick -oldTick )+tempTick) <= 1000)
	{
		FpsCount++;
	}else
	{
		tempTick = (oldTick + nowTick) - tempTick;
		oldTick = nowTick;

		OutFpsCount = FpsCount;
		FpsCount = 0;
	}

	sprintf(outBuffer ,"FPS : %d" , OutFpsCount);
	DrawString(outBuffer , 720, 580 , D3DCOLOR_XRGB(255,255,255));
}




DWORD C3DRender::getTime()
{
	DWORD dNowTime = timeGetTime();
	static DWORD dOldTime = timeGetTime();
	DWORD dTick = dNowTime - dOldTime;

	return dTick;
}
