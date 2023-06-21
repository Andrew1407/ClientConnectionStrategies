// Fill out your copyright notice in the Description page of Project Settings.


#include "Clients/Strategies/Utils/ConnectionUtils.h"
#include "Networking.h"

TSharedPtr<FSocket> ConnectionUtils::MakeSocketConnection(const FName &SocketType, const FString &SocketDescription,
    const FString& ServerHost, int32 ServerPort, bool& Connected)
{
    TSharedPtr<FSocket> Socket = MakeShareable(ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(SocketType, SocketDescription));
    TSharedRef<FInternetAddr> RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
    bool IsIpValid;
    RemoteAddress->SetIp(*ServerHost, IsIpValid);
    RemoteAddress->SetPort(ServerPort);
    if (IsIpValid) IsIpValid = Socket->Connect(*RemoteAddress);
    Connected = IsIpValid;
    return Socket;
}

bool ConnectionUtils::SendData(const TSharedPtr<FSocket>& Socket, const FString& Data)
{
    TArray<uint8> MessageArray;
    FTCHARToUTF8 ConvertedMsg(*Data);
    MessageArray.Append((uint8*)ConvertedMsg.Get(), ConvertedMsg.Length());
    int32 BytesSent = 0;
    return Socket->Send(MessageArray.GetData(), MessageArray.Num(), BytesSent);
}

const FString ConnectionUtils::ReceiveData(const TSharedPtr<FSocket>& Socket, float TimeoutSeconds, int32 BufferSize)
{
    TUniquePtr<uint8[]> Buffer = MakeUnique<uint8[]>(BufferSize);
    int32 BytesReceived = 0;
    int32 BytesRead = 0;
    bool IsRed = Socket->Wait(ESocketWaitConditions::WaitForRead, FTimespan::FromSeconds(TimeoutSeconds));
    if (!IsRed) return FString();                              // Handle timeout or error condition
    BytesRead = Socket->Recv(Buffer.Get(), BufferSize, BytesReceived);
    if (BytesRead <= 0) return FString();                      // No response received from the server
    FString ReceivedMessage = FString(UTF8_TO_TCHAR(reinterpret_cast<const ANSICHAR*>(Buffer.Get())));
    return ReceivedMessage;
}

bool ConnectionUtils::SocketConnected(const TSharedPtr<FSocket>& Socket)
{
    return Socket.IsValid() && Socket->GetConnectionState() == SCS_Connected;
}

void ConnectionUtils::ResetSocket(TSharedPtr<FSocket>& Socket)
{
    if (!Socket.IsValid()) return;
    Socket->Close();
    Socket.Reset();
}
