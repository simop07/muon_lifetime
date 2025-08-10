// ClopperPearson efficiency evaluation for scintillator

#include <iostream>

#include "TEfficiency.h"

void eff() {
  double cl = 0.95;
  int n = 12779;
  int k = 12276;

  TEfficiency teff = TEfficiency("Eff", "Eff", 1, 0, 1);
  double eff = (double(k) / n);

  std::cout << "efficiency: " << eff << '\n';

  // std::cout << "lower: " << teff.ClopperPearson(n, k, cl, false) << " upper "
  //    << teff.ClopperPearson(n, k, cl, true) << '\n';

  // gives the lower bound to the efficienc
  double low = eff - teff.ClopperPearson(n, k, cl, false);
  // gives the upper bound to the efficiency
  double high = teff.ClopperPearson(n, k, cl, true) - eff;

  std::cout << "range: " << low << " -- " << high << '\n';

  std::cout << "Efficiency value: " << eff << "^ { + " << high << " }"
            << " _ { - " << low << " }" << '\n';
}

// #stampa:
// efficiency : 0.960639 xrange : 0.00351711 --0.00330528 Efficiency
//         value : 0.960639 ^
// { +0.00330528 } _ {
//    - 0.00330528 
// }