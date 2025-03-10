#include "FModMusicInterface.h"
#include "SexyAppBase.h"
#include "FModLoader.h"

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

FModMusicInterface::FModMusicInterface(HWND theHWnd)
{	
	LoadFModDLL();

	mMasterVolume = 1.0;	
	mMaxMusicVolume = 100;
	mMaxSampleVolume = 100;
}

FModMusicInterface::~FModMusicInterface()
{
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
	return false;
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

}

