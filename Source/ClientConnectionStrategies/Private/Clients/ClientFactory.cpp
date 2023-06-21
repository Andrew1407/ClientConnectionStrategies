// Fill out your copyright notice in the Description page of Project Settings.


#include "Clients/ClientFactory.h"
#include "Clients/Interfaces/Client.h"

#include "Clients/Strategies/HttpClient.h"
#include "Clients/Strategies/WsClient.h"
#include "Clients/Strategies/UdpClient.h"
#include "Clients/Strategies/TcpClient.h"

UHttpClient* UClientFactory::CreateHttpClient(UObject* Owner, EClientLabels& Label)
{
    UHttpClient* Client = Owner ? NewObject<UHttpClient>(Owner) : nullptr;
    Label = Client ? EClientLabels::HTTP : EClientLabels::NONE;
    return Client;
}

UWsClient* UClientFactory::CreateWsClient(UObject* Owner, EClientLabels& Label)
{
    UWsClient* Client = Owner ? NewObject<UWsClient>(Owner) : nullptr;
    Label = Client ? EClientLabels::WS : EClientLabels::NONE;
    return Client;
}

UUdpClient* UClientFactory::CreateUdpClient(UObject* Owner, EClientLabels& Label)
{
    UUdpClient* Client = Owner ? NewObject<UUdpClient>(Owner) : nullptr;
    Label = Client ? EClientLabels::UDP : EClientLabels::NONE;
    return Client;
}

UTcpClient* UClientFactory::CreateTcpClient(UObject* Owner, EClientLabels& Label)
{
    UTcpClient* Client = Owner ? NewObject<UTcpClient>(Owner) : nullptr;
    Label = Client ? EClientLabels::TCP : EClientLabels::NONE;
    return Client;
}

UObject* UClientFactory::CreateByTarget(EClientLabels Target, UObject* Owner, UPARAM(ref) EClientLabels& Label)
{
    switch (Target)
    {
    case EClientLabels::HTTP:
        return CreateHttpClient(Owner, Label);
    case EClientLabels::WS:
        return CreateWsClient(Owner, Label);
    case EClientLabels::UDP:
        return CreateUdpClient(Owner, Label);
    case EClientLabels::TCP:
        return CreateTcpClient(Owner, Label);
    }
    Label = EClientLabels::NONE;
    return nullptr;
}
