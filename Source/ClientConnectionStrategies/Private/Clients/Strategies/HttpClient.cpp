// Fill out your copyright notice in the Description page of Project Settings.


#include "Clients/Strategies/HttpClient.h"
#include "Clients/DataContainers/RequestData.h"
#include "Clients/DataContainers/ResponseData.h"
#include "Clients/Strategies/Utils/ClientOptions.h"

#include "JsonObjectConverter.h"
#include "JsonUtilities.h"

#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

UHttpClient::UHttpClient(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    ClientOptions.ResponseTimeout = 3;
}

void UHttpClient::SetAddress_Implementation(const FString& Host, const int32& Port)
{
    ClientOptions.Host = Host;
    ClientOptions.Port = Port;
}

void UHttpClient::SetResponseDelegate_Implementation(const FResponseDeledate& ResponseDelegate)
{
    ClientOptions.ResponseDeledate = ResponseDelegate;
}

void UHttpClient::Send_Implementation(const FRequestData& RequestData)
{
    if (SendMethod == "GET") return GetRequest(RequestData);
    if (SendMethod == "POST") return PostRequest(RequestData);
    ClientOptions.ResponseDeledate.ExecuteIfBound({}, false);
}

void UHttpClient::GetRequest(const FRequestData& RequestData)
{
    TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetVerb("GET");
    const auto& StringifyNumber = [] (const int32& Number) { return FString::Printf(TEXT("%d"), Number); };
    const FString Denominations = FString::JoinBy(RequestData.Denominations, TEXT(","), StringifyNumber);
    const FString Url = FString::Printf(TEXT("http://%s:%d?denominations=%s&amount=%d"), *ClientOptions.Host, ClientOptions.Port, *Denominations, RequestData.Amount);
    HttpRequest->SetURL(Url);
    HttpRequest->SetTimeout(ClientOptions.ResponseTimeout);
    BindResponseHandler(HttpRequest);
    HttpRequest->ProcessRequest();
}

void UHttpClient::PostRequest(const FRequestData& RequestData)
{
    FString Content;
    bool IsParsed = FJsonObjectConverter::UStructToJsonObjectString(RequestData, Content);
    if (!IsParsed) return (void)ClientOptions.ResponseDeledate.ExecuteIfBound({}, false);
    TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetVerb("POST");
    HttpRequest->SetHeader("Content-Type", "application/json");
    HttpRequest->SetURL(FString::Printf(TEXT("http://%s:%d"), *ClientOptions.Host, ClientOptions.Port));
    HttpRequest->SetContentAsString(Content);
    HttpRequest->SetTimeout(ClientOptions.ResponseTimeout);
    BindResponseHandler(HttpRequest);
    HttpRequest->ProcessRequest();
}

void UHttpClient::BindResponseHandler(const TSharedRef<IHttpRequest>& HttpRequest)
{
    HttpRequest->OnProcessRequestComplete().BindLambda([&] (FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
    {
        FResponseData ResponseData;
        bool IsSuccess = bWasSuccessful && Response.IsValid();
        if (IsSuccess)
            IsSuccess = FJsonObjectConverter::JsonObjectStringToUStruct<FResponseData>(Response->GetContentAsString(), &ResponseData, 0, 0);
        ClientOptions.ResponseDeledate.ExecuteIfBound(ResponseData, IsSuccess);
    });
}
