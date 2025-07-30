#ifndef __COMMON_H__
#define __COMMON_H__

class A 
{
	public:
	int m_nA;
	virtual void WhoAmI();
	static void * m_sArena;
	void * operator new (unsigned int);
};   
class B : public A 
{
	public:
	int m_nB;
	virtual void WhoAmI();
};   
class C : virtual public A 
{
	public:
	int m_nC;
	virtual void WhoAmI();
};
void GetObjects(A ** pA, B ** pB, C ** pC);
#endif //__COMMON_H__

