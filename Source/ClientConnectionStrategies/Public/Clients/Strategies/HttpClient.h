// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Clients/Interfaces/Client.h"
#include "Clients/Strategies/Utils/ClientOptions.h"
#include "Clients/DataContainers/RequestData.h"
#include "HttpClient.generated.h"

UCLASS(BlueprintType, Blueprintable)
class CLIENTCONNECTIONSTRATEGIES_API UHttpClient : public UObject, public IClient
{
	GENERATED_BODY()

public:
	UHttpClient(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "HttpClient|Requests")
	void GetRequest(const FRequestData& RequestData);

	UFUNCTION(BlueprintCallable, Category = "HttpClient|Requests")
	void PostRequest(const FRequestData& RequestData);

	FORCEINLINE virtual void SetResponseDelegate_Implementation(const FResponseDeledate& ResponseDelegate);
	virtual void SetAddress_Implementation(const FString& Host, const int32& Port); 
	virtual void Send_Implementation(const FRequestData& RequestData);

	FORCEINLINE void SetSendMethod(const FName& Method) { SendMethod = Method; }
	FORCEINLINE const FName& GetSendMethod() const { return SendMethod; }

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HttpClient", meta = (AllowPrivateAccess = true))
	FClientOptions ClientOptions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HttpClient", meta = (AllowPrivateAccess = true))
	FName SendMethod = "POST";

	void BindResponseHandler(const TSharedRef<class IHttpRequest>& HttpRequest);
};
