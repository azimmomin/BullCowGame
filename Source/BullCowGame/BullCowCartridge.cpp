// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

bool UBullCowCartridge::IsIsogram(const FString &Guess)
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

void UBullCowCartridge::BeginPlay()
{
    Super::BeginPlay();
    LoadEligibleIsograms();
    SetupGame();
}

void UBullCowCartridge::LoadEligibleIsograms()
{
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArrayWithPredicate(
        EligibleIsograms, 
        *WordListPath, 
        [](const FString& Word) { return Word.Len() > 3 && Word.Len() < 8 && IsIsogram(Word); }
    );
}

void UBullCowCartridge::SetupGame()
{
    HiddenWord = ChooseAndRemoveHiddenWord(EligibleIsograms);
    RemainingGuesses = HiddenWord.Len();
    bGameOver = false;

    PrintLine(TEXT("Welcome to Bull Cows!"));
    PrintLine(TEXT("Guess the %i character word.\nYou have %i guesses."), HiddenWord.Len(), RemainingGuesses);
    PrintLine(TEXT("Type in your guess and press enter to\ncontinue."));
}

FString UBullCowCartridge::ChooseAndRemoveHiddenWord(TArray<FString>& Isograms) const
{
    if (Isograms.Num() <= 0)
    {
        // Return a fallback in the off chance the array is empty.
        return TEXT("upstage");
    }

    int32 index = FMath::RandRange(0, Isograms.Num() - 1);
    FString word = Isograms[index];

    // Remove the newly chosen word so we don't pick it again when the user plays again.
    Isograms.RemoveAt(index);

    return word;
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
        FBullCowCount count = GetBullsAndCows(Guess);
        PrintLine(TEXT("Your guess had %i Bulls and %i Cows.\nYou have %i guess(es) left."), count.Bulls, count.Cows, RemainingGuesses);
    }
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine("Press enter to play again.");
}

// We assume that IsValidGuess has been called on the Guess before being passed in.
FBullCowCount UBullCowCartridge::GetBullsAndCows(const FString& Guess) const
{
    FBullCowCount count;

    for (int32 i = 0; i < Guess.Len(); i++)
    {
        if (Guess[i] == HiddenWord[i])
        {
            count.Bulls++;
            continue;
        }

        for (int32 j = 0; j < HiddenWord.Len(); j++)
        {
            if (Guess[i] == HiddenWord[j])
            {
                count.Cows++;
                break;
            }
        }
    }

    return count;
}
