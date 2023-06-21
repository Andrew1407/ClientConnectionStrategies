// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Clients/Interfaces/Connection.h"
#include "Clients/Interfaces/Client.h"
#include "clients/ClientLabels.h"
#include "WsClient.generated.h"

struct FClientOptions;
class IWebSocket;
/**
 * 
 */
UCLASS()
class CLIENTCONNECTIONSTRATEGIES_API UWsClient : public UObject, public IClient, public IConnection
{
	GENERATED_BODY()

public:
	UWsClient(const FObjectInitializer& ObjectInitializer);

	FORCEINLINE virtual void SetResponseDelegate_Implementation(const FResponseDeledate& ResponseDelegate);
	virtual void SetAddress_Implementation(const FString& Host, const int32& Port);
	virtual void Send_Implementation(const FRequestData& RequestData);

	FORCEINLINE virtual bool Connected_Implementation();
	virtual void Connect_Implementation(const FConnectionDelegate& ConnectionDelegate);
	virtual void Disconnect_Implementation(const FConnectionDelegate& DisconnectionDelegate);

protected:
	virtual void BeginDestroy() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WsClient", meta = (AllowPrivateAccess = true))
	FClientOptions ClientOptions;

	FORCEINLINE void TriggerDisconnect(bool bSuccess = true) { OnDisconnectDelegate.ExecuteIfBound(EClientLabels::WS, bSuccess); }

	FTimerHandle ConnectionTimer;
	TSharedPtr<IWebSocket> Socket;
	FConnectionDelegate OnDisconnectDelegate;
};
