/**
 * File              : time.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 24.04.2023
 * Last Modified Date: 24.04.2023
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

/*
 * Windows can't handle time_t < 0. The function localtime()
 */

#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define YEAR0LEAP  

static inline time_t year_to_s(int64_t year)
{ // converts year to seconds since 1st Jan 1970.
  // While year is a int64_t its assumed to come from an int (32 bits) with a 1900 offset, so we don't need to worry about overflow in the conversion
 int64_t lyears;
 /* we want to factor in leap years to the year before noting year 0 is not a leap year, so 0,1,2,3,4,5,6,.. we want to count 1st leap year when year=5 (as year 4 is a leap year) 
 	for negative years the first one we want to count is the year -5 */
 if(year>=0) lyears=year-1; /* 5-1=4 so lyears/4=1.  0 => -1 but -1/4 is still 0 so thats OK */
 else lyears=year+1;/* -5 +1 = -4 so lyears/4= -1 */
  // calculation now just needs to use yday, year and lyears
#ifdef YEAR0LEAP  
	/* if year>0 then 1 more leap year to add, also adds 1 to offset for 1970 so overall this cancels out for 1970 */
 return ( (time_t)(lyears/4) - (time_t)(lyears/100) + (time_t)(lyears/400 )   + (time_t)(year>0?1:0)+  
		  (time_t)year*365 - (time_t)719528  /* offset to make 1st Jan 1970=0 */
	    )*(time_t)86400; /* 86400=24*60*60;  hours->minutes->seconds */ 
#else
 return ( (time_t)(lyears/4) - (time_t)(lyears/100) + (time_t)(lyears/400 )   +  
		  (time_t)year*365 - (time_t)719527  /* offset to make 1st Jan 1970=0 */
	    )*(time_t)86400; /* 86400=24*60*60;  hours->minutes->seconds */
#endif	    
}

bool is_leap(int64_t year) /* returns true if year [with no offset] is a leap year */
{if(year==0) 
#ifdef YEAR0LEAP 
	return true; //  year 0 IS a leap year 
#else
	return false; // year 0 is a NOT a leap year 
#endif	
 else if(year<0) year= -year; // just in case % operator does silly things with negative arguments
 return (year%4==0 && year%100!=0) || year%400==0; // no need to worry about sign of year here as just check ==0 or !=0
}

static char daytab[2][13]={ /* table of days in a month for non leap years and leap years*/
	 {31,28,31,30,31,30,31,31,30,31,30,31},
	 {31,29,31,30,31,30,31,31,30,31,30,31}
};

void month_day(int64_t year, int yearday, int*pmonth, int *pday)
/* set month, day from year (actual year with no offset) and day of year (actually days since jan 1st, 0-365) */
/* returns pmonth as 0->11, 0-Jan, and pday=1->31 */
{int i;
 bool leap =is_leap(year);
 ++yearday; // supplied as 0=jan 1st, below treats that as the 1st day of the year
 for(i=0;yearday>daytab[leap][i] && i<12;i++) // test for i<12 avoids overrunning the array if yearday is too big
	yearday-=daytab[leap][i];
 *pmonth=i;// 0->11
 *pday=yearday;
}

int day_of_week_yd(int64_t year,int yday) /* year with no offset eg 1900 and yday is days since 1st Jan 0->365 */ 
{// time_t ya_mktime_s(int64_t year, int month,int mday,const int yday,const int hour,const int min, const int sec ) /* version of mktime() that returns secs  */
 int64_t sy=ya_mktime_s(year, 0,0,yday,0,0,0 ); /* version of mktime() that returns secs since 1st Jan 1970 */
 int64_t days=sy/(3600*24); 
 days=(days+4)%7; // +4 as 1st jan 1970 (secs=0) was a Thursday . 
 if(days<0)days+=7;
 return days;
}

void sec_to_tm(time_t t,struct tm *tp) // reverse of ya_mktime_tm, converts secs since epoch to the numbers of tp
{/* note that sec_to_tm() sets all fields in tm (except tz), whereas ya_mktime_tm() does not need all fields set to work, so calling ya_mktime_tm() then sec_to_tm() will ensure all fields are set */
 int64_t year;
 int month, mday, hour, min, sec,yday;
 time_t t_y;
#ifdef YEAR0LEAP  
 year=(t+INT64_C(62167219200))/INT64_C(31556952); // +719528*86400=62,167,219,200 removes "1970" offset added in year_to_s(), 31,556,952=365.2425*24*60*60 which approximates leap years (100-4+1=97 year years in 400 years so 97/400=.2425)
#else 
 year=(t+INT64_C(62167132800))/INT64_C(31556952); // +719527*86400=62,167,132,800 removes "1970" offset added in year_to_s(), 31,556,952=365.2425*24*60*60 which approximates leap years (100-4+1=97 year years in 400 years so 97/400=.2425)
#endif 
 t_y=year_to_s(year); 
 for(int i=0; t_y<t && i<10000;++i)// i stops infinite loops, should only go around this a few times as our initial guess should be quite accurate
 	{t_y=year_to_s(++year); // have to start 1 higher
 	}
 for(int i=0; t_y>t && i<10000;++i) // i stops infinite loops , our initial guess should be out by a small amount as our approximation above is quite accurate
	{
	 t_y=year_to_s(--year); // keep reducing year till its smaller
	}	
 t-=t_y;// get remainder = secs into year
 yday=t/(24*3600);// 24 hrs in a day so this gives us days in the year
 t-=yday*24*3600; // whats left is seconds in the day
 month_day(year,yday,&month,&mday);
 hour=t/3600;
 t-=hour*3600;
 min=t/60;
 sec=t-min*60;
 if(year-1900 < -INT_MAX ) year=-INT_MAX+1900; // clip at min (subtract 1900 below)
 else if(year-1900 > INT_MAX ) year=(int64_t)INT_MAX+1900; // clip at max (subtract 1900 below)
 tp->tm_year=year-1900;
 tp->tm_mon=month;
 tp->tm_mday=mday;
 tp->tm_hour=hour;
 tp->tm_min=min;
 tp->tm_sec=sec;
 tp->tm_yday=yday;
 tp->tm_wday=day_of_week_yd(year,yday);
 // does not set  tm_isdst
}
