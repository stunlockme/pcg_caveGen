////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_TextureShader = 0;
	m_Bitmap = 0;
	m_Bitmap1 = 0;
	m_Bitmap2 = 0;
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


	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

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
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	
	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if(!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the bitmap object.
	m_Bitmap = new BitmapClass;
	if(!m_Bitmap)
	{
		return false;
	}
	m_Bitmap1 = new BitmapClass;
	if (!m_Bitmap1)
	{
		return false;
	}
	m_Bitmap2 = new BitmapClass;
	if (!m_Bitmap1)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"../Engine/data/seafloor.dds", 8, 8);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	result = m_Bitmap1->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"../Engine/data/square_red.png", 8, 8);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	result = m_Bitmap2->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"../Engine/data/square_black.png", 8, 8);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	m_CaveClass = new CaveClass;
	if (!m_CaveClass)
		return false;
	m_CaveClass->CreateMap();

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the bitmap object.
	if(m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}

	if (m_Bitmap1)
	{
		m_Bitmap1->Shutdown();
		delete m_Bitmap1;
		m_Bitmap1 = 0;
	}

	if (m_CaveClass)
	{
		delete m_CaveClass;
		m_CaveClass = 0;
	}
	// Release the texture shader object.
	if(m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
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

	return;
}


bool GraphicsClass::Frame()
{
	bool result;
	static float rotation = 0.0f;


	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.005f;
	if(rotation > 360.0f)
	{
		rotation -= 360.0f;
	}
	
	// Render the graphics scene.
	result = Render(rotation);
	if(!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::Render(float rotation)
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool result;


	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, projection, and ortho matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	int xt, yt;
	for (int x = 0; x < WIDTH; x++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			if (m_CaveClass->m_map[x][y] == 1)
			{
				result = m_Bitmap->Render(m_D3D->GetDeviceContext(), x * 8, y * 8);
				if (!result)
				{
					return false;
				}
				result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture());
				if (!result)
				{
					return false;
				}
			}
			if (m_CaveClass->m_map[x][y] == 2)
			{
				xt = x;
				yt = y;
				result = m_Bitmap1->Render(m_D3D->GetDeviceContext(), x * 8, y * 8);
				if (!result)
				{
					return false;
				}
				result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap1->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Bitmap1->GetTexture());
				if (!result)
				{
					return false;
				}
			}
		}
	}

	//if(true)
	//{
	//	for (int i = 0; i < 5; i++)
	//	{
	//		xt += 1;
	//		result = m_Bitmap1->Render(m_D3D->GetDeviceContext(), xt * 8, yt * 8);
	//		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap1->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Bitmap1->GetTexture());
	//	}
	//}

	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	//result = m_Bitmap->Render(m_D3D->GetDeviceContext(), 0, 0);
	//if(!result)
	//{
	//	return false;
	//}

	//// Render the bitmap with the texture shader.
	//result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture());
	//if(!result)
	//{
	//	return false;
	//}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}