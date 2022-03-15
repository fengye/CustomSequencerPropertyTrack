
#include "MyStruct.h"
#include "MovieSceneFwd.h"
#include "MovieSceneFrameMigration.h"
#include "Channels/MovieSceneChannelTraits.h"
#include "Evaluation/Blending/MovieSceneMultiChannelBlending.h"

namespace UE
{
	namespace MovieScene
	{
		// Marshall my struct into a multi-channel value
		inline void MultiChannelFromData(FMyStruct In, TMultiChannelValue<float, 3>& Out)
		{
			Out = { In.A, In.B, In.C.Value };
		}
		// Marshall my struct out of a static float array
		inline void ResolveChannelsToData(const TMultiChannelValue<float, 3>& In, FMyStruct& Out)
		{
			Out = { In[0], In[1], {In[2]} };
		}
	}
}

bool FCustomChannelTypeCurveData::Evaluate(FFrameTime InTime, FCustomChannelType& OutValue) const
{
	if (Times.Num())
	{
		const int32 Index = FMath::Max(0, Algo::UpperBound(Times, InTime.FrameNumber)-1);
		OutValue = KeyValues[Index];
		return true;
	}

	return false;
}

void FCustomChannelTypeCurveData::PostSerialize(const FArchive& Ar)
{
#if WITH_EDITORONLY_DATA
if (KeyTimes_DEPRECATED.Num())
{
	FFrameRate LegacyFrameRate = GetLegacyConversionFrameRate();

	TArray<FCustomChannelType> OldValues = KeyValues;
	Times.Reset(KeyTimes_DEPRECATED.Num());
	KeyValues.Reset(KeyTimes_DEPRECATED.Num());
	for (int32 Index = 0; Index < KeyTimes_DEPRECATED.Num(); ++Index)
	{
		FFrameNumber KeyTime = UpgradeLegacyMovieSceneTime(nullptr, LegacyFrameRate, KeyTimes_DEPRECATED[Index]);
		ConvertInsertAndSort<FCustomChannelType>(Index, KeyTime, OldValues[Index], Times, KeyValues);
	}
	KeyTimes_DEPRECATED.Empty();
}
#endif
}

void FCustomChannelTypeCurveData::GetKeys(const TRange<FFrameNumber>& WithinRange, TArray<FFrameNumber>* OutKeyTimes, TArray<FKeyHandle>* OutKeyHandles)
{
	GetData().GetKeys(WithinRange, OutKeyTimes, OutKeyHandles);
}

void FCustomChannelTypeCurveData::GetKeyTimes(TArrayView<const FKeyHandle> InHandles, TArrayView<FFrameNumber> OutKeyTimes)
{
	GetData().GetKeyTimes(InHandles, OutKeyTimes);
}

void FCustomChannelTypeCurveData::SetKeyTimes(TArrayView<const FKeyHandle> InHandles, TArrayView<const FFrameNumber> InKeyTimes)
{
	GetData().SetKeyTimes(InHandles, InKeyTimes);
}

void FCustomChannelTypeCurveData::DuplicateKeys(TArrayView<const FKeyHandle> InHandles, TArrayView<FKeyHandle> OutNewHandles)
{
	GetData().DuplicateKeys(InHandles, OutNewHandles);
}

void FCustomChannelTypeCurveData::DeleteKeys(TArrayView<const FKeyHandle> InHandles)
{
	GetData().DeleteKeys(InHandles);
}

void FCustomChannelTypeCurveData::ChangeFrameResolution(FFrameRate SourceRate, FFrameRate DestinationRate)
{
	GetData().ChangeFrameResolution(SourceRate, DestinationRate);
}

TRange<FFrameNumber> FCustomChannelTypeCurveData::ComputeEffectiveRange() const
{
	return GetData().GetTotalRange();
}

int32 FCustomChannelTypeCurveData::GetNumKeys() const
{
	return Times.Num();
}

void FCustomChannelTypeCurveData::Optimize(const FKeyDataOptimizationParams& InParameters)
{
	// In MovieSceneChannelTraits.h
	UE::MovieScene::Optimize(this, InParameters);
}

void FCustomChannelTypeCurveData::Offset(FFrameNumber DeltaPosition)
{
	GetData().Offset(DeltaPosition);
}

void FCustomChannelTypeCurveData::Reset()
{
	Times.Reset();
	KeyValues.Reset();
	KeyHandles.Reset();
}