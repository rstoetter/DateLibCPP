//
// cDate.cpp
//
 
#include "cDate.hpp"        // die Basisklasse
#include <limits.h>         // Grenzwerte
#include <ctype.h>          // Typüberprüfung
#include <time.h>           // Datumsfunktionen
#include <assert.h>         // assert
#include <stdlib.h>         // atoi
#include <stdio.h>          // sprintf

const cDate _cDate_Invalid ( 50, 50, 0 );
// const cDate cDate_MAX ( LONG_MAX );         // highest possible date
// const cDate cDate_MIN ( LONG_MIN );         // lowest possible date

const cDate cDate_MIN (1,1,1600);       // NOTE : ERROR bei LONG_MAX und LONG_MIN
const cDate cDate_MAX (1,1,2900);

int cDate::m_autosync = 1;                  // set today's date when initializing 

static int _GaussAry[] =
{
    31,
    28,
    31,
    30,
    31,
    30,
    31,
    31,
    30,
    31,
    30,
    31
};

char cDate :: m_strbuf[ cDate::LEN_STR_BUFFER ];    // static element for returning strings

cDate :: ~ cDate ( void ) {
;// NOP
}

cDate :: cDate ( const cDate & oDate )
{
    m_month = oDate.m_month;
    m_day = oDate.m_day;
    m_year = oDate.m_year;

    _MDY2Julian (  );
}

cDate :: cDate ( const struct tm & time )
{
    m_month = time.tm_mon;
    m_day = time.tm_mday;
    m_year = time.tm_year+1900;

    _MDY2Julian (  );
}

cDate :: cDate ( void )
{
    m_month = m_day = m_DayOfWeek = 0;
    m_year = 0;
    m_julian_date = 0;

    if ( m_autosync ) {
        this->SetToday (  );
    }
}


cDate :: cDate ( long j ) : m_julian_date ( ( unsigned long ) j )
//
// TODO : stimmt die Konvertierung nach unsigned ?
//
{
    _Julian2MDY (  );
}


cDate :: cDate ( int m, int d, int y ):m_month ( ( unsigned char ) m ),
m_day ( ( unsigned char ) d ),
m_year ( y )
{
    _MDY2Julian (  );
}

void cDate::
_MDY2Julian ( void )
{
    int a = 0;
    int b = 0;

    int month = m_month;
    int day = m_day;
    int year = m_year;

    if ( year < 0 )
        year++;

    if ( month <= 2 ) {
        year--;
        month += 12;
    }
    if ( year * 10000. + month * 100. + day >= 15821015. ) {
        a = ( int ) ( year / 100. );
        b = 2 - a + a / 4;
    }
    m_julian_date = ( long ) ( 365.25 * year ) + ( long ) ( 30.6001 * ( month + 1 ) ) +
        day + 1720994L + b;

    _Julian2Weekday (  );
}

void cDate::
_Julian2Weekday ( void )
{
    m_DayOfWeek = ( unsigned char ) ( ( m_julian_date + 2 ) % 7 + 1 );
}

void cDate::
_Julian2MDY (  )
{
    // as mentioned in the Lexikon Mathematik

    long a, b, c, d, e, z, _alpha;
    z = m_julian_date + 1;
    if ( z < 2299161L )
        a = z;
    else {
        _alpha = ( long ) ( ( z - 1867216.25 ) / 36524.25 );
        a = z + 1 + _alpha - _alpha / 4;
    }

    b = ( a > 1721423L ? a + 1524 : a + 1158 );
    c = ( long ) ( ( b - 122.1 ) / 365.25 );
    d = ( long ) ( 365.25 * c );
    e = ( long ) ( ( b - d ) / 30.6001 );

    m_day = ( unsigned char ) ( b - d - ( long ) ( 30.6001 * e ) );
    m_month = ( unsigned char ) ( ( e < 13.5 ) ? e - 1 : e - 13 );
    m_year = ( int ) ( ( m_month > 2.5 ) ? ( c - 4716 ) : c - 4715 );

    _Julian2Weekday (  );

    assert( IsValid( ) );
}


bool cDate::SetYear ( int nYear )
{
    if ( nYear > 9999 ) {
        return false;
    }

    Set ( m_month, m_day, nYear );

    return true;
}

bool cDate::SetDay ( int nDay )
{
    if ( nDay <= MonthDays (  ) )
        m_day = ( unsigned char ) nDay;
    else
        return false;

    Set ( m_month, m_day, m_year );

    return true;
}

