#pragma once

#include "CoreMinimal.h"
#include "Mover.h"
#include "Components/BoxComponent.h"
#include "TriggerComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CRYPTRAIDER_API UTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UTriggerComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetMover(UMover* NewMover);

private:
	UPROPERTY(EditAnywhere)
	FName UnlockTag;

	UMover* Mover;
	AActor* GetUnlockingActor() const;
};
