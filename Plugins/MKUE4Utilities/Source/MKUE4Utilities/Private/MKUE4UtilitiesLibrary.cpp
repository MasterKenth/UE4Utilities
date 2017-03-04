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


/** Easing functions */
const float UMKUE4UtilityLibrary::EaseInterpolate(const EasingType EaseType, const float NormalizedTime, const float From, const float To)
{
	switch (EaseType)
	{
		default:
		case ET_Linear: return EaseLinear(NormalizedTime, From, To);
		case ET_SineIn: return EaseSineIn(NormalizedTime, From, To);
		case ET_SineOut: return EaseSineOut(NormalizedTime, From, To);
		case ET_SineInOut: return EaseSineInOut(NormalizedTime, From, To);
		case ET_QuadIn: return EaseQuadIn(NormalizedTime, From, To);
		case ET_QuadOut: return EaseQuadOut(NormalizedTime, From, To);
		case ET_QuadInOut: return EaseQuadInOut(NormalizedTime, From, To);
		case ET_CubicIn: return EaseCubicIn(NormalizedTime, From, To);
		case ET_CubicOut: return EaseCubicOut(NormalizedTime, From, To);
		case ET_CubicInOut: return EaseCubicInOut(NormalizedTime, From, To);
		case ET_QuartIn: return EaseQuartIn(NormalizedTime, From, To);
		case ET_QuartOut: return EaseQuartOut(NormalizedTime, From, To);
		case ET_QuartInOut: return EaseQuartInOut(NormalizedTime, From, To);
		case ET_QuintIn: return EaseQuintIn(NormalizedTime, From, To);
		case ET_QuintOut: return EaseQuintOut(NormalizedTime, From, To);
		case ET_QuintInOut: return EaseQuintInOut(NormalizedTime, From, To);
		case ET_ExpoIn: return EaseExpoIn(NormalizedTime, From, To);
		case ET_ExpoOut: return EaseExpoOut(NormalizedTime, From, To);
		case ET_ExpoInOut: return EaseExpoInOut(NormalizedTime, From, To);
		case ET_CircIn: return EaseCircIn(NormalizedTime, From, To);
		case ET_CircOut: return EaseCircOut(NormalizedTime, From, To);
		case ET_CircInOut: return EaseCircInOut(NormalizedTime, From, To);
		case ET_BackIn: return EaseBackIn(NormalizedTime, From, To);
		case ET_BackOut: return EaseBackOut(NormalizedTime, From, To);
		case ET_BackInOut: return EaseBackInOut(NormalizedTime, From, To);
		case ET_ElasticIn: return EaseElasticIn(NormalizedTime, From, To);
		case ET_ElasticOut: return EaseElasticOut(NormalizedTime, From, To);
		case ET_ElasticInOut: return EaseElasticInOut(NormalizedTime, From, To);
		case ET_BounceIn: return EaseBounceIn(NormalizedTime, From, To);
		case ET_BounceOut: return EaseBounceOut(NormalizedTime, From, To);
		case ET_BounceInOut: return EaseBounceInOut(NormalizedTime, From, To);
	}
}

const float UMKUE4UtilityLibrary::EaseLinear(const float NormalizedTime, const float From, const float To)
{
	return From + (To - From) * NormalizedTime;
}

const float UMKUE4UtilityLibrary::EaseSineIn(const float NormalizedTime, const float From, const float To)
{
	return From + (To - From) * (FPlatformMath::Sin((NormalizedTime - 1) * HALF_PI) + 1);
}

const float UMKUE4UtilityLibrary::EaseSineOut(const float NormalizedTime, const float From, const float To)
{
	return From + (To - From) * (FPlatformMath::Sin(NormalizedTime * HALF_PI));
}

const float UMKUE4UtilityLibrary::EaseSineInOut(const float NormalizedTime, const float From, const float To)
{
	return From + (To - From) * (0.5f * (1 - FPlatformMath::Cos(NormalizedTime * PI)));
}

