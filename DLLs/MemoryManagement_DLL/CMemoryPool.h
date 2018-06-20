#pragma once
//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Memory-pool
//   to avoid allocating countless oversized 4KB blocks
//   when we want to create only small objects.
//   http://www.maxinmontreal.com/forums/viewtopic.php?f=111&t=20996
//
//******************************************************************************

#include <afxtempl.h>
#include <map>
#include "LibDef.h"

MEMORY_MANAGEMENT_DLL_API void CreateMemoryPools();
MEMORY_MANAGEMENT_DLL_API void DeleteAllMemoryPools();

class MEMORY_MANAGEMENT_DLL_API CMemoryPool {
 public:
  CMemoryPool();
  ~CMemoryPool();
 public:
  void* Allocate(size_t size);
  void ReleaseAll();
 private:
  void AllocateNewMemoryBlock();
  void AlignNextMemoryBlock();
  size_t BytesAvailableInCurrentBlock();
 private:
  bool _all_emmory_released;
  int _bytes_used_in_current_block;
  void* _current_memory_block;
 private:
  // For gouse-keeping
  // All blocks to be released at some point
  CArray <void*, void*> _memory_blocks;
};

MEMORY_MANAGEMENT_DLL_API CMemoryPool* MemoryPoolTablemaps();
MEMORY_MANAGEMENT_DLL_API CMemoryPool* MemoryPoolScraper();
MEMORY_MANAGEMENT_DLL_API CMemoryPool* MemoryPoolUserLogic();
MEMORY_MANAGEMENT_DLL_API CMemoryPool* MemoryPoolLibraryLogic();
MEMORY_MANAGEMENT_DLL_API CMemoryPool* MemoryPoolGlobal();

// Selects either MemoryPoolLibraryLogic() or MemoryPoolUserLogic()
// MemoryPoolLibraryLogic() stays till the very end
// MemoryPoolUserLogic() will be thrown away when we load a new formula
MEMORY_MANAGEMENT_DLL_API CMemoryPool* PMemoryPoolParser(bool for_read_only_openppl_library_and_default_bot);