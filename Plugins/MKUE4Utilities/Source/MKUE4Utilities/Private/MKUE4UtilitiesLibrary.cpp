#include "MKUE4UtilitiesPrivatePCH.h"
#include "MKUE4UtilitiesLibrary.h"
#include "Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h"

void UMKUE4UtilityLibrary::ShuffleArrayWithStream(const TArray<int32>& TargetArray, const FRandomStream& Stream)
{
	UE_LOG(MKUE4UtilitiesLog, Error, TEXT("Stub shuffle func called - should not happen"));
	check(0);
}

void UMKUE4UtilityLibrary::ShuffleArrayWithStream_impl(void* TargetArray, const UArrayProperty* ArrayProperty, const FRandomStream& Stream)
{
	if (TargetArray)
	{
		FScriptArrayHelper ArrayHelper(ArrayProperty, TargetArray);
		int32 LastIndex = ArrayHelper.Num() - 1;

		for (int32 i = 0; i < LastIndex; ++i)
		{
			int32 Index = Stream.RandRange(0, LastIndex);
			if (i != Index)
			{
				ArrayHelper.SwapValues(i, Index);
			}
		}
	}
}

bool UMKUE4UtilityLibrary::SetMousePosition(APlayerController* PC, const float& PosX, const float& PosY)
{
	if (!PC) return false;

	ULocalPlayer* LPlayer = Cast<ULocalPlayer>(PC->Player);

	if (!LPlayer) return false;

	UGameViewportClient* ViewportClient = Cast<UGameViewportClient>(LPlayer->ViewportClient);

	if (!ViewportClient) return false;

	FViewport* Viewport = ViewportClient->Viewport;

	if (!Viewport) return false;

	Viewport->SetMouse(int32(PosX), int32(PosY));

	return true;
}

float UMKUE4UtilityLibrary::GetActorLastRenderTime(AActor * RefActor)
{
	return RefActor->GetLastRenderTime();
}

bool UMKUE4UtilityLibrary::WasActorRenderedRecently(AActor* RefActor, float MaxRecentTime)
{
	float RenderTime = GetActorLastRenderTime(RefActor) + MaxRecentTime;
	float TimeSinceStartup = UGameplayStatics::GetRealTimeSeconds(RefActor->GetWorld());
	return (RenderTime >= TimeSinceStartup);
}

bool UMKUE4UtilityLibrary::IsActorWithinFrustum(AActor* RefActor)
{
	FVector ActorBoundsOrigin;
	FVector ActorBoundsExtents;
	RefActor->GetActorBounds(false, ActorBoundsOrigin, ActorBoundsExtents);
	
	ULocalPlayer* LocalPlayer = RefActor->GetWorld()->GetFirstLocalPlayerFromController();
	
	FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(
		LocalPlayer->ViewportClient->Viewport,
		RefActor->GetWorld()->Scene,
		LocalPlayer->ViewportClient->EngineShowFlags
	).SetRealtimeUpdate(true));
	
	FVector ViewLocation;
	FRotator ViewRotation;
	FSceneView* SceneView = LocalPlayer->CalcSceneView(
		&ViewFamily,
		ViewLocation,
		ViewRotation,
		LocalPlayer->ViewportClient->Viewport
	);
	
	return SceneView->ViewFrustum.IntersectBox(ActorBoundsOrigin, ActorBoundsExtents);
}

TArray<FColor> UMKUE4UtilityLibrary::ConvertLinearColors(const TArray<FLinearColor>& LinearColors)
{
	TArray<FColor> OutColors = TArray<FColor>();

	for (int32 i = 0; i < LinearColors.Num(); i++)
	{
		OutColors.Add(LinearColors[i].ToFColor(true));
	}

	return OutColors;
}

bool UMKUE4UtilityLibrary::HasRuntimeCooking()
{
#ifdef WITH_RUNTIME_PHYSICS_COOKING
	return true;
#else
	return false;
#endif
}

FVector UMKUE4UtilityLibrary::MoveTowardsVector(FVector From, FVector To, float MaxDistanceDelta)
{
	FVector DiffVector = To - From;
	float Length = DiffVector.Size();

	if (Length <= 0.0f || (double)Length <= (double)MaxDistanceDelta)
	{
		return To;
	}

	return From + DiffVector / Length * MaxDistanceDelta;
}

