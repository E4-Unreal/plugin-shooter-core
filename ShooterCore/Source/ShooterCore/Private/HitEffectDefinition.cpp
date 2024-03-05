// Fill out your copyright notice in the Description page of Project Settings.


#include "HitEffectDefinition.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"


void UHitEffectDefinition::SpawnHitEffectAttached(const UObject* WorldContextObject, const UHitEffectDefinition* Definition, const FHitEffectContext& Context)
{
	// 유효성 검사
	if(WorldContextObject == nullptr || Definition == nullptr) return;

	// 변수 선언
	const auto& DefaultHitEffect = Definition->DefaultHitEffect;
	const auto& ImpactPoint = Context.ImpactPoint;
	const auto& ImpactNormal = Context.ImpactNormal;
	const auto& AttachToComponent = Context.AttachToComponent;
	
	// 소리 재생
	if(DefaultHitEffect.SoundEffect)
		UGameplayStatics::PlaySoundAtLocation(WorldContextObject, DefaultHitEffect.SoundEffect, ImpactPoint, FRotator::ZeroRotator);

	// 이펙트 스폰
	if(DefaultHitEffect.VisualEffect)
	{
		if(UParticleSystem* ParticleSystem = Cast<UParticleSystem>(DefaultHitEffect.VisualEffect))
		{
			UGameplayStatics::SpawnEmitterAttached(
				ParticleSystem,
				AttachToComponent,
				NAME_None,
				ImpactPoint,
				ImpactNormal.ToOrientationRotator(),
				EAttachLocation::Type::KeepWorldPosition,
				true
				);
		}
		else if(UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(DefaultHitEffect.VisualEffect))
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(
				NiagaraSystem,
				AttachToComponent,
				NAME_None,
				ImpactPoint,
				ImpactNormal.ToOrientationRotator(),
				EAttachLocation::Type::KeepWorldPosition,
				true
				);
		}
	}
}
