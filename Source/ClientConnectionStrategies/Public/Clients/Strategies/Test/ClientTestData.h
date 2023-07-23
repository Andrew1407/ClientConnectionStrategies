#pragma once

#include "CoreMinimal.h"
#include "Clients/DataContainers/ResponseData.h"
#include "Clients/Interfaces/Client.h"

class ClientTestData
{
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestData")
	bool ResponseStatus = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestData")
	FResponseDelegate OnResponse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestData")
	FResponseData ResponseData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestData")
	FString HostParam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestData")
	int32 PortParam;
};

class ConnectionTestData : public ClientTestData
{
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestData")
	bool ConnectionState = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestData")
	bool ConnectionStatus = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestData")
	bool DisonnectionStatus = true;
};
