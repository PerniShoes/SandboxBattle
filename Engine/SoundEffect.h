#pragma once
#include <string>

// Disable warnings for SDL
#pragma warning(push, 0) // All warnings
//#pragma warning(disable: 5262) // implicit fall-through warning
#include "SDL_mixer.h"
#pragma warning(pop)

struct Mix_Chunk;
class SoundEffect final
{
public:
	explicit SoundEffect( const std::string& path );
	~SoundEffect( );
	SoundEffect(const SoundEffect& other) = delete;
	SoundEffect& operator=(const SoundEffect& rhs) = delete;
	SoundEffect( SoundEffect&& other) = delete;
	SoundEffect& operator=( SoundEffect&& rhs) = delete;

	bool IsLoaded( ) const;
	bool Play( const int loops ) const;
	void SetVolume( const int value ); 
	int GetVolume( ) const; 
	static void StopAll( );
	static void PauseAll( );
	static void ResumeAll( );

private:
	Mix_Chunk* m_pMixChunk;
};
