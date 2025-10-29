#include "Game.h"

#include "utils.h"
#include "TimeClass.h"
#include "Texture.h"
#include "OrientationManager.h"
#include "SandboxBattler.h" // Project

#include <iostream>
#include <cmath>
#include <print>
#include <thread>
#include <chrono>
#include <cstdlib> 
#include <ctime>  

// Umbrella header for all units???
#include "AllUnits.h" // Yes

// TESTING: delete?
#include "UnitAnimator.h"

Game::Game(const Window& window)
	:BaseGame(window)
	,m_UnitManager{GetViewPort()}
	,m_MouseManager{}
	,m_MapManager{GetViewPort()}
	,m_Window{ window }
	,m_RenderedFrames{0}
	
{
	Initialize();

}

Game::~Game()
{
	Cleanup();
}

/////////////////////////////////////////////////////////////////////////////////////////

void Game::Initialize()
{
	// TESTING ATLAS 
	// LOAD ALL
	GameResources::m_AtlasManager.LoadFolder("../Resources/DuelystResc/units");

	GameResources::m_AudioManager.LoadMusic("../Resources/DuelystResc/ConvertedMusic");
	GameResources::m_AudioManager.LoadSounds("../Resources/DuelystResc/ConvertedSfx");
	GameResources::m_AudioManager.SetDefaultVolumeMusic(10);
	GameResources::m_AudioManager.SetDefaultVolumeSounds(10);
	m_MusicPlayer = &GameResources::m_AudioManager.GetMusicPack();
	//m_MusicPlayer->find("battlemap_bluemonolith")->second.Play(true);
	m_MapManager.SetMap("battlemap5");
	m_UnitManager.SetSoundPack(&GameResources::m_AudioManager.GetSoundPack("unit"));

	// There has to be a way to make it easier to choose the class (and create them too)
	// For debug purposes team 0 is ally, team 1 enemy (also commented in unit.cpp inside LoadTextures)
	m_UnitManager.SetDefaultTeam(0);

	m_UnitManager.AddUnit(std::make_unique<f1_altgeneraltier2>());
	m_UnitManager.AddUnit(std::make_unique<f1_ranged>());
	m_UnitManager.AddUnit(std::make_unique<f1_backlinearcher>());
	m_UnitManager.AddUnit(std::make_unique<f1_scintilla>());
	m_UnitManager.AddUnit(std::make_unique<f1_scintilla>());
	m_UnitManager.AddUnit(std::make_unique<f1_sunbreakguardian>());
	m_UnitManager.AddUnit(std::make_unique<f1_sunbreakguardian>());
	m_UnitManager.AddUnit(std::make_unique<f1_sunbreakguardian>());


	m_UnitManager.SetDefaultTeam(1);
	m_UnitManager.AddUnit(std::make_unique<boss_invader>());
	m_UnitManager.AddUnit(std::make_unique<boss_legion>());
	m_UnitManager.AddUnit(std::make_unique<boss_shadowlord>());
	m_UnitManager.AddUnit(std::make_unique<boss_solfist>());
	m_UnitManager.AddUnit(std::make_unique<boss_cindera>());


	m_UnitManager.ScaleAllUnits(1.5f,1.5f);
	m_UnitManager.SetFrameTimeAll(0.020f);

	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	// Unit stat setting for debug
	for (int i{1}; i <= m_UnitManager.GetUnitCount(); ++i)
	{
		Unit* currentUnit = m_UnitManager.GetUnit(i);
		Stats newStats = currentUnit->GetStats();
		newStats.m_CurrentHealth = (1 + std::rand() % 12);    
		newStats.m_CurrentDamage = (2 + std::rand() % 12); 
		newStats.m_InitialDamage = newStats.m_CurrentDamage + 1 - std::rand() % 3;
		newStats.m_MaxHealth = newStats.m_CurrentHealth + 1 - std::rand() % 3;

		newStats.m_MoveSpeed = 550;
		newStats.m_MoveRangeTiles = 550;
		currentUnit->SetStats(newStats);
		currentUnit->UpdateHealthTexture();
		currentUnit->UpdateAttackTexture();
	}



	// FIX debug code
	for (int i{1}; i <= m_UnitManager.GetUnitCount() ;++i)
	{
		Unit* unitPtr = m_UnitManager.GetUnit(i);
		Point2f tpPos{abs(unitPtr->GetHitBox().left),abs(unitPtr->GetHitBox().bottom)};
		//m_UnitManager.TeleportTo(i,tpPos);
		m_UnitManager.PlaceOnGrid(i,-1,true);
	}
	 
	
	//float x{0.0f};
	//float y{0.0f};
	//float xOffset{400.0f};
	//for (int i{0} ;i < m_UnitManager.GetUnitCount(); ++i)
	//{
	//	if (i == (m_UnitManager.GetUnitCount() / 2))
	//	{
	//		y += 85.0f;
	//	}
	//	if (i < (m_UnitManager.GetUnitCount() / 2))
	//	{
	//		x = xOffset;
	//		y += 85.0f;
	//	}
	//	else
	//	{
	//		x = m_Window.width - xOffset;
	//		y -= 85.0f;
	//	}

	//	m_UnitManager.TeleportTo(i,Point2f{x,y});
	//}
	/////////////

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

		m_MouseManager.Update(elapsedSec);
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
	m_FPSCounter = std::make_unique<Texture>("FPS: " + std::to_string(m_FPS),"../Resources/Fonts/consola.ttf",16,Color4f{0,0,0,1}); // Not clean

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


	// This could work as a zoom
	// Camera movement can still be separate or ignored
	// It seems to work perfectly, only issue being mouse position reading (which should be easy to fix by using scale somewhere in mouse reading)
	glTranslatef(GetViewPort().width/2.0f,GetViewPort().height/2.0f,0);
	glScalef(1.0f,1.0f,0);
	glTranslatef(-GetViewPort().width / 2.0f,-GetViewPort().height / 2.0f,0);
	//
	m_MapManager.DrawLayerType(LayerType::Background);
	m_MapManager.DrawLayerType(LayerType::Midground);
	glPushMatrix();
	{
		PushCameraMatrix();
		using namespace PrettyColors;

		m_UnitManager.DrawAll();
		m_SandboxBattler->Draw();


		/*utils::SetColor(PrettyColors::GetColor(PrettyColors::rose));
		utils::FillRect(100.0f,100.0f,10.0f,10.0f); DEBUG*/
	}
	glPopMatrix();

	m_MapManager.DrawLayerType(LayerType::Foreground);
	glPopMatrix();
	DrawUI();
}

