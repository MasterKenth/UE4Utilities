#pragma once

#include "Engine.h"
#include "MKUE4UtilitiesLibrary.generated.h"

UCLASS()
class UMKUE4UtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	private:

	static void ShuffleArrayWithStream_impl(void* TargetArray, const UArrayProperty* ArrayProperty, const FRandomStream& Stream); // Real implementation


	public:

	UFUNCTION(BlueprintCallable, CustomThunk, meta = (DisplayName = "Shuffle by Stream", CompactNodeTitle = "SHUFFLESTREAM", ArrayParm = "TargetArray"), Category = "MK Utilities")
	static void ShuffleArrayWithStream(const TArray<int32>& TargetArray, const FRandomStream& Stream); // Stub function

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
};