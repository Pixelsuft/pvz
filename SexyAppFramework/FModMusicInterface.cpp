#include "FModMusicInterface.h"
#include "SexyAppBase.h"
#include "FModLoader.h"
#define FMOD_HAS_ERROR(res) ((res) != FMOD_OK)

using namespace Sexy;

FModMusicInfo::FModMusicInfo()
{
	mHSample = NULL;
	mHMusic = NULL;
	mVolume = 0.0;
	mVolumeAdd = 0.0;
	mVolumeCap = 1.0;
	mStopOnFade = false;
	mRepeats = false;
}

FModMusicInterface::FModMusicInterface()
{	
	LoadFModDLL();

	FMOD_RESULT err;
	if (FMOD_HAS_ERROR(err = gFMod->FMOD_System_Create(&sys, FMOD_VER))) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", (SexyString("Failed to create FMOD system: ") + FMOD_ErrorString(err)).c_str(), NULL);
		exit(0);
	}
	if (FMOD_HAS_ERROR(err = gFMod->FMOD_System_Init(sys, 32, FMOD_INIT_NORMAL, NULL))) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", (SexyString("Failed to init FMOD system: ") + FMOD_ErrorString(err)).c_str(), NULL);
		exit(0);
	}

	mMasterVolume = 1.0;	
	mMaxMusicVolume = 100;
	mMaxSampleVolume = 100;
}

FModMusicInterface::~FModMusicInterface()
{
	// TODO: free all music
	gFMod->FMOD_System_Close(sys);
	gFMod->FMOD_System_Release(sys);
	FreeFModDLL();
}

FSOUND_SAMPLE* FModMusicInterface::LoadFMODSample(const std::string& theFileName)
{
	std::string aFilename = theFileName;

	int aLastDotPos = aFilename.rfind('.');
	int aLastSlashPos = max((int) aFilename.rfind('\\'), (int) aFilename.rfind('/'));	
	if (aLastSlashPos < 0)
		aLastSlashPos = 0;

	std::string aCachedName = GetAppDataFolder() + "cached\\" + GetFileName(theFileName, true) + ".wav";
	MkDir(GetFileDir(aCachedName));

	// FSOUND_SAMPLE* aSample = gFMod->FSOUND_Sample_Load(FSOUND_FREE, aCachedName.c_str(), 0, 0);
	// FSOUND_SAMPLE* aSample = gFMod->FSOUND_Sample_Load(FSOUND_FREE, (theFileName + ".ogg").c_str(), 0, 0);
	
	return NULL;
}

bool FModMusicInterface::LoadSample(int theSongId, const std::string& theFileName, bool repeat)
{
	FSOUND_SAMPLE* aSample = LoadFMODSample(theFileName.c_str());

	if (aSample == NULL)
		return false;

	FModMusicInfo aMusicInfo;
	aMusicInfo.mHSample = aSample;	
	aMusicInfo.mRepeats = repeat;
	mMusicMap.insert(FModMusicMap::value_type(theSongId, aMusicInfo));

	return true;	
}

bool FModMusicInterface::LoadSample(int theSongId, const std::string& theIntroFileName, const std::string& theRepeatFileName, bool repeat)
{
	return false;	
}

bool FModMusicInterface::LoadMusic(int theSongId, const std::string& theFileName)
{
	/*
	FMOD_RESULT err;
	FModMusicInfo aMusicInfo;
	FMOD_SOUND* snd;
	if (FMOD_HAS_ERROR(err = gFMod->FMOD_System_CreateStream(sys, theFileName.c_str(), FMOD_LOOP_NORMAL | FMOD_2D, NULL, &snd))) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", (SexyString("Failed to load FMOD music: ") + FMOD_ErrorString(err)).c_str(), NULL);
		return false;
	}
	aMusicInfo.mHMusic = snd;
	mMusicMap.insert(FModMusicMap::value_type(theSongId, aMusicInfo));
	*/
	return true;
}


void FModMusicInterface::PlayMusic(int theSongId, int theOffset, bool noLoop)
{
}

void FModMusicInterface::PauseMusic(int theSongId)
{
}

void FModMusicInterface::ResumeMusic(int theSongId)
{
}

void FModMusicInterface::StopMusic(int theSongId)
{
}

void FModMusicInterface::StopAllMusic()
{
}

void FModMusicInterface::FadeIn(int theSongId, int theOffset, double theSpeed, bool noLoop)
{
}

void FModMusicInterface::FadeOut(int theSongId, bool stopSong, double theSpeed)
{
}

void FModMusicInterface::FadeOutAll(bool stopSong, double theSpeed)
{
}

void FModMusicInterface::SetVolume(double theVolume)
{
}

void FModMusicInterface::SetSongVolume(int theSongId, double theVolume)
{
}

bool FModMusicInterface::IsPlaying(int theSongId)
{
	return false;
}

void FModMusicInterface::Update()
{
	gFMod->FMOD_System_Update(sys);
}