const float UMKUE4UtilityLibrary::EaseQuadIn(const float NormalizedTime, const float From, const float To)
{
	return From + (To - From) * (NormalizedTime * NormalizedTime);
}

const float UMKUE4UtilityLibrary::EaseQuadOut(const float NormalizedTime, const float From, const float To)
{
	return From + (To - From) * (-(NormalizedTime * (NormalizedTime - 2)));
}

const float UMKUE4UtilityLibrary::EaseQuadInOut(const float NormalizedTime, const float From, const float To)
{
	return From + (To - From) * ((NormalizedTime < 0.5f) ? (2 * NormalizedTime * NormalizedTime) : ((-2 * NormalizedTime * NormalizedTime) + (4 * NormalizedTime) - 1));
}

const float UMKUE4UtilityLibrary::EaseCubicIn(const float NormalizedTime, const float From, const float To)
{
	return From + (To - From) * (NormalizedTime * NormalizedTime * NormalizedTime);
}

const float UMKUE4UtilityLibrary::EaseCubicOut(const float NormalizedTime, const float From, const float To)
{
	return From + (To - From) * ((NormalizedTime - 1) * (NormalizedTime - 1) * (NormalizedTime - 1) + 1);
}

const float UMKUE4UtilityLibrary::EaseCubicInOut(const float NormalizedTime, const float From, const float To)
{
	return From + (To - From) * ((NormalizedTime < 0.5f) ? (4 * NormalizedTime * NormalizedTime * NormalizedTime) : (0.5f * ((2 * NormalizedTime) - 2) * ((2 * NormalizedTime) - 2) * ((2 * NormalizedTime) - 2) + 1));
}

const float UMKUE4UtilityLibrary::EaseQuartIn(const float NormalizedTime, const float From, const float To)
{
	return From + (To - From) * (NormalizedTime * NormalizedTime * NormalizedTime * NormalizedTime);
}

const float UMKUE4UtilityLibrary::EaseQuartOut(const float NormalizedTime, const float From, const float To)
{
	return From + (To - From) * ((NormalizedTime - 1) * (NormalizedTime - 1) * (NormalizedTime - 1) * (1 - NormalizedTime) + 1);
}

const float UMKUE4UtilityLibrary::EaseQuartInOut(const float NormalizedTime, const float From, const float To)
{
	return From + (To - From) * ((NormalizedTime < 0.5f) ? (8 * NormalizedTime * NormalizedTime * NormalizedTime * NormalizedTime) : (-8 * (NormalizedTime - 1) * (NormalizedTime - 1) * (NormalizedTime - 1) * (NormalizedTime - 1) + 1));
}

const float UMKUE4UtilityLibrary::EaseQuintIn(const float NormalizedTime, const float From, const float To)
{
	return From + (To - From) * (NormalizedTime * NormalizedTime * NormalizedTime * NormalizedTime * NormalizedTime);
}

const float UMKUE4UtilityLibrary::EaseQuintOut(const float NormalizedTime, const float From, const float To)
{
	return From + (To - From) * ((NormalizedTime - 1) * (NormalizedTime - 1) * (NormalizedTime - 1) * (NormalizedTime - 1) * (NormalizedTime - 1) + 1);
}

const float UMKUE4UtilityLibrary::EaseQuintInOut(const float NormalizedTime, const float From, const float To)
{
	return From + (To - From) * ((NormalizedTime < 0.5f) ? (16 * NormalizedTime * NormalizedTime * NormalizedTime * NormalizedTime * NormalizedTime) : (0.5f * ((2 * NormalizedTime) - 2) * ((2 * NormalizedTime) - 2) * ((2 * NormalizedTime) - 2) * ((2 * NormalizedTime) - 2) * ((2 * NormalizedTime) - 2) + 1));
}

