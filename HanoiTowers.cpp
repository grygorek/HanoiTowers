/// Copyright 2023 Piotr Grygorczuk <grygorek@gmail.com>
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.


// Hanoi tower algorithm implementation.
// Usage: Run program from a command line and provide number of disks 
// as a program input. Depending on the speed of to computer, anything larger
// than 25 disks starts running slow.

#include <list>
#include <array>
#include <iostream>
#include <string>
#include <algorithm>
#include <chrono>

namespace {

using Tower = std::list<int>;
using HanoiTower = std::array<Tower, 3>;

Tower InitTower(int disks_count)
{
  Tower init;
  for (int i = 1; i <= disks_count; i++)
    init.push_back(i);

  return init;
}


void Dump(const HanoiTower& towers)
{
#if _DEBUG
  char c = 'A';
  for (auto& t : towers)
  {
    std::cout << c++ << ": ";
    std::for_each(t.rbegin(), t.rend(), [](auto i) {std::cout << " " << i; });
    std::cout << "\n";
  }
  std::cout << "\n";
#endif
}

// Move the top disk from the 'from' tower to the 'to' tower.
// Returns false if the move was illegal and was not performed.
bool Move(Tower& from, Tower& to)
{
  if (from.empty())
    return false;

  if (!to.empty())
  {// Odd cannot go on another odd.
    if ((from.front() & 1) && (to.front() & 1))
      return false;

    // Even cannot go on another even.
    if (!(from.front() & 1) && !(to.front() & 1))
      return false;
  }

  if (to.empty() || from.front() < to.front())
  {
    to.splice(to.begin(), from, from.begin(), ++from.begin());
    return true;
  }
  return false;
}

void Even(HanoiTower& tower, int& previous, int& moves)
{
  while (1)
  {
    Dump(tower);
    if (previous != 0 && Move(tower[0], tower[1]))
    {
      moves++;
      previous = 1;
      continue;
    }
    if (previous != 0 && Move(tower[0], tower[2]))
    {
      moves++;
      previous = 2;
      continue;
    }
    if (previous != 1 && Move(tower[1], tower[2]))
    {
      moves++;
      previous = 2;
      continue;
    }
    break;
  }
}

void Odd(HanoiTower& tower, int& previous, int& moves)
{
  while (1)
  {
    Dump(tower);
    if (previous != 0 && Move(tower[0], tower[2]))
    {
      moves++;
      previous = 2;
      continue;
    }
    if (previous != 0 && Move(tower[0], tower[1]))
    {
      moves++;
      previous = 1;
      continue;
    }
    if (previous != 1 && Move(tower[1], tower[2]))
    {
      moves++;
      previous = 2;
      continue;
    }
    break;
  }
}

template<typename OddEvenFun>
void Hanoi(HanoiTower& tower, OddEvenFun move)
{
  auto disks_count = tower[0].size();
  int moves = 0;
  int previous = 2; // idx of a tower on which a disk has been moved in the last move (so we do not move it back)
  while (tower[2].size() != disks_count)
  {
    move(tower, previous, moves);

    if (previous != 2 && Move(tower[2], tower[0]))
    {
      moves++;
      previous = 0;
      continue;
    }
    if (previous != 2 && Move(tower[2], tower[1]))
    {
      moves++;
      previous = 1;
      continue;
    }
    if (previous != 1 && Move(tower[1], tower[0]))
    {
      moves++;
      previous = 0;
      continue;
    }
  }

  std::cout << disks_count << " disks done in " << moves << " moves";
}

void Hanoi(HanoiTower& tower)
{
  if (tower[0].size() & 1)
  {
    Hanoi(tower, Odd);
  }
  else
  {
    Hanoi(tower, Even);
  }
}

}


int DisksCount(int argc, char* argv[])
{
  int disks_count = 3;
  if (argc < 2)
  {
    std::cout << "\nNeed provide a number of disks on program input!\nFor now taking default " 
      << disks_count << " disks\n\n";
  }
  else
  {
    disks_count = std::abs(std::stoi(argv[1]));
    if (disks_count < 1)
    {
      std::cout << "\nDisks count " << disks_count <<
        " does not sound correct. You need at least one disk.\n Will keep default 3 disks.\n\n";

      disks_count = 3;
    }

    if (disks_count > 25)
    {
      std::cout << "\nLarge number of disks may take long to move. Working on it, be patient....\n\n";
    }
  }

  return disks_count;
}

int main(int argc, char* argv[])
{
  int disks_count = DisksCount(argc,argv);

  HanoiTower tower = { InitTower(disks_count),{},{} };

  using namespace std::chrono;
  auto start = system_clock::now();
  Hanoi(tower);
  auto end = system_clock::now();
  std::cout << "\nIt took " << duration_cast<microseconds>(end - start).count() << " us";

  return 0;
}
