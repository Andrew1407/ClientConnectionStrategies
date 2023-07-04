// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ClientLabels.h"
#include "ClientFactory.generated.h"

class UHttpClient;
class UWsClient;
class UUdpClient;
class UTcpClient;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class CLIENTCONNECTIONSTRATEGIES_API UClientFactory : public UObject
{
    GENERATED_BODY()

public:
    UClientFactory(const FObjectInitializer& ObjectInitializer);

    #pragma region FACTORY_METHODS

    template <typename T>
    static T* CreateClient(EClientLabels Target, UObject* Owner, FName Name, const TSubclassOf<UObject>& InstanceClass, EClientLabels& Label);

    UFUNCTION(BlueprintCallable, Category = "UClientFactory")
    FORCEINLINE UHttpClient* CreateHttpClient(UObject* Owner, UPARAM(ref) EClientLabels& Label);

    UFUNCTION(BlueprintCallable, Category = "UClientFactory")
    FORCEINLINE UWsClient* CreateWsClient(UObject* Owner, UPARAM(ref) EClientLabels& Label);

    UFUNCTION(BlueprintCallable, Category = "UClientFactory")
    FORCEINLINE UUdpClient* CreateUdpClient(UObject* Owner, UPARAM(ref) EClientLabels& Label);

    UFUNCTION(BlueprintCallable, Category = "UClientFactory")
    FORCEINLINE UTcpClient* CreateTcpClient(UObject* Owner, UPARAM(ref) EClientLabels& Label);

    UFUNCTION(BlueprintCallable, Category = "UClientFactory")
    UObject* CreateByTarget(EClientLabels Target, UObject* Owner, UPARAM(ref) EClientLabels& Label);

    #pragma endregion

    #pragma region INSTANCE_SETTERS

    FORCEINLINE void SetHttpInstance(const TSubclassOf<UHttpClient>& Instance) { HttpInstance = Instance; }
    FORCEINLINE void SetWsInstance(const TSubclassOf<UWsClient>& Instance) { WsInstance = Instance; }
    FORCEINLINE void SetUdpInstance(const TSubclassOf<UUdpClient>& Instance) { UdpInstance = Instance; }
    FORCEINLINE void SetTcpInstance(const TSubclassOf<UTcpClient>& Instance) { TcpInstance = Instance; }

    #pragma endregion

protected:
    UFUNCTION(BlueprintCallable, Category = "UClientFactory")
    UObject* DefaultFactoryBehaviour(UObject* Owner, EClientLabels& Label);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClientFactory|Instances", meta = (AllowPrivateAccess = true))
    TSubclassOf<UHttpClient> HttpInstance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClientFactory|Instances", meta = (AllowPrivateAccess = true))
    TSubclassOf<UWsClient> WsInstance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClientFactory|Instances", meta = (AllowPrivateAccess = true))
    TSubclassOf<UUdpClient> UdpInstance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClientFactory|Instances", meta = (AllowPrivateAccess = true))
    TSubclassOf<UTcpClient> TcpInstance;
};