bool cDate::SetMonth ( int nMonth )
{
    if ( ( nMonth > 0 ) && ( nMonth < 13 ) )
        Set ( m_month = ( unsigned char ) nMonth, m_day, m_year );
    else
        return false;

    return true;
}

const cDate & cDate::
Set ( long j )
{
    m_julian_date = j;

    _Julian2MDY (  );

    return *this;
}

int cDate::
MonthDays ( void ) const
{
    return _GaussAry[m_month - 1] + ( m_month == 2 && IsLeapYear (  ) );
}

bool cDate::IsLeapYear ( void ) const
{
    return ( ( m_year >= 1582 ) ? ( m_year % 4 == 0 && m_year % 100 != 0 || 
                m_year % 400 == 0 ) : ( m_year % 4 == 0 ) );
}


const cDate & cDate::Set ( void )
{
    struct tm *newtime;
    time_t long_time;

    time ( &long_time );                // get time as long integer
    newtime = localtime ( &long_time ); // convert time to local time

    m_month = newtime->tm_mon + 1;
    m_day = newtime->tm_mday;
    m_year = ( newtime->tm_year ) + 1900;

    _MDY2Julian (  );
    _Julian2Weekday ( );

    // return cDate ( *this );
    return *this;
}


const cDate & cDate::Set ( int nMonth, int nDay, int nYear, bool doCheck )

// TODO : korrigiere modifikationen von m_year

{
    m_day = 1;
    m_month = ( unsigned char ) nMonth;
    m_year = nYear < 0 ? 9999 : nYear;
    m_year = nYear > 9999 ? 0 : nYear;

    _MDY2Julian (  );

    if ( doCheck )
        m_day = ( unsigned char ) ( nDay < MonthDays (  ) ? nDay : MonthDays (  ) );
    else {
        if ( nDay > MonthDays (  ) ) {
            *this = _cDate_Invalid;
        } else
            m_day = ( unsigned char ) ( nDay );
    }

    _MDY2Julian (  );

    return *this;
}

int cDate::
SetAutoSetToday ( void )
{
    return m_autosync;
}

void cDate::
SetAutoSetToday ( int onoff )
{
    m_autosync = onoff;
}

const cDate &
cDate::SetToday ( void )
{
    return Set (  );
}

//////////////////////////////////////////////////////////////////////////////////////////

/*
cDate cDate ::  + ( int i ) {
    return cDate ( m_julian_date + ( long ) i );
}
*/

cDate cDate :: operator + ( int i ) {
    m_julian_date += i;
    _Julian2MDY( );
    return *this;
}



cDate cDate :: operator - ( int i ) {
    m_julian_date -= i;
    _Julian2MDY( );
    return *this;
}

long cDate :: operator - ( const cDate & oDate ) {
    return ( m_julian_date - oDate.m_julian_date );
}

const cDate & cDate :: operator += ( long i ) {
    m_julian_date += i;
    _Julian2MDY (  );
    return *this;
}

const cDate & cDate :: operator -= ( long i ) {
    m_julian_date -= i;
    _Julian2MDY (  );
    return *this;
}

cDate cDate :: operator++ (  ) {
    m_julian_date++;
    _Julian2MDY (  );
    return *this;
}

cDate cDate :: operator++ ( int ) {
    cDate tmpDate = *this;         // Postfix : Wert einbehalten
    m_julian_date++;
    _Julian2MDY (  );
    return tmpDate;
}

cDate cDate :: operator-- (  ) {
    m_julian_date--;
    _Julian2MDY (  );
    return *this;
}

cDate cDate :: operator-- ( int ) {
    cDate tmpDate = *this;         // Postfix : Wert einbehalten
    m_julian_date--;
    _Julian2MDY (  );
    return tmpDate;
}


//////////////////////////////////////////////////////////////////////////////////////////

cDate
cDate::BOQ ( void ) const {

    static const char ary[] = { 1, 1, 1, 4, 4, 4, 7, 7, 7, 10, 10, 10 };
    return cDate ( ary[m_month-1] , 1, m_year );
}

cDate
cDate::EOQ ( void ) const {
    static const char ary[] = { 3, 3, 3, 6, 6, 6, 9, 9, 9, 12, 12, 12 };
    return cDate ( ary[m_month-1], 31, m_year );
}

cDate
cDate::BOQ ( int n ) const {
    static const char ary[] = { 1, 4, 7, 10 };
    //
    if ( ( n < 1 ) || ( n > 4 ) )
        return _cDate_Invalid;
    //

    return cDate ( ary[n - 1], 1, m_year );

}
cDate
cDate::EOQ ( int n ) const {
    char static const ary[] = { 3, 6, 9, 12 };
    //
    if ( ( n < 1 ) || ( n > 4 ) )
        return _cDate_Invalid;
    //

    return cDate ( ary[n - 1], 1, m_year ).EOM (  );

}

