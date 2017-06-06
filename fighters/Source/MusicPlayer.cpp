#include "Include\MusicPlayer.h"


MusicPlayer::MusicPlayer()
: mMusic()
, mFilenames()
, mVolume(50.f)
{
	mFilenames[Music::BattleTheme]    = "Media/Music/SovietCombat1.ogg";
	mFilenames[Music::DefeatedTheme] = "Media/Music/MissionTheme.ogg";
}

void MusicPlayer::play(Music::ID theme)
{
	std::string filename = mFilenames[theme];

	if (!mMusic.openFromFile(filename))
		throw std::runtime_error("Music " + filename + " could not be loaded.");

	mMusic.setVolume(mVolume);
	mMusic.setLoop(true);
	mMusic.play();
}

void MusicPlayer::stop()
{
	mMusic.stop();
}

void MusicPlayer::setVolume(float volume)
{
	mVolume = volume;
}

void MusicPlayer::setPaused(bool paused)
{
	if (paused)
		mMusic.pause();
	else
		mMusic.play();
}
