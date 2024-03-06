// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCAnimInstanceBase.h"
#include "SCAnimInstance.generated.h"

// TODO GameplayTag와 C++ 변수 연동
// 지금은 그냥 애니메이션 블루프린트에서 사용합니다.

/**
 * 애니메이션 블루프린트에서 사용할 ADS 애니메이션 구조체입니다.
 */
USTRUCT()
struct FADSAnimationPair
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TObjectPtr<UAnimSequence> Animation;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TObjectPtr<UAnimSequence> ADSAnimation;

public:
	// AimDownSight(ADS) 상태에 따라 적절한 애니메이션을 반환합니다.
	// 만약 설정된 ADS 애니메이션이 없는 경우에는 ADS 상태와 관계없이 일반 애니메이션을 반환합니다.
	FORCEINLINE static UAnimSequence* Get(const FADSAnimationPair& Pair, bool bADS) { return bADS && Pair.ADSAnimation != nullptr ? Pair.ADSAnimation : Pair.Animation; }
};

/**
 * 애니메이션 블루프린트에서 사용할 ADS 블렌드 스페이스 구조체입니다.
 */
USTRUCT()
struct FADSBlendSpacePair
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TObjectPtr<UBlendSpace> BlendSpace;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TObjectPtr<UBlendSpace> ADSBlendSpace;

public:
	// AimDownSight(ADS) 상태에 따라 적절한 블렌드 스페이스를 반환합니다.
	// 만약 설정된 ADS 블렌드 스페이스가 없는 경우에는 ADS 상태와 관계없이 일반 블렌드 스페이스을 반환합니다.
	FORCEINLINE static UBlendSpace* Get(const FADSBlendSpacePair& Pair, bool bADS) { return bADS && Pair.ADSBlendSpace != nullptr ? Pair.ADSBlendSpace : Pair.BlendSpace; }
};

/**
 * IsAiming 등 Shooter 장르 게임에서 사용하는 값들을 멤버 변수로 지닌 클래스입니다.
 */
UCLASS()
class SHOOTERCORE_API USCAnimInstance : public USCAnimInstanceBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = true))
	bool bADS = true;

	/* 애니메이션 */

	// 기본

	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (AllowPrivateAccess = true))
	FADSAnimationPair IdleAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (AllowPrivateAccess = true))
	FADSBlendSpacePair MoveBlendSpace;

	// 점프
	
	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (AllowPrivateAccess = true))
	FADSAnimationPair JumpAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (AllowPrivateAccess = true))
	FADSAnimationPair FallLoopAnimation;

	// 앉기
	
	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (AllowPrivateAccess = true))
	FADSAnimationPair CrouchIdleAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (AllowPrivateAccess = true))
	FADSBlendSpacePair CrouchMoveBlendSpace;

protected:
	/* 헬퍼 메서드 */
	
	FORCEINLINE UAnimSequence* GetAnimation(const FADSAnimationPair& Pair) const { return FADSAnimationPair::Get(Pair, bADS); }
	FORCEINLINE UBlendSpace* GetBlendSpace(const FADSBlendSpacePair& Pair) const { return FADSBlendSpacePair::Get(Pair, bADS); }
	
	/* Getter */
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsADS() const { return bADS; }
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE UAnimSequence* GetIdleAnimation() const { return GetAnimation(IdleAnimation); }

	UFUNCTION(BlueprintPure)
	FORCEINLINE UBlendSpace* GetMoveBlendSpace() const { return GetBlendSpace(MoveBlendSpace); }

	UFUNCTION(BlueprintPure)
	FORCEINLINE UAnimSequence* GetJumpAnimation() const { return GetAnimation(JumpAnimation); }
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE UAnimSequence* GetFallLoopAnimation() const { return GetAnimation(FallLoopAnimation); }

	UFUNCTION(BlueprintPure)
	FORCEINLINE UAnimSequence* GetCrouchIdleAnimation() const { return GetAnimation(CrouchIdleAnimation); }

	UFUNCTION(BlueprintPure)
	FORCEINLINE UBlendSpace* GetCrouchMoveBlendSpace() const { return GetBlendSpace(CrouchMoveBlendSpace); }
};
