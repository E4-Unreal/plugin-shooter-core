// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FireArmComponent.generated.h"


class UHitEffectDefinition;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHOOTERCORE_API UFireArmComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(Category = "Config", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<UHitEffectDefinition> HitEffectDefinition;

	// 총알 액터 클래스를 설정합니다. 설정되지 않은 경우 bHitScan 변수 값과 관계없이 히트 스캔 방식으로 동작합니다.
	UPROPERTY(Category = "Config", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<AActor> ProjectileClass;

	// 총기 스켈레탈 메시에 설정된 총구 소켓 이름
	UPROPERTY(Category = "Config", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FName MuzzleSocketName = "Muzzle";

	// 총기 애니메이션
	UPROPERTY(Category = "Config", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimationAsset> FireAnimation;

	// true 설정 시 FireToTarget 을 호출해도 Fire 가 대신 호출됩니다.
	UPROPERTY(Category = "Config", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool bUseFireCorrection = true;

	// true 설정 시 ProjectileClass 가 설정되어 있어도 히트 스캔 방식으로 동작합니다.
	UPROPERTY(Category = "Config|HitScan", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool bUseHitScan = false;

	// 히트 스캔 시 최대 발사 거리
	UPROPERTY(Category = "Config|HitScan", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float MaxRange = 200000.f;

	UPROPERTY(Category = "Config|HitScan", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TEnumAsByte<ECollisionChannel> CollisionChannel = ECC_Visibility;

#if WITH_EDITORONLY_DATA
	UPROPERTY(Category = "Config|Debug", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool bDrawDebugLine = false;
#endif

	// 총기 스켈레탈 메시
	UPROPERTY(Category = "Dependency", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USkeletalMeshComponent> FireArmMesh;

public:
	// 생성자
	UFireArmComponent();

	// 초기화
	virtual void InitializeComponent() override;

	// 총구 방향과 나란하게 총기 발사
	UFUNCTION(BlueprintCallable)
	void Fire();

	// 목표 지점을 향해 총기 발사
	UFUNCTION(BlueprintCallable)
	void FireToTarget(const FVector& Target);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void ToggleFireCorrection() { bUseFireCorrection = !bUseFireCorrection; }

protected:
	// 총구 방향과 나란하게 총기 발사
	UFUNCTION(Server, Reliable)
	virtual void ServerFire();
	
	// 목표 지점을 향해 총기 발사
	UFUNCTION(Server, Reliable)
	virtual void ServerFireToTarget(const FVector_NetQuantize& Target);

	// 총기 애니메이션 재생
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastFire();

	// 히트 스캔 결과 멀티캐스트
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHitResult(const FHitResult& HitResult);

	UFUNCTION()
	virtual void SpawnProjectile(const FVector& MuzzleSocketLocation, const FRotator& DirectionRotator);

	UFUNCTION()
	virtual void HitScan(const FVector& MuzzleSocketLocation, const FVector& Direction);

private:
	UFUNCTION(BlueprintPure)
	FORCEINLINE FVector GetMuzzleSocketLocation() const { return FireArmMesh == nullptr ? GetOwner()->GetActorLocation() : FireArmMesh->GetSocketLocation(MuzzleSocketName); }

	UFUNCTION(BlueprintPure)
	FORCEINLINE FRotator GetMuzzleSocketRotation() const { return FireArmMesh == nullptr ? GetOwner()->GetActorRotation() : FireArmMesh->GetSocketRotation(MuzzleSocketName); }
};
