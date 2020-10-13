// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void OnInput(const FString &Input) override;
private:
	void SetupGame();
	FString ChooseHiddenWord();
	bool IsGuessValid(const FString& Guess) const;
    void UpdateStateForGuess(const FString& Guess);
	bool IsIsogram(const FString& Guess) const;
	void EndGame();
private:
	FString HiddenWord;
	int32 RemainingGuesses;
	bool bGameOver;
};
