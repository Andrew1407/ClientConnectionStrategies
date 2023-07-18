// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Clients/Interfaces/Connection.h"
#include "Clients/Interfaces/Client.h"
#include "Clients/Strategies/Utils/ClientOptions.h"
#include "Clients/Strategies/Utils/ConnectionType.h"
#include "UdpClient.generated.h"

class FSocket;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class CLIENTCONNECTIONSTRATEGIES_API UUdpClient : public UObject, public IClient, public IConnection
{
	GENERATED_BODY()

public:
	UUdpClient(const FObjectInitializer& ObjectInitializer);

	FORCEINLINE virtual void SetResponseDelegate_Implementation(const FResponseDeledate& ResponseDelegate);
	virtual void SetAddress_Implementation(const FString& Host, const int32& Port);
	virtual void Send_Implementation(const FRequestData& RequestData);

	FORCEINLINE virtual bool Connected_Implementation();
	virtual void Connect_Implementation(const FConnectionDelegate& ConnectionDelegate);
	virtual void Disconnect_Implementation(const FConnectionDelegate& DisconnectionDelegate);

protected:
	virtual void BeginDestroy() override;

private:
	static const FConnectionType ConnectionType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UdpClient", meta = (AllowPrivateAccess = true))
	FClientOptions ClientOptions;

    TSharedPtr<FSocket> Socket;

	void HandleReceivedData(ENamedThreads::Type Context, TFunction<void(const FString&)> Callback);
};
