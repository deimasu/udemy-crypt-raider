#include "TriggerComponent.h"

#include "Mover.h"


UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* UnlockingActor = GetUnlockingActor();

	if (UnlockingActor)
	{
		UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(UnlockingActor->GetRootComponent());
		if (Component)
		{
			if (UnlockingActor->ActorHasTag("Attach"))
			{
				Component->SetSimulatePhysics(false);
				Component->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
				UnlockingActor->Tags.Remove("Grabbable");
			}
		}
		if (Mover)
		{
			Mover->SetShouldMove(true);
		}
	}
	else
	{
		if (Mover)
		{
			Mover->SetShouldMove(false);
		}
	}
}

void UTriggerComponent::SetMover(UMover* NewMover)
{
	Mover = NewMover;
}

AActor* UTriggerComponent::GetUnlockingActor() const
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);

	for (auto Actor : OverlappingActors)
	{
		if (Actor->ActorHasTag(UnlockTag) && !Actor->ActorHasTag("Grabbed"))
		{
			return Actor;
		}
	}

	return nullptr;
}
