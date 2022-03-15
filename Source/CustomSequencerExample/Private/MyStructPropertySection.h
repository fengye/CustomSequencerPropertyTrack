
#pragma once

#include "CoreTypes.h"
#include "MovieSceneSection.h"
#include "MyStruct.h"
#include "Channels/MovieSceneFloatChannel.h"
#include "MyStructPropertySection.generated.h"

UCLASS()
class UMyStructPropertySection : public UMovieSceneSection
{
	GENERATED_BODY()

public:

	/** Constructor that initializes the channel proxy */
	UMyStructPropertySection(const FObjectInitializer& Init);

public:

	/** Channel data mapping to FMyStruct::A */
	UPROPERTY()
	FMovieSceneFloatChannel ChannelA;

	/** Channel data mapping to FMyStruct::B */
	UPROPERTY()
	FMovieSceneFloatChannel ChannelB;

	/** Channel data mapping to FMyStruct::C */
	UPROPERTY()
	FCustomChannelTypeCurveData CustomChannelData;
};