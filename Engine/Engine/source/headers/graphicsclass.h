////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include <iostream>
#include <vector>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "headers\d3dclass.h"
#include "headers\cameraclass.h"
#include "headers\modelclass.h"
#include "headers\lightshaderclass.h"
#include "headers\lightclass.h"
#include "headers\textureshaderclass.h"
#include "headers\bitmapclass.h"
#include "headers\textclass.h"
#include "headers\skydomeclass.h"
#include "headers\skydomeshaderclass.h"
#include "headers\terrainclass.h"
#include "headers\terrainshaderclass.h"
#include "headers\rendertextureclass.h"
#include "headers\fogshaderclass.h"
#include "headers\reflectionshaderclass.h"
#include "headers\refractionshaderclass.h"
#include "headers\watershaderclass.h"
#include "headers\fireshaderclass.h"
#include "headers\firemodelclass.h"
#include "headers\soundclass.h"
#include "headers\particleshaderclass.h"
#include "headers\particlesystemclass.h"
#include "headers\caveclass.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(float dt, int fps, int cpu, float frameTime, D3DXVECTOR3 camRot, D3DXVECTOR3, int, int, bool, int, int);
private:
	bool Render(float, float, D3DXVECTOR3, int, int, bool, int, int);
	bool RenderSkydome(D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	bool RenderTerrain(D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, float, float);
	bool RenderText(D3DXMATRIX, D3DXMATRIX);
	bool RenderToTexture(float, float, float, int, int, bool, int, int);
	bool RenderScene(float, float, float, int, int, bool, int, int);
	bool RenderRefractionToTexture();
	bool RenderReflectionToTexture(float);

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Model[8];
	FireModelClass* m_FireModel[2];
	LightShaderClass* m_LightShader;
	LightClass* m_Light;
	TextureShaderClass* m_TextureShader;
	BitmapClass* m_Bitmap;
	TextClass* m_Text;
	SkyDomeClass* m_SkyDome;
	SkyDomeShaderClass* m_SkyDomeShader;
	TerrainClass* m_Terrain;
	TerrainShaderClass* m_TerrainShader;
	RenderTextureClass* m_RenderTexture, *m_RefractionTexture, *m_ReflectionTexture;
	FogShaderClass* m_FogShader;
	ReflectionShaderClass* m_ReflectionShader;
	RefractionShaderClass* m_RefractionShader;
	WaterShaderClass* m_WaterShader;
	FireShaderClass* m_FireShader;
	ParticleShaderClass* m_ParticleShader;
	ParticleSystemClass* m_ParticleSystem;
	CaveClass* m_CaveClass;
	//std::vector<FireModelClass*> m_fireModelVec;
private:
	float m_waterHeight, m_waterTranslation;
	int m_lengthModel, m_lengthFireModel;
	bool m_beginCheck;
	bool m_erase;
};

#endif