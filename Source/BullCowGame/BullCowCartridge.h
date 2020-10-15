// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

struct FBullCowCount
{
	int32 Bulls = 0;
	int32 Cows = 0;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

public:
	static bool IsIsogram(const FString& Guess);
	virtual void BeginPlay() override;
	virtual void OnInput(const FString &Input) override;
private:
    void LoadEligibleIsograms();
	void ShowStartScreen();
	bool GetDifficulty(const FString& Input);
	void SetupGame();
	FString ChooseAndRemoveHiddenWord(TArray<FString>& Isograms) const;
	bool IsGuessValid(const FString& Guess) const;
    void UpdateStateForGuess(const FString& Guess);
	void EndGame();
	FBullCowCount GetBullsAndCows(const FString& Guess) const;
private:
    TArray<FString> EligibleIsograms;
	FString HiddenWord;
	int32 difficulty;
	int32 RemainingGuesses;
	bool bGameOver;
	bool bIsOnStartScreen;
};
