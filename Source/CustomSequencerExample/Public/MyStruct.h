
#pragma once

#include "CoreTypes.h"
#include "UObject/ObjectMacros.h"
#include "Curves/KeyHandle.h"
#include "Channels/MovieSceneChannel.h"
#include "Channels/MovieSceneChannelData.h"
#include "Channels/MovieSceneChannelTraits.h"
#include "MyStruct.generated.h"

/**
 * Custom struct type that is to be animated with a channel of data in FCustomChannelTypeCurveData
 */
USTRUCT(BlueprintType)
struct FCustomChannelType
{
	GENERATED_BODY()

	friend bool operator==(FCustomChannelType A, FCustomChannelType B) { return A.Value == B.Value; }

	/** For example purposes, the channel comprises a single float */
	UPROPERTY(EditAnywhere, Category="Struct")
	float Value;
};

/**
 * Custom struct type that is to be animated with a custom Movie Scene Section comprising 3 channels of data: A, B and C.
 */
USTRUCT(BlueprintType)
struct FMyStruct
{
	GENERATED_BODY()

	/** The 'A' component of this struct */
	UPROPERTY(EditAnywhere, Category="Struct")
	float A;

	/** The 'B' component of this struct */
	UPROPERTY(EditAnywhere, Category="Struct")
	float B;

	/** The 'C' component of this struct */
	UPROPERTY(EditAnywhere, Category="Struct")
	FCustomChannelType C;
};


/**
 * A custom channel of curve data that returns FCustomChannelType values
 */
USTRUCT()
struct FCustomChannelTypeCurveData : public FMovieSceneChannel
{
	GENERATED_BODY()

	/**
	 * Common mutable interface to the time/value pairs
	 */
	FORCEINLINE TMovieSceneChannelData<FCustomChannelType> GetData()
	{
		return TMovieSceneChannelData<FCustomChannelType>(&Times, &KeyValues, &KeyHandles);
	}

	/**
	 * Common read-only interface to the time/value pairs
	 */
	FORCEINLINE TMovieSceneChannelData<const FCustomChannelType> GetData() const
	{
		return TMovieSceneChannelData<const FCustomChannelType>(&Times, &KeyValues);
	}

	/**
	 * Evaluate this curve at the specified time, returning whether or not a valid output value was assigned
	 *
	 * @param InTime        The time at which to evaluate
	 * @param OutValue      A value to receive the evaluated result, if possible
	 * @return True if the data was evaluated successfully, false otherwise
	 */
	bool Evaluate(FFrameTime InTime, FCustomChannelType& OutValue) const;

	/**
	 * Called after this section data has been serialized to upgrade old data from floating point times to integer ticks
	 */
	void PostSerialize(const FArchive& Ar);

	// ~ FMovieSceneChannel Interface
	virtual void GetKeys(const TRange<FFrameNumber>& WithinRange, TArray<FFrameNumber>* OutKeyTimes, TArray<FKeyHandle>* OutKeyHandles) override;
	virtual void GetKeyTimes(TArrayView<const FKeyHandle> InHandles, TArrayView<FFrameNumber> OutKeyTimes) override;
	virtual void SetKeyTimes(TArrayView<const FKeyHandle> InHandles, TArrayView<const FFrameNumber> InKeyTimes) override;
	virtual void DuplicateKeys(TArrayView<const FKeyHandle> InHandles, TArrayView<FKeyHandle> OutNewHandles) override;
	virtual void DeleteKeys(TArrayView<const FKeyHandle> InHandles) override;
	virtual void ChangeFrameResolution(FFrameRate SourceRate, FFrameRate DestinationRate) override;
	virtual TRange<FFrameNumber> ComputeEffectiveRange() const override;
	virtual int32 GetNumKeys() const override;
	virtual void Optimize(const FKeyDataOptimizationParams& InParameters) override;
	virtual void Offset(FFrameNumber DeltaPosition) override;
	virtual void Reset() override;

private:

	/** Sorted array of key times as frame numbers in the sequence's tick resolution */
	UPROPERTY()
	TArray<FFrameNumber> Times;

	/** Array of values that correspond to each key time */
	UPROPERTY()
	TArray<FCustomChannelType> KeyValues;

	/** Transient key handles */
	FKeyHandleLookupTable KeyHandles;

#if WITH_EDITORONLY_DATA

	UPROPERTY()
	TArray<float> KeyTimes_DEPRECATED;

#endif
};

/**
 * Movie Scene traits class defining several properties of FCustomChannelTypeCurveData.
 * This specialization is only necessary if you want to change the default traits.
 */
template<>
struct TMovieSceneChannelTraits<FCustomChannelTypeCurveData> : TMovieSceneChannelTraitsBase<FCustomChannelTypeCurveData>
{
	/* This custom channel does not have default values */
	enum { SupportsDefaults = false };

#if WITH_EDITOR

	/* This custom channel supports retrieving an external value from an object */
	typedef TMovieSceneExternalValue<FCustomChannelType> ExtendedEditorDataType;

#endif
};