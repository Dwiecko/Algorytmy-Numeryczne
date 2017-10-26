/*
  Author: Dawid Więcko
  Description: compile with -lm argument, 
  sample: gcc -Wall -std=c99 alg_num_1.c -lm
*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//Declaration of function prototypes
double getPositiveNumber(double number);
double my_pow(double number, int pow);

double* CreateArrayForPredecessor(double number,int limit);
double* createArrayForSeries(double number,int limit);

double sumForwardForPattern(double number, int limit);
double sumBackwardForPattern(double number, int limit);
double sumForwardForPredecessor(double number, int limit);
double sumBackwardForPredecessor(double number, int limit);

void  showResultForConcreteNumber(double currentNumber, int limitOfCurrentNumber);
void  generateStatistics(double start,double end, double step, int limit);


int main()
{
  int limit = 100000;
  printf("Program porównuje rożne sposoby sumy szeregu arctan(x)  \n");
  generateStatistics(-0.99,1,0.01,limit);
  //showResultForConcreteNumber(-0.99,limit);

  return 0;
}

//Implementation of my abs(x) function
double getPositiveNumber(double number)
{
  double tempNumber = number;
  if(tempNumber < 0) 
    tempNumber *= -1;

  return tempNumber;
}
//Return Pow of number
double my_pow(double number, int pow)
{
	if(pow == 0)
		return 1;

	if( pow%2 == 1)
		return my_pow(number, (pow-1))*number;

	double res= my_pow(number, pow/2);
  
  return res*res;
}
            /*Function creates array for computing values
                     using Taylor's series */
double* createArrayForSeries(double number,int limit)
{
  double* array = calloc(limit+1,sizeof(double));
  int n, powNumber;

  for(n=0; n<limit; n++)
  {
    powNumber = 2*n+1;
    array[n] = (my_pow(-1,n) * my_pow(number,powNumber)) / ((2*n)+1);
  }

  return array;
}

double sumForwardForPattern(double number, int limit)
{
  int i;
  double* tempArray = createArrayForSeries(number,limit);
  double tempValue = 0.0;

  for( i = 0; i <= limit; i++)
  {
     tempValue += tempArray[i];
  }

  free(tempArray);
  return tempValue;
}

double sumBackwardForPattern(double number, int limit)
{
  int i;
  double* tempArray = createArrayForSeries(number,limit);
  double tempValue = 0.0;

  for( i = limit; i >= 0; i--)
  {
     tempValue += tempArray[i];
  }
  
  free(tempArray);
  return tempValue;
}
            /*Function creates array for computing values
                     using predecessor method  */
double* CreateArrayForPredecessor(double number,int limit)
{
  double* array = calloc(limit+1,sizeof(double));
  double nextElement = number;
  int i;
  double a;

  for( i = 0; i <= limit; i++)
  {
     a = ((-(number * number) * (2*i+1)) / (2*i+3));
     nextElement *= a;
     array[i] = nextElement;
  }
  return array;
}

  double sumForwardForPredecessor(double number, int limit)
  {
    int i;
    double* tempArray = CreateArrayForPredecessor(number,limit);
    double tempValue = number;
  
    for( i = 0; i <= limit; i++)
    {
       tempValue += tempArray[i];
    }
    free(tempArray);
    return tempValue;
  }

  double sumBackwardForPredecessor(double number, int limit)
  {
    int i;
    double* tempArray = CreateArrayForPredecessor(number,limit);
    double tempValue = number;
  
    for( i = limit; i >= 0; i--)
    {
       tempValue += tempArray[i];
    }
    free(tempArray);
    return tempValue;
  }

  void showResultForConcreteNumber(double currentNumber, int limitOfCurrentNumber)
  {
    printf( "Liczba        %2.3lf \nBiblioteka    %3.60lf\n"
            "SzeregP-K     %3.60lf\nSzeregK-P     %3.60lf\n"
            "PoprzednikP-K %3.60lf\nPoprzednikK-P %3.60lf\n",
            currentNumber,
            atan(currentNumber),
            sumForwardForPattern(currentNumber,limitOfCurrentNumber),
            sumBackwardForPattern(currentNumber,limitOfCurrentNumber),
            sumForwardForPredecessor(currentNumber, limitOfCurrentNumber), 
            sumBackwardForPredecessor(currentNumber,limitOfCurrentNumber));
  }

  void generateStatistics(double start,double end, double step, int limit)
  {
    double i;
    double  library = 0,
            averageSeriesFromBeginToEnd = 0,
            averageSeriesFromEndToBegin = 0,
            averageNextElementFromBeginToEnd = 0,
            averageNextElementFromEndToBegin = 0;
    int numberOfRounds = 0;

  printf("\n Błąd bezwzględny: \n");

  for(i=start; i <= end; i+= step)
  {
    library = atan(i);

    // UNCOMMENT THIS SEGMENT, COMMENT NEXT ONE AND 'PODSUMOWANIE' TO SEE THE PLAIN DATA
    // averageSeriesFromBeginToEnd      = sumForwardForPattern(i,limit);
    // averageSeriesFromEndToBegin      = sumBackwardForPattern(i,limit);
    // averageNextElementFromBeginToEnd = sumForwardForPredecessor(i, limit);
    // averageNextElementFromEndToBegin = sumBackwardForPredecessor(i,limit);
    
    averageSeriesFromBeginToEnd      += getPositiveNumber(library-sumForwardForPattern(i,limit));
    averageSeriesFromEndToBegin      += getPositiveNumber(library-sumBackwardForPattern(i,limit));
    averageNextElementFromBeginToEnd += getPositiveNumber(library-sumForwardForPredecessor(i, limit));
    averageNextElementFromEndToBegin += getPositiveNumber(library-sumBackwardForPredecessor(i,limit));
     
     numberOfRounds++;
    printf("Liczba        %2.3lf \nBiblioteka    %.50lf \n"         //Easy to read format
           "SzeregP-K     %3.50lf\nSzeregK-P     %3.50lf\n"
           "PoprzednikP-K %3.50lf\nPoprzednikK-P %3.50lf\n\n",
    // printf("%2.3lf;%.20lf;%3.20lf;%3.20lf;%.20lf;%.20lf \n",     // CSV FORMAT
            i, library, averageSeriesFromBeginToEnd, 
            averageSeriesFromEndToBegin, 
            averageNextElementFromBeginToEnd,
            averageNextElementFromEndToBegin);
  }
    printf("\n Podsumowanie: \n");
    printf("Licznik: %d\n",numberOfRounds);
    printf( "SzeregP-K     %3.50lf\nSzeregK-P     %3.50lf\n"
            "PoprzednikP-K %3.50lf\nPoprzednikK-P %3.50lf\n",
            (averageSeriesFromBeginToEnd/numberOfRounds), 
            (averageSeriesFromEndToBegin/numberOfRounds), 
            (averageNextElementFromBeginToEnd/numberOfRounds), 
            (averageNextElementFromEndToBegin/numberOfRounds));
  }
