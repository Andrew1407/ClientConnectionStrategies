// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class FSocket;
/**
 * 
 */
class CLIENTCONNECTIONSTRATEGIES_API ConnectionUtils
{
public:
	static TSharedPtr<FSocket> MakeSocketConnection(const FName &SocketType, const FString &SocketDescription,
		const FString& ServerHost, int32 ServerPort, bool& Connected);

	static bool SendData(const TSharedPtr<FSocket>& Socket, const FString& Data);
	static const FString ReceiveData(const TSharedPtr<FSocket>& Socket, float TimeoutSeconds, int32 BufferSize);

	FORCEINLINE static bool SocketConnected(const TSharedPtr<FSocket>& Socket);
	FORCEINLINE static void ResetSocket(TSharedPtr<FSocket>& Socket);
};
