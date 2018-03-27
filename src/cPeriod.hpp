#ifndef cPeriod_HPP
#define cPeriod_HPP

//
//  cPeriod.hpp
//
// cPeriod implements a span of time represented by a start and an ending date. it
// has a minimum length of one day, when start and anding date are equal. therefore 
// the minimum length of a class is 1 day.
// operator overloads touch both dates, when incrementing or decrementing. therefore 
// the operations do not change the length of the period. 
// the indexoperator is zero-based 

#ifndef __cplusplus
#error Must use C++ for the type cPeriod.
#endif

#include            "classtypes.hpp"    // cForEach
#include            "cDate.hpp"         // cDate
#include            <assert.h>          // assert
#include            <ostream>           // ostream

class cPeriod;

cPeriod operator+(const cPeriod& , int );
cPeriod operator-(const cPeriod& , int );

int compare(const cPeriod&, const cPeriod&);
int compare(const cPeriod&, const cDate&);
	// return 0  if same start of period and date
	// return 1  if period starts after date
	// return -1 if period starts before date
void
AdjustPeriods ( const cPeriod &p1, cPeriod *  p2);
// assure that p2 fits into p1

class cPeriod : 
cForEach
 {

public:
    // -----------------[ constructors
    cPeriod( void );                    // period is set on today
    cPeriod( const cDate &dtfirst, const cDate &dtlast);	
    cPeriod( const cDate & );           // period spans one day
    cPeriod( const cDate &, int );	// dStartday and spans len days
    cPeriod( const cPeriod&);           // copy constructor
    ~cPeriod( void );
    // -----------------[ get / set member variables
    virtual cDate GetFirst( void ) const;	
    int SetFirst(const cDate *, int doCorrect = 1); // swap dates if dStart > end
    int SetFirst(const cDate &);	

    cDate GetLast( void ) const;
    virtual int SetLast(const cDate *, int doCorrect = 1);  // swap dates if dStart > end
    int SetEnd(const cDate *);
    int SetEnd(const cDate &);
    int SetLast(const cDate &);

    void Set(const cDate &dStart, int);             // start date and len in days (min len = 1)
    void Set(const cDate &dStart, const cDate &dEnd);
    // -----------------[ set the length of the period
    void SetLen(int lenDays);   
        // a minimum of 1 day
    void Resize(unsigned int lenDays);  
        // aka SetLen() - a minimum of 1 day 
    void SetLenInWeekdays( int );                        
        // a minimum of 1 day

    // -----------------[ diagnostics
    int IsValid(void) const;
    // -----------------[ accessing parts of the period
    virtual void ForEach( iterFuncType, void * );
    virtual void ForEachWeekday( iterFuncType, void * );

    cDate operator[] (int) const;   // return nth day start index is 0
    // -----------------[ scheduling - shifting both ends of the period
    bool AddDays(int count=1);
    bool AddWeeks(int count=1);
    bool AddMonths(int count=1);    // TODO : hier wird das Enddatum verändert 
          // TODO : sollte Möglichkeit geben, dieselbe Länge beizubehalten
    bool AddYears(int count=1);

    int AdjustBothEnds( int );

    // -----------------[ diagnosing the order
    bool StartsBefore(const cPeriod &) const;    // when GetStart() < p2.GetStart()
    bool StartsBefore(const cDate &) const; 
    bool StartsAfter(const cPeriod &) const;     // when GetStart() > p2.GetStart()
    bool StartsAfter(const cDate &) const;
    // -----------------[ compound operators
    cPeriod& operator-=(int lenDays);
    cPeriod& operator+=(int lenDays);
    cPeriod operator++();
    cPeriod operator++(int);
    cPeriod operator--();
    cPeriod operator--(int);
    // -----------------[ comparison operators
    int operator==(const cPeriod &) const;
    int operator!=(const cPeriod &) const;
    // -----------------[ set operations
    bool Contains(const cDate &) const;	         // period contains a special date ?
    bool Contains(const cDate *) const;	         // period contains a special date ?
    bool Contains(const cPeriod &) const;	// period contains another one
    bool Contains(const cPeriod *) const;	// period contains another one
    bool Intersects(const cPeriod &) const;	// period overlaps with another
    bool Overlaps(const cPeriod &) const;        // period overlaps with another	
    // -----------------[ length of the period
    long GetDiffDays(void) const;	// len in days : 0 if (GetEnd() == GetStart())
    long GetDays(void) const;	// len in days : 1 if (GetEnd() == GetStart())
    // -----------------[ assignment
    cPeriod & operator=(const cPeriod & );
    // -----------------[ counting days
    int GetWeekdayCount(void) const;
    int GetMonthCount ( void );
    int GetWeekCount ( void );
    int GetYearCount ( void );
    //------------------[ streaming
    friend std::ostream &operator <<( std::ostream &, const cPeriod & );
    virtual void PrintOn( std::ostream& /*= std::cout*/) const;


protected:
    cDate m_start;
    cDate m_end; 

};  // of class cPeriod





#endif      //#ifdef cPeriod_HPP