cDate
cDate::EOW ( void ) const
{
    cDate 
        tmp ( m_julian_date );

    if ( IsValid (  ) )
        while ( tmp.DOW (  ) != 1 )
            tmp++;
    return tmp;
}

cDate
cDate::BOW ( void ) const
{
    cDate 
        tmp ( m_julian_date );

    if ( IsValid (  ) )
        while ( tmp.DOW (  ) != 2 )
            tmp--;
    return tmp;
}

const char * cDate::AsString( void ) {

    sprintf(this->m_strbuf, "%02d.%02d.%02d", m_day, m_month, m_year);
    return this->m_strbuf;

}

const char * cDate::AsSQLString( void ) {
//2008-03-24
    sprintf(this->m_strbuf, "%d-%02d-%02d", m_year, m_month, m_day);
    return this->m_strbuf;

}

int cDate::
DOY ( void ) const
{
    cDate tmpDate ( 1, 1, m_year );
    return ( int ) ( m_julian_date - tmpDate.m_julian_date + 1 );
}

int
cDate::NOQ ( void ) const {
    if ( !IsValid (  ) )
        return -1;
    if ( m_month < 4 )
        return 1;
    if ( m_month < 7 )
        return 2;
    if ( m_month < 10 )
        return 3;
    return 4;
}

int cDate::
WOY ( void ) const
{
    cDate doTemp ( 1, 1, m_year );
    doTemp = doTemp.BOW (  );

    return ( int ) ( ( ( m_julian_date - doTemp.m_julian_date + 1 ) / 7 ) + 1 );
}

int cDate::
WOM ( void ) const
{
    int nAbsDay = m_day + FWDOM (  ) - 1;
    return ( nAbsDay - DOW (  ) ) / 7 + 1;
}
////////////////////////////////////////////////////////////////////////////////////
const cDate & cDate::
AddWeeks ( int nCount )
{
    Set ( m_julian_date + ( long ) nCount * 7 );
    return *this;
}

const cDate & cDate::
AddMonths ( int nCount )
{
    nCount += m_month;
    while ( nCount < 1 ) {
        nCount += 12;
        m_year--;
    }
    while ( nCount > 12 ) {
        nCount -= 12;
        m_year++;
    }

    Set ( ( unsigned char ) nCount, m_day, m_year );

    return *this;
}

const cDate & cDate::
AddYears ( int nCount )
{
    
    Set ( m_month, m_day, m_year + nCount );

    if ( ( m_month == 2 ) && ( m_day==29 ) && ( !IsLeapYear( ) ) )
        Set ( 3, 1, m_year );   // auf ersten März

    return *this;
}


void cDate::
SkipToNextWeekday( void ) {

    m_julian_date += 1;
    _Julian2MDY (  );

    while ( !IsWeekday( ) ) {
        m_julian_date += 1;
        _Julian2MDY (  );
    }

}

void cDate::
SkipToLastWeekday( void ) {

    m_julian_date -= 1;
    _Julian2MDY (  );

    while ( !IsWeekday( ) ) {
        m_julian_date -= 1;
        _Julian2MDY (  );
    }

}


void cDate::
SkipWeekdays ( int nCount )
{

    int fertig = 0;

    if (nCount >0) {
        if (!IsWeekday( )) {
            SkipToLastWeekday( );
        }

        for (int i=0;i<nCount; i++) {
            SkipToNextWeekday( );
        }

    } else if (nCount <0) {
        if (!IsWeekday( )) {
            SkipToNextWeekday( );
        }
        for (int i=nCount;i<0; i++) {
            SkipToLastWeekday( );
        }

    }

}



////////////////////////////////////////////////////////////////////////////////////

/*
cDate :: cDate ( const time_t & time )
{
    m_month = time.month;
    m_day = time.day;
    m_year = time.year;

    _MDY2Julian (  );
}
*/

int cDate::
FWDOM ( void ) const
{
    return cDate ( m_month, 1, m_year ).DOW (  );
}
///////////////////////////////////////////////////////////////////////////////////
int
cDate::IsValid ( void ) const
{
    return ( ( m_month > 0 ) && ( m_month < 13 ) && ( m_DayOfWeek > 0 ) && ( m_DayOfWeek < 8 ) && ( m_day > 0 ) && ( m_day <= MonthDays (  ) ) );
}
////////////////////////////////////////////////////////////////////////////////////



