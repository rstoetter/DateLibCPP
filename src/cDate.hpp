#ifndef cDate_HPP
#define cDate_HPP

#ifndef __cplusplus
#error Must use C++ for the type cDate.
#endif

#include            <time.h>
#include            <ostream>

#define LEN_SHORT_MONTHS    3                   // length of short month names

////////////////////////////////////////////////////////////////////////////////////////////
// class support

// relativ zu Ostern berechnete fixe Daten

#define		TAGE_OSTERN_BIS_FASTNACHT           (-48)
#define		TAGE_OSTERN_BIS_KARFREITAG          (-2)
#define		TAGE_OSTERN_BIS_PFINGSTEN           (+49)
#define		TAGE_OSTERN_BIS_FRONLEICHNAM	    (+60)

//
class cDate;         // our new class as forward declaration
//
extern const cDate _cDate_Invalid;  // a global date which return functions containing bad data
extern const cDate cDate_MAX;       // highest possible date
extern const cDate cDate_MIN;       // lowest possible date

extern "C" {
    
    int DOW ( int day, int month, int year );
    int MonthsBetween ( const cDate & d1, const cDate & d2 );   // full months between, -1 err
    int MonthChanges ( const cDate & d1, const cDate & d2 );    // month changes between, -1 err
}

//
bool IsFuture ( const cDate & );	  // is date in the future relative to now ?
bool IsPast ( const cDate & );	  // is date in the past relative to now ?
bool IsToday ( const cDate & );	  // is date same as now's date ?
//
int between ( const cDate & dCmp, const cDate & d1, const cDate & d2 ); // dCmp between [d1..d2]
int compare ( const cDate &, const cDate & );   // 0 if equal, -1 if d1<d2, 1 if d1>d2
void swap ( cDate &, cDate & );
//
cDate _max ( const cDate &, const cDate & );
cDate _min ( const cDate &, const cDate & );

cDate Easter ( int year );          // easter sunday of a certain year

std::ostream & operator << ( std::ostream & , const cDate &  );

std::ostream & operator << ( std::ostream & , const cDate &  );



class cDate {

public:
    // -----------------[ constructors
    cDate ( void );
    cDate ( long julian );              // do not use
    cDate ( int month, int day, int year );    
    cDate ( const cDate & oDate );
    cDate ( const struct tm & );

    ~ cDate ( void );
    // -----------------[ reading member variables
    int Day ( void ) const;             // actual day
    int GetDay ( void ) const;          // actual day
    int SetDay ( void ) const;          // actual day
    int SetMonth ( void ) const;        // actual month    
    int GetMonth ( void ) const;        // actual month
    int Month ( void ) const;           // actual month
    int GetYear ( void ) const;         // actual year with century
    int SetYear ( void ) const;         // actual year with century
    int GetYearWithCentury ( void ) const;	// actual year with century

    // -----------------[ modifying the date
    
    bool SetMonth ( int nMonth );	// returns 1 if successful otherwise 0
    bool SetDay ( int nDay );            // returns 1 if successful otherwise 0
    bool SetYear ( int nYear );	        // returns 1 if successful otherwise 0

    const cDate & Set ( void );             // set actual date
    const cDate & Set ( long JulianDate );  
    const cDate & Set ( int nMonth, int nDay, int nYear, bool  try2correctwrongdates = 1 );

    const cDate & SetToday ( void );	// set internal date to actual date
    void FromSQL( const char *);        // read date from a sql string ie 2003-03-27
    void FromGermanDate( const char *); // read date from a german date ie 27.03.2007

