// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSubsystem.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Online/OnlineSessionNames.h"

void PrintString(const FString& InputString)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, InputString);
	}
}

UMultiplayerSubsystem::UMultiplayerSubsystem()
{
	//PrintString("MSS Constructor");
	CreateServerAfterDestroy = false;
	DestroyServerName = "";
	ServerNameToFind = "";
	MySessionName = FName("Co-Op Adventure Session Name");
}

void UMultiplayerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	/*PrintString("MSS Initialize");*/
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		FString SubsystemName = OnlineSubsystem->GetSubsystemName().ToString();
		PrintString(SubsystemName);

		SessionInterface = OnlineSubsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMultiplayerSubsystem::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UMultiplayerSubsystem::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMultiplayerSubsystem::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMultiplayerSubsystem::OnJoinSessionComplete);
		}
	}
}

void UMultiplayerSubsystem::Deinitialize()
{
	//UE_LOG(LogTemp, Warning, TEXT("MSS: Deinitialize"));
}

void UMultiplayerSubsystem::CreateServer(FString ServerName)
{
	PrintString("Creating Server: " + ServerName);

	if (ServerName.IsEmpty())
	{
		PrintString("Server name cannot be empty");
		ServerCreateDel.Broadcast(false);
		return;
	}


	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(MySessionName);
	if (ExistingSession)
	{
		PrintString("Session already exists, destroying...");
		CreateServerAfterDestroy = true;
		DestroyServerName = ServerName;
		SessionInterface->DestroySession(MySessionName);
		return;
	}
	FOnlineSessionSettings SessionSettings;

	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.NumPublicConnections = 2;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bAllowJoinViaPresence = true;
	bool IsLAN = false;
	if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
	{
		IsLAN = true;
	}
	SessionSettings.bIsLANMatch = IsLAN;

	SessionSettings.Set(FName("SERVER_NAME"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	SessionInterface->CreateSession(0, MySessionName, SessionSettings);

}

void UMultiplayerSubsystem::FindServer(FString ServerName)
{
	PrintString("Finding A Server: " + ServerName);

	if (ServerName.IsEmpty())
	{
		PrintString("Server Name cannot be empty");
		ServerJoinDel.Broadcast(false);
		return;
	}
	bool IsLAN = false;
	if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
	{
		IsLAN = true;
	}

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = IsLAN;
	SessionSearch->MaxSearchResults = 9999;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	ServerNameToFind = ServerName;
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UMultiplayerSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccesful)
{
	PrintString(FString::Printf(TEXT("OnCreateSession Complete: %d"), bWasSuccesful));
	ServerCreateDel.Broadcast(bWasSuccesful);
	if (bWasSuccesful)
	{
		FString Path = "/Game/ThirdPerson/Maps/ThirdPersonMap?listen";
		if (!InitialMapName.IsEmpty())
		{
			Path = FString::Printf(TEXT("%s?listen"), *InitialMapName);
		}
		GetWorld()->ServerTravel(Path);
	}
}

void UMultiplayerSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccesful)
{
	if (bWasSuccesful && CreateServerAfterDestroy)
	{
		CreateServerAfterDestroy = false;
		CreateServer(DestroyServerName);
	}

}

void UMultiplayerSubsystem::OnFindSessionComplete(bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		ServerJoinDel.Broadcast(false);
		return;
	}
	if (ServerNameToFind.IsEmpty())
	{
		ServerJoinDel.Broadcast(false);
		return;
	}
	TArray<FOnlineSessionSearchResult> Results = SessionSearch->SearchResults;
	FOnlineSessionSearchResult* CorrectResult = 0;
	if (Results.Num() > 0)
	{
		FString Msg = FString::Printf(TEXT("%d sessions found."), Results.Num());
		PrintString(Msg);
		for (FOnlineSessionSearchResult Result : Results)
		{
			if (Result.IsValid())
			{
				FString ServerName = "No-name";
				Result.Session.SessionSettings.Get(FName("SERVER_NAME"), ServerName);

				if (ServerName.Equals(ServerNameToFind))
				{
					CorrectResult = &Result;
					Msg = FString::Printf(TEXT("Found Server With Name: %s"), *ServerName);
					PrintString(Msg);
					break;
				}
			}
		}

		if (CorrectResult)
		{
			ServerJoinDel.Broadcast(true);
			SessionInterface->JoinSession(0, MySessionName, *CorrectResult);
		}
		else
		{
			PrintString("Couldn't find Server");
			ServerJoinDel.Broadcast(false);
			ServerNameToFind = "";
		}
	}
	else
	{
		PrintString("No Sessions Found!");
		ServerJoinDel.Broadcast(false);
	}
}

void UMultiplayerSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	ServerJoinDel.Broadcast(Result == EOnJoinSessionCompleteResult::Success);

	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		FString Msg = FString::Printf(TEXT("Successfully joined Session: %s"), *SessionName.ToString());
		PrintString(Msg);
		FString Address = "";
		bool Success = SessionInterface->GetResolvedConnectString(SessionName, Address);
		if (Success)
		{
			PrintString(FString::Printf(TEXT("Address: %s"), *Address));
			APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if (PlayerController)
			{
				PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
		}
		else
		{
			PrintString("GetREsolvedConnectString returned False!");
		}
	}
	else
	{
		PrintString("OnJoinSessionComplete: Failed");
	}
}