const float UMKUE4UtilityLibrary::EaseExpoIn(const float NormalizedTime, const float From, const float To)
{
	return From + (To - From) * ((NormalizedTime <= 0.0f) ? (NormalizedTime) : (FPlatformMath::Pow(2, 10 * (NormalizedTime - 1))));
}

const float UMKUE4UtilityLibrary::EaseExpoOut(const float NormalizedTime, const float From, const float To)
{
	return From + (To - From) * ((NormalizedTime >= 1.0f) ? (NormalizedTime) : (1 - FPlatformMath::Pow(2, -10 * NormalizedTime)));
}

const float UMKUE4UtilityLibrary::EaseExpoInOut(const float NormalizedTime, const float From, const float To)
{
	if (NormalizedTime == 1.0f || NormalizedTime == 0.0f) return NormalizedTime;
	return From + (To - From) * 
		((NormalizedTime < 0.5f) ? 
		(0.5f * FPlatformMath::Pow(2, (20 * NormalizedTime) - 10)) : 
		 (-0.5f * FPlatformMath::Pow(2, (-20 * NormalizedTime) + 10) + 1)
		 );
}

const float UMKUE4UtilityLibrary::EaseCircIn(const float NormalizedTime, const float From, const float To)
{
	return From + (To - From) * (1 - FPlatformMath::Sqrt(1 - (NormalizedTime * NormalizedTime)));
}

const float UMKUE4UtilityLibrary::EaseCircOut(const float NormalizedTime, const float From, const float To)
{
	return From + (To - From) * (FPlatformMath::Sqrt((2 - NormalizedTime) * NormalizedTime));
}

const float UMKUE4UtilityLibrary::EaseCircInOut(const float NormalizedTime, const float From, const float To)
{
	return From + (To - From) * ((NormalizedTime < 0.5f) ? (0.5f * (1 - FPlatformMath::Sqrt(1 - 4 * (NormalizedTime * NormalizedTime)))) : (0.5f * (FPlatformMath::Sqrt(-((2 * NormalizedTime) - 3) * ((2 * NormalizedTime) - 1)) + 1)));
}

const float UMKUE4UtilityLibrary::EaseBackIn(const float NormalizedTime, const float From, const float To)
{
	return From + (To - From) * (NormalizedTime * NormalizedTime * NormalizedTime - NormalizedTime * FPlatformMath::Sin(NormalizedTime * PI));
}

const float UMKUE4UtilityLibrary::EaseBackOut(const float NormalizedTime, const float From, const float To)
{
	return From + (To - From) * (1 - ((1 - NormalizedTime) * (1 - NormalizedTime) * (1 - NormalizedTime) - (1 - NormalizedTime) * FPlatformMath::Sin((1 - NormalizedTime) * PI)));
}

const float UMKUE4UtilityLibrary::EaseBackInOut(const float NormalizedTime, const float From, const float To)
{
	return From + (To - From) * ((NormalizedTime < 0.5f) ? (0.5f * ((2 * NormalizedTime) * (2 * NormalizedTime) * (2 * NormalizedTime) - (2 * NormalizedTime) * FPlatformMath::Sin((2 * NormalizedTime) * PI))) : (0.5f * (1 - ((1 - (2 * NormalizedTime - 1)) * (1 - (2 * NormalizedTime - 1)) * (1 - (2 * NormalizedTime - 1)) - (1 - (2 * NormalizedTime - 1)) * FPlatformMath::Sin((1 - (2 * NormalizedTime - 1)) * PI))) + 0.5f));
}

const float UMKUE4UtilityLibrary::EaseElasticIn(const float NormalizedTime, const float From, const float To)
{
	return From + (To - From) * (FPlatformMath::Sin(13 * HALF_PI * NormalizedTime) * FPlatformMath::Pow(2, 10 * (NormalizedTime - 1)));
}

const float UMKUE4UtilityLibrary::EaseElasticOut(const float NormalizedTime, const float From, const float To)
{
	return From + (To - From) * (FPlatformMath::Sin(-13 * HALF_PI * (NormalizedTime + 1)) * FPlatformMath::Pow(2, -10 * NormalizedTime) + 1);
}