    // -----------------[ diagnostics : inspecting the date
    int NOQ ( void ) const;	        // number of quarter or -1 if date is not valid
    int MonthDays ( void ) const;	// days in month [1..31]
    bool IsLeapYear ( void ) const;	// is actual year a leapyear ?
    int DOW ( void ) const;	        // day of week (1..7)
    int DOY ( void ) const;             // nth day in year
    bool IsWeekend ( void ) const;	// is it a saturday or a sunday ?
    bool IsWeekday ( void ) const;	// is it not weekend ?
    int WOM ( void ) const;	        // nth week in month [1..6]
    int WOY ( void ) const;	        // nth week in year [1..52]
    int FWDOM ( void ) const;           // first weekday of month [1..7]
    // -----------------[
    cDate BOQ ( void ) const;           // first day of actual quarter
    cDate EOQ ( void ) const;           // last day of actual quarter
    cDate BOQ ( int ) const;            // first day of special quarter
    cDate EOQ ( int ) const;            // last day of special quarter
    cDate BOY ( void ) const;           // first day of actual year
    cDate EOY ( void ) const;           // last day of actual year
    cDate BOW ( void ) const;           // ??monday?? of actual week ???????????
    cDate EOW ( void ) const;           //  sunday of actual week ????????????????
    cDate BOM ( void ) const;           // first day of actual month
    cDate EOM ( void ) const;           // last day of actual month

    // -----------------[ skipping around

    cDate operator + ( int i );
    cDate operator - ( int i );
    long operator - ( const cDate &  );

    const cDate & operator += ( long i );
    const cDate & operator -= ( long i );

    cDate operator++ ( void );
    cDate operator++ ( int );
    cDate operator-- ( void );
    cDate operator-- ( int );

    // NOTE : TODO : GoBOW(int) usw

    void GoBOW ( void );
    void GoEOW ( void );
    void GoBOM ( void );
    void GoEOM ( void );
    void GoBOQ ( void );
    void GoEOQ ( void );
    void GoBOQ ( int );
    void GoEOQ ( int );
    void GoBOY ( void );
    void GoEOY ( void );

    void GoEaster( void );                          // skip to easter sunday of actual year

    void SkipWeekdays( int = 1 );                   // skip weekdays
    void SkipToLastWeekday( void );
    void SkipToNextWeekday( void );

    const cDate & AddWeeks ( int nCount = 1 );  // skipping nCount weeks
    const cDate & AddMonths ( int nCount = 1 ); // skipping nCount Months (uses latest day in month if day is too big for this month )
    const cDate & AddYears ( int nCount = 1 );  // skipping nCount Years (uses latest day in month if day is too big for this month ie 29th february)

    // -----------------[

    int LOW ( void ) const;             // length of a week in days
    int LOM ( void ) const;             // length of this month in days
    int LOQ ( void ) const;             // length of this quarter in days
    int LOY ( void ) const;             // length of this year in days

    // -----------------[ streaming
    friend std::ostream & operator << ( std::ostream & , const cDate &  ); 

    // -----------------[ diagnostics : validation
    int IsValid ( void ) const;	          // is the date valid ?

    // -----------------[ conversions
    long AsJulian ( void ) const;       // return (internal !) julian date
    operator char *( void );            // return string
    const char *AsString ( void );      // return string
    const char *AsSQLString ( void );      // return string

    // -----------------[ overloaded comparison operators

    int operator < ( const cDate &  ) const;
    int operator <= ( const cDate & ) const;
    int operator > ( const cDate &  ) const;
    int operator >= ( const cDate & ) const;
    int operator == ( const cDate & ) const;
    int operator != ( const cDate & ) const;

    // const char * AsStr( void );

    // -----------------[ features
    static int SetAutoSetToday ( void );
    static void SetAutoSetToday ( int onoff = 1 );

protected:
    

    long m_julian_date;
    int m_year;
    unsigned char m_month;
    unsigned char m_day;
    unsigned char m_DayOfWeek;	// [1..7]  := [sunday..saturday]
    static int m_autosync;
    enum { LEN_STR_BUFFER = 50 };
    

private:
    void _Julian2MDY ( void );         // convert julian date to mdy
    void _Julian2Weekday ( void );     // convert julian date to weekday
    void _MDY2Julian ( void );         // convert mdy to julian date
    void _OnDateChange ( void );
    static char m_strbuf[LEN_STR_BUFFER];

};   // of class cDate



#endif		//#ifdef cDate_HPP

