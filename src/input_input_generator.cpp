#include<bits/stdc++.h>
#define PRECISION 1000000
int main()
{
  std::random_device rand_dev; // Uniformly distributed integer RNG 
  std::mt19937 generator(rand_dev()); // Mersene twister for RNG for a more uniform distribution
  std::uniform_int_distribution<int> distn(1,5); 
  std::uniform_int_distribution<int> distp(0,PRECISION); 
  int n = distn(generator);
  double p = (double)(distp(generator))/(double)PRECISION;

  std::cout << n << " " << p << std::endl;;
}
