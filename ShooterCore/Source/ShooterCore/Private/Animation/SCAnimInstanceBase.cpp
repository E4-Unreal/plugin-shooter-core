// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/SCAnimInstanceBase.h"

#include "KismetAnimationLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

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
}
