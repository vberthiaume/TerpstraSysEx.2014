/*
  ==============================================================================

    Interval.h
    Created: 8 May 2022 4:32:40pm
    Author:  Vincenzo

    A class for representing tuning intervals. 
    
    TODO: Use monzos internally and support more formats

  ==============================================================================
*/

#pragma once

class Interval
{
    double value = 0;

    Interval(double centsValue) : value(centsValue) {}

public:

    Interval() {}

    double getCents() const { return value; }

    static Interval CentsInterval(double cents)
    {
        return Interval(cents);
    }
};
