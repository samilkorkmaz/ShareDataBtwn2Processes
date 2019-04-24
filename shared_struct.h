//Reference: http://read.pudn.com/downloads179/sourcecode/math/833872/Util/General/CSharedStruct.h__.htm
#ifndef _SHARED_STRUCT_
#define _SHARED_STRUCT_

#include "Aclapi.h"

template <class StructType>
class CSharedStruct
{
private:
   HANDLE m_hFileMapping;
   char   m_hSharedName[MAX_PATH];
   DWORD  m_dwMaxDataSize;
   StructType *m_pvData;

   BOOL m_bCreated;      

public:
   CSharedStruct();
   CSharedStruct(char *Name);
   ~CSharedStruct();

   VOID Release();
   BOOL Acquire(char *Name);

   StructType *operator->();
};


template <class StructType>
StructType *CSharedStruct<StructType>::operator->()
{
   return m_pvData;
}

template <class StructType>
CSharedStruct<StructType>::CSharedStruct()
{
   m_hFileMapping = NULL;
}

template <class StructType>
CSharedStruct<StructType>::~CSharedStruct()
{
   Release();
}


template <class StructType>
CSharedStruct<StructType>::CSharedStruct(char *Name)
{
   CSharedStruct();
   Acquire(Name);
}

template <class StructType>
VOID CSharedStruct<StructType>::Release()
{
   if (m_pvData)
   {
      UnmapViewOfFile(m_pvData);
      m_pvData = NULL;
   }

   if (m_hFileMapping)
   {
      CloseHandle(m_hFileMapping);
   }
}

template <class StructType>
BOOL CSharedStruct<StructType>::Acquire(char *Name)
{
   m_dwMaxDataSize = 0;
   m_hFileMapping = CreateFileMapping (INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(StructType), Name);

   if (m_hFileMapping == NULL)
   {
      int err = GetLastError();
      return FALSE;
   }
   SetNamedSecurityInfo(Name, SE_KERNEL_OBJECT, DACL_SECURITY_INFORMATION, 0, 0, (PACL) NULL, NULL);

   m_dwMaxDataSize = sizeof(StructType);
   strncpy(m_hSharedName, Name, MAX_PATH - 1);

   m_pvData = (StructType *) MapViewOfFile( m_hFileMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

   if (m_pvData == NULL)
   {
      CloseHandle(m_hFileMapping);
      return FALSE;
   }

   return TRUE;
}

#endif // !_SHARED_STRUCT_
