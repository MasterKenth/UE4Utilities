#pragma once

#include "Engine.h"
#include "MKUE4UtilitiesLibrary.generated.h"

UENUM(BlueprintType)
enum EasingType
{
	ET_Linear UMETA(DisplayName = "Linear"),
	ET_SineIn UMETA(DisplayName = "Sine In"),
	ET_SineOut UMETA(DisplayName = "Sine Out"),
	ET_SineInOut UMETA(DisplayName = "Sine In Out"),
	ET_QuadIn UMETA(DisplayName = "Quad In"),
	ET_QuadOut UMETA(DisplayName = "Quad Out"),
	ET_QuadInOut UMETA(DisplayName = "Quad In Out"),
	ET_CubicIn UMETA(DisplayName = "Cubic In"),
	ET_CubicOut UMETA(DisplayName = "Cubic Out"),
	ET_CubicInOut UMETA(DisplayName = "Cubic In Out"),
	ET_QuartIn UMETA(DisplayName = "Quartic In"),
	ET_QuartOut UMETA(DisplayName = "Quartic Out"),
	ET_QuartInOut UMETA(DisplayName = "Quartic In Out"),
	ET_QuintIn UMETA(DisplayName = "Quintic In"),
	ET_QuintOut UMETA(DisplayName = "Quintic Out"),
	ET_QuintInOut UMETA(DisplayName = "Quintic In Out"),
	ET_ExpoIn UMETA(DisplayName = "Exponential In"),
	ET_ExpoOut UMETA(DisplayName = "Exponential Out"),
	ET_ExpoInOut UMETA(DisplayName = "Exponential In Out"),
	ET_CircIn UMETA(DisplayName = "Circular In"),
	ET_CircOut UMETA(DisplayName = "Circular Out"),
	ET_CircInOut UMETA(DisplayName = "Circular In Out"),
	ET_BackIn UMETA(DisplayName = "Back In"),
	ET_BackOut UMETA(DisplayName = "Back Out"),
	ET_BackInOut UMETA(DisplayName = "Back In Out"),
	ET_ElasticIn UMETA(DisplayName = "Elastic In"),
	ET_ElasticOut UMETA(DisplayName = "Elastic Out"),
	ET_ElasticInOut UMETA(DisplayName = "Elastic In Out"),
	ET_BounceIn UMETA(DisplayName = "Bounce In"),
	ET_BounceOut UMETA(DisplayName = "Bounce Out"),
	ET_BounceInOut UMETA(DisplayName = "Bounce In Out")
};