void Game::DrawUI() const
{
	DrawPausedText();
	// m_SandboxBattler->DrawUI(GetViewPort());
	m_FPSCounter->Draw(Point2f{10.0f, GetViewPort().height - m_FPSCounter->GetHeight() - 4.0f},Rectf{});


	// Make sure it's last 
	m_MouseManager.Draw();
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
	switch (e.keysym.sym)
	{
	case SDLK_q:

		break;
	case SDLK_y:
		
		break;
	case SDLK_u:
		m_UnitManager.DeSellectAll();
		m_MouseManager.SetMouseState(MouseState::Default);
		break;
	case SDLK_i:

		break;
	case SDLK_o:
		Start();
		break;
	case SDLK_p:
		Pause();
		break;
	case SDLK_z:

		break;
	case SDLK_m:
		m_MusicPlayer->begin()->second.Play(true);
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

	m_MouseManager.OnMouseMotion(m_LastMousePos);
	m_SandboxBattler->OnMouseMotion(m_LastMousePos);
	m_UnitManager.OnMouseMotion(m_LastMousePos);

	// This could probably be better :v
	// Mouse Sprite
	if (m_UnitManager.GetHoverAlly())
	{
		m_MouseManager.SetMouseState(MouseState::HoverAlly);
	}
	else if (m_UnitManager.GetHoverEnemy())
	{
		m_MouseManager.SetMouseState(MouseState::HoverEnemy);
	}
	else if (m_UnitManager.GetHoverGround())
	{
		m_MouseManager.SetMouseState(MouseState::Move);
	}
	else if (m_UnitManager.IsAnySelected())
	{
		m_MouseManager.SetMouseState(MouseState::Selected);
	}
	else
	{
		m_MouseManager.SetMouseState(MouseState::Default);
	}


}
void Game::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
	switch (e.button)
	{
	case SDL_BUTTON_LEFT:
		m_LeftClickHeld = true;
		m_UnitManager.OnLeftButtonDown();
		if (m_UnitManager.IsAnySelected())
		{
			m_MouseManager.SetMouseState(MouseState::Selected);
		}
		else
		{
			m_MouseManager.SetMouseState(MouseState::Default);
		}

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
