////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "headers\graphicsclass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_lengthModel = sizeof(m_Model) / sizeof(m_Model[0]);
	for (int i = 0; i < m_lengthModel; i++)
	{
		m_Model[i] = 0;
	}
	m_LightShader = 0;
	m_Light = 0;
	m_TextureShader = 0;
	m_Bitmap = 0;
	m_Text = 0;
	m_SkyDome = 0;
	m_SkyDomeShader = 0;
	m_Terrain = 0;
	m_TerrainShader = 0;
	m_RenderTexture = 0;
	m_FogShader = 0;
	m_ReflectionShader = 0;
	m_RefractionShader = 0;
	m_WaterShader = 0;
	m_lengthFireModel = sizeof(m_FireModel) / sizeof(m_FireModel[0]);
	for (int i = 0; i < m_lengthFireModel; i++)
	{
		m_FireModel[i] = 0;
	}
	m_FireShader = 0;
	m_erase = false;
	m_ParticleShader = 0;
	m_ParticleSystem = 0;
	m_CaveClass = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	D3DXMATRIX baseViewMatrix;

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
		return false;

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
		return false;

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->SetRotation(0.0f, 0.0f, 0.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);
	
	//Initialize ptr to arrays.
	for (int i = 0; i < m_lengthModel; i++)
	{
		m_Model[i] = new ModelClass;
		if (!m_Model[i])
			return false;
	}
	//Initialize model objects.
	result = m_Model[0]->Initialize(m_D3D->GetDevice(), "../Engine/data/sphere.txt", L"../Engine/data/seafloor.dds");
	if (!result)
		return false;
	result = m_Model[1]->Initialize(m_D3D->GetDevice(), "../Engine/data/killer_whale.txt", L"../Engine/data/tree_green.png");
	if (!result)
		return false;
	result = m_Model[2]->Initialize(m_D3D->GetDevice(), "../Engine/data/nature_77.txt", L"../Engine/data/stone_brown.png");
	if (!result)
		return false;
	result = m_Model[3]->Initialize(m_D3D->GetDevice(), "../Engine/data/ground.txt", L"../Engine/data/ground01.dds");
	if (!result)
		return false;
	result = m_Model[4]->Initialize(m_D3D->GetDevice(), "../Engine/data/wall.txt", L"../Engine/data/holidays.png");
	if (!result)
		return false;
	result = m_Model[5]->Initialize(m_D3D->GetDevice(), "../Engine/data/bath.txt", L"../Engine/data/blue_bath.jpg");
	if (!result)
		return false;
	result = m_Model[6]->Initialize(m_D3D->GetDevice(), "../Engine/data/water.txt", L"../Engine/data/water01.dds");
	if (!result)
		return false;
	result = m_Model[7]->Initialize(m_D3D->GetDevice(), "../Engine/data/nature_53.txt", L"../Engine/data/tree_green.png");
	if (!result)
		return false;
	
	for (int i = 0; i < m_lengthFireModel; i++)
	{
		//create the fire model object
		m_FireModel[i] = new FireModelClass;
		if (!m_FireModel)
			return false;
	}
	//initialize fire model object
	result = m_FireModel[0]->Initialize(m_D3D->GetDevice(), "../Engine/data/sphere.txt", L"../Engine/data/fire01.dds",
		L"../Engine/data/noise01.dds", L"../Engine/data/alpha01.dds");
	if (!result)
		return false;
	result = m_FireModel[1]->Initialize(m_D3D->GetDevice(), "../Engine/data/nature_flat_1.txt", L"../Engine/data/fire01.dds",
		L"../Engine/data/noise01.dds", L"../Engine/data/alpha01.dds");
	if (!result)
		return false;

	//create the fire shader object
	m_FireShader = new FireShaderClass;
	if (!m_FireShader)
		return false;
	//initialize fire shader object
	result = m_FireShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
		return false;

	// create the texture shader object.
	m_TextureShader = new TextureShaderClass();
	if (!m_TextureShader)
		return false;

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the bitmap object.
	m_Bitmap = new BitmapClass;
	if (!m_Bitmap)
		return false;

	// Initialize the bitmap object.
	result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"../Engine/data/mouse.dds", 32, 32);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Initialize that the user has not clicked on the screen to try an intersection test yet.
	m_beginCheck = false;

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if(!m_LightShader)
		return false;

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	//create the sky dome object.
	m_SkyDome = new SkyDomeClass;
	if (!m_SkyDome)
		return false;

	//Initialize the sky dome object.
	result = m_SkyDome->Initialize(m_D3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky dome object.", L"Error", MB_OK);
		return false;
	}

	//create the sky dome shader object.
	m_SkyDomeShader = new SkyDomeShaderClass;
	if (!m_SkyDomeShader)
		return false;
	
	//Initialize the sky dome shader object.
	result = m_SkyDomeShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky dome object.", L"Error", MB_OK);
		return false;
	}

	// Create the terrain object.
	m_Terrain = new TerrainClass;
	if (!m_Terrain)
	{
		return false;
	}

	// Initialize the terrain object.
	result = m_Terrain->Initialize(m_D3D->GetDevice(), "../Engine/data/heightmap01.bmp", L"../Engine/data/toon_floor.jpg");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
		return false;
	}

	// Create the terrain shader object.
	m_TerrainShader = new TerrainShaderClass;
	if (!m_TerrainShader)
	{
		return false;
	}

	// Initialize the terrain shader object.
	result = m_TerrainShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain shader object.", L"Error", MB_OK);
		return false;
	}

	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// Create the render to texture object.
	m_RenderTexture = new RenderTextureClass;
	if (!m_RenderTexture)
	{
		return false;
	}

	// Initialize the render to texture object.
	result = m_RenderTexture->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		return false;
	}

	// Create the reflection shader object.
	m_ReflectionShader = new ReflectionShaderClass;
	if (!m_ReflectionShader)
		return false;

	// Initialize the reflection shader object.
	result = m_ReflectionShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the reflection shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the refraction render to texture object.
	m_RefractionTexture = new RenderTextureClass;
	if (!m_RefractionTexture)
	{
		return false;
	}

	// Initialize the refraction render to texture object.
	result = m_RefractionTexture->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the refraction render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the reflection render to texture object.
	m_ReflectionTexture = new RenderTextureClass;
	if (!m_ReflectionTexture)
	{
		return false;
	}

	// Initialize the reflection render to texture object.
	result = m_ReflectionTexture->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the reflection render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the refraction shader object.
	m_RefractionShader = new RefractionShaderClass;
	if (!m_RefractionShader)
	{
		return false;
	}

	// Initialize the refraction shader object.
	result = m_RefractionShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the refraction shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the water shader object.
	m_WaterShader = new WaterShaderClass;
	if (!m_WaterShader)
	{
		return false;
	}

	// Initialize the water shader object.
	result = m_WaterShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the water shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the fog shader object.
	m_FogShader = new FogShaderClass;
	if (!m_FogShader)
		return false;

	// Initialize the fog shader object.
	result = m_FogShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the fog shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the particle shader object.
	m_ParticleShader = new ParticleShaderClass;
	if (!m_ParticleShader)
	{
		return false;
	}

	// Initialize the particle shader object.
	result = m_ParticleShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the particle shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the particle system object.
	m_ParticleSystem = new ParticleSystemClass;
	if (!m_ParticleSystem)
	{
		return false;
	}

	// Initialize the particle system object.
	result = m_ParticleSystem->Initialize(m_D3D->GetDevice(), L"../Engine/data/star.dds");
	if (!result)
	{
		return false;
	}

	//create the cave object
	m_CaveClass = new CaveClass();
	if (!m_CaveClass)
		return false;
	m_CaveClass->CreateMap();

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.05f, 0.05f, 0.05f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(1.0f, -1.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	// Set the height of the water.
	m_waterHeight = 2.75f;

	// Initialize the position of the water.
	m_waterTranslation = 0.0f;

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the particle system object.
	if (m_ParticleSystem)
	{
		m_ParticleSystem->Shutdown();
		delete m_ParticleSystem;
		m_ParticleSystem = 0;
	}

	// Release the particle shader object.
	if (m_ParticleShader)
	{
		m_ParticleShader->Shutdown();
		delete m_ParticleShader;
		m_ParticleShader = 0;
	}

	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the reflection shader object.
	if (m_ReflectionShader)
	{
		m_ReflectionShader->Shutdown();
		delete m_ReflectionShader;
		m_ReflectionShader = 0;
	}

	// Release the water shader object.
	if (m_WaterShader)
	{
		m_WaterShader->Shutdown();
		delete m_WaterShader;
		m_WaterShader = 0;
	}

	// Release the refraction shader object.
	if (m_RefractionShader)
	{
		m_RefractionShader->Shutdown();
		delete m_RefractionShader;
		m_RefractionShader = 0;
	}


	// Release the reflection render to texture object.
	if (m_ReflectionTexture)
	{
		m_ReflectionTexture->Shutdown();
		delete m_ReflectionTexture;
		m_ReflectionTexture = 0;
	}

	// Release the refraction render to texture object.
	if (m_RefractionTexture)
	{
		m_RefractionTexture->Shutdown();
		delete m_RefractionTexture;
		m_RefractionTexture = 0;
	}

	// Release the fog shader object.
	if (m_FogShader)
	{
		m_FogShader->Shutdown();
		delete m_FogShader;
		m_FogShader = 0;
	}

	// Release the terrain shader object.
	if (m_TerrainShader)
	{
		m_TerrainShader->Shutdown();
		delete m_TerrainShader;
		m_TerrainShader = 0;
	}

	// Release the sky dome shader object.
	if (m_SkyDomeShader)
	{
		m_SkyDomeShader->Shutdown();
		delete m_SkyDomeShader;
		m_SkyDomeShader = 0;
	}

	// Release the sky dome object.
	if (m_SkyDome)
	{
		m_SkyDome->Shutdown();
		delete m_SkyDome;
		m_SkyDome = 0;
	}

	// Release the render to texture object.
	if (m_RenderTexture)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the text object.
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// Release the bitmap object.
	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}


	// Release the light shader object.
	if(m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the model object.
	for (int i = 0; i < m_lengthModel; i++)
	{
		if (m_Model[i])
		{
			m_Model[i]->Shutdown();
			delete m_Model[i];
			m_Model[i] = 0;
		}
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	// Release the terrain object.
	if (m_Terrain)
	{
		m_Terrain->Shutdown();
		delete m_Terrain;
		m_Terrain = 0;
	}

	// Release the fire shader object.
	if (m_FireShader)
	{
		m_FireShader->Shutdown();
		delete m_FireShader;
		m_FireShader = 0;
	}

	for (int i = 0; i < m_lengthFireModel; i++)
	{
		//release the fire model
		if (m_FireModel[i])
		{
			m_FireModel[i]->Shutdown();
			delete m_FireModel[i];
			m_FireModel[i] = 0;
		}
	}
	return;
}


bool GraphicsClass::Frame(float dt, int fps, int cpu, float frameTime, D3DXVECTOR3 camRot, D3DXVECTOR3 camPos, int screenWidth, int screenHeight, bool keyDown, int mouseX, int mouseY)
{
	bool result;
	static float rotation = 0.0f;
	static float delta = 0.0f;


	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.01f;
	if(rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// Update the delta variable each frame. (keep this between 0 and 1)
	delta += 0.001f;
	if(delta >1.0f)
	{
		delta -=1.0f;
	}

	// Set the frames per second.
	result = m_Text->SetFps(fps, m_D3D->GetDeviceContext());
	if (!result)
		return false;

	// Set the cpu usage.
	result = m_Text->SetCpu(cpu, m_D3D->GetDeviceContext());
	if (!result)
		return false;

	// Update the position of the water to simulate motion.
	m_waterTranslation += 0.001f;
	if (m_waterTranslation > 1.0f)
	{
		m_waterTranslation -= 1.0f;
	}

	//Set the rotation of the camera.
	m_Camera->SetRotation(camRot.x, camRot.y, camRot.z);

	//set the position of the camera.
	m_Camera->SetPosition(camPos.x, camPos.y, camPos.z);

	// Run the frame processing for the particle system.
	m_ParticleSystem->Frame(frameTime, m_D3D->GetDeviceContext());
	
	// Render the graphics scene.
	result = Render(rotation, delta, camPos, screenWidth, screenHeight, keyDown, mouseX, mouseY);
	if(!result)
		return false;

	return true;
}


bool GraphicsClass::Render(float rotation, float deltavalue, D3DXVECTOR3 camPos, int screenWidth, int screenHeight, bool keyDown, int mouseX, int mouseY)
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix, scalingMatrix, translationMatrix;
	bool result;
	D3DXVECTOR3 camRot;
	D3DXVECTOR4 color;
	float fogColor, fogStart, fogEnd;

	// Set the color of the fog to grey.
	fogColor = 0.5f;

	// Set the start and end of the fog.
	fogStart = 0.0f;
	fogEnd = 50.0f;

	// Render the entire scene to the texture first.
	result = RenderToTexture(deltavalue, fogStart, fogEnd, screenWidth, screenHeight, keyDown, mouseX, mouseY);
	if (!result)
		return false;
	// Render the refraction of the scene to a texture.
	result = RenderRefractionToTexture();
	if (!result)
		return false;

	// Render the reflection of the scene to a texture.
	result = RenderReflectionToTexture(deltavalue);
	if (!result)
		return false;


	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(fogColor, fogColor, fogColor, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	//Turn off the Z buffer to draw the skydome.
	m_D3D->TurnZBufferOff();
	//Turn off the back face culling.
	m_D3D->TurnOffCulling();
	//Translate the sky dome to be centered around the camera position.
	D3DXMatrixTranslation(&worldMatrix, camPos.x, camPos.y, camPos.z);
	result = RenderSkydome(worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
		return false;
	// Render the scene as normal to the back buffer.
	/*result = RenderScene(deltavalue, fogStart, fogEnd, screenWidth, screenHeight, keyDown, mouseX, mouseY);
	if (!result)
		return false;*/

	m_D3D->TurnOffCulling();
	m_D3D->TurnZBufferOn();
	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

bool GraphicsClass::RenderSkydome(D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	m_SkyDome->Render(m_D3D->GetDeviceContext());

	if (m_SkyDomeShader->Render(m_D3D->GetDeviceContext(), m_SkyDome->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_SkyDome->GetApexColor(), m_SkyDome->GetCenterColor()))
		return true;

	return false;
}

bool GraphicsClass::RenderTerrain(D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, float fogStart, float fogEnd)
{
	//D3DXMATRIX scalingMatrix;
	//m_D3D->GetWorldMatrix(worldMatrix);
	//D3DXMatrixScaling(&scalingMatrix, 0.1f, 1.0f, 0.1f);
	//D3DXMatrixTranslation(&worldMatrix, 0.0f, 1.0f, 0.0f);
	//D3DXMatrixMultiply(&worldMatrix, &scalingMatrix, &worldMatrix);
	m_Terrain->Render(m_D3D->GetDeviceContext());

	/*if (m_TerrainShader->Render(m_D3D->GetDeviceContext(), m_Terrain->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_Terrain->GetTexture()))
		return true;*/

	if (m_FogShader->Render(m_D3D->GetDeviceContext(), m_Terrain->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Terrain->GetTexture(), fogStart, fogEnd))
		return true;

	return false;
}

bool GraphicsClass::RenderText(D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix)
{
	if (m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix))
		return true;

	return false;
}

bool GraphicsClass::RenderToTexture(float deltavalue, float fogStart, float fogEnd, int screenWidth, int screenHeight, bool keyDown, int mouseX, int mouseY)
{
	bool result;
	D3DXMATRIX reflectionViewMatrix, worldMatrix, projectionMatrix;

	// Set the render target to be the render to texture.
	m_RenderTexture->SetRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView());

	// Clear the render to texture.
	m_RenderTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView(), 0.0f, 0.0f, 1.0f, 1.0f);

	// Use the camera to calculate the reflection matrix.
	m_Camera->RenderReflection(0.5f);

	// Get the camera reflection view matrix instead of the normal view matrix.
	reflectionViewMatrix = m_Camera->GetReflectionViewMatrix();

	// Render the scene now and it will draw to the render to texture instead of the back buffer.
	result = RenderScene(deltavalue, fogStart, fogEnd, screenWidth, screenHeight, keyDown, mouseX, mouseY);
	if (!result)
		return false;

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_D3D->SetBackBufferRenderTarget();

	return true;
}

bool GraphicsClass::RenderScene(float deltavalue, float fogStart, float fogEnd, int screenWidth, int screenHeight, bool keyDown, int mouseX, int mouseY)
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, translationMatrix, scalingMatrix[4], orthoMatrix, reflectionMatrix;
	bool result;
	D3DXVECTOR4 color;
	D3DXVECTOR3 camRot, camPos, scrollSpeeds, scales, modelPos;
	D3DXVECTOR2 distortion1, distortion2, distortion3;
	float distortionScale, distortionBias;
	static float frameTime = 0.0f;

	//increment the frame time counter
	frameTime += 0.01f;
	if (frameTime > 1000.0f)
		frameTime = 0.0f;

	//set the three scrolling speeds for the three different noise textures
	scrollSpeeds = D3DXVECTOR3(1.3f, 2.1f, 2.3f);

	//set the three scales which will be used to create the three different noise octave textures
	scales = D3DXVECTOR3(1.0f, 2.0f, 3.0f);

	//set the three different x and y distortion factors for the three different noise textures
	distortion1 = D3DXVECTOR2(0.1f, 0.2f);
	distortion2 = D3DXVECTOR2(0.1f, 0.3f);
	distortion3 = D3DXVECTOR2(0.1f, 0.1f);

	//the scale and bias of the texture coordinate sampling perturbation
	distortionScale = 0.8f;
	distortionBias = 0.5f;
	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	m_D3D->TurnOnCulling();
	m_D3D->TurnZBufferOn();
	result = RenderTerrain(worldMatrix, viewMatrix, projectionMatrix, fogStart, fogEnd);
	if (!result)
		return false;

	m_D3D->GetWorldMatrix(worldMatrix);
	D3DXMatrixTranslation(&worldMatrix, 30.0f, 6.0f, 28.0f);
	//wall object
	m_Model[4]->Render(m_D3D->GetDeviceContext());
	//render the wall object using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[4]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(),
		m_Light->GetSpecularPower(), deltavalue, m_Model[4]->GetTexture());
	if (!result)
		return false;

	m_D3D->GetWorldMatrix(worldMatrix);
	D3DXMatrixTranslation(&worldMatrix, 30.0f, 2.0f, 20.0f);
	//bath object
	m_Model[5]->Render(m_D3D->GetDeviceContext());
	//render the bath object using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[5]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(),
		m_Light->GetSpecularPower(), deltavalue, m_Model[5]->GetTexture());
	if (!result)
		return false;

	m_D3D->GetWorldMatrix(worldMatrix);
	D3DXMatrixTranslation(&worldMatrix, 28.0f, 2.0f, 20.0f);
	//fish object
	m_Model[1]->Render(m_D3D->GetDeviceContext());
	// Render the fish model using the refraction shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[1]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(),
		m_Light->GetSpecularPower(), deltavalue, m_Model[1]->GetTexture());
	if (!result)
		return false;

	m_D3D->GetWorldMatrix(worldMatrix);
	D3DXMatrixTranslation(&worldMatrix, 30.0f, m_waterHeight, 20.0f);
	//water object
	m_Model[6]->Render(m_D3D->GetDeviceContext());
	//render the water model using the water shader.
	result = m_WaterShader->Render(m_D3D->GetDeviceContext(), m_Model[6]->GetIndexCount(), worldMatrix, viewMatrix,
		projectionMatrix, reflectionMatrix, m_ReflectionTexture->GetShaderResourceView(),
		m_RefractionTexture->GetShaderResourceView(), m_Model[6]->GetTexture(),
		m_waterTranslation, 0.01f);
	if (!result)
		return false;

	m_D3D->GetWorldMatrix(worldMatrix);
	D3DXMatrixTranslation(&worldMatrix, 22.0f, 1.0f, 20.0f);
	//tree
	m_Model[7]->Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[7]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(),
		m_Light->GetSpecularPower(), deltavalue, m_Model[7]->GetTexture());
	if (!result)
		return false;

	m_D3D->GetWorldMatrix(worldMatrix);
	D3DXMatrixTranslation(&worldMatrix, 18.0f, 1.0f, 20.0f);
	D3DXMatrixScaling(&scalingMatrix[2], 2.0f, 2.0f, 2.0f);
	D3DXMatrixMultiply(&worldMatrix, &scalingMatrix[2], &worldMatrix);
	//stone
	m_Model[2]->Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[2]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(),
		m_Light->GetSpecularPower(), deltavalue, m_Model[2]->GetTexture());
	if (!result)
		return false;
	
	//fire models
	m_D3D->TurnOnAlphaBlending();
	m_D3D->GetWorldMatrix(worldMatrix);
	D3DXMatrixTranslation(&worldMatrix, 20.0f, 2.0f, 21.0f);
	D3DXMatrixScaling(&scalingMatrix[1], 0.5f, 0.5f, 0.5f);
	D3DXMatrixMultiply(&worldMatrix, &scalingMatrix[1], &worldMatrix);
	//model 1
	m_FireModel[0]->Render(m_D3D->GetDeviceContext());
	result = m_FireShader->Render(m_D3D->GetDeviceContext(), m_FireModel[0]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_FireModel[0]->GetTexture1(), m_FireModel[0]->GetTexture2(), m_FireModel[0]->GetTexture3(), frameTime, scrollSpeeds,
		scales, distortion1, distortion2, distortion3, distortionScale, distortionBias);
	if (!result)
		return false;

	m_D3D->TurnOffAlphaBlending();

	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->TurnZBufferOff();
	m_D3D->TurnOnAlphaBlending();
	//render the mouse cursor with the texture shader
	result = m_Bitmap->Render(m_D3D->GetDeviceContext(), mouseX, mouseY, camRot);
	//camPos = m_Camera->GetPosition();
	//result = m_DebugWindow->Render(m_D3D->GetDeviceContext(), mouseX, mouseY, camPos.z);
	if (!result)
		return false;
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), 
								worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture());

	m_D3D->GetWorldMatrix(worldMatrix);
	D3DXMatrixTranslation(&worldMatrix, 23.0f, 10.0f, 22.0f);
	// Put the particle system vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_ParticleSystem->Render(m_D3D->GetDeviceContext());

	// Render the model using the texture shader.
	result = m_ParticleShader->Render(m_D3D->GetDeviceContext(), m_ParticleSystem->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_ParticleSystem->GetTexture());
	if (!result)
		return false;

	result = RenderText(worldMatrix, orthoMatrix);
	
	m_D3D->TurnOffAlphaBlending();
	return true;
}

