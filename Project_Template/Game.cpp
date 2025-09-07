#include "Game.h"

#include "utils.h"
#include "TimeClass.h"
#include "Texture.h"
#include "OrientationManager.h"
#include "SandboxBattler.h" // Project

#include <iostream>
#include <cmath>
#include <print>


Game::Game(const Window& window)
	:BaseGame(window)
	,m_UnitManager{}
	,m_Window{ window }
	,m_RenderedFrames{0}
	
{
	Initialize();
}

Game::~Game()
{
	Cleanup();
}

void Game::Initialize()
{
	// TESTING ATLAS SPRITE LOADING
	// LOAD ALL
	m_AtlasManager.LoadFolder("../Resources/DuelystResc/units");
	for (int i = 0; i < m_AtlasManager.m_Atlases.size(); ++i)
	{
		std::cout << i << ": " << m_AtlasManager.m_Atlases[i].name
			<< " (" << m_AtlasManager.m_Atlases[i].frames.size() << " frames)\n";
	}
	std::cout << "\n \n";


	// Get specific one
	atlas = m_AtlasManager.GetAtlas("neutral_zurael"); // replace with an actual atlas name

	if (!atlas)
	{
		std::cerr << "Atlas not found!\n";
		return;
	}

	std::cout << "Frames in atlas " << atlas->name << ": \n";
	for (const auto& pair : atlas->frames)
	{
		const FrameData& f = pair.second;
		std::cout << pair.first << " -> x:" << f.x << " y:" << f.y
			<< " w:" << f.w << " h:" << f.h << "\n";
	}
	m_AtlasTestTexture = std::make_unique<Texture>(atlas->pngPath);
	f = atlas->frames.at("neutral_zurael_attack_011.png");

	///////////////////////////////////
	
	// UnitManager
	int unitAmount{1};
	for (int i{0}; i < unitAmount; ++i)
	{
		m_UnitManager.AddUnit(std::make_unique<Unit>(UnitType{},Point2f{50.0f, 50.0f + 25.0f * i},Stats{},5.0f));
	}
	// FOR TESTING PURPOSED FIX OR DELETE
	m_TestTexture = std::make_unique<Texture>("../Resources/Images/PixelBackPackGuy.png");

	m_TestSkeleton = std::make_unique<Texture>("../Resources/Images/SkeletonArcher-Sheet.png");
	// Default
	m_SandboxBattler = std::make_unique<SandboxBattler>(); // Project
	m_FPSCounter = std::make_unique<Texture>("placeHolder","../Resources/Fonts/consola.ttf",16,Color4f{1,1,1,1});
	m_TargetFPS = 60.0f;
	m_Time = std::make_unique<TimeClass>(0.0f);
	m_AccumulatedTime = std::make_unique<TimeClass>(0.0f);
	m_TimeState = std::make_unique<Texture>("Paused","../Resources/Fonts/consola.ttf",24,Color4f{1,1,1,1});
	OrientationManager::UpdateCamera(GetViewPort());
	// Default
}

void Game::Cleanup()
{
	
}

void Game::Update(float elapsedSec)
{
	// Check keyboard state
	using namespace std;

	// Camera code
	float maxCameraSpeedDivider{1.5f};
	float minCameraSpeedDivider{0.5f};
	float cameraSpeedDivider{1.0f};

	Point2f mousePosDiff{m_LastMousePos-m_PreviousMousePos};
	m_PreviousMousePos = m_LastMousePos;

	if (m_AltHeld && m_LeftClickHeld)
	{
		m_CameraPos.left -= (mousePosDiff.x / cameraSpeedDivider);
		m_CameraPos.bottom -= (mousePosDiff.y/ cameraSpeedDivider);
		
	}

	OrientationManager::UpdateCamera(m_CameraPos);
	// FIX PUT IN SEPARATE CLASS

	m_Time->Update(elapsedSec);
	m_AccumulatedTime->Update(elapsedSec); // void UpdateTimers(float elapsedSec)????

	const float frameDuration{1.0f / m_TargetFPS};
	// Limits the frequency of code inside   // Put this into a separate function?
	if (m_AccumulatedTime->GetTime() >= frameDuration)
	{

		m_UnitManager.UpdateAll(elapsedSec);
		m_SandboxBattler->Update(elapsedSec);
		m_AccumulatedTime->Restart();
		UpdateTextures();
	}

	m_RenderedFrames += 1;
}
void Game::UpdateTextures()
{
	int m_FPS = int((float)m_RenderedFrames / m_Time->GetTime());
	m_FPSCounter = std::make_unique<Texture>("FPS: " + std::to_string(m_FPS),"../Resources/Fonts/consola.ttf",16,Color4f{0,0,0,1}); // Has to be better

	UpdatePausedText();
	m_SandboxBattler->UpdateTextures();
}
void Game::UpdatePausedText()
{
	using namespace PrettyColors;
	int ptSize{24};
	Color4f color{GetColor(black)};
	const char* path{"../Resources/Fonts/consola.ttf"};
	const char* text{"No state"};
	if (m_Paused == true)
	{
		text = "Paused";
	}
	else
	{
		text = "Going";
	}
	m_TimeState = std::make_unique<Texture>(text,path,ptSize,color);
}

