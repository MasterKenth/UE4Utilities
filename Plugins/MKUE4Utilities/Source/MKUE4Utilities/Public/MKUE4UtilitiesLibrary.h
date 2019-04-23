#pragma once

#include "Engine.h"
#include "MKUE4UtilitiesLibrary.generated.h"

UENUM(BlueprintType)
enum class EEasingType : uint8
{
	Linear UMETA(DisplayName = "Linear"),
	SineIn UMETA(DisplayName = "Sine In"),
	SineOut UMETA(DisplayName = "Sine Out"),
	SineInOut UMETA(DisplayName = "Sine In Out"),
	QuadIn UMETA(DisplayName = "Quad In"),
	QuadOut UMETA(DisplayName = "Quad Out"),
	QuadInOut UMETA(DisplayName = "Quad In Out"),
	CubicIn UMETA(DisplayName = "Cubic In"),
	CubicOut UMETA(DisplayName = "Cubic Out"),
	CubicInOut UMETA(DisplayName = "Cubic In Out"),
	QuartIn UMETA(DisplayName = "Quartic In"),
	QuartOut UMETA(DisplayName = "Quartic Out"),
	QuartInOut UMETA(DisplayName = "Quartic In Out"),
	QuintIn UMETA(DisplayName = "Quintic In"),
	QuintOut UMETA(DisplayName = "Quintic Out"),
	QuintInOut UMETA(DisplayName = "Quintic In Out"),
	ExpoIn UMETA(DisplayName = "Exponential In"),
	ExpoOut UMETA(DisplayName = "Exponential Out"),
	ExpoInOut UMETA(DisplayName = "Exponential In Out"),
	CircIn UMETA(DisplayName = "Circular In"),
	CircOut UMETA(DisplayName = "Circular Out"),
	CircInOut UMETA(DisplayName = "Circular In Out"),
	BackIn UMETA(DisplayName = "Back In"),
	BackOut UMETA(DisplayName = "Back Out"),
	BackInOut UMETA(DisplayName = "Back In Out"),
	ElasticIn UMETA(DisplayName = "Elastic In"),
	ElasticOut UMETA(DisplayName = "Elastic Out"),
	ElasticInOut UMETA(DisplayName = "Elastic In Out"),
	BounceIn UMETA(DisplayName = "Bounce In"),
	BounceOut UMETA(DisplayName = "Bounce Out"),
	BounceInOut UMETA(DisplayName = "Bounce In Out"),

	SineOutIn UMETA(DisplayName = "Sine Out In"),
	QuadOutIn UMETA(DisplayName = "Quad Out In"),
	CubicOutIn UMETA(DisplayName = "Cubic Out In"),
	QuartOutIn UMETA(DisplayName = "Quartic Out In"),
	QuintOutIn UMETA(DisplayName = "Quintic Out In"),
	ExpoOutIn UMETA(DisplayName = "Exponential Out In"),
	CircOutIn UMETA(DisplayName = "Circular Out In"),
	BackOutIn UMETA(DisplayName = "Back Out In"),
	ElasticOutIn UMETA(DisplayName = "Elastic Out In"),
	BounceOutIn UMETA(DisplayName = "Bounce Out In"),
};

DECLARE_DELEGATE_OneParam(FMKTweenDelegate, float);
DECLARE_DYNAMIC_DELEGATE_OneParam(FMKTweenDynamicDelegate, float, TweenedValue);

struct FMKUnifiedDelegate
{
	FMKTweenDelegate FuncDelegate;
	FMKTweenDynamicDelegate FuncDynDelegate;

	FMKUnifiedDelegate() {};
	FMKUnifiedDelegate(FMKTweenDelegate const& D) : FuncDelegate(D) {};
	FMKUnifiedDelegate(FMKTweenDynamicDelegate const& D) : FuncDynDelegate(D) {};

