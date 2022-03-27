// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"
#include <string>

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    Isograms = GetValidWords(WordList);

    SetupGame(); //Setting up the game

    // PrintLine(TEXT("The number of possible words is %i"), WordList.Num());
    // PrintLine(TEXT("The number of valid words is %i"), GetValidWords(WordList).Num());
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else //checking player guess
    {
        ProcessGuess(Input);
    }

}

void UBullCowCartridge::SetupGame()
{
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
    Lives = HiddenWord.Len();
    bGameOver = false;
    
    //Welcoming the Player
    PrintLine(TEXT("Hi There!\nWelcome to the game Bull Cows!\n"));

    //Prompt for guess
    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("Lives left: %i"), Lives);
    PrintLine(TEXT("Press 'Tab' now to type your guess,\nand press Enter to continue..."));
    PrintLine(TEXT("The Hidden Word is: %s."), *HiddenWord); //debug line
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Press enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("Correct!"));
        EndGame();
        return;
    }

    if (HiddenWord.Len() != Guess.Len())
    {
        PrintLine(TEXT("Wrong amount of letters.\nGuess the %i letter word!"), HiddenWord.Len());
        return;
    }
    
    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters! Try again."));
        return;
    }

    PrintLine(TEXT("Lost a life!"));
    PrintLine(TEXT("Lives left: %i"), --Lives);

    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("Out of lives!"));
        PrintLine(TEXT("The hidden word was %s"), *HiddenWord);
        EndGame();
        return;
    }

    //Show the player Bulls and Cows
    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);
    PrintLine(TEXT("%s"), *BullCowString(Guess));
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
        
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords; 

    for (FString Word : WordList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }   
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }
        
        for (int32 HWordIndex = 0; HWordIndex < HiddenWord.Len(); HWordIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HWordIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}

FString UBullCowCartridge::BullCowString(const FString& Guess) const
{
    FString BCString = "";
    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            BCString += Guess[GuessIndex];
            continue;
        }
        else
        {
            BCString += "_";
            continue;
        }
    }
    return BCString;
}