FRotator UMKUE4UtilityLibrary::MoveTowardsRotator(FRotator From, FRotator To, float MaxDistanceDelta)
{
	// NOTE: There is probably a better implementation for this
	float X, Y, Z;
	X = MoveTowardsAngle(From.Pitch, To.Pitch, MaxDistanceDelta);
	Y = MoveTowardsAngle(From.Yaw, To.Yaw, MaxDistanceDelta);
	Z = MoveTowardsAngle(From.Roll, To.Roll, MaxDistanceDelta);
	return FRotator(X, Y, Z);
}

float UMKUE4UtilityLibrary::MoveTowardsAngle(float From, float To, float MaxDelta)
{
	To = From + DeltaAngle(From, To);
	return MoveTowards(From, To, MaxDelta);
}

float UMKUE4UtilityLibrary::MoveTowards(float From, float To, float MaxDelta)
{
	if ((double)FMath::Abs(To - From) <= (double)MaxDelta)
		return To;
	return From + FMath::Sign(To - From) * MaxDelta;
}

float UMKUE4UtilityLibrary::DeltaAngle(float From, float To)
{
	float Num = RepeatFloat(To - From, 360.0f);
	if ((double)Num > 180.0)
		Num -= 360.0f;
	return Num;
}

float UMKUE4UtilityLibrary::RepeatFloat(float T, float Length)
{
	return T - FMath::FloorToInt(T / Length) * Length;
}

bool UMKUE4UtilityLibrary::PointIsInsideBox(const FBox2D& Box, const FVector2D& Point)
{
	return Box.IsInside(Point);
}

bool UMKUE4UtilityLibrary::BoxIsInsideBox(const FBox2D& Box, const FBox2D& OtherBox)
{
	return Box.IsInside(OtherBox);
}

bool UMKUE4UtilityLibrary::BoxIntersectsBox(const FBox2D& Box, const FBox2D& OtherBox)
{
	return Box.Intersect(OtherBox);
}

void UMKUE4UtilityLibrary::MinAndMaxOfFloatArray(const TArray<float>& Floats, float& Min, float& Max)
{
	check(Floats.Num() > 0);

	Min = Floats[0];
	Max = Floats[0];

	if (Floats.Num() > 1)
	{
		for (int32 i = 1; i < Floats.Num(); i++)
		{
			Min = FMath::Min(Floats[i], Min);
			Max = FMath::Max(Floats[i], Max);
		}
	}
}

FBox2D UMKUE4UtilityLibrary::MakeLiteralFBox2D(FVector2D Min, FVector2D Max)
{
	return FBox2D(Min, Max);
}

bool UMKUE4UtilityLibrary::VectorIsNearlyZero(const FVector& Vector)
{
	return Vector.IsNearlyZero();
}