bool GraphicsClass::RenderRefractionToTexture()
{
	D3DXVECTOR4 clipPlane;
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	// Setup a clipping plane based on the height of the water to clip everything above it.
	clipPlane = D3DXVECTOR4(0.0f, -1.0f, 0.0f, m_waterHeight + 0.1f);

	// Set the render target to be the refraction render to texture.
	m_RefractionTexture->SetRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView());

	// Clear the refraction render to texture.
	m_RefractionTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Translate to where the bath model will be rendered.
	D3DXMatrixTranslation(&worldMatrix, 30.0f, 2.0f, 20.0f);
	//bath object.
	m_Model[5]->Render(m_D3D->GetDeviceContext());

	// Render the bath model using the refraction shader.
	result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_Model[5]->GetIndexCount(), worldMatrix, viewMatrix,
		projectionMatrix, m_Model[5]->GetTexture(), m_Light->GetDirection(),
		m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), clipPlane);
	if (!result)
	//	return false;

	m_D3D->GetWorldMatrix(worldMatrix);
	D3DXMatrixTranslation(&worldMatrix, 28.0f, 2.0f, 20.0f);
	//whale object
	m_Model[1]->Render(m_D3D->GetDeviceContext());
	// Render the frog model using the refraction shader.
	result = m_RefractionShader->Render(m_D3D->GetDeviceContext(), m_Model[1]->GetIndexCount(), worldMatrix, viewMatrix,
		projectionMatrix, m_Model[1]->GetTexture(), m_Light->GetDirection(),
		m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), clipPlane);
	if (!result)
		return false;

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_D3D->SetBackBufferRenderTarget();

	return true;
}

