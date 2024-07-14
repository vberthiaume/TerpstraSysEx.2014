/*
  ==============================================================================

    CommonFunctions.h
    Created: 17 Jun 2020 9:54:26pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#ifndef LUMATONE_EDITOR_LIBRARY_MATH_H
#define LUMATONE_EDITOR_LIBRARY_MATH_H

#include <JuceHeader.h>

const double PI = 3.14159265359f;

static int PRIMES[100] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
        31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
        73, 79, 83, 89, 97, 101, 103, 107, 109, 113,
        127, 131, 137, 139, 149, 151, 157, 163, 167, 173,
        179, 181, 191, 193, 197, 199, 211, 223, 227, 229,
        233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
        283, 293, 307, 311, 313, 317, 331, 337, 347, 349,
        353, 359, 367, 373, 379, 383, 389, 397, 401, 409,
        419, 421, 431, 433, 439, 443, 449, 457, 461, 463,
        467, 479, 487, 491, 499, 503, 509, 521, 523, 541
    };


static int modulo(int numIn, int mod)
{
	int val = 0;

	if (mod != 0)
		val = ((numIn % mod) + mod) % mod;

	return val;
}

static double roundToNDecimals(double numIn, int numDecimals)
{
	int f = pow(10, numDecimals);
	return round(numIn * f) / f;
}

static float roundToNDecimals(float numIn, int numDecimals)
{
	int f = pow(10, numDecimals);
	return roundf(numIn * f) / f;
}

static int getGCD(int num1, int num2)
{
	if (num1 == 0 || num2 == 0)
		return num1 + num2;
	else if (num1 == 1 || num2 == 1)
		return 1;
	else if (num1 == num2)
		return num1;

	return getGCD(num2, num1 % num2);
}

static int getLCM(int num1, int num2)
{
	if (num1 == 0 || num2 == 0)
		return 0;

	long gcd = getGCD(num1, num2);
	return (int)(juce::jmax(num1, num2) / gcd * (juce::jmin(num1, num2)));
}

static juce::Array<int> getCoprimes(int numIn)
{
    juce::Array<int> coprimes = {1};
    int m, d, t;
    for (int i = 2; i < numIn - 1; i++)
    {
        m = numIn;
        d = i;
        while (d > 1)
        {
            m %= d;
            t = d;
            d = m;
            m = t;
        }

		if (d > 0)
		{
			coprimes.add(i);
		}
    }

    coprimes.add(numIn-1);

    return coprimes;
}

static juce::Array<int> getFactors(int numIn)
{
    juce::Array<int> factors;

	double factorized = numIn;

	int maxPrime = (int)sqrt(numIn);

	while (factorized > 1.0)
	{
		int i = 0;
		double p = 1;
		double q = factorized;

		while (factorized > 1 && p < maxPrime && i < 100)
		{
			int exp = 0;
			p = PRIMES[i];

			q = factorized / p;

			// See if it divides more than once
			while ((int)q == q)
			{
				factorized = q;
				exp++;
				q = factorized / p;
			}

			factors.set(i, exp);
			i++;
		}
	}

    return factors;
}

static juce::Array<int> getContinuedFraction(double num, int maxDepth=20, double round0Thresh=10e-8)
{
    juce::Array<int> cf;
    double f = num;
    double nextF;

    for (int i = 0; i < maxDepth; i++)
    {
        cf.add((int)f);
        nextF = f - cf[i];
        if (nextF > round0Thresh)
            f = 1.0 / nextF;
        else
            break;
    }

    return cf;
}

#endif //LUMATONE_EDITOR_LIBRARY_MATH_H
