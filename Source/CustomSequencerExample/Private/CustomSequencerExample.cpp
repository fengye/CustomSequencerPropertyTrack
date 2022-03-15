// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MyStructPropertyTrackEditor.h"
#include "ISequencerModule.h"
#include "MyStructPropertySection.h"
#include "MovieSceneClipboard.h"
#include "SequencerChannelInterface.h"

#define LOCTEXT_NAMESPACE "FCustomSequencerExampleModule"

namespace MovieSceneClipboard
{
	/** Specialization required for clipboard interaction for FMyCustomChannelType keys */
	template<> inline FName GetKeyTypeName<FCustomChannelType>()
	{
		return "CustomChannelType";
	}
}


/**
 * Overload that defines we can create key editors for FCustomChannelTypeCurveData channels.
 * Called from TSequencerChannelInterface<FCustomChannelTypeCurveData>::CanCreateKeyEditor_Raw, instantiated in RegisterChannelInterface.
 * Overloads the default implementation found in SequencerChannelTraits.h.
 */
FORCEINLINE bool CanCreateKeyEditor(const FCustomChannelTypeCurveData* InChannel)
{
	return true;
}


/**
 * Overload that defines how to create a key editor (shown on the sequencer outliner tree view row) for FCustomChannelTypeCurveData
 * Called from TSequencerChannelInterface<FCustomChannelTypeCurveData>::CreateKeyEditor_Raw, instantiated in RegisterChannelInterface.
 * Overloads the default implementation found in SequencerChannelTraits.h.
 */
TSharedRef<SWidget> CreateKeyEditor(
	TMovieSceneChannelHandle<FCustomChannelTypeCurveData> InChannel,
	UMovieSceneSection*                      InOwningSection,
	const FGuid&                             InObjectBindingID,
	TWeakPtr<FTrackInstancePropertyBindings> InPropertyBindings,
	TWeakPtr<ISequencer>                     InSequencer
	)
{
	return SNew(STextBlock).Text(NSLOCTEXT("MyStructPropertySectionInterface", "KeyEditorText", "Custom Key Editor"));
}


/**
 * Overload that defines how to draw keys for FCustomChannelTypeCurveData. Overloads the default implementation found in SequencerChannelTraits.h.
 */
void DrawKeys(FCustomChannelTypeCurveData* Channel, TArrayView<const FKeyHandle> InHandles, TArrayView<FKeyDrawParams> OutKeyDrawParams)
{
	// Custom key drawing for our channel type
	FKeyDrawParams DefaultParams;
	DefaultParams.BorderBrush = DefaultParams.FillBrush = FEditorStyle::Get().GetBrush("Sequencer.KeyDiamond");

	for (FKeyDrawParams& Param : OutKeyDrawParams)
	{
		Param = DefaultParams;
	}
}

/**
 * Module implementation that registers the track editor and channel interface
 */
class FCustomSequencerExampleModule : public IModuleInterface
{
public:

	virtual void StartupModule() override
	{
		ISequencerModule& SequencerModule = FModuleManager::Get().LoadModuleChecked<ISequencerModule>( "Sequencer" );
		SequencerModule.RegisterPropertyTrackEditor<FMyStructPropertyTrackEditor>();

		// This instantiates the interface for all sequencer editor interaction with FCustomChannelTypeCurveData channels using the 
		// overloaded functions above.

		// Alternatively pass your own implementation of an ISequencerChannelInterface that static casts to FCustomChannelTypeCurveData
		// in each of the _Raw functions
		SequencerModule.RegisterChannelInterface<FCustomChannelTypeCurveData>();
	}
};

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCustomSequencerExampleModule, CustomSequencerExample)