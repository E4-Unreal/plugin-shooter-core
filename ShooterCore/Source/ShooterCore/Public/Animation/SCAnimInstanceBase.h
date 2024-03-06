// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SCAnimInstanceBase.generated.h"

class UCharacterMovementComponent;
/**
 * 속도, 방향 등 대부분의 캐릭터 애니메이션 블루프린트에서 필요한 값들을 멤버 변수로 지닌 클래스입니다.
 */
UCLASS()
class SHOOTERCORE_API USCAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Dependency", meta = (AllowPrivateAccess = true))
	TObjectPtr<ACharacter> Character;

	UPROPERTY(BlueprintReadOnly, Category = "Dependency", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCharacterMovementComponent> CharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = true))
	FVector Velocity;

	UPROPERTY(BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = true))
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = true))
	float Direction;

	UPROPERTY(BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = true))
	bool bShouldMove;

	UPROPERTY(BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = true))
	bool bFalling;

	UPROPERTY(BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = true))
	bool bCrouching;

protected:
	UFUNCTION(BlueprintPure)
	FORCEINLINE ACharacter* GetCharacter() const { return Character; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE UCharacterMovementComponent* GetCharacterMovement() const { return CharacterMovement; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE FVector GetVelocity() const { return Velocity; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetGroundSpeed() const { return GroundSpeed; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetDirection() const { return Direction; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsShouldMove() const { return bShouldMove; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsFalling() const { return bFalling; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsCrouching() const { return bCrouching; }

public:
	/* AnimInstance */

	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
};