const float UMKUE4UtilityLibrary::EaseElasticInOut(const float NormalizedTime, const float From, const float To)
{
	return From + (To - From) * (
		(NormalizedTime < 0.5f) ? 
		(0.5f * FPlatformMath::Sin(13 * HALF_PI * (2 * NormalizedTime)) * FPlatformMath::Pow(2, 10 * ((2 * NormalizedTime) - 1))) : 
		(0.5f* (FPlatformMath::Sin(-13 * HALF_PI * ((2 * NormalizedTime - 1) + 1)) * FPlatformMath::Pow(2, -10 * (2 * NormalizedTime - 1)) + 2)
		 ));
}

const float UMKUE4UtilityLibrary::EaseBounceIn(const float NormalizedTime, const float From, const float To)
{
	float p = (1 - NormalizedTime);
	float x;
	if (p < 4 / 11.0f)
	{
		x = (121 * p * p) / 16.0f;
	}
	else if (p < 8 / 11.0f)
	{
		x = (363 / 40.0f * p * p) - (99 / 10.0f * p) + 17 / 5.0f;
	}
	else if (p < 9 / 10.0f)
	{
		x = (4356 / 361.0f * p * p) - (35442 / 1805.0f * p) + 16061 / 1805.0f;
	}
	else
	{
		x = (54 / 5.0f * p * p) - (513 / 25.0f * p) + 268 / 25.0f;
	}

	return From + (To - From) * (1 - (1 - x));
}

const float UMKUE4UtilityLibrary::EaseBounceOut(const float NormalizedTime, const float From, const float To)
{
	float p = NormalizedTime;
	float x;
	if (p < 4 / 11.0f)
	{
		x = (121 * p * p) / 16.0f;
	}
	else if (p < 8 / 11.0f)
	{
		x = (363 / 40.0f * p * p) - (99 / 10.0f * p) + 17 / 5.0f;
	}
	else if (p < 9 / 10.0f)
	{
		x = (4356 / 361.0f * p * p) - (35442 / 1805.0f * p) + 16061 / 1805.0f;
	}
	else
	{
		x = (54 / 5.0f * p * p) - (513 / 25.0f * p) + 268 / 25.0f;
	}
	return From + (To - From) * (x);
}

const float UMKUE4UtilityLibrary::EaseBounceInOut(const float NormalizedTime, const float From, const float To)
{
	float x;

	if (NormalizedTime < 0.5f)
	{
		float p = (NormalizedTime * 2);
		if (p < 4 / 11.0f)
		{
			x = (121 * p * p) / 16.0f;
		}
		else if (p < 8 / 11.0f)
		{
			x = (363 / 40.0f * p * p) - (99 / 10.0f * p) + 17 / 5.0f;
		}
		else if (p < 9 / 10.0f)
		{
			x = (4356 / 361.0f * p * p) - (35442 / 1805.0f * p) + 16061 / 1805.0f;
		}
		else
		{
			x = (54 / 5.0f * p * p) - (513 / 25.0f * p) + 268 / 25.0f;
		}
		x = 0.5f * x;
	}
	else
	{
		float p = (NormalizedTime * 2 - 1);
		if (p < 4 / 11.0f)
		{
			x = (121 * p * p) / 16.0f;
		}
		else if (p < 8 / 11.0f)
		{
			x = (363 / 40.0f * p * p) - (99 / 10.0f * p) + 17 / 5.0f;
		}
		else if (p < 9 / 10.0f)
		{
			x = (4356 / 361.0f * p * p) - (35442 / 1805.0f * p) + 16061 / 1805.0f;
		}
		else
		{
			x = (54 / 5.0f * p * p) - (513 / 25.0f * p) + 268 / 25.0f;
		}
		x = 0.5f * x + 0.5f;
	}

	return From + (To - From) * x;
}