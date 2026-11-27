// Copyright Epic Games, Inc. All Rights Reserved.

#include "CITA417FINALCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "CITA417FINAL.h"
#include "WeaponComponent.h"

ACITA417FINALCharacter::ACITA417FINALCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
	Weapon = CreateDefaultSubobject<UWeaponComponent>(TEXT("Weapon"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ACITA417FINALCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACITA417FINALCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ACITA417FINALCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACITA417FINALCharacter::Look);
		
		if (FireAction)
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ACITA417FINALCharacter::OnFirePressed);
		else
			UE_LOG(LogTemp, Warning, TEXT("FireAction not set on %s"), *GetName());
	}
	else
	{
		UE_LOG(LogCITA417FINAL, Error, TEXT("'%s' Failed to find an Enhanced Input component! ..."), *GetNameSafe(this));
	}
}

void ACITA417FINALCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void ACITA417FINALCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ACITA417FINALCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void ACITA417FINALCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ACITA417FINALCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void ACITA417FINALCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

float ACITA417FINALCharacter::GetMovementSpeed() const
{
	return GetVelocity().Size2D();
}

float ACITA417FINALCharacter::GetMovementDirection() const
{
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	if (Velocity.IsNearlyZero()) return 0.f;
	const FRotator YawOnly(0.f, GetActorRotation().Yaw, 0.f);
	const FVector Local = YawOnly.UnrotateVector(Velocity.GetSafeNormal());
	return FMath::RadiansToDegrees(FMath::Atan2(Local.Y, Local.X));
}

bool ACITA417FINALCharacter::IsAirborne() const
{
	return GetCharacterMovement()->IsFalling();
}

void ACITA417FINALCharacter::OnFirePressed()
{
	if (Weapon) Weapon->Fire();
	else UE_LOG(LogTemp, Warning, TEXT("No WeaponComponent on %s"), *GetName());
}

bool ACITA417FINALCharacter::IsAttacking() const
{
	return Weapon && Weapon->IsFiring();
}
