// Fill out your copyright notice in the Description page of Project Settings.

#include "FireArmComponent.h"

#include "HitEffectDefinition.h"


UFireArmComponent::UFireArmComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void UFireArmComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// 총기 스켈레탈 메시 할당 및 초기화
	FireArmMesh = GetOwner()->GetComponentByClass<USkeletalMeshComponent>();
}

void UFireArmComponent::Fire()
{
	ServerFire();
}

void UFireArmComponent::FireToTarget(const FVector& Target)
{
	if(bUseFireCorrection)
		ServerFireToTarget(Target);
	else
		ServerFire();
}

void UFireArmComponent::ServerFire_Implementation()
{
	// 총기 소켓 Transform 가져오기
	const FVector MuzzleSocketLocation = GetMuzzleSocketLocation();
	const FRotator DirectionRotator = GetMuzzleSocketRotation();

	// 총기 발사
	if(bUseHitScan || ProjectileClass == nullptr)
		HitScan(MuzzleSocketLocation, DirectionRotator.Vector());
	else
		SpawnProjectile(MuzzleSocketLocation, DirectionRotator);

	// 멀티캐스트
	MulticastFire();
}

void UFireArmComponent::ServerFireToTarget_Implementation(const FVector_NetQuantize& Target)
{
	// 총기 소켓 Transform 가져오기
	const FVector MuzzleSocketLocation = GetMuzzleSocketLocation();
	const FVector Direction = (Target - MuzzleSocketLocation).GetSafeNormal();

	// 총기 발사
	if(bUseHitScan || ProjectileClass == nullptr)
		HitScan(MuzzleSocketLocation, Direction);
	else
		SpawnProjectile(MuzzleSocketLocation, Direction.Rotation());

	// 멀티캐스트
	MulticastFire();
}

void UFireArmComponent::MulticastFire_Implementation()
{
	// 총기 애니메이션 재생
	if(FireArmMesh != nullptr)
	{
		FireArmMesh->PlayAnimation(FireAnimation, false);
	}
}

void UFireArmComponent::MulticastHitResult_Implementation(const FHitResult& HitResult)
{
	const FHitEffectContext HitEffectContext
	{
		HitResult.GetComponent(),
		HitResult.ImpactPoint,
		HitResult.ImpactNormal
	};
	
	HitEffectDefinition->SpawnHitEffectAttached(GetWorld(), HitEffectDefinition, HitEffectContext);
}

void UFireArmComponent::SpawnProjectile(const FVector& MuzzleSocketLocation, const FRotator& DirectionRotator)
{
	// 설정 유효성 검사
	if(ProjectileClass == nullptr) return;

	// World 유효성 검사
	UWorld* World = GetWorld();
	if(World == nullptr) return;
	
	// 스폰 정보 작성
	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.Owner = GetOwner();
	ActorSpawnParameters.Instigator = ActorSpawnParameters.Owner->GetInstigator();

	// 총알 스폰
	World->SpawnActor(ProjectileClass, &MuzzleSocketLocation, &DirectionRotator, ActorSpawnParameters);
}

void UFireArmComponent::HitScan(const FVector& MuzzleSocketLocation, const FVector& Direction)
{
	// World 유효성 검사
	UWorld* World = GetWorld();
	if(World == nullptr) return;
	
	// 라인 트레이스
	const FVector End = MuzzleSocketLocation + Direction * MaxRange;
		
	FHitResult HitResult;
	FCollisionQueryParams CollisionParameters;
	World->LineTraceSingleByChannel(
		HitResult,
		MuzzleSocketLocation,
		End,
		CollisionChannel
		);

	// 히트 시 HitResult 멀티캐스트
	if(HitResult.bBlockingHit) MulticastHitResult(HitResult);

#if WITH_EDITOR
	if(bDrawDebugLine) DrawDebugLine(World, MuzzleSocketLocation, End, FColor(255, 0 ,0), false, 3, 0, 0.5);
#endif
}
