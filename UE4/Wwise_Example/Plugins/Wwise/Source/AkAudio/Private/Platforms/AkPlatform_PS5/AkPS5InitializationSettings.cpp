/*******************************************************************************
The content of this file includes portions of the proprietary AUDIOKINETIC Wwise
Technology released in source code form as part of the game integration package.
The content of this file may not be used without valid licenses to the
AUDIOKINETIC Wwise Technology.
Note that the use of the game engine is subject to the Unreal(R) Engine End User
License Agreement at https://www.unrealengine.com/en-US/eula/unreal
 
License Usage
 
Licensees holding valid licenses to the AUDIOKINETIC Wwise Technology may use
this file in accordance with the end user license agreement provided with the
software or, alternatively, in accordance with the terms contained
in a written agreement between you and Audiokinetic Inc.
Copyright (c) 2023 Audiokinetic Inc.
*******************************************************************************/



#include "Platforms/AkPlatform_PS5/AkPS5InitializationSettings.h"
#include "AkAudioDevice.h"

#ifdef AK_PS5
namespace AkLowLevelMemory
{
	constexpr auto PELLGRINO_TRUEPAGESIZE = 16 * 1024;
	void* Alloc(size_t Size)
	{
		const auto AlignedSize = Align(Size, PELLGRINO_TRUEPAGESIZE);
		return FMemory::Malloc(AlignedSize, PELLGRINO_TRUEPAGESIZE);
	}

	void Free(void* Addr, size_t Size)
	{
		FMemory::Free(Addr);
	}
}
#endif // AK_PS5

///////////////////////////////////////////////////////////////////////////
// UAkPS5InitializationSettings

void FAkPS5AdvancedInitializationSettings::FillInitializationStructure(FAkInitializationStructure& InitializationStructure) const
{
	Super::FillInitializationStructure(InitializationStructure);
	
#ifdef AK_PS5
	InitializationStructure.PlatformInitSettings.bHwCodecLowLatencyMode = UseHardwareCodecLowLatencyMode;
	InitializationStructure.PlatformInitSettings.bVorbisHwAcceleration = bVorbisHwAcceleration;
#endif
}


//////////////////////////////////////////////////////////////////////////
// UAkPS5InitializationSettings

UAkPS5InitializationSettings::UAkPS5InitializationSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CommonSettings.SamplesPerFrame = 512;
}

void UAkPS5InitializationSettings::FillInitializationStructure(FAkInitializationStructure& InitializationStructure) const
{
	InitializationStructure.SetPluginDllPath("PS5");

	CommonSettings.FillInitializationStructure(InitializationStructure);
	CommunicationSettings.FillInitializationStructure(InitializationStructure);
	AdvancedSettings.FillInitializationStructure(InitializationStructure);
	
#ifdef AK_PS5
	InitializationStructure.SetupLLMAllocFunctions(AkLowLevelMemory::Alloc, AkLowLevelMemory::Free, false);
#endif
}
