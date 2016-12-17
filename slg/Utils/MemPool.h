#pragma once

typedef unsigned int	UINT;

#define DEFAULT_ELES	16

//#include <new>

class CClassCache
{
public:
	CClassCache(UINT uEleSize,UINT uBaseEles = DEFAULT_ELES,UINT uAddEles = DEFAULT_ELES, const char *sName = 0);
	~CClassCache(void);
    
	void *	NewEle(void);			// ����һ��Ԫ��
	void	DelEle(void *pEle);		// �ͷ�һ��Ԫ��
	UINT	GetAllocSize(void);
	UINT	GetMaxAllocSize(void);
	void	Clear(void);
    
private:
	class HeadList
	{
	public:
		HeadList *	pPrevHead;
		HeadList *	pNextHead;
		UINT		uSize;
		char		pData[4];	// Ϊ���ֽڶ���
	};
	char	m_sPoolName[256];	// ���������
	UINT	m_uEleSize;			// ÿ��Ԫ�صķ���Ĵ�С
	UINT	m_uBaseEles;		// ���ķ����Ԫ�ظ���
	UINT	m_uAddEles;			// ��һ����������ӵĲ���
    
	UINT	m_uAllocEles;		// �Ѿ����õ���Ԫ�ظ���
	UINT	m_uAllocMaxEles;	// ��󱻷���Ԫ�صĸ���
    
	void  *	m_pFreeFirst;		// ����ָ������
    
	HeadList m_HeadList;		// ����ָ��
    
	bool	ExtendPool(void);	//
	void	Init(void);
};

#define DECLARE_POOL(ClassName)	\
private:\
static CClassCache s_MemPool;\
public:\
static ClassName * 	NEW_##ClassName(void);\
static void			DEL_##ClassName(ClassName * pDelClass);\
static void			CLR_##ClassName();


#define IMPLEMENT_POOL(ClassName,BaseEles,AddEles)\
CClassCache	ClassName::s_MemPool(sizeof(ClassName),BaseEles,AddEles,#ClassName);\
ClassName * ClassName::NEW_##ClassName(void)\
{\
    ClassName * pNewClass = (ClassName *)(s_MemPool.NewEle());\
    if(pNewClass)\
	{\
		new (pNewClass) ClassName;\
	}\
    return pNewClass;\
}\
void ClassName::DEL_##ClassName(ClassName* pDelClass)\
{\
    if(pDelClass)\
    {\
        pDelClass->~ClassName();\
        s_MemPool.DelEle(pDelClass);\
    }\
}\
void ClassName::CLR_##ClassName()\
{\
    s_MemPool.Clear();\
}