	inline void Execute(float val)
	{
		if (FuncDelegate.IsBound())
		{
			FuncDelegate.Execute(val);
		}
		else if (FuncDynDelegate.IsBound())
		{
			FuncDynDelegate.Execute(val);
		}
	}

	inline bool IsBound() const
	{
		return (FuncDelegate.IsBound() || FuncDynDelegate.IsBound());
	}

	inline void Unbind()
	{
		FuncDelegate.Unbind();
		FuncDynDelegate.Unbind();
	}
};

UCLASS()
class MKUE4UTILITIES_API UMKUE4UtilityLibrary : public UBlueprintFunctionLibrary
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

	// C++ version of simple array shuffle
	template<typename T>
	FORCEINLINE static void ShuffleArray(TArray<T>& TargetArray)
	{
		for (int32 i = TargetArray.Num() - 1; i >= 0; i--)
		{
			int32 r = FMath::RandRange(0, TargetArray.Num() - 1);
			if (i != r) TargetArray.Swap(i, r);
		}
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

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Min and Max of Array (float)"), Category = "MK Utilities")
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
	static const float EaseInterpolate(const EEasingType EaseType, const float NormalizedTime, const float From, const float To);

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

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
		static const float EaseSineOutIn(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
		static const float EaseQuadOutIn(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
		static const float EaseCubicOutIn(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
		static const float EaseQuartOutIn(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
		static const float EaseQuintOutIn(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
		static const float EaseExpoOutIn(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
		static const float EaseCircOutIn(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
		static const float EaseBackOutIn(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
		static const float EaseElasticOutIn(const float NormalizedTime, const float From, const float To);

	UFUNCTION(BlueprintPure, Category = "MK Utilities|Math|Easing")
		static const float EaseBounceOutIn(const float NormalizedTime, const float From, const float To);


	UFUNCTION(BlueprintCallable, Category = "MK Utilities|Tween", meta = (DisplayName = "Tween Custom"))
	static int32 TweenCustomBP(const EEasingType EaseType, float Duration, float From, float To, const FMKTweenDynamicDelegate& TweenDelegate);

	static int32 TweenCustom(const EEasingType EaseType, float Duration, float From, float To, const FMKTweenDelegate& TweenDelegate);

	UFUNCTION(BlueprintCallable, Category = "MK Utilities|Tween")
	static void StopTween(int32 TweenID);

	/** C++ only functions */

	template<typename T>
	static FString EnumToString(const FString& enumName, const T value);
};

USTRUCT()
struct MKUE4UTILITIES_API FMKTweenDataStruct
{
	GENERATED_BODY()

public:

	FMKTweenDataStruct()
	{
	}

	FMKTweenDataStruct(EEasingType EaseType, float Duration, float From, float To, FMKUnifiedDelegate TweenDelegate)
		: TweenID(-1),
		EaseType(EaseType),
		Duration(Duration),
		From(From),
		To(To),
		TweenDelegate(TweenDelegate),
		ElapsedTime(0)
	{
	}

	FORCEINLINE bool operator==(const FMKTweenDataStruct& Other) const
	{
		return Other.TweenID == TweenID;
	}

	int32 TweenID;
	EEasingType EaseType;
	FMKUnifiedDelegate TweenDelegate;
	float Duration;
	float From;
	float To;
	float ElapsedTime;
};

UCLASS()
class MKUE4UTILITIES_API UMKTweenManager : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	static UMKTweenManager& Get();

	void Tick(float DeltaTime) override;
	bool IsTickable() const override;
	bool IsTickableInEditor() const override;
	bool IsTickableWhenPaused() const override;
	TStatId GetStatId() const override;

	UWorld* GetWorld() const override;

	int32 AddTween(FMKTweenDataStruct& NewTween);
	void StopTween(int32 TweenID);

private:
	static UMKTweenManager* Instance;

	TArray<FMKTweenDataStruct> ActiveTweens;
	int32 TweenCounter;
};