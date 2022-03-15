
#include "MyStructPropertyTrack.h"
#include "Evaluation/Blending/MovieSceneMultiChannelBlending.h"


// Specify a unique runtime type identifier for 3d transform track tokens
template<> FMovieSceneAnimTypeID GetBlendingDataType<FMyStruct>()
{
	static FMovieSceneAnimTypeID TypeId = FMovieSceneAnimTypeID::Unique();
	return TypeId;
}

template<> struct TBlendableTokenTraits<FMyStruct>
{
	typedef UE::MovieScene::TMaskedBlendable<float,3> WorkingDataType;
};

namespace MovieScene
{
	void MultiChannelFromData(FMyStruct InSourceData, UE::MovieScene::TMultiChannelValue<float, 3>& OutChannelData)
	{
		OutChannelData = { InSourceData.A, InSourceData.B, InSourceData.C.Value };
	}

	void ResolveChannelsToData(const UE::MovieScene::TMultiChannelValue<float, 3>& InChannelData, FMyStruct& OutData)
	{
		OutData.A = InChannelData[0];
		OutData.B = InChannelData[1];
		OutData.C = FCustomChannelType{InChannelData[2]};
	}
}

FMyStructPropertyEvaluationTemplate::FMyStructPropertyEvaluationTemplate(const UMyStructPropertySection& Section, const UMovieScenePropertyTrack& Track)
	: FMovieScenePropertySectionTemplate(Track.GetPropertyName(), Track.GetPropertyPath().ToString())
	, BlendType(Section.GetBlendType().Get())
	, ChannelA(Section.ChannelA)
	, ChannelB(Section.ChannelB)
	, ChannelC(Section.CustomChannelData)
{}

void FMyStructPropertyEvaluationTemplate::Evaluate(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context, const FPersistentEvaluationData& PersistentData, FMovieSceneExecutionTokens& ExecutionTokens) const
{
	// Actuator type ID for this property
	FMovieSceneBlendingActuatorID ActuatorTypeID = EnsureActuator<FMyStruct>(ExecutionTokens.GetBlendingAccumulator());

	FFrameTime Time = Context.GetTime();

	// Add the blendable to the accumulator
	UE::MovieScene::TMultiChannelValue<float, 3> AnimatedData;

	float Value = 0.f;
	FCustomChannelType CustomValue;

	if (ChannelA.Evaluate(Time, Value))        { AnimatedData.Set(0, Value); }
	if (ChannelB.Evaluate(Time, Value))        { AnimatedData.Set(1, Value); }
	if (ChannelC.Evaluate(Time, CustomValue))  { AnimatedData.Set(2, CustomValue.Value); }

	const float Weight = EvaluateEasing(Context.GetTime());

	ExecutionTokens.BlendToken(ActuatorTypeID, TBlendableToken<FMyStruct>(AnimatedData, BlendType, Weight));
}