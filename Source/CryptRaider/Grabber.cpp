// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetPhysicsHandle() && GetPhysicsHandle()->GetGrabbedComponent())
	{
		const FVector TargetLocation = GetComponentLocation() + GetForwardVector() * GrabCarryDistance;
		GetPhysicsHandle()->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
}

void UGrabber::Grab()
{
	if (GetPhysicsHandle())
	{
		FHitResult HitResult;

		if (GetGrabbableInReach(HitResult))
		{
			UPrimitiveComponent* HitComponent = HitResult.GetComponent();
			if (HitResult.GetActor()->ActorHasTag("Grabbable"))
			{
				HitComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
				HitComponent->WakeAllRigidBodies();
				GetPhysicsHandle()->GrabComponentAtLocationWithRotation(
					HitComponent,
					NAME_None,
					HitResult.ImpactPoint,
					GetComponentRotation());
				HitResult.GetActor()->Tags.Add("Grabbed");
			}
		}
	}
}


void UGrabber::Release()
{
	if (GetPhysicsHandle())
	{
		UPrimitiveComponent* GrabbedComponent = GetPhysicsHandle()->GetGrabbedComponent();

		if (GrabbedComponent)
		{
			GrabbedComponent->WakeAllRigidBodies();
			GrabbedComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
			GrabbedComponent->GetOwner()->Tags.Remove("Grabbed");
			GetPhysicsHandle()->ReleaseComponent();
		}
	}
}


UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot find instance of PhysicsHandle in %s"),
		       *GetOwner()->GetActorNameOrLabel());
	}
	return PhysicsHandle;
}


bool UGrabber::GetGrabbableInReach(FHitResult& OutHitResult)
{
	const FVector Start = GetOwner()->GetActorLocation();
	const FVector End = Start + GetForwardVector() * MaxGrabDistance;

	const FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);

	return GetWorld()->SweepSingleByChannel(
		OutHitResult,
		Start, End,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		Sphere);
}
