---
layout: codesamples
title:  "Boggle Solver"
date:   2016-06-24 18:34:36 -0700
categories: jekyll update
---

This is a solver for the popular game of Boggle. It accepts NxM boardsize.

{% highlight cpp %}
//Boggle.h
#ifndef BOGGLE_H
#define BOGGLE_H

#include <string>
#include <vector>
#include <list>
#include <memory> //shared_ptr
#include <set>
/**********************************************************************************************//**
 * @class Dictionary
 *
 * @brief The dictionary is represented as a trie to allow fast look up
 **************************************************************************************************/

class Dictionary
{
public:
  Dictionary(std::string fileName);
  void InsertWord(std::string word);
  int FindWord(std::string& word);

private:
  struct Node
  {
    //A waste of memory for speed when looking up if a word exists
    //Each node represents a lowercase a-z character
    std::shared_ptr<Node> children[26];
    //Mark that if it's a word in case of "apple" is a word and so is "applepie" when traversing
    bool isWord = false;
  };

  //shared_ptr for easy memory cleanup..
  std::shared_ptr<Node> root_;
};

/**********************************************************************************************//**
 * @class BoggleGame
 *
 * @brief A traditional game of boggle except 'Q' is represented as "QU"
 **************************************************************************************************/

class BoggleGame
{
public:
  BoggleGame(std::string boggleDictionary, std::string boggleBoardFileName, std::string boggleResultsFileName);
  void BuildBoggleBoard(std::string boggleBoardFileName);
  void WriteResults(void);
  std::list<std::string>& Solve(void);

private:
  void SolveRecursive(std::string currentWord, std::vector<std::vector<bool> > currentBoard, unsigned short x, unsigned short y);

  std::vector<std::vector<std::string> > board_;
  Dictionary dictionary_;
  std::set<std::string> results_;
  std::string saveFileName_;
  short rowSize_, columnSize_;
};

#endif
{% endhighlight %}

{% highlight cpp %}
//Boggle.cpp
#include "boggle.h"
#include <fstream> 
#include <ctype.h> //tolower
#include <iostream>
#include <sstream>

BoggleGame::BoggleGame(std::string boggleBoardFileName, std::string boggleDictionary, std::string boggleResultsFileName) : dictionary_(boggleDictionary), saveFileName_(boggleResultsFileName)
{
  BuildBoggleBoard(boggleBoardFileName);
}

std::list<std::string>& BoggleGame::Solve(void)
{
  //Recursively call all iterations on each board index
  for (short i = 0; i < rowSize_; ++i)
  {
    for (short j = 0; j < columnSize_; ++j)
    {
      //Need to keep track of each board state if the letter was used
      std::vector<std::vector<bool>> currentBoard(rowSize_, std::vector<bool>(columnSize_, false));
      SolveRecursive("", currentBoard, i, j);
    }
  }
  //Sort and make sure there are no duplicates
  //results_.sort();
  //results_.unique();
  return std::list<std::string>();
}

void BoggleGame::BuildBoggleBoard(std::string boggleBoardFileName)
{
  std::fstream boardFile;
  boardFile.open(boggleBoardFileName, std::fstream::in);
  if (boardFile.good())
  {
    std::string currentLine;
    while (std::getline(boardFile, currentLine))
    {
      std::stringstream currentLineStream(currentLine);
      std::string currentLetter;
      board_.push_back(std::vector<std::string>());
      while (currentLineStream >> currentLetter)
      {
        //Keep the board lowercased

        currentLetter = tolower(currentLetter[0]);
        board_.back().push_back(currentLetter);

        //Add the u following a Q on the board since in this version, a "q" will always represent "qu"
        if (currentLetter[0] == 'q')
          board_.back().back() += "u";
      }
    }
    boardFile.close();
    rowSize_ = board_.size();
    columnSize_ = board_.back().size();
  }
  else
  {
    std::cerr << "Could not open " << boggleBoardFileName << "to read.\n" << std::endl;
  }
}

void BoggleGame::WriteResults(void)
{
  std::fstream resultingBoardFile;
  resultingBoardFile.open(saveFileName_, std::fstream::out);

  if (resultingBoardFile.good())
  {
    for (auto word : results_)
    {
      resultingBoardFile << word << "\n";
    }
    resultingBoardFile.close();
  }
  else
  {
    std::cerr << "Could not open " << saveFileName_ << "to write.\n" << std::endl;
  }
}

void BoggleGame::SolveRecursive(std::string currentWord, std::vector<std::vector<bool>> currentBoard, unsigned short x, unsigned short y)
{
  //mark current index as visited
  currentBoard[x][y] = true;
  currentWord += board_[x][y];

  //check if it's a word
  switch (dictionary_.FindWord(currentWord))
  {
    //This is definitely a word, but kept checking since "per" is a word and so is "perfect"
  case 1:
    results_.insert(currentWord);
    break;
    //There is no way any future combinations of this will be a word
  case 0:
    return;
    //Similar to case 1 except "perfe" is not a word, but still can traverse the tree
  case -1:
    break;
  }

  //check the 8 directions around current index
  for (int i = x - 1; i <= x + 1; ++i)
  {
    for (int j = y - 1; j <= y + 1; ++j)
    {
      if (i >= 0 && i < rowSize_ && j >= 0 && j < columnSize_ && currentBoard[i][j] == false)
      {

        SolveRecursive(currentWord, currentBoard, i, j);
      }
    }
  }
}

Dictionary::Dictionary(std::string fileName)
{
  root_ = std::make_shared<Node>();
  std::fstream dictionaryFile;
  dictionaryFile.open(fileName, std::fstream::in);
  //Buffer for at least one of the longest word sizes
  std::string word(45, ' ');
  if (dictionaryFile.good())
  {
    while (dictionaryFile >> word)
    {
      //All words must be a minimum of 3 characters in length.
      if (word.size() >= 3)
        InsertWord(word);
    }
  }
}

void Dictionary::InsertWord(std::string word)
{
  std::shared_ptr<Node> currentLevel = root_;
  //iterate letter by letter to insert into the trie
  for (char letter : word)
  {
    //check if it exists on current level to keep traversing down
    if (currentLevel->children['z' - letter])
    {
      currentLevel = currentLevel->children['z' - letter];
    }
    //Otherwise add it
    else
    {
      currentLevel = currentLevel->children['z' - letter] = std::make_shared<Node>();
    }
  }
  currentLevel->isWord = true;
}

int Dictionary::FindWord(std::string& word)
{
  std::shared_ptr<Node> currentLevel = root_;
  //iterate letter by letter to insert into the dictionary
  for (unsigned i = 0; i < word.size(); ++i)
  {
    //Check if the letter exists on current level to keep indexing
    if (currentLevel->children['z' - word[i]])
    {
      currentLevel = currentLevel->children['z' - word[i]];
    }
    //If it can't continue further then it'll never be a word
    else
    {
      return 0;
    }
  }

  //Last check to see if it's a word or it can possibly be a word
  if (currentLevel->isWord)
    return 1;
  //Otherwise it still can be a word. ex 'supplme' can be built into 'supplement'
  else
    return -1;
}

{% endhighlight %}