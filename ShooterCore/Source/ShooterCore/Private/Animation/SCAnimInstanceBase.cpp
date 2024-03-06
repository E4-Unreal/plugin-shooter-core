// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/SCAnimInstanceBase.h"

#include "KismetAnimationLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void USCAnimInstanceBase::NativeInitializeAnimation()
{
	Character = Cast<ACharacter>(GetOwningActor());
	
	if(Character)
		CharacterMovement = Character->GetCharacterMovement();
}

void USCAnimInstanceBase::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	// 유효성 검사
	if(Character == nullptr) return;

	/* CharacterMovement */

	// 속도
	Velocity = CharacterMovement->GetLastUpdateVelocity();
	GroundSpeed = FVector(Velocity.X, Velocity.Y, 0.f).Length();
	bShouldMove = !CharacterMovement->GetCurrentAcceleration().IsNearlyZero() && GroundSpeed > 3.f;
	Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, Character->GetActorRotation());

	// 상태
	bFalling = CharacterMovement->IsFalling();
	bCrouching = CharacterMovement->IsCrouching();

	// 에임 오프셋
	const FRotator TargetRotator = (Character->GetControlRotation() - Character->GetActorRotation()).GetNormalized();
	const FRotator CurrentRotator = FRotator(AimPitch, AimYaw, 0.f);
	const FRotator DesiredRotator = FMath::RInterpTo(CurrentRotator, TargetRotator, DeltaSeconds, InterpSpeed);
	
	AimPitch = FMath::Clamp(DesiredRotator.Pitch, -90.f, 90.f);
	AimYaw = FMath::Clamp(DesiredRotator.Yaw, -90.f, 90.f);
	AimYaw += AimYaw - DesiredRotator.Yaw; // 초과한 각도만큼 다시 원복함으로써 -180 에서 180 으로 한 번에 넘어가는 것을 방지
}
