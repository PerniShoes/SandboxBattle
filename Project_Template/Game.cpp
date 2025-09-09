#include "Game.h"

#include "utils.h"
#include "TimeClass.h"
#include "Texture.h"
#include "OrientationManager.h"
#include "SandboxBattler.h" // Project

#include <iostream>
#include <cmath>
#include <print>


// TESTING:
#include "UnitAnimator.h"


Game::Game(const Window& window)
	:BaseGame(window)
	,m_UnitManager{}
	,m_Window{ window }
	,m_RenderedFrames{0}
	
{
	Initialize();
	InitTest();
}

Game::~Game()
{
	Cleanup();
}

///////// TEST FUNCITONS:

void Game::InitTest()
{
	// Load Atlas
	std::string testedAnim2{"boss_treatdemon"};
	std::string testedAnim{"boss_unhallowed"};

	std::string currentAnim{testedAnim};
	testAtlas = GameResources::m_AtlasManager.GetAtlas(currentAnim);
	testTexture = std::make_unique<Texture>(testAtlas->pngPath);

	m_TestUA = std::make_unique<UnitAnimator>(testAtlas,testTexture.get());
	m_TestUA->Play("breathing");

	// Grab one animation clip (e.g. idle or death)
	/*testAnim = testAtlas->animations.count("attack") ?
		&testAtlas->animations["attack"] : nullptr;*/
	//testAnim->loop = false;

	// Load the texture of the atlas
	//testTexture = std::make_unique<Texture>(testAtlas->pngPath);
}

void Game::UpdateTest(float dt)
{
	if (!testAnim) return;

	elapsed += dt;
	if (elapsed >= frameTime)
	{
		elapsed = 0.0f;
		currentFrame++;
		if (currentFrame >= (int)testAnim->frameNames.size())
		{
			if (testAnim->loop)
				currentFrame = 0;
			else
				currentFrame = (int)testAnim->frameNames.size() - 1;
		}
	}
}

void Game::DrawTest() const
{
	if (!testAnim || !testTexture) return;
	if (testAnim->frameNames.empty()) return;

	const std::string& fname = testAnim->frameNames[currentFrame];

	auto it = testAtlas->frames.find(fname);

	const FrameData& fd = it->second;

	// Draw at screen pos (100,100), using your existing DrawShade
	Rectf src{(float)fd.x, (float)fd.y, (float)fd.w, (float)fd.h};
	Rectf dst{100.f, 100.f, (float)fd.w, (float)fd.h};

	testTexture->DrawShade(dst,src,{});
}

/////////////////////////////////////////////////////////////////////////////////////////

void Game::Initialize()
{
	// TESTING ATLAS SPRITE LOADING
	// LOAD ALL
	GameResources::m_AtlasManager.LoadFolder("../Resources/DuelystResc/units");


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



		//UpdateTest(elapsedSec);

		m_TestUA->Update(elapsedSec);
		if (m_TestUA->IsDone())
		{
			m_TestUA->Play("breathing");
		}


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


void Game::Draw() const
{
	ClearBackground();
	glPushMatrix();
	{
		PushCameraMatrix();
		using namespace PrettyColors;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		glPushMatrix();
		glScalef(2.0f,2.0f,1.0f);
		m_TestUA->DrawShade({});
		//DrawTest();
		// 
		//m_AtlasTestTexture->DrawShade(Rectf{50.0f,50.0f,120.0f,120.0f},f.ToRectf(),{});
		//m_AtlasTestTexture->DrawShade(Rectf{50.0f,50.0f,120.0f,120.0f},Rectf{(f.x), 241.0f, f.w, f.h},{});
		glPopMatrix();
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
		m_TestUA->Play("idle");
		break;
	case SDLK_w:
		m_TestUA->Play("attack");
		break;
	case SDLK_e:
		m_TestUA->Play("breathing");
		break;
	case SDLK_r:
		m_TestUA->Play("death");
		break;
	case SDLK_t:
		m_TestUA->Play("hit");
		break;
	case SDLK_y:
		m_TestUA->Play("run");
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
