#if !defined( __CLASSTYPES_HPP )
#define __CLASSTYPES_HPP

// ??? #include "afx.h"

typedef unsigned int hashValueType;
typedef unsigned int sizeType;

typedef void (*iterFuncType )( void *, void * );
typedef int (*condFuncType )( void *, void * );
typedef int  countType;

/*

*/

class cForEach {                                
//
// implements a common iterating procedure for containers
//

public:
	
	virtual void ForEach( iterFuncType, void * )=0;    
    virtual ~cForEach() {}
};

class cFirstThat {
public:
	virtual void * firstThat( condFuncType, void * ) const;	
    virtual ~cFirstThat() {}
};


class cLastThat {
public:
	virtual const void* lastThat( condFuncType, void * ) const;	
    virtual ~cLastThat() {}
};

#endif      // __CLASSTYPES_HPP
