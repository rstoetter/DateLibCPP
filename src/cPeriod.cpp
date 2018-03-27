//
// cPeriod.cpp
//

#include "cPeriod.hpp"        // die Basisklasse

#include    <iostream>

cDate cPeriod::GetFirst ( void ) const
{
    // std::cout<<"cPeriod::GetFirst\n";
    assert ( this );
    assert( m_start.IsValid ( ) );
    // std::cout<<"cPeriod::GetFirst returning"<<m_start<<" \n";
    return cDate ( m_start );
    // return ( m_start );
}

cPeriod::cPeriod (  )
{
    m_start.SetToday (  );
    m_end.SetToday ( );
}

cPeriod::cPeriod ( const cPeriod & oPeriod ):
cForEach()
{
    m_start = oPeriod.m_start;
    m_end = oPeriod.m_end;
}

cPeriod::~cPeriod (  )
{
    // NOP
}

cPeriod::cPeriod ( const cDate & dFirst, const cDate & dLast )
:m_start ( dFirst ), 
m_end ( dLast )
{				
}


cPeriod::cPeriod ( const cDate & dFirst )
:m_start ( dFirst ), 
m_end ( dFirst )
{				
}

cPeriod::cPeriod ( const cDate & dFirst, int n )	
:
m_start ( dFirst ), 
m_end ( dFirst.AsJulian (  ) + n )
{
}


int
cPeriod::SetFirst ( const cDate * dStart, int doCorrect )
{
	// return 1 if dEnd<dStart => dEnd=dStart
	//
	assert ( dStart );
	assert ( dStart->IsValid (  ) );
	//
	m_start = *dStart;
	assert ( m_start.IsValid (  ) );
	//
	if ( ( doCorrect ) && ( m_end < m_start ) ) {
		m_end = m_start;
		return 1;
	}
	//
	return 0;
}

int
cPeriod::SetLast ( const cDate * dEnd, int doCorrect )
{
	assert ( dEnd );
	assert ( dEnd->IsValid (  ) );
	//
	m_end = *dEnd;
	//
	assert ( m_end.IsValid (  ) );
	//
	if ( ( doCorrect ) && ( m_start > m_end ) ) {
		m_start = m_end;
		return 1;
	}
	//
	return 0;
}



void
cPeriod::Set ( const cDate & dStart, const cDate & dEnd )
{
	assert ( dStart.IsValid (  ) );
	assert ( dEnd.IsValid (  ) );
	//
	if ( dStart <= dEnd ) {
		m_start = dStart;
		m_end = dEnd;
	} else {
		m_start = dEnd;
		m_end = dStart;
	}
}


void
cPeriod::ForEach ( iterFuncType f, void *args )
{
    for ( cDate d = m_start; d <= m_end; d++ ) {
        ( *f ) ( &d, args );
    }
}

void
cPeriod::ForEachWeekday ( iterFuncType f, void *args )
{
    for ( cDate d = m_start; d <= m_end; d++ ) {
        if (d.IsWeekday( ) ) ( *f ) ( &d, args );
    }
}


void cPeriod::
Resize ( unsigned int lenDays )
{
	// a period has a minimum of one day 

	assert ( lenDays > 0 );

        m_end = m_start;
        m_end +=  (lenDays -1);
        
}


bool
cPeriod::Intersects ( const cPeriod & aPeriod ) const
{

    return cPeriod::Contains ( aPeriod.m_start ) ||
                    Contains ( aPeriod.m_end ) ||
                    aPeriod.Contains ( m_start ) || 
                    aPeriod.Contains ( m_end );
}


void
AdjustPeriods ( const cPeriod & plimit, cPeriod * p2 )
{
	// assert that <p2> fits into <plimit>

	if ( p2->GetFirst (  ) < plimit.GetFirst (  ) )
		p2->SetFirst ( plimit.GetFirst (  ) );
	//
	if ( p2->GetLast (  ) > plimit.GetLast (  ) )
		p2->SetLast ( plimit.GetLast (  ) );
	//
	assert ( plimit.Contains ( *p2 ) );
}

