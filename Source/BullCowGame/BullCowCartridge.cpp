// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

void UBullCowCartridge::BeginPlay()
{
    Super::BeginPlay();
    SetupGame();
}

void UBullCowCartridge::SetupGame()
{
    HiddenWord = TEXT("upstage");
    RemainingGuesses = HiddenWord.Len();
    bGameOver = false;

    PrintLine(TEXT("Welcome to Bull Cows!"));
    PrintLine(TEXT("Guess the %i character word.\nYou have %i guesses."), HiddenWord.Len(), RemainingGuesses);
    PrintLine(TEXT("Type in your guess and press enter to\ncontinue."));
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
    return true;
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine("Press enter to play again.");
}