
#include "MyStructPropertySection.h"
#include "Channels/MovieSceneChannelProxy.h"

#if WITH_EDITOR

/**
 * Editor data structure used to statically initialize the editor meta-data required for populating a UMyStructPropertySection's channel proxy
 */
struct FMyStructChannelEditorData
{
	FMyStructChannelEditorData()
	{
		MetaData[0].SetIdentifiers("MyStruct.A", NSLOCTEXT("MyStructChannelEditorData", "A", "A"));
		MetaData[0].SortOrder = 0;

		MetaData[1].SetIdentifiers("MyStruct.B", NSLOCTEXT("MyStructChannelEditorData", "B", "B"));
		MetaData[1].SortOrder = 1;

		MetaData[2].SetIdentifiers("MyStruct.C", NSLOCTEXT("MyStructChannelEditorData", "C", "C"));
		MetaData[2].SortOrder = 2;

		ExternalFloatValues[0].OnGetExternalValue = ExtractChannelA;
		ExternalFloatValues[1].OnGetExternalValue = ExtractChannelB;
		ExternalCustomValue.OnGetExternalValue = ExtractChannelC;
	}

	static TOptional<float> ExtractChannelA(UObject& InObject, FTrackInstancePropertyBindings* Bindings)
	{
		return Bindings ? Bindings->GetCurrentValue<FMyStruct>(InObject).A : TOptional<float>();
	}
	static TOptional<float> ExtractChannelB(UObject& InObject, FTrackInstancePropertyBindings* Bindings)
	{
		return Bindings ? Bindings->GetCurrentValue<FMyStruct>(InObject).B : TOptional<float>();
	}
	static TOptional<FCustomChannelType> ExtractChannelC(UObject& InObject, FTrackInstancePropertyBindings* Bindings)
	{
		return Bindings ? Bindings->GetCurrentValue<FMyStruct>(InObject).C : TOptional<FCustomChannelType>();
	}

	/** Meta-data for each channel A, B and C respectively */
	FMovieSceneChannelMetaData MetaData[3];

	/** External values specified as Extended Editor Data for channels A and B  respectively */
	TMovieSceneExternalValue<float> ExternalFloatValues[2];

	/** External value specified as Extended Editor Data for channel C */
	TMovieSceneExternalValue<FCustomChannelType> ExternalCustomValue;
};
#endif


UMyStructPropertySection::UMyStructPropertySection(const FObjectInitializer& Init)
	: Super(Init)
{
	BlendType = EMovieSceneBlendType::Absolute;

	FMovieSceneChannelProxyData Channels;

#if WITH_EDITOR

	const FMyStructChannelEditorData EditorData;
	Channels.Add(ChannelA, EditorData.MetaData[0], EditorData.ExternalFloatValues[0]);
	Channels.Add(ChannelB, EditorData.MetaData[1], EditorData.ExternalFloatValues[1]);
	Channels.Add(CustomChannelData, EditorData.MetaData[2], EditorData.ExternalCustomValue);

#else

	Channels.Add(ChannelA);
	Channels.Add(ChannelB);
	Channels.Add(CustomChannelData);

#endif

	// Populate the channel proxy - if any of our channels were ever reallocated, we'd need to repopulate the proxy,
	// but since ours are all value member types, we only need to populate in the constructor
	ChannelProxy = MakeShared<FMovieSceneChannelProxy>(MoveTemp(Channels));
}