void
cPeriod::SetLenInWeekdays ( int n )
{
	//
	assert ( this );
	assert ( n >0 );
	//
	cDate
		oDate ( GetFirst (  ) );
	//
	if ( ( oDate.IsValid (  ) ) && ( n != 0 ) ) {
	   oDate.SkipWeekdays( n -1 );
	   SetLast ( oDate );
	}
	assert ( oDate.IsValid (  ) );
}

int
cPeriod::GetWeekdayCount ( void ) const
{
	int ret = 0;
	cDate dWork;
	//
	for ( dWork = m_start; dWork <= m_end; dWork++ ) {
		if ( dWork.IsWeekday (  ) )
			ret++;
	}
	//
	return ret;
}

int
cPeriod::GetMonthCount (  )
{
    int ret = 0;
		//
    assert( IsValid() );

    cDate d = GetFirst (  );
    assert( d.IsValid( ) );

    while ( d <= GetLast (  ) ) {
        d.AddMonths (  );
        ret++;
        assert ( d.IsValid (  ) );
    }

    return ret;
}

int
cPeriod::GetWeekCount ( )
{
    int ret = 0;

    assert ( IsValid (  ) );

    cDate d = GetFirst (  );
    assert ( d.IsValid (  ) );

    while ( d <= GetLast (  ) ) {
        d.AddWeeks (  );
        ret++;
        assert ( d.IsValid (  ) );
    }

    return ret;
}

int
cPeriod::GetYearCount ( )
{
    int ret = 0;
    //
    assert ( IsValid (  ) ); 
    
    cDate d = GetFirst (  );
    assert( d.IsValid (  ) );

    while ( d <= GetLast (  ) ) {
        d.AddYears (  );
        ret++;
        assert ( d.IsValid (  ) );
    }

    return ret;

}


std::ostream & operator << ( std::ostream & out, const cPeriod & obj ) {
	obj.PrintOn ( out );
	return out;
}

void
cPeriod::PrintOn ( std::ostream & outputStream ) const
{
	outputStream << "cPeriod(" << m_start << " .. " << m_end << ")";
}

 int cPeriod::
IsValid ( void ) const
{
    return m_start.IsValid (  ) && m_end.IsValid (  ) && ( m_start <= m_end );
}

 cDate cPeriod::operator[] ( int nIndex ) const {
    // return nth day start index is 0

    return ( cDate ( m_start.AsJulian (  ) + nIndex ) );
}

 bool cPeriod::
AddDays ( int n )
{
    m_start += n;
    m_end += n;

    return ( IsValid (  ) );
}

 bool cPeriod::
AddWeeks ( int n )
{
    m_start.AddWeeks ( n );
    m_end.AddWeeks ( n );

    return ( IsValid (  ) );
}

 bool cPeriod::
AddMonths ( int n )

{

    m_start.AddMonths ( n );
    m_end.AddMonths ( n );

    return ( IsValid (  ) );
}

 bool cPeriod::
AddYears ( int n )
{
    m_start.AddYears ( n );
    m_end.AddYears ( n );

    return ( IsValid (  ) );
}

 bool
cPeriod::StartsBefore ( const cPeriod & oPeriod ) const
{
    return ( m_start < oPeriod.m_start );
}           

 bool
cPeriod::StartsBefore ( const cDate & oDate ) const
{
    return ( m_start < oDate );
}

 bool
cPeriod::StartsAfter ( const cPeriod & oPeriod ) const
{
    return ( m_start > oPeriod.m_start );
}               

 bool
cPeriod::StartsAfter ( const cDate & oDate ) const
{
    return ( m_start > oDate );
}

 cPeriod & 
cPeriod::operator += ( int lenDays ) {

    m_start += lenDays;
    m_end += lenDays;

    return *this;
}

 cPeriod & 
cPeriod::operator -= ( int lenDays ) {
    m_start -= lenDays;
    m_end -= lenDays;

    return *this;
}

 cPeriod 
cPeriod :: operator++ (  ) {
    m_start++;
    m_end++;
    return *this;
}

 cPeriod 
