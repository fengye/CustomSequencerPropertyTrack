// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ISequencer.h"
#include "ISequencerSection.h"
#include "ISequencerTrackEditor.h"
#include "PropertyTrackEditor.h"
#include "MyStructPropertyTrack.h"
#include "MyStructPropertySection.h"

/**
 * A property track editor for FMyStruct types.
 */
class FMyStructPropertyTrackEditor
	: public FPropertyTrackEditor<UMyStructPropertyTrack>
{
public:

	/**
	 * Construction from a sequencer ptr
	 */
	FMyStructPropertyTrackEditor(TSharedRef<ISequencer> InSequencer)
		: FPropertyTrackEditor(InSequencer, GetAnimatedPropertyTypes())
	{}

	/**
	 * Retrieve the types of property that this track can operate on
	 */
	static TArray<FAnimatedPropertyKey, TInlineAllocator<1>> GetAnimatedPropertyTypes()
	{
		return TArray<FAnimatedPropertyKey, TInlineAllocator<1>>({ FAnimatedPropertyKey::FromStructType(FMyStruct::StaticStruct()) });
	}

	/**
	 * Create a new instance of this track editor
	 */
	static TSharedRef<ISequencerTrackEditor> CreateTrackEditor(TSharedRef<ISequencer> OwningSequencer)
	{
		return MakeShared<FMyStructPropertyTrackEditor>(OwningSequencer);
	}

protected:

	/**
	 * Generates a list of key values that should be applied in response to an external property change event
	 */
	virtual void GenerateKeysFromPropertyChanged(const FPropertyChangedParams& PropertyChangedParams, UMovieSceneSection* SectionToKey, FGeneratedTrackKeys& OutGeneratedKeys) override
	{
		FPropertyPath StructPath = PropertyChangedParams.StructPathToKey;
		FName ChannelName = StructPath.GetNumProperties() != 0 ? StructPath.GetLeafMostProperty().Property->GetFName() : NAME_None;

		FMyStruct CurrentPropertyValue = PropertyChangedParams.GetPropertyValue<FMyStruct>();

		// Explicitly add a key to the channels if we're keying the whole struct property, or an individual channel
		// When false, the channel's default value will be updated if it contains no keys
		const bool bKeyA = ChannelName == NAME_None || ChannelName == "A";
		const bool bKeyB = ChannelName == NAME_None || ChannelName == "B";
		const bool bKeyC = ChannelName == NAME_None || ChannelName == "C";

		// Use the current value for channel A on the first FMovieSceneFloatChannel in the proxy
		OutGeneratedKeys.Add(FMovieSceneChannelValueSetter::Create<FMovieSceneFloatChannel>(0, CurrentPropertyValue.A, bKeyA));
		// Use the current value for channel B on the second FMovieSceneFloatChannel in the proxy
		OutGeneratedKeys.Add(FMovieSceneChannelValueSetter::Create<FMovieSceneFloatChannel>(1, CurrentPropertyValue.B, bKeyB));
		// Use the current value for channel C on the first FCustomChannelTypeCurveData in the proxy
		OutGeneratedKeys.Add(FMovieSceneChannelValueSetter::Create<FCustomChannelTypeCurveData>(0,  CurrentPropertyValue.C, bKeyC));
	}
};