void Game::PushCameraMatrix() const
{

	glTranslatef(-m_CameraPos.left,-m_CameraPos.bottom,0);
	
}

// Example variables (put these outside the loop, so they persist)
static float lastTime = 0.0f;
static float elapsed = 0.0f;
static int frame = 1;
const int frameCount = 11;     // how many frames in your sprite sheet
const float frameTime = 0.10f; // seconds per frame

void Game::Draw() const
{
	ClearBackground();
	glPushMatrix();
	{
		PushCameraMatrix();
		using namespace PrettyColors;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		glPushMatrix();
		glScalef(3.0f,3.0f,1.0f);
		m_AtlasTestTexture->DrawShade(Rectf{50.0f,50.0f,120.0f,120.0f},f.ToRectf(),{});
		//m_AtlasTestTexture->DrawShade(Rectf{50.0f,50.0f,120.0f,120.0f},Rectf{(f.x), 241.0f, f.w, f.h},{});
		glPopMatrix();
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		float now = m_Time->GetTime();  
		float dt = now - lastTime;      
		lastTime = now;

		elapsed += dt;
		if (elapsed >= frameTime)
		{
			elapsed = 0.0f;
			frame++;
			if (frame >= frameCount) frame = 1; // loop back, skip frame 0
		}

		// VARIABLES ABOVE ARE TO BE REMOVED TOO
		// Each frame is 32px wide in your sheet
		float srcX = frame * 32.0f;
		Rectf srcRect{srcX, 0.0f, 32.0f, 32.0f};
		Rectf dstRect{50.0f, 200.0f, 128.0f, 128.0f};

		auto rgb = [](int r,int g,int b) { return Color4f(RGBtoColor4f(r,g,b)); };
		auto col = [](auto c) { return Color4f(GetColor(c)); };

		// 67 148 48

		//m_TestSkeleton->DrawShade(dstRect,srcRect,{{rgb(67, 148, 48), col(yellow)}});
		//dstRect.left += 100.0f;
		//m_TestSkeleton->DrawShade(dstRect,srcRect,{{rgb(67, 148, 48), col(orange)}});
		//dstRect.left += 100.0f;
		//m_TestSkeleton->DrawShade(dstRect,srcRect,{{rgb(67, 148, 48), col(blue)}});
		//dstRect.left += 100.0f;
		//m_TestSkeleton->DrawShade(dstRect,srcRect,{{rgb(67, 148, 48), col(white)}});
		//dstRect.left += 100.0f;
		//m_TestSkeleton->DrawShade(dstRect,srcRect,{{rgb(67, 148, 48), col(dGreen)}});
		//dstRect.left += 100.0f;
		//m_TestSkeleton->DrawShade(dstRect,srcRect,{{rgb(67, 148, 48), col(black)}});
		//dstRect.left += 100.0f;
		//m_TestSkeleton->DrawShade(dstRect,srcRect,{{rgb(67, 148, 48), col(rose)}});
		//dstRect.left += 100.0f;
		//m_TestSkeleton->DrawShade(dstRect,srcRect,{{rgb(67, 148, 48), col(purple)}});
		//dstRect.left += 100.0f;
		//m_TestSkeleton->DrawShade(dstRect,srcRect,{{rgb(67, 148, 48), col(brown)}});


		// m_TestSkeleton->DrawShade(Rectf{200.0f,200.0f,128.0f,128.0f},Rectf{0.0f,0.0f,32.0f,32.0f},{});


	/*	m_TestTexture->DrawShade(Rectf{0.0f  ,600.0f,200.0f,200.0f},{},{{Color4f(RGBtoColor4f(89, 77,62)), Color4f(GetColor(purple))} ,{Color4f(1,1,1,1), Color4f(GetColor(black))}
			,{Color4f(GetColor(black)),Color4f(GetColor(white))},{Color4f(RGBtoColor4f(129,65,64)),Color4f(GetColor(black))},{Color4f(RGBtoColor4f(230,206,93)),Color4f(GetColor(purple))},{Color4f(RGBtoColor4f(190,191,190)),Color4f(GetColor(black))}});

		m_TestTexture->DrawShade(Rectf{200.0f,600.0f,200.0f,200.0f},{},{{Color4f(RGBtoColor4f(89, 77,62)), Color4f(GetColor(black))} ,{Color4f(1,1,1,1), Color4f(GetColor(black))}
			,{Color4f(GetColor(black)),Color4f(GetColor(rose))}   ,{Color4f(RGBtoColor4f(129,65,64)),Color4f(GetColor(dRed))} ,{Color4f(RGBtoColor4f(230,206,93)),Color4f(GetColor(dRed))},{Color4f(RGBtoColor4f(190,191,190)),Color4f(GetColor(dRed))}});

		m_TestTexture->DrawShade(Rectf{400.0f,600.0f,200.0f,200.0f},{},{{Color4f(RGBtoColor4f(89, 77,62)), Color4f(GetColor(orange))} ,{Color4f(1,1,1,1), Color4f(GetColor(orange))}
			,{Color4f(GetColor(black)),Color4f(GetColor(black))}  ,{Color4f(RGBtoColor4f(129,65,64)),Color4f(GetColor(cobalt))} ,{Color4f(RGBtoColor4f(230,206,93)),Color4f(GetColor(dBlue))},{Color4f(RGBtoColor4f(190,191,190)),Color4f(GetColor(black))}});

		m_TestTexture->DrawShade(Rectf{600.0f,600.0f,200.0f,200.0f},{},{{Color4f(RGBtoColor4f(89, 77,62)), Color4f(GetColor(black))} ,{Color4f(1,1,1,1), Color4f(GetColor(yellow))}
			,{Color4f(GetColor(black)),Color4f(GetColor(black))} ,{Color4f(RGBtoColor4f(129,65,64)),Color4f(GetColor(lBlue))},{Color4f(RGBtoColor4f(230,206,93)),Color4f(GetColor(lBlue))},{Color4f(RGBtoColor4f(190,191,190)),Color4f(GetColor(lBlue))}});

		m_TestTexture->DrawShade(Rectf{800.0f,600.0f,200.0f,200.0f},{},{{Color4f(RGBtoColor4f(89, 77,62)), Color4f(GetColor(black))} ,{Color4f(1,1,1,1), Color4f(GetColor(dRed))}
			,{Color4f(GetColor(black)),Color4f(GetColor(dRed))},{Color4f(RGBtoColor4f(129,65,64)),Color4f(GetColor(white))},{Color4f(RGBtoColor4f(230,206,93)),Color4f(GetColor(gray))},{Color4f(RGBtoColor4f(190,191,190)),Color4f(GetColor(black))}});*/

		// Use this in the future, make this more reusable 
	/*	auto rgb = [](int r,int g,int b) { return Color4f(RGBtoColor4f(r,g,b)); };
		auto col = [](auto c) { return Color4f(GetColor(c)); };

		m_TestTexture->DrawShade(Rectf{0.0f,400.0f,200.0f,200.0f},{},
		{
			{ rgb(89, 77, 62), col(white) },{ Color4f(1,1,1,1), col(brown) }, { col(black), col(black) },
			{ rgb(129,65,64), col(brown) },{ rgb(230,206,93), col(dGreen) },{ rgb(190,191,190), col(brown) }
		}
		);*/


		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		m_UnitManager.DrawAll();
		m_SandboxBattler->Draw();

		/*utils::SetColor(PrettyColors::GetColor(PrettyColors::rose));
		utils::FillRect(100.0f,100.0f,10.0f,10.0f); DEBUG*/
	}
	glPopMatrix();

	DrawUI(); // FIX VVV
	m_SandboxBattler->DrawUI(GetViewPort());
	m_FPSCounter->Draw(Point2f{10.0f, GetViewPort().height - m_FPSCounter->GetHeight() - 4.0f},Rectf{});

}

