// Fill out your copyright notice in the Description page of Project Settings.


#include "Clients/ClientFactory.h"
#include "Clients/Interfaces/Client.h"

#include "Clients/Strategies/HttpClient.h"
#include "Clients/Strategies/WsClient.h"
#include "Clients/Strategies/UdpClient.h"
#include "Clients/Strategies/TcpClient.h"

UHttpClient* UClientFactory::CreateHttpClient(UObject* Owner, EClientLabels& Label)
{
    return CreateClient<UHttpClient>(EClientLabels::HTTP, TEXT("HttpClient"), Owner, Label);
}

UWsClient* UClientFactory::CreateWsClient(UObject* Owner, EClientLabels& Label)
{
    return CreateClient<UWsClient>(EClientLabels::WS, TEXT("WsClient"), Owner, Label);
}

UUdpClient* UClientFactory::CreateUdpClient(UObject* Owner, EClientLabels& Label)
{
    return CreateClient<UUdpClient>(EClientLabels::UDP, TEXT("UdpClient"), Owner, Label);
}

UTcpClient* UClientFactory::CreateTcpClient(UObject* Owner, EClientLabels& Label)
{
    return CreateClient<UTcpClient>(EClientLabels::TCP, TEXT("TcpClient"), Owner, Label);
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

template <typename T>
T* UClientFactory::CreateClient(EClientLabels Target, FName Name, UObject* Owner, EClientLabels& Label)
{
    T* Client = Owner ? NewObject<T>(Owner, Name) : nullptr;
    Label = Client ? Target : EClientLabels::NONE;
    return Client;
}