extern "C"
int
DOW ( int date, int month, int year )
{
    int day;
    int yr;
    int mn;
    int n1;
    int n2;

    #define ZELLER_OFFSET   -1

//	The parameter ZELLER_OFFSET is used to convert the day of the week from Zeller to local.


    yr = year;
    mn = month;


    if ( mn < 3 ) {             // if January or February
        // .. then make these part of last year
        mn += 12;
        yr -= 1;
    }
    n1 = ( 26 * ( mn + 1 ) ) / 10;
    n2 = ( int ) ( ( 125 * ( long ) yr ) / 100 );

    day = ( ( date + n1 + n2 - ( yr / 100 ) + ( yr / 400 ) + ZELLER_OFFSET ) % 7 );
    return day;
}


#pragma optimize( "", off )		// turn off all optimization:

cDate
Easter ( int year )
{
//
// using algorithm from mr. Gauss
//
    int c, n, k, i, j, l, m, d;
    c = year / 100;
    n = year - 19 * ( year / 19 );
    k = ( c - 17 ) / 25;
    i = c - c / 4 - ( c - k ) / 3 + 19 * n + 15;
    i = i - 30 * ( i / 30 );
    i = i - ( i / 28 ) * ( 1 - ( i / 28 ) * ( 29 / ( i + 1 ) ) * ( ( 21 - n ) / 11 ) );
    j = year + year / 4 + i + 2 - c + c / 4;
    j = j - 7 * ( j / 7 );
    l = i - j;
    m = 3 + ( l + 40 ) / 44;
    d = l + 28 - 31 * ( m / 4 );

    return cDate ( m, d, year );
}
#pragma optimize( "", on )		

void cDate::GoEaster( void ) {
    cDate easter = Easter( m_year );
    
    m_day = easter.SetDay( );
    m_month = easter.SetMonth( );

    _MDY2Julian (  );
}


extern "C" int
MonthsBetween ( const cDate & d1, const cDate & d2 )
{
    cDate d = d1;
    int
      i = 0;

    if ( d <= d2 ) {
        while ( d <= d2 ) {
            d.AddMonths (  );
            if ( d <= d2 )
                i++;
        }
        return i;
    }
    return -1;

}

extern "C" int
MonthChanges ( const cDate & d1, const cDate & d2 )
{

    cDate d = d1;
    int
      i = 0;

    if ( d <= d2 ) {
        while ( d <= d2 ) {
            d.AddMonths (  );
            i++;
        }
        //
        return i - 1;
    }
    return -1;

}
////////////////////////////////////////////////////////////////////////////////////
void cDate::
FromSQL( const char *sqlstr) {
    // 2008-03-25

    assert (sqlstr[4] == '-');
    assert (sqlstr[7]== '-');

    m_year = atoi( sqlstr );
    m_month = atoi( sqlstr +5 );
    m_day = atoi( sqlstr +8 );   

    _MDY2Julian (  );

}

void cDate::
FromGermanDate( const char *str) {
    // 27.05.2008

    assert (str[2] == '.');
    assert (str[5]== '.');

    m_day = atoi( str );
    m_month = atoi( str +3 );
    m_year = atoi( str +6 );   
    
    _MDY2Julian (  );
}

////////////////////////////////////////////////////////////////////////////////////

std::ostream & operator << ( std::ostream & os, const cDate & oDate ) {
    return ( os << ( ( cDate ) oDate ).AsString (  ) );
}

 int cDate::
SetMonth ( void ) const
{
    return m_month;
}

 int cDate::
SetDay ( void ) const
{
    return m_day;
}

 int cDate::
SetYear ( void ) const
{
    return m_year;
}

 int cDate::
GetDay ( void ) const
{
    // Nummer Wochentag
    return Day (  );
}

 int cDate::
GetMonth ( void ) const
{
    // number of month
    return Month (  );
}

 int
cDate::GetYear ( void ) const
{
    // return century
    return GetYearWithCentury (  );
}

 int 
cDate::GetYearWithCentury ( void ) const
{
    return m_year;
}

 int cDate::
Month ( void ) const
{
    return m_month;
}

 int cDate::
Day ( void ) const
{
    return m_day;
}

 int cDate::
DOW ( void ) const
{
    return m_DayOfWeek;
}

bool cDate::IsWeekend ( void ) const
{
    return ( DOW (  ) == 7 ) || ( DOW (  ) == 1 );
}

