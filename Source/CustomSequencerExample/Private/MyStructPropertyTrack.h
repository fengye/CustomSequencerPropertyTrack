
#pragma once

#include "CoreTypes.h"
#include "UObject/ObjectMacros.h"
#include "Tracks/MovieScenePropertyTrack.h"
#include "MyStructPropertySection.h"
#include "Evaluation/MovieScenePropertyTemplate.h"
#include "Compilation/IMovieSceneTrackTemplateProducer.h"
#include "MyStructPropertyTrack.generated.h"

USTRUCT()
struct FMyStructPropertyEvaluationTemplate : public FMovieScenePropertySectionTemplate
{
	GENERATED_BODY()

	FMyStructPropertyEvaluationTemplate(){}
	FMyStructPropertyEvaluationTemplate(const UMyStructPropertySection& Section, const UMovieScenePropertyTrack& Track);

	virtual UScriptStruct& GetScriptStructImpl() const override { return *StaticStruct(); }
	virtual void Evaluate(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context, const FPersistentEvaluationData& PersistentData, FMovieSceneExecutionTokens& ExecutionTokens) const override;

	UPROPERTY()
	EMovieSceneBlendType BlendType;

	UPROPERTY()
	FMovieSceneFloatChannel ChannelA;

	UPROPERTY()
	FMovieSceneFloatChannel ChannelB;

	UPROPERTY()
	FCustomChannelTypeCurveData ChannelC;
};

UCLASS()
class UMyStructPropertyTrack : public UMovieScenePropertyTrack, public IMovieSceneTrackTemplateProducer
{
	GENERATED_BODY()

	virtual FMovieSceneEvalTemplatePtr CreateTemplateForSection(const UMovieSceneSection& InSection) const override
	{
		return FMyStructPropertyEvaluationTemplate(*CastChecked<const UMyStructPropertySection>(&InSection), *this);
	}

	virtual UMovieSceneSection* CreateNewSection() override
	{
		return NewObject<UMyStructPropertySection>(this, NAME_None, RF_Transactional);
	}
};