cPeriod :: operator++ ( int ) {
    cPeriod temp = *this;   // Postfix : Wert einbehalten

    m_start++;
    m_end++;
    return temp;
}

 cPeriod 
cPeriod :: operator-- (  ) {
    m_start--;
    m_end--;
    return *this;
}

 cPeriod 
cPeriod :: operator-- ( int ) {
    cPeriod tmp = *this;

    m_start--;
    m_end--;

    return tmp;
}

 int
cPeriod::operator == ( const cPeriod & aPeriod ) const {
    return ( m_start == aPeriod.m_start ) && ( m_end == aPeriod.m_end );
}

 int
cPeriod::operator != ( const cPeriod & aPeriod ) const {
    return ( m_start != aPeriod.m_start ) || ( m_end != aPeriod.m_end );
}

 int
cPeriod::AdjustBothEnds ( int lenDays )
{
    m_start += lenDays;
    m_end += lenDays;
    return 1;
}

 bool
cPeriod::Contains ( const cDate & aDate ) const
{
    return ( aDate >= m_start ) && ( aDate <= m_end );
}

 bool
cPeriod::Contains ( const cDate * aDate ) const
{
    return ( ( *aDate ) >= m_start ) && ( ( *aDate ) <= m_end );
}

 bool
cPeriod::Contains ( const cPeriod & aPeriod ) const
{
    return ( aPeriod.m_start >= m_start ) && ( aPeriod.m_end <= m_end );
}

 bool
cPeriod::Contains ( const cPeriod * aPeriod ) const
{
    return ( aPeriod->m_start >= m_start ) && ( aPeriod->m_end <= m_end );
}

 bool
cPeriod::Overlaps ( const cPeriod & aPeriod ) const 
{
    return Intersects ( aPeriod );
}

 long
cPeriod::GetDiffDays ( void ) const
 // length in days, NOTE : returns 0 if (dEnd==dStart)
{
    assert ( this );
    assert ( IsValid (  ) );
    //
    return m_end.AsJulian (  ) - m_start.AsJulian (  );
}

 long
cPeriod::GetDays ( void ) const
 // length in days, NOTE : returns 1 if (dEnd==dStart)
{
    assert ( IsValid (  ) );
    //
    return GetDiffDays (  ) + 1;
}

 cPeriod & cPeriod::operator = ( const cPeriod & src ) {
    m_start = src.m_start;
    m_end = src.m_end;

    return *this;
}

 int
compare ( const cPeriod & oPeriod, const cDate & oDate )
{
    return oPeriod.GetFirst (  ) > oDate ? 1 : ( oPeriod.GetFirst (  ) < oDate ) ? -1 : 0;
}

 int
compare ( const cPeriod & p1, const cPeriod & p2 )
{
    return ( p1.GetFirst (  ) > p2.GetFirst (  ) ) ? 1 : ( p1.GetFirst (  ) < p2.GetFirst (  ) ) ? -1 : 0;
}

 cPeriod operator + ( const cPeriod & a, int n ) {
    return cPeriod ( a.GetFirst (  ).AsJulian (  ) + n, a.GetLast (  ).AsJulian (  ) + n );
}

 cPeriod operator - ( const cPeriod & a, int n ) {
    return cPeriod ( a.GetFirst (  ).AsJulian (  ) - n, a.GetLast (  ).AsJulian (  ) - n );
}

 int
cPeriod::SetFirst ( const cDate & dStart )
{
    return SetFirst ( &dStart );
}

 cDate
cPeriod::GetLast ( void ) const
{
    return cDate ( m_end );
}

 int
cPeriod::SetEnd ( const cDate * dEnd )
{
    return SetLast ( dEnd );
}
 int
cPeriod::SetLast ( const cDate & dEnd )
{
    return SetLast ( &dEnd );
}

 int cPeriod::
SetEnd ( const cDate & dEnd )
{
    return SetLast ( &dEnd );
}

 void cPeriod::
Set ( const cDate & dStart, int lenDays )
{
    m_start = dStart;
    SetLen ( lenDays );
}

 void cPeriod::
SetLen ( int lenDays )
{
        assert ( lenDays > 0 );
    Resize ( lenDays );
}

