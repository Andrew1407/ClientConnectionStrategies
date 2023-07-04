// Fill out your copyright notice in the Description page of Project Settings.


#include "Clients/ClientFactory.h"
#include "Clients/Interfaces/Client.h"

#include "Clients/Strategies/HttpClient.h"
#include "Clients/Strategies/WsClient.h"
#include "Clients/Strategies/UdpClient.h"
#include "Clients/Strategies/TcpClient.h"

UClientFactory::UClientFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    HttpInstance = UHttpClient::StaticClass();
    WsInstance = UWsClient::StaticClass();
    UdpInstance = UUdpClient::StaticClass();
    TcpInstance = UTcpClient::StaticClass();
}

template <typename T>
T* UClientFactory::CreateClient(EClientLabels Target, UObject* Owner, FName Name, const TSubclassOf<UObject>& InstanceClass, EClientLabels& Label)
{
    bool IsArgsValid = Owner && InstanceClass;
    T* Client = IsArgsValid ? NewObject<T>(Owner, InstanceClass, Name) : nullptr;
    Label = Client ? Target : EClientLabels::NONE;
    return Client;
}

UHttpClient* UClientFactory::CreateHttpClient(UObject* Owner, EClientLabels& Label)
{
    return CreateClient<UHttpClient>(EClientLabels::HTTP, Owner, TEXT("HttpClient"), HttpInstance, Label);
}

UWsClient* UClientFactory::CreateWsClient(UObject* Owner, EClientLabels& Label)
{
    return CreateClient<UWsClient>(EClientLabels::WS, Owner, TEXT("WsClient"), WsInstance, Label);
}

UUdpClient* UClientFactory::CreateUdpClient(UObject* Owner, EClientLabels& Label)
{
    return CreateClient<UUdpClient>(EClientLabels::UDP, Owner, TEXT("UdpClient"), UdpInstance, Label);
}

UTcpClient* UClientFactory::CreateTcpClient(UObject* Owner, EClientLabels& Label)
{
    return CreateClient<UTcpClient>(EClientLabels::TCP, Owner, TEXT("TcpClient"), TcpInstance, Label);
}

UObject* UClientFactory::CreateByTarget(EClientLabels Target, UObject* Owner, EClientLabels& Label)
{
    switch (Target)
    {
        case EClientLabels::HTTP: return CreateHttpClient(Owner, Label);
        case EClientLabels::WS: return CreateWsClient(Owner, Label);
        case EClientLabels::UDP: return CreateUdpClient(Owner, Label);
        case EClientLabels::TCP: return CreateTcpClient(Owner, Label);
        default: return DefaultFactoryBehaviour(Owner, Label);
    }
}

UObject* UClientFactory::DefaultFactoryBehaviour(UObject* Owner, EClientLabels& Label)
{
    Label = EClientLabels::NONE;
    return nullptr;
}