bool cDate::IsWeekday ( void ) const
{                               
    return ( ! IsWeekend (  ) );
}



 long cDate::
AsJulian ( void ) const
{
    return m_julian_date;
}


 cDate cDate::
BOY ( void ) const
{
    return ( cDate ( 1, 1, m_year ) );
}

 cDate cDate::
EOY ( void ) const
{
    return ( cDate ( 1, 1, m_year + 1 ) - 1 );
}

 cDate cDate::
BOM ( void ) const
{
    return ( cDate ( m_month, 1, m_year ) );
}

 cDate cDate::
EOM ( void ) const

{
    return ( cDate ( m_month , MonthDays() , m_year )  );
}

 cDate :: operator char *( void ) {
    // AsString ist const char !
    AsString (  );
    return m_strbuf;
}

 void
cDate::GoBOY ( void )
{
    Set ( BOY (  ).AsJulian( ) );
}
 void
cDate::GoEOY ( void )
{
    Set ( EOY (  ).AsJulian( )  );
}

 void
cDate::GoEOQ ( void )
{
    Set ( EOQ (  ).AsJulian( )  );
}

 void
cDate::GoBOQ ( void )
{
    Set ( BOQ (  ).AsJulian( )  );
}

 void
cDate::GoBOM ( void )
{
    Set ( BOM (  ).AsJulian( )  );
}
 void
cDate::GoEOM ( void )
{
    Set ( EOM (  ).AsJulian( )  );
}
 void
cDate::GoBOW ( void )
{
    Set ( BOW (  ).AsJulian( )  );
}
 void
cDate::GoEOW ( void )
{
    Set ( EOW (  ).AsJulian( )  );
}

 void
cDate::GoBOQ ( int n )
{                              

    Set ( BOQ ( n ).AsJulian( )  );

}


 void
cDate::GoEOQ ( int n )
{

    Set ( EOQ ( n ).AsJulian( )  );

}

 int
between ( const cDate & dCmp, const cDate & d1, const cDate & d2 )
{
    return ( ( dCmp >= d1 ) && ( dCmp <= d2 ) );
}

 int
compare ( const cDate & d1, const cDate & d2 )
{
    if ( d1 == d2 )
        return 0;
    else if ( d1 < d2 )
        return -1;
    else
        return 1;
}

 cDate
_max ( const cDate & d1, const cDate & d2 )
{
    return ( ( d1 > d2 ) ? d1 : d2 );
}

 cDate
_min ( const cDate & d1, const cDate & d2 )
{
    return ( ( d1 < d2 ) ? d1 : d2 );
}

 void
swap ( cDate & d1, cDate & d2 )
{
    cDate tmpdate = d1;

    d1 = d2;
    d2 = tmpdate;
}


 int cDate::
operator < ( const cDate & oDate ) const
{
    return ( m_julian_date < oDate.m_julian_date );
}

 int cDate::
operator <= ( const cDate & oDate ) const
{
    return ( ( m_julian_date == oDate.m_julian_date ) || ( m_julian_date < oDate.m_julian_date ) );
}

 int cDate:: 
operator > ( const cDate & oDate ) const
{
    return ( m_julian_date > oDate.m_julian_date );
}

 int cDate ::
operator >= ( const cDate & oDate ) const
{
    return ( ( m_julian_date == oDate.m_julian_date ) || ( m_julian_date > oDate.m_julian_date ) );
}

 int cDate ::
operator == ( const cDate & oDate ) const
{
    return ( m_julian_date == oDate.m_julian_date );
}

 int cDate ::
operator != ( const cDate & oDate ) const
{
    return ( m_julian_date != oDate.m_julian_date );
}

 bool
IsPast ( cDate const & d )
{                               // Datum<Systemdatum ?
    cDate tmpDate;
    tmpDate.SetToday (  );
    return ( d < tmpDate );
}

 bool
IsFuture ( const cDate & d )
{                               // Datum>Systemdatum ?
    cDate tmpDate;
    tmpDate.SetToday (  );
    return ( d > tmpDate );
}

 bool
IsToday ( const cDate & d )
{                               // Datum==Systemdatum ?
    cDate tmpDate;
    tmpDate.SetToday (  );
    return ( d == tmpDate );
}

 int cDate::
LOM ( void ) const
{
    return ( EOM (  ).AsJulian (  ) - BOM (  ).AsJulian (  ) ) + 1;
}

 int cDate::
LOY ( void ) const
{
    return ( EOY (  ).AsJulian (  ) - BOY (  ).AsJulian (  ) ) + 1;
}


