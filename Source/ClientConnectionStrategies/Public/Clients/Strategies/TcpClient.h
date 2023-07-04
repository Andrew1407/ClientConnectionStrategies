// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Clients/Interfaces/Connection.h"
#include "Clients/Interfaces/Client.h"
#include "TcpClient.generated.h"

struct FConnectionType;
struct FClientOptions;
class FSocket;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class CLIENTCONNECTIONSTRATEGIES_API UTcpClient : public UObject, public IClient, public IConnection
{
	GENERATED_BODY()

public:
	UTcpClient(const FObjectInitializer& ObjectInitializer);

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TcpClient", meta = (AllowPrivateAccess = true))
	FClientOptions ClientOptions;

    TSharedPtr<FSocket> Socket;
};
