// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay()
{
    Super::BeginPlay();
    
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArray(EligibleIsograms, *WordListPath);

    SetupGame();
}

void UBullCowCartridge::SetupGame()
{
    HiddenWord = ChooseHiddenWord();
    RemainingGuesses = HiddenWord.Len() - 1;
    bGameOver = false;

    PrintLine(TEXT("Welcome to Bull Cows!"));
    PrintLine(TEXT("Guess the %i character word.\nYou have %i guesses."), HiddenWord.Len(), RemainingGuesses);
    PrintLine(TEXT("Type in your guess and press enter to\ncontinue."));
}

FString UBullCowCartridge::ChooseHiddenWord()
{
    return TEXT("upstage");
}


void UBullCowCartridge::OnInput(const FString &Input)
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else if (IsGuessValid(Input))
    {
        UpdateStateForGuess(Input);
    }
}

bool UBullCowCartridge::IsGuessValid(const FString &Guess) const
{
    if (Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("The guess is not the right length.\nTry again."));
        return false;
    }

    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("The guess is not an isogram.\nTry again."));
        return false;
    }

    return true;
}

void UBullCowCartridge::UpdateStateForGuess(const FString &Guess)
{
    if (Guess.ToLower() == HiddenWord.ToLower())
    {
        PrintLine(TEXT("You got it!"));
        EndGame();
        return;
    }

    --RemainingGuesses;
    if (RemainingGuesses <= 0)
    {
        PrintLine(TEXT("You lost.\nThe hidden word was %s."), *HiddenWord);
        EndGame();
    }
    else
    {
        // Check how far off the guess was and display the info.
        PrintLine(TEXT("That's not right.\nYou have %i guess(es) left."), RemainingGuesses);
    }
}

bool UBullCowCartridge::IsIsogram(const FString &Guess) const
{
    if (Guess.Len() <= 1)
        return true;

    for (int32 i = 0; i < Guess.Len() - 1; i++)
    {
        for (int32 j = i + 1; j < Guess.Len(); j++)
        {
            if (Guess[i] == Guess[j])
                return false;
        }
    }

    return true;
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine("Press enter to play again.");
}