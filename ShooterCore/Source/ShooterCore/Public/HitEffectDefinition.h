// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HitEffectDefinition.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FHitEffectConfig
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UFXSystemAsset* VisualEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* SoundEffect;
};

USTRUCT(Atomic, BlueprintType)
struct FHitEffectContext
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* AttachToComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ImpactPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ImpactNormal;

	// TODO 피지컬 에셋
};

UCLASS(BlueprintType)
class SHOOTERCORE_API UHitEffectDefinition : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(Category = "Config", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FHitEffectConfig DefaultHitEffect;

	// TODO 피지컬 에셋별 이펙트

public:
	UFUNCTION(BlueprintCallable)
	static void SpawnHitEffectAttached(const UObject* WorldContextObject, const UHitEffectDefinition* Definition, const FHitEffectContext& Context);
};