void UMKUE4UtilityLibrary::GetActorScreenBox(const APlayerController* Controller, const AActor* TargetActor, bool bOnlyCollidingComponents, bool bScaleByDPI, FBox2D& Box, bool& bAllProjectedSuccessfully)
{
	ensure(Controller != NULL);
	ensure(TargetActor != NULL);


	// Get Actor Bounds
	FVector ActorOrigin;
	FVector ActorExtents;
	TargetActor->GetActorBounds(bOnlyCollidingComponents, ActorOrigin, ActorExtents);

	// Make box vertices
	FVector Top_Left_Forward = ActorOrigin + FVector(-ActorExtents.X, ActorExtents.Y, ActorExtents.Z);	// -++
	FVector Top_Right_Forward = ActorOrigin + FVector(ActorExtents.X, ActorExtents.Y, ActorExtents.Z);	// +++
	FVector Top_Right_Back = ActorOrigin + FVector(-ActorExtents.X, -ActorExtents.Y, ActorExtents.Z);	// +-+
	FVector Top_Left_Back = ActorOrigin + FVector(-ActorExtents.X, -ActorExtents.Y, ActorExtents.Z);	// --+

	FVector Bottom_Left_Forward = ActorOrigin + FVector(-ActorExtents.X, ActorExtents.Y, -ActorExtents.Z);	// -+-
	FVector Bottom_Right_Forward = ActorOrigin + FVector(ActorExtents.X, ActorExtents.Y, -ActorExtents.Z);	// ++-
	FVector Bottom_Right_Back = ActorOrigin + FVector(ActorExtents.X, -ActorExtents.Y, -ActorExtents.Z);	// +--
	FVector Bottom_Left_Back = ActorOrigin + FVector(-ActorExtents.X, -ActorExtents.Y, -ActorExtents.Z);	// ---

																											// Convert to screen space
	FVector2D TLF;
	FVector2D TRF;
	FVector2D TRB;
	FVector2D TLB;

	FVector2D BLF;
	FVector2D BRF;
	FVector2D BRB;
	FVector2D BLB;

	bool bTLF = Controller->ProjectWorldLocationToScreen(Top_Left_Forward, TLF);
	bool bTRF = Controller->ProjectWorldLocationToScreen(Top_Right_Forward, TRF);
	bool bTRB = Controller->ProjectWorldLocationToScreen(Top_Right_Back, TRB);
	bool bTLB = Controller->ProjectWorldLocationToScreen(Top_Left_Back, TLB);

	bool bBLF = Controller->ProjectWorldLocationToScreen(Bottom_Left_Forward, BLF);
	bool bBRF = Controller->ProjectWorldLocationToScreen(Bottom_Right_Forward, BRF);
	bool bBRB = Controller->ProjectWorldLocationToScreen(Bottom_Right_Back, BRB);
	bool bBLB = Controller->ProjectWorldLocationToScreen(Bottom_Left_Back, BLB);

	bAllProjectedSuccessfully = (bTLF&& bTRF&& bTRB&& bTLB&& bBLF&& bBRF&& bBRB&& bBLB);

	TArray<float> Xs;

	Xs.Add(TLF.X);
	Xs.Add(TRF.X);
	Xs.Add(TRB.X);
	Xs.Add(TLB.X);

	Xs.Add(BLF.X);
	Xs.Add(BRF.X);
	Xs.Add(BRB.X);
	Xs.Add(BLB.X);

	TArray<float> Ys;

	Ys.Add(TLF.Y);
	Ys.Add(TRF.Y);
	Ys.Add(TRB.Y);
	Ys.Add(TLB.Y);

	Ys.Add(BLF.Y);
	Ys.Add(BRF.Y);
	Ys.Add(BRB.Y);
	Ys.Add(BLB.Y);

	// Optionally scale by DPI
	if (bScaleByDPI)
	{
		float DPIScale = UWidgetLayoutLibrary::GetViewportScale((UObject*)Controller);

		for (int32 i = 0; i < Xs.Num(); i++)
		{
			Xs[i] = Xs[i] / DPIScale;
		}

		for (int32 i = 0; i < Ys.Num(); i++)
		{
			Ys[i] = Ys[i] / DPIScale;
		}
	}

	// Get the extremes
	float Xmin, Xmax, Ymin, Ymax;
	MinAndMaxOfFloatArray(Xs, Xmin, Xmax);
	MinAndMaxOfFloatArray(Ys, Ymin, Ymax);

	// Construct final screen box
	FVector2D MinValues = FVector2D(Xmin, Ymin);
	FVector2D MaxValues = FVector2D(Xmax, Ymax);

	Box = FBox2D(MinValues, MaxValues);
}

void UMKUE4UtilityLibrary::RemoveAllChildren(USceneComponent* parentComp)
{
	if (parentComp == nullptr)
	{
		return;
	}

	while (parentComp->GetNumChildrenComponents() > 0)
	{
		parentComp->GetChildComponent(0)->DestroyComponent();
	}
}

bool UMKUE4UtilityLibrary::WorldPointIsInsideBox(const FVector& WorldPoint, const FVector& BoxCenter, const FVector& BoxExtents)
{
	return
		(WorldPoint.X >= BoxCenter.X - BoxExtents.X) && (WorldPoint.X <= BoxCenter.X + BoxExtents.X) &&
		(WorldPoint.Y >= BoxCenter.Y - BoxExtents.Y) && (WorldPoint.Y <= BoxCenter.Y + BoxExtents.Y) &&
		(WorldPoint.Z >= BoxCenter.Z - BoxExtents.Z) && (WorldPoint.Z <= BoxCenter.Z + BoxExtents.Z);
}