UCLASS()
class UMKUE4UtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	private:

	static void ShuffleArrayWithStream_impl(void* TargetArray, const UArrayProperty* ArrayProperty, const FRandomStream& Stream); // Real implementation


	public:

	/** Wildcard functions */
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (DisplayName = "Shuffle by Stream", CompactNodeTitle = "SHUFFLESTREAM", ArrayParm = "TargetArray"), Category = "MK Utilities")
	static void ShuffleArrayWithStream(const TArray<int32>& TargetArray, const FRandomStream& Stream); // Stub function

	/** Wildcard thunks */
	DECLARE_FUNCTION(execShuffleArrayWithStream)
	{
		Stack.MostRecentProperty = nullptr;
		Stack.StepCompiledIn<UArrayProperty>(NULL);
		void* ArrayAddr = Stack.MostRecentPropertyAddress;

		//P_GET_OBJECT(UArrayProperty, ArrayProperty); // Old 4.8 code
		UArrayProperty* ArrayProperty = Cast<UArrayProperty>(Stack.MostRecentProperty); // New 4.9 code
		if (!ArrayProperty)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to get array context property"));
			Stack.bArrayContextFailed = true;
			return;
		}

		P_GET_STRUCT_REF(FRandomStream, Stream);
		P_FINISH;

		ShuffleArrayWithStream_impl(ArrayAddr, ArrayProperty, Stream);
	}
	

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Mouse Position", Keywords = "Set Mouse Position"), Category = "MK Utilities")
	static bool SetMousePosition(APlayerController* PC, const float& PosX, const float& PosY);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Actor Last Render Time", Keywords = "Actor Render Time"), Category = "MK Utilities")
	static float GetActorLastRenderTime(AActor* RefActor);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Was Actor Rendered Recently", Keywords = "Actor Rendered Recently"), Category = "MK Utilities")
	static bool WasActorRenderedRecently(AActor* RefActor, float MaxRecentTime = 0.1f);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Is Actor Within Frustum", Keywords = "Actor Within Frustum"), Category = "MK Utilities")
	static bool IsActorWithinFrustum(AActor* RefActor);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Convert Linear Colors", Keywords = "Color Convert Linear"), Category = "MK Utilities")
	static TArray<FColor> ConvertLinearColors(const TArray<FLinearColor>& LinearColors);

	UFUNCTION(BlueprintPure, Category = "Testing")
	static bool HasRuntimeCooking();

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Move Towards (Vector)", Keywords = "Vector FVector Move Towards"), Category = "MK Utilities")
	static FVector MoveTowardsVector(FVector From, FVector To, float MaxDistanceDelta);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Move Towards (Rotator)", Keywords = "Rotator FRotator Move Towards"), Category = "MK Utilities")
	static FRotator MoveTowardsRotator(FRotator From, FRotator To, float MaxDistanceDelta);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Move Towards (Angle)", Keywords = "Angle Move Towards"), Category = "MK Utilities")
	static float MoveTowardsAngle(float From, float To, float MaxDelta);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Move Towards (Float)", Keywords = "Float Move Towards"), Category = "MK Utilities")
	static float MoveTowards(float From, float To, float MaxDelta);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Delta Angle", Keywords = "Angle Delta"), Category = "MK Utilities")
	static float DeltaAngle(float From, float To);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Repeat Float", Keywords = "Repeat Float"), Category = "MK Utilities")
	static float RepeatFloat(float T, float Length);

	UFUNCTION(BluePrintPure, meta = (DisplayName = "Point Is Inside Box", Keywords = "Point Inside Box"), Category = "MK Utilities")
	static bool PointIsInsideBox(const FBox2D& Box, const FVector2D& Point);

	UFUNCTION(BluePrintPure, meta = (DisplayName = "Box Is Inside Box", Keywords = "Box Inside Box2D FBox2D"), Category = "MK Utilities")
	static bool BoxIsInsideBox(const FBox2D& Box, const FBox2D& OtherBox);

	UFUNCTION(BluePrintPure, meta = (DisplayName = "Box Intersects Box", Keywords = "Box Intersect Box2D FBox2D"), Category = "MK Utilities")
	static bool BoxIntersectsBox(const FBox2D& Box, const FBox2D& OtherBox);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Min and Max of Array (float)", CommutativeAssociativeBinaryOperator = "true"), Category = "MK Utilities")
	static void MinAndMaxOfFloatArray(const TArray<float>& Floats, float& Min, float& Max);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Make Literal Box2D"), Category = "MK Utilities")
	static FBox2D MakeLiteralFBox2D(FVector2D Min, FVector2D Max);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Vector is nearly zero", Keywords = "Vector FVector Zero Nearly"), Category = "MK Utilities")
	static bool VectorIsNearlyZero(const FVector& Vector);

	UFUNCTION(BluePrintPure, meta = (DisplayName = "Get Actor Screen Box", Keywords = "Actor Screen Box FBox2D"), Category = "MK Utilities")
	static void GetActorScreenBox(const APlayerController* Controller, const AActor* TargetActor, bool bOnlyCollidingComponents, bool bScaleByDPI, FBox2D& Box, bool& bAllProjectedSuccessfully);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Remove All Children", Keywords = "Clear Destroy Remove Kill Child Children"), Category = "MK Utilities")
	static void RemoveAllChildren(USceneComponent* parentComp);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Is World Point Inside Box", Keywords = "World Point Inside Within Intersect Box"), Category = "MK Utilities")
	static bool WorldPointIsInsideBox(const FVector& WorldPoint, const FVector& BoxCenter, const FVector& BoxExtents);


	/** Easing functions */

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
	static const float EaseInterpolate(const EasingType EaseType, const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
	static const float EaseLinear(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
	static const float EaseSineIn(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
	static const float EaseSineOut(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
	static const float EaseSineInOut(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
	static const float EaseQuadIn(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
	static const float EaseQuadOut(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
	static const float EaseQuadInOut(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
	static const float EaseCubicIn(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
	static const float EaseCubicOut(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
	static const float EaseCubicInOut(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
	static const float EaseQuartIn(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
	static const float EaseQuartOut(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
	static const float EaseQuartInOut(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
	static const float EaseQuintIn(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
	static const float EaseQuintOut(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
	static const float EaseQuintInOut(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
	static const float EaseExpoIn(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
	static const float EaseExpoOut(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
	static const float EaseExpoInOut(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
	static const float EaseCircIn(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
	static const float EaseCircOut(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
	static const float EaseCircInOut(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
	static const float EaseBackIn(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
	static const float EaseBackOut(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
	static const float EaseBackInOut(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
	static const float EaseElasticIn(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
	static const float EaseElasticOut(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
	static const float EaseElasticInOut(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
	static const float EaseBounceIn(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
	static const float EaseBounceOut(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
	static const float EaseBounceInOut(const float NormalizedTime, const float From, const float To);
};