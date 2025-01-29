#include "derivatives.h"

Derivatives::Derivatives(){}//Derivatives

//Compute first derivative
void Derivatives::first_derivative(vector<double>& f)
{
      vector<double> firstderv;
      double df_dx;

      int size = f.size();
      for(int x = 1; x < size - 1; x++)
      {
        df_dx = f[x + 1] - f[x]; //first-order derivative of a one-dimensional function
        firstderv.push_back(df_dx);
        //QTextStream(stdout) << x << ":" << df_dx << " ";
      }
      f = firstderv;
}// first_derivative

// Compute second derivative
void Derivatives::second_derivative(vector<double>& f_2)
{
    std::vector<double> scndderv;
    double d2f_fx2 = 0.0;

     for(int x = 1; x < f_2.size() - 1; x++)
     {
       d2f_fx2 = f_2[x + 1] - 2 * (f_2[x]) + f_2[x - 1];// second-order derivative of f(x)
       scndderv.push_back(d2f_fx2);
       //QTextStream(stdout) << x << ":" << d2f_fx2 << " ";
     }
     f_2 = scndderv;

}// second_derivative

// Print out results
void Derivatives::printDerivativeValues(vector<double> vect)
{
    for (int i = 0; i < vect.size(); i++)
    {
        cout << vect[i] << ", ";
    }
    cout << endl;
}// printDerivativeValues