void Game::DrawUI() const
{
	DrawPausedText();
}
void Game::DrawPausedText() const
{
	Rectf viewport = GetViewPort();
	float edgeOffset{20.0f};
	Rectf drawDst{viewport.width - m_TimeState->GetWidth() - edgeOffset
	,viewport.bottom + edgeOffset
	,m_TimeState->GetWidth()
	,m_TimeState->GetHeight()};
	m_TimeState->Draw(drawDst);
}

void Game::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{
	// It's kind of an overkill to check all letters everytime when I am using like 20% of them
	// Also makes it so the switch is annoying to search through // FIX ? (in keyUp too)
	switch (e.keysym.sym)
	{
	case SDLK_q:

		break;
	case SDLK_w:

		break;
	case SDLK_e:

		break;
	case SDLK_r:

		break;
	case SDLK_t:

		break;
	case SDLK_y:

		break;
	case SDLK_u:
		m_UnitManager.DeSellectAll();
		break;
	case SDLK_i:

		break;
	case SDLK_o:
		Start();
		break;
	case SDLK_p:
		Pause();
		break;
	case SDLK_a:

		break;
	case SDLK_s:

		break;
	case SDLK_d:

		break;
	case SDLK_f:

		break;
	case SDLK_g:

		break;
	case SDLK_h:

		break;
	case SDLK_j:

		break;
	case SDLK_k:

		break;
	case SDLK_l:

		break;
	case SDLK_z:

		break;
	case SDLK_x:

		break;
	case SDLK_c:

		break;
	case SDLK_v:

		break;
	case SDLK_b:

		break;
	case SDLK_n:

		break;
	case SDLK_m:

		break;
	case SDLK_COMMA:
		m_TargetFPS -= 5.0f;
		if (m_TargetFPS <= 0.0f)
		{
			m_TargetFPS = 5.0f;
		}
		break;
	case SDLK_PERIOD:
		m_TargetFPS += 5.0f;
		if (m_TargetFPS >= 120.0f)
		{
			m_TargetFPS = 120.0f;
		}
	case SDLK_LALT:
		m_AltHeld = true;
		break;
	case SDLK_LSHIFT:
		m_UnitManager.EnableQueuing();
		break;
	case SDLK_ESCAPE:
		// Not sure about this, but works ¯\_(^^)_/¯
		SDL_Event e{SDL_QUIT};
		SDL_PushEvent(&e);
		break;
	}
}