bool GraphicsClass::RenderReflectionToTexture(float deltavalue)
{
	D3DXMATRIX reflectionViewMatrix, worldMatrix, projectionMatrix;
	bool result;
	D3DXVECTOR3 cameraPos;

	// Set the render target to be the reflection render to texture.
	m_ReflectionTexture->SetRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView());

	// Clear the reflection render to texture.
	m_ReflectionTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Use the camera to render the reflection and create a reflection view matrix.
	m_Camera->RenderReflection(m_waterHeight);

	// Get the camera reflection view matrix instead of the normal view matrix.
	reflectionViewMatrix = m_Camera->GetReflectionViewMatrix();

	// Get the world and projection matrices from the d3d object.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Translate to where the wall model will be rendered.
	D3DXMatrixTranslation(&worldMatrix, 30.0f, 6.0f, 28.0f);
	//wall object
	m_Model[4]->Render(m_D3D->GetDeviceContext());

	//render the wall object using the light shader and the reflection view matrix.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[4]->GetIndexCount(), worldMatrix, reflectionViewMatrix, projectionMatrix,
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(),
		m_Light->GetSpecularPower(), deltavalue, m_Model[4]->GetTexture());
	if (!result)
		return false; 

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_D3D->SetBackBufferRenderTarget();

	return true;
}