// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

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

	PhysicsHandle = GetPhysicsHandle();
	check(PhysicsHandle != nullptr);
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(PhysicsHandle != nullptr && PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		const FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
}

//Grabs an actor upon sweep hit
void UGrabber::Grab()
{
	if (PhysicsHandle != nullptr)
	{
		if (FHitResult HitResult; GetGrabbableInReach(HitResult))
		{
			UPrimitiveComponent* HitComponent = HitResult.GetComponent();
			HitComponent->SetSimulatePhysics(true);
			HitComponent->WakeAllRigidBodies();

			AActor* HitActor = HitResult.GetActor();
			HitActor->Tags.Add("Grabbed");
			HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

			PhysicsHandle->GrabComponentAtLocationWithRotation(
				HitComponent,
				NAME_None,
				HitResult.ImpactPoint,
				GetComponentRotation()
			);
			DrawDebugSphere(GetWorld(), HitResult.Location, 10, 10, FColor::Green, false, 5);
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Red, false, 5);
			UE_LOG(LogTemp, Display, TEXT("Hit Actor: %s"), *HitResult.GetActor()->GetActorNameOrLabel());
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("No Actor Hit"));
		}
	}
}

void UGrabber::Release()
{
	if (UPrimitiveComponent* GrabbedComponent = PhysicsHandle->GetGrabbedComponent(); GrabbedComponent != nullptr)
	{
		GrabbedComponent->WakeAllRigidBodies(); //wake it up in case it is grabbed and not moved around for a while

		//get the owning actor of the grabbed component
		//remove a tag if it already exists in the grabbed actor's tags array
		if (AActor* GrabbedActor = GrabbedComponent->GetOwner(); GrabbedActor->Tags.Contains("Grabbed"))
		{
			GrabbedActor->Tags.Remove("Grabbed");
		}

		//release the grabbed component
		PhysicsHandle->ReleaseComponent();
		UE_LOG(LogTemp, Display, TEXT("Released grabber"));
	}
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	return GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
}

bool UGrabber::GetGrabbableInReach(FHitResult& OutHitResult) const
{
	//Draw a debug line for line trace
	const FVector Start = GetComponentLocation();
	const FVector End = Start + GetForwardVector() * MaxGrabDistance;
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Blue, false, 5);


	//Return the actor who got hit
	const FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	
	const bool HasHit = GetWorld()->SweepSingleByChannel(
		OutHitResult,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		Sphere);
	return HasHit;
}