void Game::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
	switch (e.keysym.sym)
	{
	case SDLK_q:

		break;
	case SDLK_w:

		break;
	case SDLK_e:

		break;
	case SDLK_r:

		break;
	case SDLK_t:

		break;
	case SDLK_y:

		break;
	case SDLK_u:

		break;
	case SDLK_i:

		break;
	case SDLK_o:

		break;
	case SDLK_p:
	
		break;
	case SDLK_a:

		break;
	case SDLK_s:

		break;
	case SDLK_d:

		break;
	case SDLK_f:

		break;
	case SDLK_g:

		break;
	case SDLK_h:

		break;
	case SDLK_j:

		break;
	case SDLK_k:

		break;
	case SDLK_l:

		break;
	case SDLK_z:

		break;
	case SDLK_x:

		break;
	case SDLK_c:

		break;
	case SDLK_v:

		break;
	case SDLK_b:

		break;
	case SDLK_n:

		break;
	case SDLK_m:

		break;
	case SDLK_LALT:
		m_AltHeld = false;
		break;
	case SDLK_LSHIFT:
		m_UnitManager.DisableQueuing();
		break;
	}
	
}
void Game::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
	m_LastMousePos = Point2f{ float(e.x),float(e.y) };

	m_SandboxBattler->OnMouseMotion(m_LastMousePos);
	m_UnitManager.OnMouseMotion(m_LastMousePos);

}
void Game::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
	switch (e.button)
	{
	case SDL_BUTTON_LEFT:
		m_LeftClickHeld = true;
		m_UnitManager.OnLeftButtonDown();
		break;
	case SDL_BUTTON_RIGHT:
		m_UnitManager.OnRightButtonDown();
		break;
	case SDL_BUTTON_MIDDLE:
		break;
	}

	m_SandboxBattler->OnMouseDown();
}
void Game::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
{

	switch ( e.button )
	{
	case SDL_BUTTON_LEFT:
		m_LeftClickHeld = false;
		break;
	case SDL_BUTTON_RIGHT:
		
		break;
	case SDL_BUTTON_MIDDLE:
		
		break;
	}
	m_SandboxBattler->OnMouseUp();
}
void Game::ProcessMouseWheelEvent(const SDL_MouseWheelEvent& e)
{
	// Wheel up
	if (e.y > 0)
	{
		ZoomIn();
	}
	// Wheel down
	else if (e.y < 0)
	{
		ZoomOut();
	}	
}

void Game::Start()
{
	m_TimeStateChanged = true;
	m_Paused = false;
}
void Game::Pause()
{
	m_TimeStateChanged = true;
	m_Paused = true;
}
void Game::Restart()
{

}

void Game::ZoomIn()
{
	m_ScaleFactor += 0.1f;
	m_ZoomTarget = m_LastMousePos;
}
void Game::ZoomOut()
{
	m_ScaleFactor -= 0.1f;
	if (m_ScaleFactor < 0.1f)
	{
		m_ScaleFactor = 0.1f;
	}
	m_ZoomTarget = m_LastMousePos;

}
void Game::ResetZoom()
{
	m_ScaleFactor = 1.0f;
}
float Game::GetScale() const
{
	return m_ScaleFactor;
}

void Game::ClearBackground() const
{
	glClearColor(0.35f, 0.35f, 0.45f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}
