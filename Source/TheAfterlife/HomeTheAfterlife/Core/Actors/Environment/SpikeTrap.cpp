#include "Components/BoxComponent.h"
#include "Engine/DamageEvents.h"
#include "SpikeTrap.h"

ASpikeTrap::ASpikeTrap()
{
	PrimaryActorTick.bCanEverTick = true;

	//TrapMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TrapMesh"));
	//TrapMesh->SetupAttachment(RootComponent);

	UBoxComponent* BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));

	BoxCollider->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	BoxCollider->SetupAttachment(RootComponent);

	BoxCollider->SetCollisionProfileName(CollisionProfilePawnInteractionVolume);
	BoxCollider->SetGenerateOverlapEvents(true);

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ASpikeTrap::OnOverlapBegin);
	BoxCollider->OnComponentEndOverlap.AddDynamic(this, &ASpikeTrap::OnOverlapEnd);
}

void ASpikeTrap::SetCanAttack(bool CanAttack)
{
	//bCanAttack = CanAttack;
}

void ASpikeTrap::BeginPlay()
{
	Super::BeginPlay();

	//GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &ASpikeTrap::StartAttackAnimation, AnimationInterval, true);
}

void ASpikeTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TryAttack();
}

void ASpikeTrap::StartAttackAnimation()
{	
	//AnimInstance = TrapMesh->GetAnimInstance();

	//TrapMesh->PlayAnimation(AnimMontage, false);
}

void ASpikeTrap::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(OtherActor);
	//CachedCharacter = BaseCharacter;
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("XYI"));
	CachedCharacter = Cast<ABaseCharacter>(OtherActor);
}

void ASpikeTrap::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CachedCharacter = nullptr;
}

void ASpikeTrap::TryAttack()
{	
	if (!IsValid(CachedCharacter))
	{
		return;
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Goida"));
	//
	//if (!bCanAttack)
	//{
	//	return;
	//}

	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Goida"));

	//CachedCharacter->TakeDamage(Damage, FDamageEvent(), nullptr, GetOwner());
}