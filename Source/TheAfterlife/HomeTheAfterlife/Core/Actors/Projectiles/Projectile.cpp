#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectile::AProjectile()
{
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->InitSphereRadius(5.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
    SetRootComponent(CollisionComponent);

    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
    ProjectileMovementComponent->InitialSpeed = 2000.f;

}

void AProjectile::LaunchProjectile(FVector Diretion)
{   
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
    ProjectileMovementComponent->Velocity = Diretion * ProjectileMovementComponent->InitialSpeed;
    CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
    GetWorldTimerManager().SetTimer(CollisionTimerHandle, this, &AProjectile::EnableCollision, 0.01f, false);
    OnProjectileLaunched();

}

void AProjectile::OnProjectileLaunched()
{
}

void AProjectile::BeginPlay()
{
    Super::BeginPlay();

    CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnCollisionHit);
}

void AProjectile::OnCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OnProjectileHit.IsBound())
    {
        OnProjectileHit.Broadcast(Hit, ProjectileMovementComponent->Velocity.GetSafeNormal());
    }
}

void AProjectile::EnableCollision()
{
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
}

