// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ANACHRONIST_API UGrabber : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Release();

	UFUNCTION(BlueprintCallable)
	void Grab();

private:
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UPROPERTY(EditAnywhere)
		float MaxGrabDistance = 400;

	//static void PrintDamage(const float& Damage);
	//static bool HasDamage(float& Damage);
	UPROPERTY(EditAnywhere)
		float GrabRadius = 100;

	UPROPERTY(EditAnywhere)
		float HoldDistance = 200;

	UFUNCTION(BlueprintCallable)
		UPhysicsHandleComponent* GetPhysicsHandle() const;

	UFUNCTION(BlueprintCallable)
		bool GetGrabbableInReach(FHitResult& OutHitResult) const;
};
