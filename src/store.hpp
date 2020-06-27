// basic data store that holds key value pairs

#pragma once
#include <iostream>
#include "observer.hpp"
#include <map>
#include <vector>

class Store
{
private:
  std::map<int, int> data;
  std::vector<Observer *> observers;

public:
  Store()
  {
  }
  void create(int key, int value)
  {
    data.insert(std::pair<int, int>(key, value));
  }
  int read(int key)
  {
    std::map<int, int>::iterator it = data.find(key);
    if (it != data.end())
    {
      return it->second;
    }
  }
  void update(int key, int newValue)
  {
    std::map<int, int>::iterator it = data.find(key);
    if (it != data.end())
    {
      it->second = newValue;
      notify();
    }
  }

  void attach(Observer *observer)
  {
    observers.push_back(observer);
  }
  void notify()
  {
    for (int i = 0; i < observers.size(); i++)
    {
      observers[i]->notify();
    }
  }

  ~Store()
  {
    for (int i = 0; i < observers.size(); i++)
    {
      delete observers[i];
    }
  }
};