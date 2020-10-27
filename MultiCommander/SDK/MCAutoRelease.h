//  RIIA Helper lasses
//
//
#pragma once

#include "MCNamespace.h"
#include "Collections.h"

MCNSBEGIN

class AutoFileItemCollection
{
public:
  AutoFileItemCollection(IFileItemCollection* p = nullptr)
    : m_pCollection(p)
  {

  }

  ~AutoFileItemCollection()
  {
    Release();
  }
  
  bool isValid()
  {
    if (m_pCollection)
      return true;

    return false;
  }
  void Release()
  {
    if (isValid())
    {
      m_pCollection->Release();
      m_pCollection = nullptr;
    }
  }

  AutoFileItemCollection& operator=(IFileItemCollection* p)
  {
    Release();
    m_pCollection = p;
    return *this;
  }

  operator IFileItemCollection*()
  {
    return m_pCollection;
  }

  IFileItemCollection* operator->()
  {
    return m_pCollection;
  }

private:
  IFileItemCollection* m_pCollection;
};

MCNSEND
