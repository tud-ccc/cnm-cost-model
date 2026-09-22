
#include <upmem_cost_model/ScatterGatherCm.h>

#include <cmath> // std::log2, std::exp
#include <limits>

// Fitted by model_tree.py on 34345 measured configs (upmemcm/scatter_cost/plots/block), relRMSE 6.90%.
namespace {
double scatterBlockCostMsModel(int num_dpus, int block_size) {
  if (block_size <= 29696) {
    if (num_dpus <= 62) {
      if (num_dpus <= 1) {
        if (block_size <= 1024) {
          return 0.02445235 + -0.008213792 * num_dpus + 2.978938e-07 * block_size + 7.102343e-14 * (double)num_dpus * block_size;
        } else {
          return 0.01619632 + -1.281508e-16 * num_dpus + 8.605126e-07 * block_size + 2.051622e-13 * (double)num_dpus * block_size;
        }
      } else {
        if (num_dpus <= 2) {
          if (block_size <= 1024) {
            return 0.01748827 + -0.0007018879 * num_dpus + 3.151592e-07 * block_size + 1.502796e-13 * (double)num_dpus * block_size;
          } else {
            return 0.01601812 + 1.956107e-15 * num_dpus + 8.710994e-07 * block_size + 4.153725e-13 * (double)num_dpus * block_size;
          }
        } else {
          if (num_dpus <= 3) {
            if (block_size <= 1024) {
              return 0.0273873 + -0.00371685 * num_dpus + 3.254964e-07 * block_size + 2.340849e-13 * (double)num_dpus * block_size;
            } else {
              return 0.0161575 + -2.451633e-10 * num_dpus + 9.239315e-07 * block_size + 6.608473e-13 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 6) {
              return 0.01672115 + -0.0001306697 * num_dpus + 2.75449e-07 * block_size + 1.6214e-07 * (double)num_dpus * block_size;
            } else {
              return 0.01582244 + 0.0001022445 * num_dpus + 5.944503e-07 * block_size + 7.198824e-08 * (double)num_dpus * block_size;
            }
          }
        }
      }
    } else {
      if (num_dpus <= 400) {
        if (num_dpus <= 64) {
          if (block_size <= 1024) {
            return 0.01465229 + 8.575594e-05 * num_dpus + 1.451604e-05 * block_size + 2.214971e-10 * (double)num_dpus * block_size;
          } else {
            return 0.01868029 + 5.791616e-05 * num_dpus + 4.12662e-06 * block_size + 6.296722e-11 * (double)num_dpus * block_size;
          }
        } else {
          if (block_size <= 168) {
            if (num_dpus <= 80) {
              return 0.0125651 + 0.0002627977 * num_dpus + -7.625204e-06 * block_size + 2.668404e-07 * (double)num_dpus * block_size;
            } else {
              return 0.02409778 + 6.964125e-05 * num_dpus + 1.91525e-05 * block_size + 2.155361e-08 * (double)num_dpus * block_size;
            }
          } else {
            if (block_size <= 1920) {
              return 0.02648893 + 8.158031e-05 * num_dpus + 1.653404e-05 * block_size + 1.184939e-09 * (double)num_dpus * block_size;
            } else {
              return 0.02623402 + 6.280551e-05 * num_dpus + 7.59238e-06 * block_size + 8.752472e-09 * (double)num_dpus * block_size;
            }
          }
        }
      } else {
        if (num_dpus <= 448) {
          if (block_size <= 800) {
            return 0.1739477 + -0.000249877 * num_dpus + -7.947799e-05 * block_size + 2.912719e-07 * (double)num_dpus * block_size;
          } else {
            if ((double)num_dpus * block_size <= 884736) {
              return 0.1393603 + -0.0001042337 * num_dpus + -2.566391e-05 * block_size + 8.340381e-08 * (double)num_dpus * block_size;
            } else {
              return 0.05227525 + 3.680158e-05 * num_dpus + 6.284668e-05 * block_size + -1.100116e-07 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (block_size <= 1920) {
            if (block_size <= 672) {
              return 0.0434894 + 7.674546e-05 * num_dpus + 6.942876e-05 * block_size + -7.863442e-09 * (double)num_dpus * block_size;
            } else {
              return 0.08867501 + 7.219518e-05 * num_dpus + 7.058848e-06 * block_size + 3.845656e-09 * (double)num_dpus * block_size;
            }
          } else {
            if (block_size <= 7680) {
              return 0.04489747 + 7.230935e-05 * num_dpus + 2.110714e-05 * block_size + 2.121523e-09 * (double)num_dpus * block_size;
            } else {
              return 0.08406892 + -3.420902e-05 * num_dpus + 1.722056e-05 * block_size + 1.45496e-08 * (double)num_dpus * block_size;
            }
          }
        }
      }
    }
  } else {
    if (num_dpus <= 64) {
      if ((double)num_dpus * block_size <= 15237120) {
        if (num_dpus <= 32) {
          if (num_dpus <= 8) {
            if (num_dpus <= 2) {
              return 0.007984273 + 0.0005475631 * num_dpus + 1.047497e-06 * block_size + -5.108047e-09 * (double)num_dpus * block_size;
            } else {
              return 0.01000012 + 0.0006663873 * num_dpus + 9.162231e-07 * block_size + 3.095321e-08 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 16) {
              return -0.02207953 + 0.002129061 * num_dpus + 2.315521e-06 * block_size + -3.987082e-08 * (double)num_dpus * block_size;
            } else {
              return 0.001548618 + 0.0002808099 * num_dpus + 1.640711e-06 * block_size + 4.204662e-08 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (block_size <= 51200) {
            if (num_dpus <= 44) {
              return 0.01096417 + 0.0001678436 * num_dpus + 4.82042e-06 * block_size + -7.986688e-09 * (double)num_dpus * block_size;
            } else {
              return 0.01653396 + 3.266797e-05 * num_dpus + 4.529555e-06 * block_size + -1.86937e-10 * (double)num_dpus * block_size;
            }
          } else {
            if (block_size <= 180224) {
              return -0.01678643 + -1.715905e-05 * num_dpus + 5.042328e-06 * block_size + 1.146744e-09 * (double)num_dpus * block_size;
            } else {
              return -0.0194627 + -0.002935186 * num_dpus + 4.658506e-06 * block_size + 2.346645e-08 * (double)num_dpus * block_size;
            }
          }
        }
      } else {
        if (num_dpus <= 32) {
          if (num_dpus <= 16) {
            if (num_dpus <= 8) {
              return -5.591451 + 0.2130193 * num_dpus + 1.109639e-06 * block_size + 2.812052e-07 * (double)num_dpus * block_size;
            } else {
              return -1.905059 + 0.006773202 * num_dpus + 2.936591e-06 * block_size + 2.502031e-08 * (double)num_dpus * block_size;
            }
          } else {
            if ((double)num_dpus * block_size <= 25165824) {
              return -3.007257 + 0.02857975 * num_dpus + 4.032636e-06 * block_size + 9.366381e-08 * (double)num_dpus * block_size;
            } else {
              return -4.167603 + 0.07856616 * num_dpus + 5.959101e-06 * block_size + 2.77907e-08 * (double)num_dpus * block_size;
            }
          }
        } else {
          if ((double)num_dpus * block_size <= 30474240) {
            if ((double)num_dpus * block_size <= 18841600) {
              return -1.460508 + 0.0001560188 * num_dpus + 5.839205e-06 * block_size + 8.499632e-08 * (double)num_dpus * block_size;
            } else {
              return -5.051979 + 0.02671476 * num_dpus + 1.064442e-05 * block_size + 1.062803e-07 * (double)num_dpus * block_size;
            }
          } else {
            if (block_size <= 4456448) {
              return -2.028238 + 0.0107707 * num_dpus + 1.10426e-05 * block_size + 4.306077e-08 * (double)num_dpus * block_size;
            } else {
              return -9.446037 + 0.0717639 * num_dpus + 1.25899e-05 * block_size + 3.215797e-08 * (double)num_dpus * block_size;
            }
          }
        }
      }
    } else {
      if ((double)num_dpus * block_size <= 25165824) {
        if ((double)num_dpus * block_size <= 13778944) {
          if (num_dpus <= 72) {
            return -0.002288789 + 0.0002046084 * num_dpus + 9.029165e-06 * block_size + -1.61883e-08 * (double)num_dpus * block_size;
          } else {
            if (num_dpus <= 216) {
              return -0.01114076 + -1.595907e-05 * num_dpus + 1.030189e-05 * block_size + 1.668381e-09 * (double)num_dpus * block_size;
            } else {
              return -0.3205193 + 0.001198748 * num_dpus + 1.429857e-05 * block_size + -1.311866e-08 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (block_size <= 163840) {
            if (num_dpus <= 448) {
              return -1.122693 + 0.002159693 * num_dpus + 1.696375e-05 * block_size + 1.06931e-08 * (double)num_dpus * block_size;
            } else {
              return -0.8078512 + 0.0009892851 * num_dpus + 4.235816e-05 * block_size + -1.292819e-08 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 84) {
              return 2.681357 + -0.06649318 * num_dpus + -2.96863e-05 * block_size + 6.761448e-07 * (double)num_dpus * block_size;
            } else {
              return -1.420168 + -0.01545418 * num_dpus + 9.056821e-06 * block_size + 2.006921e-07 * (double)num_dpus * block_size;
            }
          }
        }
      } else {
        if (num_dpus <= 448) {
          if (num_dpus <= 352) {
            if (num_dpus <= 72) {
              return -7.001222 + 0.07134967 * num_dpus + 2.061565e-05 * block_size + -6.061758e-09 * (double)num_dpus * block_size;
            } else {
              return -1.790389 + 0.0006440192 * num_dpus + 2.306566e-05 * block_size + 4.096765e-08 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 416) {
              return -2.662838 + 0.003274289 * num_dpus + -8.093045e-05 * block_size + 3.215821e-07 * (double)num_dpus * block_size;
            } else {
              return -1.33595 + -0.0009195327 * num_dpus + 3.483799e-05 * block_size + 4.514479e-08 * (double)num_dpus * block_size;
            }
          }
        } else {
          if ((double)num_dpus * block_size <= 251658240) {
            if (num_dpus <= 992) {
              return -2.140741 + 0.0005227914 * num_dpus + 4.780116e-05 * block_size + 4.984088e-08 * (double)num_dpus * block_size;
            } else {
              return -1.059909 + -0.000714917 * num_dpus + 3.818154e-05 * block_size + 5.194942e-08 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 1920) {
              return 1.600746 + 0.0001086875 * num_dpus + 5.599456e-05 * block_size + 2.061429e-08 * (double)num_dpus * block_size;
            } else {
              return -74.20358 + 0.03553348 * num_dpus + 0.0006143894 * block_size + -2.463615e-07 * (double)num_dpus * block_size;
            }
          }
        }
      }
    }
  }
}
} // namespace

double upmem_cm::scatterBlockCostMs(int num_dpus, int block_size) {
  // No transfer beats the fastest rate this sweep measured (4.62408e+07 B/ms);
  // outside its fitted region a leaf's linear model can slope below zero.
  return std::fmax(scatterBlockCostMsModel(num_dpus, block_size), (double)num_dpus * block_size / 4.624084e+07);
}

// Fitted by model_tree.py on 34040 measured configs (upmemcm/scatter_cost/plots/shared), relRMSE 7.32%.
namespace {
double scatterSharedCostMsModel(int num_dpus, int block_size) {
  if (num_dpus <= 64) {
    if (num_dpus <= 31) {
      if (num_dpus <= 1) {
        if (block_size <= 1024) {
          return 0.01109974 + 0.005103254 * num_dpus + 1.574006e-07 * block_size + 1.537115e-10 * (double)num_dpus * block_size;
        } else {
          if (block_size <= 17408) {
            return 0.01607777 + -1.597803e-15 * num_dpus + 8.568791e-07 * block_size + 8.367959e-10 * (double)num_dpus * block_size;
          } else {
            if (block_size <= 294912) {
              return 0.01291056 + -4.390665e-16 * num_dpus + 9.740788e-07 * block_size + 9.512487e-10 * (double)num_dpus * block_size;
            } else {
              return -0.02051234 + 0 * num_dpus + 1.069578e-06 * block_size + 1.04451e-09 * (double)num_dpus * block_size;
            }
          }
        }
      } else {
        if (num_dpus <= 2) {
          if (block_size <= 1024) {
            return 0.01966393 + -0.001416765 * num_dpus + 2.928386e-06 * block_size + 5.719504e-09 * (double)num_dpus * block_size;
          } else {
            if (block_size <= 17408) {
              return 0.01737249 + 0 * num_dpus + 1.58037e-06 * block_size + 3.08666e-09 * (double)num_dpus * block_size;
            } else {
              return 0.01996447 + 2.596455e-16 * num_dpus + 1.036103e-06 * block_size + 2.02364e-09 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (num_dpus <= 3) {
            if (block_size <= 1024) {
              return 0.03128289 + -0.004575746 * num_dpus + 2.131557e-06 * block_size + 6.235119e-09 * (double)num_dpus * block_size;
            } else {
              return 0.01079981 + 0.001914807 * num_dpus + 1.017023e-06 * block_size + 2.954579e-09 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 4) {
              return 0.03015723 + -0.002954991 * num_dpus + 1.048699e-06 * block_size + 4.096479e-09 * (double)num_dpus * block_size;
            } else {
              return 0.01344813 + 0.0002917619 * num_dpus + 6.652423e-07 * block_size + 7.623087e-08 * (double)num_dpus * block_size;
            }
          }
        }
      }
    } else {
      if (num_dpus <= 32) {
        if (block_size <= 1984) {
          return 0.05242153 + -0.0009094648 * num_dpus + 4.660616e-06 * block_size + 1.456443e-07 * (double)num_dpus * block_size;
        } else {
          if (block_size <= 34816) {
            return 0.02357994 + -2.028341e-05 * num_dpus + 1.574748e-06 * block_size + 4.921086e-08 * (double)num_dpus * block_size;
          } else {
            if (block_size <= 2031616) {
              return 0.04900883 + 3.672639e-15 * num_dpus + 1.402801e-06 * block_size + 4.383753e-08 * (double)num_dpus * block_size;
            } else {
              return -0.4735432 + 0 * num_dpus + 1.481018e-06 * block_size + 4.628182e-08 * (double)num_dpus * block_size;
            }
          }
        }
      } else {
        if (num_dpus <= 34) {
          if (block_size <= 1728) {
            return 0.05174445 + -0.0007449136 * num_dpus + 4.302516e-06 * block_size + 1.428552e-07 * (double)num_dpus * block_size;
          } else {
            return -0.002979044 + 0.0008557288 * num_dpus + 2.117561e-06 * block_size + 7.030974e-08 * (double)num_dpus * block_size;
          }
        } else {
          if (num_dpus <= 36) {
            if (block_size <= 1024) {
              return 0.01455622 + 0.0001912083 * num_dpus + 4.533936e-06 * block_size + 1.593965e-07 * (double)num_dpus * block_size;
            } else {
              return 0.03669738 + -0.0003584419 * num_dpus + 1.986014e-06 * block_size + 6.98201e-08 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 38) {
              return 0.02113721 + 7.214413e-05 * num_dpus + 1.85668e-06 * block_size + 6.889988e-08 * (double)num_dpus * block_size;
            } else {
              return 0.01871558 + 0.000143031 * num_dpus + 4.458321e-06 * block_size + 3.89672e-10 * (double)num_dpus * block_size;
            }
          }
        }
      }
    }
  } else {
    if (num_dpus <= 384) {
      if (block_size <= 200) {
        if (num_dpus <= 76) {
          return 0.08008999 + -0.0005801 * num_dpus + 1.583046e-05 * block_size + -3.454616e-08 * (double)num_dpus * block_size;
        } else {
          if (num_dpus <= 88) {
            return 0.03687101 + 3.519971e-05 * num_dpus + -0.0001209243 * block_size + 1.707832e-06 * (double)num_dpus * block_size;
          } else {
            if (num_dpus <= 100) {
              return 0.06474288 + -0.0002596725 * num_dpus + 3.347906e-05 * block_size + -1.431084e-07 * (double)num_dpus * block_size;
            } else {
              return 0.02947621 + 8.230768e-05 * num_dpus + 2.297825e-05 * block_size + 1.088504e-08 * (double)num_dpus * block_size;
            }
          }
        }
      } else {
        if (num_dpus <= 72) {
          if (block_size <= 1600) {
            return 0.05214266 + -0.0001783764 * num_dpus + 2.448835e-05 * block_size + -7.695528e-08 * (double)num_dpus * block_size;
          } else {
            if (block_size <= 28672) {
              return 0.06540088 + -0.0003331572 * num_dpus + 7.916236e-06 * block_size + 1.408902e-08 * (double)num_dpus * block_size;
            } else {
              return -0.7551727 + 0.01167681 * num_dpus + 9.347528e-06 * block_size + -3.002862e-08 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (block_size <= 864) {
            return 0.03024834 + 7.434719e-05 * num_dpus + 2.498724e-05 * block_size + 3.485895e-08 * (double)num_dpus * block_size;
          } else {
            if (block_size <= 1984) {
              return 0.03965439 + 0.0001005015 * num_dpus + 1.135852e-05 * block_size + -1.078697e-08 * (double)num_dpus * block_size;
            } else {
              return 0.02866341 + 0.0001194638 * num_dpus + 8.859677e-06 * block_size + 1.36498e-09 * (double)num_dpus * block_size;
            }
          }
        }
      }
    } else {
      if (num_dpus <= 448) {
        if (block_size <= 26624) {
          if (block_size <= 1024) {
            if (num_dpus <= 400) {
              return 0.1010982 + -6.369633e-05 * num_dpus + 2.85441e-07 * block_size + 1.115004e-07 * (double)num_dpus * block_size;
            } else {
              return 0.04434444 + 6.572967e-05 * num_dpus + -5.93213e-05 * block_size + 2.441526e-07 * (double)num_dpus * block_size;
            }
          } else {
            if ((double)num_dpus * block_size <= 933888) {
              return 0.1878495 + -0.000235281 * num_dpus + -9.301025e-05 * block_size + 2.595579e-07 * (double)num_dpus * block_size;
            } else {
              return 0.08443258 + -2.996661e-05 * num_dpus + 1.843972e-05 * block_size + -2.525943e-09 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (block_size <= 86016) {
            return 1.038792 + -0.002029698 * num_dpus + -2.358982e-05 * block_size + 8.744409e-08 * (double)num_dpus * block_size;
          } else {
            if (num_dpus <= 416) {
              return -6.153751 + 0.01553981 * num_dpus + 2.378436e-05 * block_size + -2.56251e-08 * (double)num_dpus * block_size;
            } else {
              return -0.4673483 + 0.001267824 * num_dpus + 1.517536e-05 * block_size + -3.87274e-09 * (double)num_dpus * block_size;
            }
          }
        }
      } else {
        if (block_size <= 928) {
          if (num_dpus <= 480) {
            return 0.03533763 + 0.0001086489 * num_dpus + 0.0001047804 * block_size + -9.399123e-08 * (double)num_dpus * block_size;
          } else {
            if (num_dpus <= 832) {
              return 0.04182681 + 9.374919e-05 * num_dpus + 6.78545e-05 * block_size + 2.029011e-09 * (double)num_dpus * block_size;
            } else {
              return 0.05511202 + 7.097407e-05 * num_dpus + 7.447095e-05 * block_size + -6.165894e-09 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (block_size <= 1984) {
            if (block_size <= 1024) {
              return 0.5922741 + 5.325641e-05 * num_dpus + -0.0004842016 * block_size + 1.873513e-08 * (double)num_dpus * block_size;
            } else {
              return 0.05942802 + 6.487445e-05 * num_dpus + 3.449821e-05 * block_size + 4.933389e-09 * (double)num_dpus * block_size;
            }
          } else {
            if ((double)num_dpus * block_size <= 7225344) {
              return 0.07067291 + 4.522857e-05 * num_dpus + 1.524752e-05 * block_size + 1.014712e-08 * (double)num_dpus * block_size;
            } else {
              return 0.1387594 + 2.406374e-05 * num_dpus + 1.573487e-05 * block_size + 4.634186e-09 * (double)num_dpus * block_size;
            }
          }
        }
      }
    }
  }
}
} // namespace

double upmem_cm::scatterSharedCostMs(int num_dpus, int block_size) {
  // No transfer beats the fastest rate this sweep measured (8.58259e+07 B/ms);
  // outside its fitted region a leaf's linear model can slope below zero.
  return std::fmax(scatterSharedCostMsModel(num_dpus, block_size), (double)num_dpus * block_size / 8.582594e+07);
}

double upmem_cm::scatterReplicatedCostMs(int num_dpus, int block_size, double unique_bytes) {
  const double written = (double)num_dpus * block_size;
  if (unique_bytes >= written)
    return scatterBlockCostMs(num_dpus, block_size);
  // T_xfer(D, B) + T_read(U, D). The read of U unique bytes on D DPUs is what
  // distinct data costs over a shared source at the block size that has U
  // bytes in all: for distinct data U = D * B, so B_read = U / D.
  const int read_block = (int)std::fmax(1.0, std::round(unique_bytes / num_dpus));
  const double read = scatterBlockCostMs(num_dpus, read_block) - scatterSharedCostMs(num_dpus, read_block);
  // Below the knee the two trees differ by noise of either sign; and no
  // source costs more than distinct data of the same geometry.
  return std::fmin(scatterSharedCostMs(num_dpus, block_size) + std::fmax(0.0, read),
                   scatterBlockCostMs(num_dpus, block_size));
}

// Fitted by model_tree.py on 34212 measured configs (upmemcm/scatter_cost/plots/gather), relRMSE 7.78%.
namespace {
double gatherCostMsModel(int num_dpus, int block_size) {
  if (num_dpus <= 64) {
    if ((double)num_dpus * block_size <= 5586944) {
      if (num_dpus <= 14) {
        if (num_dpus <= 1) {
          if (block_size <= 1024) {
            return 0.02191889 + 0 * num_dpus + 3.641079e-06 * block_size + 8.889351e-10 * (double)num_dpus * block_size;
          } else {
            if (block_size <= 18432) {
              return 0.02008055 + 0 * num_dpus + 4.102755e-06 * block_size + 1.001649e-09 * (double)num_dpus * block_size;
            } else {
              return 0.02066055 + -4.96515e-16 * num_dpus + 4.075728e-06 * block_size + 9.950508e-10 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (num_dpus <= 2) {
            if (block_size <= 1024) {
              return 0.02994615 + -0.004240095 * num_dpus + 4.672222e-06 * block_size + 2.281358e-09 * (double)num_dpus * block_size;
            } else {
              return 0.02039203 + -2.877088e-16 * num_dpus + 4.07329e-06 * block_size + 1.988911e-09 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 3) {
              return 0.01804256 + 0.001152638 * num_dpus + 3.970825e-06 * block_size + 2.906662e-09 * (double)num_dpus * block_size;
            } else {
              return 0.02146288 + 0.0002854237 * num_dpus + 4.223511e-06 * block_size + 1.566628e-08 * (double)num_dpus * block_size;
            }
          }
        }
      } else {
        if (block_size <= 1920) {
          if (num_dpus <= 15) {
            return 0.02349903 + 0 * num_dpus + 8.047635e-06 * block_size + 2.947132e-08 * (double)num_dpus * block_size;
          } else {
            if (num_dpus <= 16) {
              return 0.02452969 + 0 * num_dpus + 8.734861e-06 * block_size + 3.412055e-08 * (double)num_dpus * block_size;
            } else {
              return 0.0171645 + 0.0002357056 * num_dpus + 1.944119e-05 * block_size + 3.349908e-07 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (num_dpus <= 32) {
            if (num_dpus <= 16) {
              return 0.07632419 + -0.003215625 * num_dpus + 4.435021e-06 * block_size + -1.350654e-09 * (double)num_dpus * block_size;
            } else {
              return 0.01933245 + 0.0002071742 * num_dpus + 8.348742e-06 * block_size + 4.116021e-10 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 60) {
              return 0.02847075 + -4.685471e-05 * num_dpus + 1.641309e-05 * block_size + 5.490876e-09 * (double)num_dpus * block_size;
            } else {
              return -1.282761 + 0.0210838 * num_dpus + -1.260307e-05 * block_size + 4.727303e-07 * (double)num_dpus * block_size;
            }
          }
        }
      }
    } else {
      if (num_dpus <= 32) {
        if (num_dpus <= 16) {
          if ((double)num_dpus * block_size <= 24510464) {
            if ((double)num_dpus * block_size <= 13565952) {
              return 0.232738 + -0.03174003 * num_dpus + 3.868935e-06 * block_size + 7.487674e-08 * (double)num_dpus * block_size;
            } else {
              return 0.225471 + -0.1481511 * num_dpus + 3.682988e-06 * block_size + 2.016711e-07 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 7) {
              return -7.505733 + 0.4403123 * num_dpus + 3.552122e-06 * block_size + 4.264799e-07 * (double)num_dpus * block_size;
            } else {
              return -6.236155 + 0.2231363 * num_dpus + 6.631722e-06 * block_size + 2.971615e-08 * (double)num_dpus * block_size;
            }
          }
        } else {
          if ((double)num_dpus * block_size <= 16515072) {
            if (block_size <= 425984) {
              return 0.1242969 + -0.007950161 * num_dpus + 7.716706e-06 * block_size + 4.030752e-08 * (double)num_dpus * block_size;
            } else {
              return 0.4591393 + -0.06128193 * num_dpus + 6.582891e-06 * block_size + 1.76986e-07 * (double)num_dpus * block_size;
            }
          } else {
            if (block_size <= 1114112) {
              return -0.4923229 + -0.0879438 * num_dpus + 7.320605e-06 * block_size + 2.457676e-07 * (double)num_dpus * block_size;
            } else {
              return -7.004457 + 0.1349104 * num_dpus + 1.325378e-05 * block_size + 2.864367e-08 * (double)num_dpus * block_size;
            }
          }
        }
      } else {
        if ((double)num_dpus * block_size <= 14942208) {
          if ((double)num_dpus * block_size <= 11141120) {
            if (block_size <= 139264) {
              return -1.727686 + 0.03224061 * num_dpus + 3.08291e-05 * block_size + -2.614022e-07 * (double)num_dpus * block_size;
            } else {
              return 0.09409118 + -0.002892203 * num_dpus + 1.61152e-05 * block_size + 2.082676e-08 * (double)num_dpus * block_size;
            }
          } else {
            return -0.8403893 + 0.002920683 * num_dpus + 1.819476e-05 * block_size + 3.505336e-08 * (double)num_dpus * block_size;
          }
        } else {
          if (block_size <= 1114112) {
            if ((double)num_dpus * block_size <= 24510464) {
              return -3.549847 + 0.01041811 * num_dpus + 2.078501e-05 * block_size + 1.347533e-07 * (double)num_dpus * block_size;
            } else {
              return -6.874934 + 0.05861217 * num_dpus + 2.770389e-05 * block_size + 4.06132e-08 * (double)num_dpus * block_size;
            }
          } else {
            if ((double)num_dpus * block_size <= 409993216) {
              return -7.838779 + 0.0685446 * num_dpus + 2.663744e-05 * block_size + 4.087748e-08 * (double)num_dpus * block_size;
            } else {
              return 8.364624 + -0.02184495 * num_dpus + 2.582475e-05 * block_size + 3.497096e-08 * (double)num_dpus * block_size;
            }
          }
        }
      }
    }
  } else {
    if ((double)num_dpus * block_size <= 24510464) {
      if (block_size <= 960) {
        if (num_dpus <= 416) {
          if (num_dpus <= 80) {
            if (num_dpus <= 72) {
              return -0.01393102 + 0.0007819741 * num_dpus + 5.944326e-06 * block_size + 9.727544e-07 * (double)num_dpus * block_size;
            } else {
              return 0.1019767 + -0.000810456 * num_dpus + 6.026312e-05 * block_size + 2.636605e-07 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 116) {
              return 0.0244609 + 0.0001561971 * num_dpus + 3.476525e-07 * block_size + 1.044791e-06 * (double)num_dpus * block_size;
            } else {
              return 0.02839603 + 9.51941e-05 * num_dpus + 9.366046e-05 * block_size + 1.306287e-07 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (num_dpus <= 464) {
            if (block_size <= 184) {
              return -0.09704914 + 0.0004008848 * num_dpus + -0.0004595587 * block_size + 1.468153e-06 * (double)num_dpus * block_size;
            } else {
              return -0.110661 + 0.0004290713 * num_dpus + -0.0004671044 * block_size + 1.500613e-06 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 896) {
              return 0.05244535 + 8.711628e-05 * num_dpus + 0.0002206339 * block_size + 5.819635e-08 * (double)num_dpus * block_size;
            } else {
              return 0.0681193 + 6.380887e-05 * num_dpus + 0.0002381461 * block_size + 2.041713e-08 * (double)num_dpus * block_size;
            }
          }
        }
      } else {
        if (num_dpus <= 448) {
          if (block_size <= 1920) {
            if (num_dpus <= 352) {
              return 0.03598177 + 0.0001550412 * num_dpus + 6.178284e-05 * block_size + -2.796526e-08 * (double)num_dpus * block_size;
            } else {
              return -0.135209 + 0.0005624804 * num_dpus + 0.0002354018 * block_size + -3.597646e-07 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 384) {
              return 0.01879232 + 0.0002635586 * num_dpus + 3.016453e-05 * block_size + 2.426433e-08 * (double)num_dpus * block_size;
            } else {
              return -0.4176737 + 0.001287378 * num_dpus + 0.0001950273 * block_size + -3.258881e-07 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (block_size <= 1920) {
            if (block_size <= 1024) {
              return 0.2401747 + 0.00281737 * num_dpus + -9.220355e-06 * block_size + -2.691001e-06 * (double)num_dpus * block_size;
            } else {
              return 0.08250055 + 4.586715e-05 * num_dpus + 0.0001077884 * block_size + 4.760621e-08 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 960) {
              return 0.09682515 + 0.0001024661 * num_dpus + 4.722438e-05 * block_size + 5.108922e-08 * (double)num_dpus * block_size;
            } else {
              return 0.3101715 + -7.337618e-05 * num_dpus + 2.126034e-05 * block_size + 5.281688e-08 * (double)num_dpus * block_size;
            }
          }
        }
      }
    } else {
      if (num_dpus <= 448) {
        if (num_dpus <= 384) {
          if (num_dpus <= 72) {
            if ((double)num_dpus * block_size <= 98041856) {
              return 5.556363 + -0.1300075 * num_dpus + 2.512719e-05 * block_size + 2.590751e-07 * (double)num_dpus * block_size;
            } else {
              return -11.28636 + 0.0930933 * num_dpus + 4.708983e-05 * block_size + -6.890673e-08 * (double)num_dpus * block_size;
            }
          } else {
            if ((double)num_dpus * block_size <= 39911424) {
              return -3.059682 + 0.001458298 * num_dpus + 4.796057e-05 * block_size + 8.298355e-08 * (double)num_dpus * block_size;
            } else {
              return -2.207048 + 0.003350901 * num_dpus + 5.280952e-05 * block_size + 3.199724e-08 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (block_size <= 139264) {
            return -5.836775 + 0.007168429 * num_dpus + 0.0001194531 * block_size + -2.726254e-08 * (double)num_dpus * block_size;
          } else {
            if ((double)num_dpus * block_size <= 409993216) {
              return 1.979561 + -0.007289729 * num_dpus + 6.158121e-05 * block_size + 7.796775e-08 * (double)num_dpus * block_size;
            } else {
              return 16.52689 + -0.02878572 * num_dpus + 4.661622e-05 * block_size + 9.874663e-08 * (double)num_dpus * block_size;
            }
          }
        }
      } else {
        if ((double)num_dpus * block_size <= 144703488) {
          if (block_size <= 45056) {
            if (num_dpus <= 1024) {
              return 0.9708035 + -0.002426013 * num_dpus + 2.681059e-05 * block_size + 1.250414e-07 * (double)num_dpus * block_size;
            } else {
              return 0.01098418 + -0.0002163832 * num_dpus + 2.792781e-05 * block_size + 6.719725e-08 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 928) {
              return -4.29597 + 0.003384822 * num_dpus + 0.0001134858 * block_size + 4.203855e-08 * (double)num_dpus * block_size;
            } else {
              return 0.5119057 + -0.003202554 * num_dpus + 2.823688e-05 * block_size + 1.314367e-07 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (block_size <= 278528) {
            if (num_dpus <= 1920) {
              return -6.489185 + 0.00185412 * num_dpus + 8.574424e-05 * block_size + 9.282208e-08 * (double)num_dpus * block_size;
            } else {
              return 149.5656 + -0.07648676 * num_dpus + 0.001030108 * block_size + -3.834115e-07 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 1024) {
              return -1.913445 + 0.004528961 * num_dpus + 9.6107e-05 * block_size + 5.311315e-08 * (double)num_dpus * block_size;
            } else {
              return -16.40117 + 0.02365748 * num_dpus + 8.992732e-05 * block_size + 3.366586e-08 * (double)num_dpus * block_size;
            }
          }
        }
      }
    }
  }
}
} // namespace

double upmem_cm::gatherCostMs(int num_dpus, int block_size) {
  // No transfer beats the fastest rate this sweep measured (1.66422e+07 B/ms);
  // outside its fitted region a leaf's linear model can slope below zero.
  return std::fmax(gatherCostMsModel(num_dpus, block_size), (double)num_dpus * block_size / 1.664216e+07);
}

// Fitted by model_tree.py on 34212 measured configs (upmemcm/scatter_cost/plots/broadcast), relRMSE 8.51%.
namespace {
double broadcastCostMsModel(int num_dpus, int block_size) {
  if (num_dpus <= 62) {
    if (num_dpus <= 1) {
      if (block_size <= 1024) {
        return 0.01780013 + 0 * num_dpus + 3.628914e-07 * block_size + 8.859652e-11 * (double)num_dpus * block_size;
      } else {
        if (block_size <= 59392) {
          return 0.01786201 + -1.346887e-16 * num_dpus + 8.413098e-07 * block_size + 2.053979e-10 * (double)num_dpus * block_size;
        } else {
          if (block_size <= 983040) {
            return 0.01118395 + -2.686983e-17 * num_dpus + 1.000936e-06 * block_size + 2.443691e-10 * (double)num_dpus * block_size;
          } else {
            return -0.07365775 + -7.67925e-18 * num_dpus + 1.140031e-06 * block_size + 2.78328e-10 * (double)num_dpus * block_size;
          }
        }
      }
    } else {
      if (num_dpus <= 2) {
        if (block_size <= 1024) {
          return 0.01847631 + -0.0007467947 * num_dpus + 1.843712e-06 * block_size + 9.002501e-10 * (double)num_dpus * block_size;
        } else {
          if (block_size <= 55296) {
            return 0.01806694 + 7.259424e-16 * num_dpus + 8.506462e-07 * block_size + 4.153546e-10 * (double)num_dpus * block_size;
          } else {
            if (block_size <= 917504) {
              return 0.009228773 + 0 * num_dpus + 1.031195e-06 * block_size + 5.03513e-10 * (double)num_dpus * block_size;
            } else {
              return -0.05167651 + 0 * num_dpus + 1.140372e-06 * block_size + 5.568222e-10 * (double)num_dpus * block_size;
            }
          }
        }
      } else {
        if (num_dpus <= 3) {
          if (block_size <= 3456) {
            return 0.01756121 + 0 * num_dpus + 1.118218e-06 * block_size + 8.190071e-10 * (double)num_dpus * block_size;
          } else {
            if (block_size <= 55296) {
              return 0.01783924 + -1.138197e-10 * num_dpus + 8.602433e-07 * block_size + 6.300608e-10 * (double)num_dpus * block_size;
            } else {
              return 0.002414629 + 0 * num_dpus + 1.079803e-06 * block_size + 7.908703e-10 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (num_dpus <= 4) {
            if (block_size <= 1024) {
              return 0.02881024 + -0.002744338 * num_dpus + 6.227067e-07 * block_size + 6.081119e-10 * (double)num_dpus * block_size;
            } else {
              return 0.0108129 + 0.001561918 * num_dpus + 1.025948e-06 * block_size + 1.001902e-09 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 5) {
              return 0.01769329 + -3.194973e-14 * num_dpus + 1.025192e-06 * block_size + 1.251454e-09 * (double)num_dpus * block_size;
            } else {
              return 0.01715156 + 5.734094e-05 * num_dpus + 6.876352e-07 * block_size + 7.488391e-08 * (double)num_dpus * block_size;
            }
          }
        }
      }
    }
  } else {
    if (num_dpus <= 416) {
      if (num_dpus <= 64) {
        if (block_size <= 1024) {
          return 0.01521409 + 6.370091e-05 * num_dpus + 7.584957e-06 * block_size + 1.18515e-07 * (double)num_dpus * block_size;
        } else {
          return 0.00537172 + 0.0002518778 * num_dpus + 2.22631e-06 * block_size + 3.478608e-08 * (double)num_dpus * block_size;
        }
      } else {
        if (num_dpus <= 384) {
          if (block_size <= 200) {
            if (num_dpus <= 76) {
              return 0.04624317 + -0.0002294026 * num_dpus + -2.812516e-05 * block_size + 5.422748e-07 * (double)num_dpus * block_size;
            } else {
              return 0.02576444 + 3.364138e-05 * num_dpus + 2.045434e-05 * block_size + 1.713649e-08 * (double)num_dpus * block_size;
            }
          } else {
            if (block_size <= 960) {
              return 0.02535217 + 3.007508e-05 * num_dpus + 2.276512e-05 * block_size + 4.571158e-08 * (double)num_dpus * block_size;
            } else {
              return 0.02875496 + 5.328071e-05 * num_dpus + 8.167961e-06 * block_size + 3.875303e-09 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (block_size <= 1920) {
            if (block_size <= 256) {
              return 0.04748377 + 7.148324e-06 * num_dpus + 0.000170978 * block_size + -3.142785e-07 * (double)num_dpus * block_size;
            } else {
              return 0.0724927 + -2.548268e-05 * num_dpus + 2.189041e-05 * block_size + -4.047818e-09 * (double)num_dpus * block_size;
            }
          } else {
            if (block_size <= 9216) {
              return 0.1063095 + -0.0001344529 * num_dpus + -4.869006e-06 * block_size + 4.43856e-08 * (double)num_dpus * block_size;
            } else {
              return 0.1553033 + -0.0002648572 * num_dpus + 8.008523e-06 * block_size + 1.385415e-08 * (double)num_dpus * block_size;
            }
          }
        }
      }
    } else {
      if (num_dpus <= 448) {
        if (block_size <= 960) {
          return -0.03129656 + 0.0001849194 * num_dpus + -3.47711e-05 * block_size + 1.997605e-07 * (double)num_dpus * block_size;
        } else {
          if (block_size <= 90112) {
            if (block_size <= 4096) {
              return -0.007423468 + 0.0001850367 * num_dpus + 0.0001017837 * block_size + -2.076565e-07 * (double)num_dpus * block_size;
            } else {
              return 0.2581581 + -0.0004680944 * num_dpus + 9.493931e-06 * block_size + 1.723949e-08 * (double)num_dpus * block_size;
            }
          } else {
            if (block_size <= 360448) {
              return -5.619917 + 0.01314745 * num_dpus + 4.241446e-05 * block_size + -6.684184e-08 * (double)num_dpus * block_size;
            } else {
              return 1.985483 + -0.004558749 * num_dpus + 1.407915e-05 * block_size + -8.823235e-10 * (double)num_dpus * block_size;
            }
          }
        }
      } else {
        if (block_size <= 960) {
          if (num_dpus <= 800) {
            return 0.03986232 + 5.180502e-05 * num_dpus + 5.04218e-05 * block_size + 2.864704e-08 * (double)num_dpus * block_size;
          } else {
            if (num_dpus <= 1408) {
              return 0.04156218 + 4.455405e-05 * num_dpus + 8.637348e-05 * block_size + -1.814406e-08 * (double)num_dpus * block_size;
            } else {
              return -0.02216266 + 8.502255e-05 * num_dpus + 7.121874e-05 * block_size + -5.039265e-09 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (block_size <= 1920) {
            if (block_size <= 1024) {
              return 0.09589522 + 0.0008511618 * num_dpus + -2.796452e-06 * block_size + -7.973438e-07 * (double)num_dpus * block_size;
            } else {
              return 0.0411297 + 4.448673e-05 * num_dpus + 3.918157e-05 * block_size + 1.389513e-10 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 1024) {
              return 0.06429794 + 2.150133e-05 * num_dpus + 1.477999e-05 * block_size + 8.668307e-09 * (double)num_dpus * block_size;
            } else {
              return 0.0255709 + 5.528338e-05 * num_dpus + 1.810985e-05 * block_size + 3.580018e-09 * (double)num_dpus * block_size;
            }
          }
        }
      }
    }
  }
}
} // namespace

double upmem_cm::broadcastCostMs(int num_dpus, int block_size) {
  // No transfer beats the fastest rate this sweep measured (8.67706e+07 B/ms);
  // outside its fitted region a leaf's linear model can slope below zero.
  return std::fmax(broadcastCostMsModel(num_dpus, block_size), (double)num_dpus * block_size / 8.677058e+07);
}

namespace {
double scatterSgTreeCostMsModel(int num_dpus, int blocks_per_dpu, int block_size) {
  if ((double)num_dpus * blocks_per_dpu * block_size <= 30670848) {
    if (num_dpus <= 448) {
      if ((double)blocks_per_dpu * block_size <= 1920) {
        if ((double)blocks_per_dpu * block_size <= 960) {
          if (num_dpus <= 352) {
            if (num_dpus <= 320) {
              return 0.05349763 + -2.496183e-11 * num_dpus + 0.0002770205 * blocks_per_dpu + -5.452396e-06 * block_size + 2.4834e-08 * (double)num_dpus * blocks_per_dpu + -1.497546e-08 * (double)num_dpus * block_size + 3.48963e-05 * (double)blocks_per_dpu * block_size + 9.584557e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.05412584 + -8.813484e-11 * num_dpus + 0.001005796 * blocks_per_dpu + 3.189583e-06 * block_size + 8.735513e-08 * (double)num_dpus * blocks_per_dpu + 9.6365e-09 * (double)num_dpus * block_size + 2.661696e-05 * (double)blocks_per_dpu * block_size + 8.041624e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (num_dpus <= 384) {
              return 0.05393074 + -9.399178e-12 * num_dpus + 0.001430276 * blocks_per_dpu + 1.560031e-06 * block_size + 1.261455e-07 * (double)num_dpus * blocks_per_dpu + 5.141705e-09 * (double)num_dpus * block_size + 2.423316e-05 * (double)blocks_per_dpu * block_size + 7.987002e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.044576 + 6.411299e-05 * num_dpus + -0.001561593 * blocks_per_dpu + 1.753845e-07 * block_size + 6.648011e-06 * (double)num_dpus * blocks_per_dpu + 1.559689e-08 * (double)num_dpus * block_size + -9.063309e-05 * (double)blocks_per_dpu * block_size + 3.894116e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        } else {
          if (num_dpus <= 384) {
            if ((double)num_dpus * blocks_per_dpu <= 1408) {
              return 0.5061491 + -0.001299468 * num_dpus + -0.2275758 * blocks_per_dpu + -0.0005399781 * block_size + 0.0006467307 * (double)num_dpus * blocks_per_dpu + 1.589985e-06 * (double)num_dpus * block_size + 0.000374281 * (double)blocks_per_dpu * block_size + -9.908481e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -0.03466409 + 0.000225883 * num_dpus + 0.004166749 * blocks_per_dpu + 0.001227682 * block_size + -6.993355e-06 * (double)num_dpus * blocks_per_dpu + -3.198949e-06 * (double)num_dpus * block_size + -9.736332e-05 * (double)blocks_per_dpu * block_size + 3.230937e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (blocks_per_dpu <= 4) {
              return -0.2946679 + 0.0008787647 * num_dpus + 0.1487979 * blocks_per_dpu + 0.0004500744 * block_size + -0.0003608126 * (double)num_dpus * blocks_per_dpu + -1.093295e-06 * (double)num_dpus * block_size + -0.0002941118 * (double)blocks_per_dpu * block_size + 8.144146e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -0.01381201 + 0.0001966249 * num_dpus + 0.001506775 * blocks_per_dpu + 0.0004572623 * block_size + -6.727471e-07 * (double)num_dpus * blocks_per_dpu + -1.103845e-06 * (double)num_dpus * block_size + -5.206257e-06 * (double)blocks_per_dpu * block_size + 1.060607e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        }
      } else {
        if (num_dpus <= 384) {
          if ((double)blocks_per_dpu * block_size <= 45056) {
            if ((double)num_dpus * blocks_per_dpu <= 640) {
              return -0.1274951 + 0.0004376705 * num_dpus + 0.0252836 * blocks_per_dpu + 5.262223e-06 * block_size + 2.35632e-06 * (double)num_dpus * blocks_per_dpu + -1.620645e-08 * (double)num_dpus * block_size + 1.691701e-05 * (double)blocks_per_dpu * block_size + 1.580437e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.06058633 + -3.694829e-05 * num_dpus + -0.002967695 * blocks_per_dpu + -4.030457e-06 * block_size + 1.119006e-05 * (double)num_dpus * blocks_per_dpu + 4.915718e-09 * (double)num_dpus * block_size + 1.747947e-05 * (double)blocks_per_dpu * block_size + -3.052686e-09 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (blocks_per_dpu <= 4) {
              return 7.551542 + -0.0236067 * num_dpus + -2.405952 * blocks_per_dpu + -7.444147e-05 * block_size + 0.007028665 * (double)num_dpus * blocks_per_dpu + 2.196388e-07 * (double)num_dpus * block_size + 1.026782e-05 * (double)blocks_per_dpu * block_size + 4.922671e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -4.376458 + 0.009455275 * num_dpus + 0.2655173 * blocks_per_dpu + 0.001405597 * block_size + -0.0006686782 * (double)num_dpus * blocks_per_dpu + -3.630822e-06 * (double)num_dpus * block_size + -8.460888e-05 * (double)blocks_per_dpu * block_size + 2.961666e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        } else {
          if ((double)blocks_per_dpu * block_size <= 45056) {
            if (blocks_per_dpu <= 4) {
              return -0.04850616 + 0.0001250451 * num_dpus + -0.01652046 * blocks_per_dpu + 9.512694e-06 * block_size + 9.404796e-05 * (double)num_dpus * blocks_per_dpu + -1.432609e-09 * (double)num_dpus * block_size + 1.283505e-05 * (double)blocks_per_dpu * block_size + 1.837111e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.01449883 + 0.0001061991 * num_dpus + 0.0005225735 * blocks_per_dpu + -3.68085e-05 * block_size + 6.603971e-07 * (double)num_dpus * blocks_per_dpu + 5.974347e-08 * (double)num_dpus * block_size + 3.937181e-05 * (double)blocks_per_dpu * block_size + -3.411873e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if ((double)num_dpus * blocks_per_dpu * block_size <= 24772608) {
              return -1.089658 + 0.0009978086 * num_dpus + 0.007482328 * blocks_per_dpu + -7.435603e-06 * block_size + -1.362915e-05 * (double)num_dpus * blocks_per_dpu + 1.762336e-08 * (double)num_dpus * block_size + 3.220655e-05 * (double)blocks_per_dpu * block_size + 1.742329e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -3.153352 + 0.002114492 * num_dpus + 0.045578 * blocks_per_dpu + 1.849061e-05 * block_size + -0.0001017343 * (double)num_dpus * blocks_per_dpu + -4.282382e-08 * (double)num_dpus * block_size + 3.154167e-05 * (double)blocks_per_dpu * block_size + 8.319991e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        }
      }
    } else {
      if ((double)blocks_per_dpu * block_size <= 1920) {
        if ((double)blocks_per_dpu * block_size <= 960) {
          if (num_dpus <= 480) {
            if (blocks_per_dpu <= 2) {
              return 0.09317479 + 0 * num_dpus + -0.0004375242 * blocks_per_dpu + -1.368733e-06 * block_size + -4.742963e-08 * (double)num_dpus * blocks_per_dpu + -5.638995e-09 * (double)num_dpus * block_size + 3.333289e-05 * (double)blocks_per_dpu * block_size + 1.373273e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.08157744 + -3.47592e-09 * num_dpus + 0.001767066 * blocks_per_dpu + 1.885e-05 * block_size + 1.916072e-07 * (double)num_dpus * blocks_per_dpu + 7.765956e-08 * (double)num_dpus * block_size + 2.950377e-05 * (double)blocks_per_dpu * block_size + 1.215517e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (blocks_per_dpu <= 7) {
              return 0.07325158 + 4.503111e-05 * num_dpus + -0.0001264026 * blocks_per_dpu + 2.268959e-05 * block_size + 6.235012e-07 * (double)num_dpus * blocks_per_dpu + -1.316849e-08 * (double)num_dpus * block_size + 0.0001026749 * (double)blocks_per_dpu * block_size + 1.128663e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.04969169 + 5.028692e-05 * num_dpus + 0.002475048 * blocks_per_dpu + 0.0001112686 * block_size + 7.52459e-08 * (double)num_dpus * blocks_per_dpu + -6.215732e-08 * (double)num_dpus * block_size + 0.0001089717 * (double)blocks_per_dpu * block_size + 3.437707e-09 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        } else {
          if ((double)blocks_per_dpu * block_size <= 1024) {
            if (num_dpus <= 896) {
              return 4.019601 + 0.0001378134 * num_dpus + 0.001164092 * blocks_per_dpu + -2.943102e-05 * block_size + 6.210653e-07 * (double)num_dpus * blocks_per_dpu + 7.396077e-08 * (double)num_dpus * block_size + -0.003825823 * (double)blocks_per_dpu * block_size + -7.249299e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 3.821459 + 0.0003803202 * num_dpus + 0.003100701 * blocks_per_dpu + 8.593953e-05 * block_size + -1.032621e-06 * (double)num_dpus * blocks_per_dpu + -5.317673e-08 * (double)num_dpus * block_size + -0.003670541 * (double)blocks_per_dpu * block_size + -2.844042e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (num_dpus <= 1024) {
              return 0.04702406 + 5.706749e-05 * num_dpus + 0.001130894 * blocks_per_dpu + -2.323422e-05 * block_size + 9.151046e-07 * (double)num_dpus * blocks_per_dpu + 5.500427e-08 * (double)num_dpus * block_size + 5.831689e-05 * (double)blocks_per_dpu * block_size + -1.09063e-09 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.09691191 + 1.641948e-05 * num_dpus + 0.0005476503 * blocks_per_dpu + 1.906337e-05 * block_size + 5.629583e-07 * (double)num_dpus * blocks_per_dpu + -1.138791e-08 * (double)num_dpus * block_size + 9.279831e-07 * (double)blocks_per_dpu * block_size + 4.759397e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        }
      } else {
        if (num_dpus <= 896) {
          if ((double)num_dpus * blocks_per_dpu * block_size <= 21299200) {
            if (num_dpus <= 704) {
              return 0.04606816 + 4.569275e-05 * num_dpus + 0.0009437613 * blocks_per_dpu + -9.700228e-06 * block_size + 1.332805e-06 * (double)num_dpus * blocks_per_dpu + 2.149163e-08 * (double)num_dpus * block_size + 1.511792e-05 * (double)blocks_per_dpu * block_size + 3.266863e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.1995186 + -0.0001273012 * num_dpus + -0.01886466 * blocks_per_dpu + 4.962814e-05 * block_size + 2.351909e-05 * (double)num_dpus * blocks_per_dpu + -5.748442e-08 * (double)num_dpus * block_size + 1.738357e-05 * (double)blocks_per_dpu * block_size + 3.323646e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (num_dpus <= 768) {
              return -4.4805 + 0.004971126 * num_dpus + 0.04062623 * blocks_per_dpu + 5.021451e-06 * block_size + -5.587422e-05 * (double)num_dpus * blocks_per_dpu + -4.186315e-09 * (double)num_dpus * block_size + 8.835325e-05 * (double)blocks_per_dpu * block_size + -2.108242e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -7.865481 + 0.008064043 * num_dpus + -0.02301359 * blocks_per_dpu + 0.0001998095 * block_size + 4.204778e-05 * (double)num_dpus * blocks_per_dpu + -2.223404e-07 * (double)num_dpus * block_size + 0.0003535765 * (double)blocks_per_dpu * block_size + -3.210268e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        } else {
          if (blocks_per_dpu <= 8) {
            if ((double)num_dpus * blocks_per_dpu <= 2048) {
              return -1.345659 + 0.001451832 * num_dpus + 1.424407 * blocks_per_dpu + 0.0006618224 * block_size + -0.001427742 * (double)num_dpus * blocks_per_dpu + -6.592658e-07 * (double)num_dpus * block_size + -0.0006196111 * (double)blocks_per_dpu * block_size + 6.663042e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.03312787 + 5.092689e-05 * num_dpus + 0.00177619 * blocks_per_dpu + -3.209713e-05 * block_size + -4.293836e-10 * (double)num_dpus * blocks_per_dpu + 1.865044e-08 * (double)num_dpus * block_size + 4.840163e-05 * (double)blocks_per_dpu * block_size + 2.210387e-09 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (num_dpus <= 1024) {
              return 0.9841745 + -0.0009191748 * num_dpus + -0.05081012 * blocks_per_dpu + -0.007243996 * block_size + 5.377182e-05 * (double)num_dpus * blocks_per_dpu + 7.238222e-06 * (double)num_dpus * block_size + 0.0004939972 * (double)blocks_per_dpu * block_size + -4.48857e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.03087348 + 4.377297e-05 * num_dpus + 0.001797649 * blocks_per_dpu + -0.0005173304 * block_size + 8.114207e-08 * (double)num_dpus * blocks_per_dpu + 2.862643e-07 * (double)num_dpus * block_size + 4.625952e-05 * (double)blocks_per_dpu * block_size + 4.888836e-09 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        }
      }
    }
  } else {
    if (num_dpus <= 512) {
      if (num_dpus <= 384) {
        if ((double)blocks_per_dpu * block_size <= 159744) {
          if (blocks_per_dpu <= 4) {
            return -2.871796 + -0.0004611337 * num_dpus + -2.702071 * blocks_per_dpu + -9.317297e-05 * block_size + 0.007959759 * (double)num_dpus * blocks_per_dpu + 2.71213e-07 * (double)num_dpus * block_size + 0.0001312456 * (double)blocks_per_dpu * block_size + -2.162923e-07 * (double)num_dpus * blocks_per_dpu * block_size;
          } else {
            if (blocks_per_dpu <= 8) {
              return 57.90868 + -0.1678077 * num_dpus + -9.622883 * blocks_per_dpu + -0.0003831007 * block_size + 0.02687392 * (double)num_dpus * blocks_per_dpu + 1.54606e-06 * (double)num_dpus * block_size + 0.000199751 * (double)blocks_per_dpu * block_size + -4.71553e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -12.57223 + 0.02522747 * num_dpus + 0.4848802 * blocks_per_dpu + 0.0009354857 * block_size + -0.001253279 * (double)num_dpus * blocks_per_dpu + -2.45252e-06 * (double)num_dpus * block_size + -1.920468e-05 * (double)blocks_per_dpu * block_size + 2.062849e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        } else {
          if (blocks_per_dpu <= 4) {
            if ((double)num_dpus * blocks_per_dpu * block_size <= 566231040) {
              return -2.366389 + 0.003434286 * num_dpus + -0.3935845 * blocks_per_dpu + -2.399202e-06 * block_size + 0.001242917 * (double)num_dpus * blocks_per_dpu + 7.633441e-09 * (double)num_dpus * block_size + 3.767011e-05 * (double)blocks_per_dpu * block_size + 2.181636e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -188.4207 + 0.4806058 * num_dpus + 133.2722 * blocks_per_dpu + 0.0001126996 * block_size + -0.3430804 * (double)num_dpus * blocks_per_dpu + -2.847602e-07 * (double)num_dpus * block_size + -5.616537e-05 * (double)blocks_per_dpu * block_size + 2.601997e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (blocks_per_dpu <= 6) {
              return 83.65502 + -0.219604 * num_dpus + -12.25508 * blocks_per_dpu + 0.001178932 * block_size + 0.03183618 * (double)num_dpus * blocks_per_dpu + -3.071647e-06 * (double)num_dpus * block_size + -0.0001701682 * (double)blocks_per_dpu * block_size + 5.628249e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -2.174457 + 0.003413941 * num_dpus + 0.08695564 * blocks_per_dpu + 6.093502e-05 * block_size + -0.0002003831 * (double)num_dpus * blocks_per_dpu + -1.613139e-07 * (double)num_dpus * block_size + 3.052493e-05 * (double)blocks_per_dpu * block_size + 4.038111e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        }
      } else {
        if (num_dpus <= 448) {
          if ((double)num_dpus * blocks_per_dpu * block_size <= 50462720) {
            if ((double)num_dpus * block_size <= 3440640) {
              return 10.18859 + -0.03239523 * num_dpus + -0.6397151 * blocks_per_dpu + -0.003364579 * block_size + 0.00156197 * (double)num_dpus * blocks_per_dpu + 8.147155e-06 * (double)num_dpus * block_size + 0.0002185267 * (double)blocks_per_dpu * block_size + -3.39919e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -9.376699 + 0.01351721 * num_dpus + 0.2987947 * blocks_per_dpu + 3.651466e-06 * block_size + -0.0006705844 * (double)num_dpus * blocks_per_dpu + -8.25074e-09 * (double)num_dpus * block_size + 8.869739e-05 * (double)blocks_per_dpu * block_size + -1.057047e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (blocks_per_dpu <= 7) {
              return -3.31769 + 0.005006792 * num_dpus + -0.0557313 * blocks_per_dpu + -7.960871e-07 * block_size + 0.0001652426 * (double)num_dpus * blocks_per_dpu + 2.338293e-09 * (double)num_dpus * block_size + 4.410333e-05 * (double)blocks_per_dpu * block_size + 4.529346e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -6.781649 + 0.01175931 * num_dpus + 0.06968355 * blocks_per_dpu + 0.0007155984 * block_size + -7.186184e-05 * (double)num_dpus * blocks_per_dpu + -1.555244e-06 * (double)num_dpus * block_size + 1.318142e-05 * (double)blocks_per_dpu * block_size + 1.113754e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        } else {
          if ((double)blocks_per_dpu * block_size <= 98304) {
            if (blocks_per_dpu <= 8) {
              return -4.958669 + 0.002883378 * num_dpus + -0.3154184 * blocks_per_dpu + -1.674709e-05 * block_size + 0.0006245277 * (double)num_dpus * blocks_per_dpu + 3.412136e-08 * (double)num_dpus * block_size + 8.527427e-05 * (double)blocks_per_dpu * block_size + 3.846877e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -20.88573 + 0.03370891 * num_dpus + 0.5882033 * blocks_per_dpu + 0.00422411 * block_size + -0.001065739 * (double)num_dpus * blocks_per_dpu + -8.085215e-06 * (double)num_dpus * block_size + -0.0001343097 * (double)blocks_per_dpu * block_size + 4.46078e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (blocks_per_dpu <= 5) {
              return 11.37737 + -0.02570647 * num_dpus + -5.394419 * blocks_per_dpu + -7.823923e-05 * block_size + 0.01112628 * (double)num_dpus * blocks_per_dpu + 1.617664e-07 * (double)num_dpus * block_size + 7.765388e-05 * (double)blocks_per_dpu * block_size + 5.975483e-09 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.8612528 + -0.004259905 * num_dpus + 0.01625133 * blocks_per_dpu + -6.938406e-05 * block_size + -1.291285e-05 * (double)num_dpus * blocks_per_dpu + 1.449721e-07 * (double)num_dpus * block_size + 4.257824e-05 * (double)blocks_per_dpu * block_size + 7.92648e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        }
      }
    } else {
      if ((double)num_dpus * blocks_per_dpu * block_size <= 82575360) {
        if (num_dpus <= 832) {
          if (num_dpus <= 640) {
            if (blocks_per_dpu <= 14) {
              return -7.065544 + 0.006582435 * num_dpus + 0.8378992 * blocks_per_dpu + 0.0001757813 * block_size + -0.001327074 * (double)num_dpus * blocks_per_dpu + -2.881531e-07 * (double)num_dpus * block_size + -8.72451e-06 * (double)blocks_per_dpu * block_size + 1.907418e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -45.5672 + 0.07158993 * num_dpus + 1.480264 * blocks_per_dpu + 0.003998053 * block_size + -0.002451466 * (double)num_dpus * blocks_per_dpu + -6.736407e-06 * (double)num_dpus * block_size + -1.340269e-05 * (double)blocks_per_dpu * block_size + 1.930927e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (block_size <= 6144) {
              return -21.61262 + 0.02497907 * num_dpus + 0.7777689 * blocks_per_dpu + 0.004235692 * block_size + -0.001003444 * (double)num_dpus * blocks_per_dpu + -5.543271e-06 * (double)num_dpus * block_size + -0.0001889394 * (double)blocks_per_dpu * block_size + 3.986376e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -6.617857 + 0.005952133 * num_dpus + -0.4639483 * blocks_per_dpu + -2.21055e-05 * block_size + 0.000579921 * (double)num_dpus * blocks_per_dpu + 3.587517e-08 * (double)num_dpus * block_size + 0.0001207055 * (double)blocks_per_dpu * block_size + -1.011119e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        } else {
          if ((double)num_dpus * blocks_per_dpu * block_size <= 50462720) {
            if (num_dpus <= 1152) {
              return -7.953866 + 0.005790196 * num_dpus + 0.08961308 * blocks_per_dpu + -5.328442e-05 * block_size + -8.153452e-05 * (double)num_dpus * blocks_per_dpu + 5.778972e-08 * (double)num_dpus * block_size + 0.0002415948 * (double)blocks_per_dpu * block_size + -1.258685e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -1.907512 + 0.0008473778 * num_dpus + -0.006884798 * blocks_per_dpu + 1.728583e-05 * block_size + 5.880187e-06 * (double)num_dpus * blocks_per_dpu + -1.004163e-08 * (double)num_dpus * block_size + 7.811456e-05 * (double)blocks_per_dpu * block_size + 1.989454e-09 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (num_dpus <= 1280) {
              return -0.6055884 + -0.001466503 * num_dpus + -0.08760041 * blocks_per_dpu + -0.0001027955 * block_size + 9.238366e-05 * (double)num_dpus * blocks_per_dpu + 1.039578e-07 * (double)num_dpus * block_size + 0.0001283509 * (double)blocks_per_dpu * block_size + -1.09463e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -4.835558 + 0.001458395 * num_dpus + -0.03007221 * blocks_per_dpu + 3.380437e-06 * block_size + 2.092775e-05 * (double)num_dpus * blocks_per_dpu + -1.372468e-09 * (double)num_dpus * block_size + 9.380719e-05 * (double)blocks_per_dpu * block_size + 2.942725e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        }
      } else {
        if (num_dpus <= 832) {
          if ((double)num_dpus * blocks_per_dpu <= 8960) {
            if ((double)num_dpus * blocks_per_dpu <= 1920) {
              return 0.5487804 + -0.001791406 * num_dpus + -2.325876 * blocks_per_dpu + -1.1492e-05 * block_size + 0.004256551 * (double)num_dpus * blocks_per_dpu + 2.045649e-08 * (double)num_dpus * block_size + 6.289481e-05 * (double)blocks_per_dpu * block_size + 3.404281e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -5.347601 + 0.006495251 * num_dpus + 0.2835219 * blocks_per_dpu + 1.204291e-05 * block_size + -0.0003110192 * (double)num_dpus * blocks_per_dpu + -2.964063e-09 * (double)num_dpus * block_size + 5.657021e-05 * (double)blocks_per_dpu * block_size + 4.6691e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (num_dpus <= 640) {
              return 3.662504 + -0.008517624 * num_dpus + 0.2296196 * blocks_per_dpu + -0.001407678 * block_size + -0.0003511203 * (double)num_dpus * blocks_per_dpu + 2.33287e-06 * (double)num_dpus * block_size + 8.275618e-05 * (double)blocks_per_dpu * block_size + 8.068289e-09 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -1.320658 + -0.0008027699 * num_dpus + -0.2070659 * blocks_per_dpu + -0.0004723935 * block_size + 0.000353434 * (double)num_dpus * blocks_per_dpu + 7.623493e-07 * (double)num_dpus * block_size + 7.560024e-05 * (double)blocks_per_dpu * block_size + 1.858771e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        } else {
          if ((double)blocks_per_dpu * block_size <= 98304) {
            if (num_dpus <= 1792) {
              return -9.144217 + 0.004476063 * num_dpus + 0.04100543 * blocks_per_dpu + 1.76644e-05 * block_size + -2.534274e-05 * (double)num_dpus * blocks_per_dpu + -1.285024e-08 * (double)num_dpus * block_size + 0.0001486049 * (double)blocks_per_dpu * block_size + -8.965354e-09 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -17.99802 + 0.008365181 * num_dpus + -0.05913854 * blocks_per_dpu + -5.720537e-05 * block_size + 3.52903e-05 * (double)num_dpus * blocks_per_dpu + 2.512114e-08 * (double)num_dpus * block_size + 0.0004635074 * (double)blocks_per_dpu * block_size + -1.605256e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (num_dpus <= 1792) {
              return -3.086653 + 0.002376528 * num_dpus + 0.03368056 * blocks_per_dpu + -4.98459e-06 * block_size + -1.958277e-05 * (double)num_dpus * blocks_per_dpu + 2.659644e-09 * (double)num_dpus * block_size + 8.815582e-05 * (double)blocks_per_dpu * block_size + 1.261866e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 44.0817 + -0.02161056 * num_dpus + -0.03716564 * blocks_per_dpu + -8.332692e-05 * block_size + 1.900845e-05 * (double)num_dpus * blocks_per_dpu + 4.057281e-08 * (double)num_dpus * block_size + -5.61853e-05 * (double)blocks_per_dpu * block_size + 8.738268e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        }
      }
    }
  }
}
} // namespace

double upmem_cm::scatterSgCostMs(int num_dpus, int blocks_per_dpu, int block_size) {
  // No transfer beats the fastest rate this sweep measured (3.77832e+07 B/ms);
  // outside its fitted region a leaf's linear model can slope below zero.
  return std::fmax(scatterSgTreeCostMsModel(num_dpus, blocks_per_dpu, block_size), (double)num_dpus * blocks_per_dpu * block_size / 3.778321e+07);
}

namespace {
double scatterSgGatherTreeCostMsModel(int num_dpus, int blocks_per_dpu, int block_size) {
  if (num_dpus <= 64) {
    if ((double)blocks_per_dpu * block_size <= 1920) {
      if (num_dpus <= 16) {
        if ((double)blocks_per_dpu * block_size <= 320) {
          if (num_dpus <= 1) {
            return 0.02107279 + -1.068804e-09 * num_dpus + 3.604287e-05 * blocks_per_dpu + -5.113134e-07 * block_size + 2.472343e-09 * (double)num_dpus * blocks_per_dpu + -3.120799e-11 * (double)num_dpus * block_size + 7.691244e-06 * (double)blocks_per_dpu * block_size + 7.510981e-09 * (double)num_dpus * blocks_per_dpu * block_size;
          } else {
            if (num_dpus <= 2) {
              return 0.02174645 + -4.418518e-11 * num_dpus + 4.60232e-05 * blocks_per_dpu + 2.716164e-06 * block_size + 2.924209e-09 * (double)num_dpus * blocks_per_dpu + 3.315626e-10 * (double)num_dpus * block_size + 6.663904e-06 * (double)blocks_per_dpu * block_size + 1.301544e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.0215099 + 0.0001341285 * num_dpus + -1.955777e-05 * blocks_per_dpu + -6.057153e-06 * block_size + 2.015762e-05 * (double)num_dpus * blocks_per_dpu + 8.565015e-07 * (double)num_dpus * block_size + 3.286646e-06 * (double)blocks_per_dpu * block_size + 1.687581e-06 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        } else {
          return 0.0220172 + 0.000782713 * num_dpus + 5.215406e-07 * blocks_per_dpu + -1.307885e-07 * block_size + 1.4757e-05 * (double)num_dpus * blocks_per_dpu + 2.042353e-08 * (double)num_dpus * block_size + 2.955997e-06 * (double)blocks_per_dpu * block_size + 6.377924e-07 * (double)num_dpus * blocks_per_dpu * block_size;
        }
      } else {
        if ((double)blocks_per_dpu * block_size <= 528) {
          if (num_dpus <= 18) {
            if (blocks_per_dpu <= 3) {
              return 0.02460671 + -5.084547e-10 * num_dpus + -0.0001303628 * blocks_per_dpu + -1.249184e-06 * block_size + 8.133088e-08 * (double)num_dpus * blocks_per_dpu + -1.372509e-09 * (double)num_dpus * block_size + 3.418466e-05 * (double)blocks_per_dpu * block_size + 6.009023e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.02336496 + 0 * num_dpus + 0.0002707685 * blocks_per_dpu + 5.652415e-06 * block_size + 1.722336e-07 * (double)num_dpus * blocks_per_dpu + 6.209944e-09 * (double)num_dpus * block_size + 3.292583e-05 * (double)blocks_per_dpu * block_size + 5.787745e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (num_dpus <= 20) {
              return 0.02507907 + 0 * num_dpus + 0.0002559863 * blocks_per_dpu + 7.542797e-06 * block_size + 9.142476e-09 * (double)num_dpus * blocks_per_dpu + 9.207505e-09 * (double)num_dpus * block_size + 2.925432e-05 * (double)blocks_per_dpu * block_size + 5.713734e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.02080636 + 0.0001743876 * num_dpus + 3.375299e-05 * blocks_per_dpu + -2.860237e-06 * block_size + 1.146367e-05 * (double)num_dpus * blocks_per_dpu + 1.109619e-07 * (double)num_dpus * block_size + 1.299243e-06 * (double)blocks_per_dpu * block_size + 2.108088e-06 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        } else {
          if (num_dpus <= 24) {
            if (num_dpus <= 18) {
              return 0.02357065 + 5.799657e-11 * num_dpus + 0.0002418794 * blocks_per_dpu + 3.039324e-07 * block_size + -2.992204e-09 * (double)num_dpus * blocks_per_dpu + 3.339303e-10 * (double)num_dpus * block_size + 3.378992e-05 * (double)blocks_per_dpu * block_size + 5.939634e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.02562146 + -6.886939e-05 * num_dpus + -3.27006e-05 * blocks_per_dpu + 1.071339e-05 * block_size + 1.577873e-05 * (double)num_dpus * blocks_per_dpu + -4.500837e-07 * (double)num_dpus * block_size + 9.970311e-06 * (double)blocks_per_dpu * block_size + 1.69666e-06 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if ((double)blocks_per_dpu * block_size <= 896) {
              return 0.02153756 + 0.0001470598 * num_dpus + -4.723668e-05 * blocks_per_dpu + -1.484113e-06 * block_size + 1.384028e-05 * (double)num_dpus * blocks_per_dpu + 7.289695e-08 * (double)num_dpus * block_size + 3.855201e-06 * (double)blocks_per_dpu * block_size + 2.080019e-06 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -0.006505027 + 0.001147202 * num_dpus + 1.145899e-05 * blocks_per_dpu + 7.460869e-08 * block_size + 1.209658e-05 * (double)num_dpus * blocks_per_dpu + 1.433609e-08 * (double)num_dpus * block_size + 7.928573e-05 * (double)blocks_per_dpu * block_size + -5.231174e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        }
      }
    } else {
      if ((double)num_dpus * blocks_per_dpu * block_size <= 2949120) {
        if (num_dpus <= 6) {
          if (num_dpus <= 1) {
            if (blocks_per_dpu <= 1) {
              return -1.47141 + 0.0003637767 * num_dpus + 1.490029 * blocks_per_dpu + 5.382312e-06 * block_size + 3.552506e-07 * (double)num_dpus * blocks_per_dpu + 3.285104e-10 * (double)num_dpus * block_size + 3.312776e-07 * (double)blocks_per_dpu * block_size + 3.235133e-10 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.01847604 + 6.127593e-14 * num_dpus + 3.129011e-05 * blocks_per_dpu + 4.193317e-08 * block_size + 3.300519e-10 * (double)num_dpus * blocks_per_dpu + 2.559446e-12 * (double)num_dpus * block_size + 5.714999e-06 * (double)blocks_per_dpu * block_size + 5.581053e-09 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (block_size <= 12288) {
              return 0.01927828 + 0.00088518 * num_dpus + 1.70134e-05 * blocks_per_dpu + 2.590095e-08 * block_size + 8.962369e-06 * (double)num_dpus * blocks_per_dpu + -2.847475e-08 * (double)num_dpus * block_size + 5.261186e-06 * (double)blocks_per_dpu * block_size + 1.555034e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.01869741 + 0.0004475593 * num_dpus + -0.007240765 * blocks_per_dpu + -7.589787e-08 * block_size + -0.0003309211 * (double)num_dpus * blocks_per_dpu + -9.030546e-08 * (double)num_dpus * block_size + 5.692245e-06 * (double)blocks_per_dpu * block_size + 2.201642e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        } else {
          if (num_dpus <= 32) {
            if (num_dpus <= 16) {
              return 0.006889053 + 0.001871969 * num_dpus + 6.351247e-05 * blocks_per_dpu + -1.028459e-06 * block_size + -4.886565e-06 * (double)num_dpus * blocks_per_dpu + 2.267714e-07 * (double)num_dpus * block_size + 6.815826e-06 * (double)blocks_per_dpu * block_size + 4.052603e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.02120415 + 0.0001034966 * num_dpus + 0.0001038127 * blocks_per_dpu + 2.793848e-07 * block_size + 9.273415e-06 * (double)num_dpus * blocks_per_dpu + -9.071546e-09 * (double)num_dpus * block_size + 1.719552e-05 * (double)blocks_per_dpu * block_size + 7.347064e-09 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if ((double)blocks_per_dpu * block_size <= 2048) {
              return 1.361276 + 0.06229832 * num_dpus + 9.393692e-05 * blocks_per_dpu + -7.48419e-06 * block_size + 1.055573e-05 * (double)num_dpus * blocks_per_dpu + 1.525394e-07 * (double)num_dpus * block_size + -0.0006129688 * (double)blocks_per_dpu * block_size + -3.05042e-05 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.02578945 + -1.790694e-05 * num_dpus + -5.868729e-05 * blocks_per_dpu + -3.206717e-07 * block_size + 1.357427e-05 * (double)num_dpus * blocks_per_dpu + 7.428991e-09 * (double)num_dpus * block_size + 3.443865e-05 * (double)blocks_per_dpu * block_size + 1.131637e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        }
      } else {
        if (num_dpus <= 32) {
          if (num_dpus <= 7) {
            if (num_dpus <= 5) {
              return -3.902703 + 0.1867274 * num_dpus + -0.066926 * blocks_per_dpu + -4.449741e-07 * block_size + 0.02008037 * (double)num_dpus * blocks_per_dpu + 2.403369e-07 * (double)num_dpus * block_size + 6.601917e-06 * (double)blocks_per_dpu * block_size + 9.943727e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 3.723849 + -0.6967496 * num_dpus + -0.3816906 * blocks_per_dpu + 1.019424e-05 * block_size + 0.05563637 * (double)num_dpus * blocks_per_dpu + -1.522709e-06 * (double)num_dpus * block_size + -1.684375e-05 * (double)blocks_per_dpu * block_size + 4.481129e-06 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (num_dpus <= 16) {
              return -0.8917289 + 0.01587655 * num_dpus + 0.02528572 * blocks_per_dpu + -8.467201e-07 * block_size + -0.002895008 * (double)num_dpus * blocks_per_dpu + 4.146368e-08 * (double)num_dpus * block_size + 1.56508e-05 * (double)blocks_per_dpu * block_size + 1.514897e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -0.8444448 + 0.01429245 * num_dpus + 0.00295639 * blocks_per_dpu + -1.462462e-08 * block_size + -6.629334e-05 * (double)num_dpus * blocks_per_dpu + 6.133584e-11 * (double)num_dpus * block_size + 1.904834e-05 * (double)blocks_per_dpu * block_size + 3.757697e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        } else {
          if ((double)num_dpus * blocks_per_dpu * block_size <= 12042240) {
            if ((double)num_dpus * blocks_per_dpu * block_size <= 5505024) {
              return 0.002608299 + 0.001234226 * num_dpus + 0.002148628 * blocks_per_dpu + 1.482369e-07 * block_size + -2.476929e-05 * (double)num_dpus * blocks_per_dpu + -2.961415e-09 * (double)num_dpus * block_size + 3.468899e-05 * (double)blocks_per_dpu * block_size + -4.109866e-09 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -0.1903954 + 0.003148556 * num_dpus + 0.00116539 * blocks_per_dpu + 6.448948e-08 * block_size + 1.17446e-05 * (double)num_dpus * blocks_per_dpu + -6.052692e-10 * (double)num_dpus * block_size + 3.57628e-05 * (double)blocks_per_dpu * block_size + -1.183677e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if ((double)num_dpus * blocks_per_dpu * block_size <= 30277632) {
              return -3.571069 + 0.02642797 * num_dpus + 0.005102158 * blocks_per_dpu + 8.959205e-08 * block_size + 1.717818e-05 * (double)num_dpus * blocks_per_dpu + 1.016114e-09 * (double)num_dpus * block_size + 4.077831e-05 * (double)blocks_per_dpu * block_size + 4.959793e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -3.23222 + 0.03845521 * num_dpus + 0.05757713 * blocks_per_dpu + 3.783607e-07 * block_size + -0.0006869306 * (double)num_dpus * blocks_per_dpu + -7.372785e-09 * (double)num_dpus * block_size + 4.151326e-05 * (double)blocks_per_dpu * block_size + 1.58782e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        }
      }
    }
  } else {
    if ((double)blocks_per_dpu * block_size <= 1920) {
      if ((double)blocks_per_dpu * block_size <= 896) {
        if (num_dpus <= 448) {
          if (num_dpus <= 128) {
            if (num_dpus <= 72) {
              return 0.04316866 + 0 * num_dpus + 0.0005686376 * blocks_per_dpu + 3.68328e-06 * block_size + -2.287026e-07 * (double)num_dpus * blocks_per_dpu + 1.618626e-08 * (double)num_dpus * block_size + 2.540557e-05 * (double)blocks_per_dpu * block_size + 1.786329e-06 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.03567624 + 8.991303e-05 * num_dpus + 0.0001731366 * blocks_per_dpu + -1.661138e-05 * block_size + 5.03649e-06 * (double)num_dpus * blocks_per_dpu + 2.402251e-07 * (double)num_dpus * block_size + 3.55113e-06 * (double)blocks_per_dpu * block_size + 2.088736e-06 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (num_dpus <= 384) {
              return 0.03748172 + 7.126691e-05 * num_dpus + 0.0003984533 * blocks_per_dpu + -6.073147e-07 * block_size + 1.80618e-06 * (double)num_dpus * blocks_per_dpu + 6.496815e-08 * (double)num_dpus * block_size + 0.000202244 * (double)blocks_per_dpu * block_size + 1.950177e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.07459875 + 1.84009e-05 * num_dpus + -0.004934989 * blocks_per_dpu + -0.0004590162 * block_size + 1.341679e-05 * (double)num_dpus * blocks_per_dpu + 1.084658e-06 * (double)num_dpus * block_size + -4.844016e-05 * (double)blocks_per_dpu * block_size + 9.958663e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        } else {
          if (num_dpus <= 480) {
            if (blocks_per_dpu <= 2) {
              return -0.773872 + 0.001836995 * num_dpus + -0.004635334 * blocks_per_dpu + -7.155646e-07 * block_size + 1.846547e-06 * (double)num_dpus * blocks_per_dpu + -2.096355e-08 * (double)num_dpus * block_size + 2.175172e-06 * (double)blocks_per_dpu * block_size + 1.019611e-06 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.0956945 + -3.602948e-10 * num_dpus + 0.0009536976 * blocks_per_dpu + 3.067605e-06 * block_size + 2.197876e-07 * (double)num_dpus * blocks_per_dpu + 8.98711e-08 * (double)num_dpus * block_size + 2.084594e-06 * (double)blocks_per_dpu * block_size + 9.771535e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (num_dpus <= 576) {
              return 0.05801676 + 8.092555e-05 * num_dpus + 0.002971364 * blocks_per_dpu + -1.47828e-05 * block_size + -3.082151e-06 * (double)num_dpus * blocks_per_dpu + 5.664648e-08 * (double)num_dpus * block_size + 0.0005639836 * (double)blocks_per_dpu * block_size + -4.073713e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.09635048 + 2.853306e-05 * num_dpus + 0.0007688408 * blocks_per_dpu + 1.305425e-05 * block_size + 5.406406e-07 * (double)num_dpus * blocks_per_dpu + 7.259583e-09 * (double)num_dpus * block_size + 0.0005424743 * (double)blocks_per_dpu * block_size + -8.219105e-10 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        }
      } else {
        if (num_dpus <= 384) {
          if ((double)blocks_per_dpu * block_size <= 1024) {
            if (num_dpus <= 96) {
              return 0.1057233 + 0.006853215 * num_dpus + 0.002990782 * blocks_per_dpu + 9.620206e-05 * block_size + -3.020414e-05 * (double)num_dpus * blocks_per_dpu + -1.243254e-06 * (double)num_dpus * block_size + -6.006133e-05 * (double)blocks_per_dpu * block_size + -4.979304e-06 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 1.324625 + 0.002343285 * num_dpus + 0.0003142953 * blocks_per_dpu + -1.930214e-05 * block_size + 1.056825e-06 * (double)num_dpus * blocks_per_dpu + 2.741367e-08 * (double)num_dpus * block_size + -0.001109952 * (double)blocks_per_dpu * block_size + -2.212755e-06 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (num_dpus <= 72) {
              return 0.04107185 + -1.466546e-09 * num_dpus + 0.0005568862 * blocks_per_dpu + 1.121056e-06 * block_size + 9.137389e-08 * (double)num_dpus * blocks_per_dpu + 4.926443e-09 * (double)num_dpus * block_size + 2.30088e-05 * (double)blocks_per_dpu * block_size + 1.617806e-06 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.03557599 + 4.98893e-05 * num_dpus + 0.000517122 * blocks_per_dpu + -2.840655e-06 * block_size + 1.616231e-06 * (double)num_dpus * blocks_per_dpu + 3.388139e-08 * (double)num_dpus * block_size + 0.0001379383 * (double)blocks_per_dpu * block_size + -4.751018e-10 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        } else {
          if ((double)blocks_per_dpu * block_size <= 1024) {
            if (blocks_per_dpu <= 12) {
              return 3.286849 + 0.0006184356 * num_dpus + 0.002015635 * blocks_per_dpu + -7.399184e-06 * block_size + -8.202922e-08 * (double)num_dpus * blocks_per_dpu + -7.895866e-11 * (double)num_dpus * block_size + -0.002903915 * (double)blocks_per_dpu * block_size + -5.152983e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 3.088309 + 0.001891126 * num_dpus + -0.01147553 * blocks_per_dpu + -0.004380639 * block_size + 6.971371e-05 * (double)num_dpus * blocks_per_dpu + 3.29978e-05 * (double)num_dpus * block_size + -0.002206495 * (double)blocks_per_dpu * block_size + -4.954601e-06 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (num_dpus <= 448) {
              return 0.07552689 + -5.56376e-06 * num_dpus + -0.001157365 * blocks_per_dpu + -1.191191e-05 * block_size + 4.65009e-06 * (double)num_dpus * blocks_per_dpu + 3.557756e-08 * (double)num_dpus * block_size + 0.0002127655 * (double)blocks_per_dpu * block_size + -1.407044e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.07304057 + 3.419129e-05 * num_dpus + 0.001183185 * blocks_per_dpu + 2.150001e-05 * block_size + 3.273383e-08 * (double)num_dpus * blocks_per_dpu + -6.354877e-09 * (double)num_dpus * block_size + 0.0002606817 * (double)blocks_per_dpu * block_size + 2.34629e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        }
      }
    } else {
      if (num_dpus <= 448) {
        if ((double)num_dpus * blocks_per_dpu * block_size <= 12042240) {
          if (num_dpus <= 384) {
            if (num_dpus <= 72) {
              return 0.03810135 + 3.050125e-11 * num_dpus + 0.0006593242 * blocks_per_dpu + -8.696184e-08 * block_size + 1.095183e-08 * (double)num_dpus * blocks_per_dpu + -3.821405e-10 * (double)num_dpus * block_size + 8.681563e-06 * (double)blocks_per_dpu * block_size + 6.104222e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.03115404 + 5.675547e-05 * num_dpus + 0.0005798389 * blocks_per_dpu + -1.180114e-06 * block_size + 1.268501e-06 * (double)num_dpus * blocks_per_dpu + 1.482412e-08 * (double)num_dpus * block_size + 6.986055e-05 * (double)blocks_per_dpu * block_size + 1.861842e-09 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if ((double)blocks_per_dpu * block_size <= 2400) {
              return 0.3151838 + -0.0001030674 * num_dpus + 1.886301e-05 * blocks_per_dpu + 5.983977e-07 * block_size + 2.45218e-06 * (double)num_dpus * blocks_per_dpu + -1.10371e-08 * (double)num_dpus * block_size + 8.419666e-08 * (double)blocks_per_dpu * block_size + 3.576429e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.07179117 + -1.844549e-05 * num_dpus + 0.00056394 * blocks_per_dpu + 1.438535e-05 * block_size + 5.23526e-07 * (double)num_dpus * blocks_per_dpu + -3.350685e-08 * (double)num_dpus * block_size + 0.0001061898 * (double)blocks_per_dpu * block_size + 7.457349e-10 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        } else {
          if (num_dpus <= 384) {
            if (num_dpus <= 80) {
              return 1.056152 + -0.04334461 * num_dpus + -0.02146912 * blocks_per_dpu + -1.452558e-06 * block_size + 0.0003657451 * (double)num_dpus * blocks_per_dpu + 1.60304e-08 * (double)num_dpus * block_size + -0.0001233262 * (double)blocks_per_dpu * block_size + 2.608746e-06 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -1.829993 + 0.002988071 * num_dpus + 0.004957676 * blocks_per_dpu + -5.266015e-07 * block_size + -8.205691e-06 * (double)num_dpus * blocks_per_dpu + -1.315871e-09 * (double)num_dpus * block_size + 8.254317e-05 * (double)blocks_per_dpu * block_size + 2.513874e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if ((double)num_dpus * blocks_per_dpu * block_size <= 27525120) {
              return -0.5131364 + 0.0001048474 * num_dpus + -0.008360267 * blocks_per_dpu + 1.371519e-05 * block_size + 2.522958e-05 * (double)num_dpus * blocks_per_dpu + -3.151143e-08 * (double)num_dpus * block_size + 0.000106996 * (double)blocks_per_dpu * block_size + 3.352826e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 2.401794 + -0.007022566 * num_dpus + -0.03476691 * blocks_per_dpu + 4.118786e-05 * block_size + 9.695641e-05 * (double)num_dpus * blocks_per_dpu + -9.709598e-08 * (double)num_dpus * block_size + 7.831016e-05 * (double)blocks_per_dpu * block_size + 1.262683e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        }
      } else {
        if ((double)blocks_per_dpu * block_size <= 26880) {
          if ((double)blocks_per_dpu * block_size <= 2048) {
            if (num_dpus <= 960) {
              return 0.4396256 + 0.02402107 * num_dpus + -0.0003051758 * blocks_per_dpu + -4.581858e-05 * block_size + 2.308642e-06 * (double)num_dpus * blocks_per_dpu + 8.827542e-08 * (double)num_dpus * block_size + -1.63469e-05 * (double)blocks_per_dpu * block_size + -1.174297e-05 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.3475683 + 0.0109209 * num_dpus + 0.001262665 * blocks_per_dpu + 1.127162e-05 * block_size + 1.316009e-07 * (double)num_dpus * blocks_per_dpu + -2.481493e-09 * (double)num_dpus * block_size + -3.457531e-06 * (double)blocks_per_dpu * block_size + -5.295966e-06 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (num_dpus <= 1664) {
              return 0.06179398 + 3.695237e-05 * num_dpus + 0.001498846 * blocks_per_dpu + 7.234596e-06 * block_size + -1.628433e-07 * (double)num_dpus * blocks_per_dpu + 1.799018e-09 * (double)num_dpus * block_size + 0.0001306314 * (double)blocks_per_dpu * block_size + 2.061701e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -0.1661801 + 0.0001517861 * num_dpus + -0.01516131 * blocks_per_dpu + 4.910693e-05 * block_size + 9.21205e-06 * (double)num_dpus * blocks_per_dpu + -2.619975e-08 * (double)num_dpus * block_size + 5.581753e-05 * (double)blocks_per_dpu * block_size + 6.462746e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        } else {
          if ((double)num_dpus * blocks_per_dpu * block_size <= 117440512) {
            if (num_dpus <= 1152) {
              return -0.6511898 + -0.001071895 * num_dpus + -0.004291177 * blocks_per_dpu + -3.286188e-06 * block_size + 1.600845e-05 * (double)num_dpus * blocks_per_dpu + 9.100968e-09 * (double)num_dpus * block_size + 0.000150859 * (double)blocks_per_dpu * block_size + 6.233433e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.8497658 + -0.00220218 * num_dpus + -0.03792727 * blocks_per_dpu + 5.26084e-05 * block_size + 2.521868e-05 * (double)num_dpus * blocks_per_dpu + -3.101901e-08 * (double)num_dpus * block_size + 2.824452e-05 * (double)blocks_per_dpu * block_size + 1.520696e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (num_dpus <= 1024) {
              return 0.5603752 + -0.001354654 * num_dpus + 0.06613064 * blocks_per_dpu + 5.644531e-06 * block_size + -5.525119e-05 * (double)num_dpus * blocks_per_dpu + -1.192208e-08 * (double)num_dpus * block_size + 0.0001415939 * (double)blocks_per_dpu * block_size + 6.883311e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -3.921803 + 0.003133937 * num_dpus + 0.1659777 * blocks_per_dpu + 3.227427e-05 * block_size + -9.387165e-05 * (double)num_dpus * blocks_per_dpu + -2.226766e-08 * (double)num_dpus * block_size + 0.0001214703 * (double)blocks_per_dpu * block_size + 6.932413e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        }
      }
    }
  }
}
} // namespace

double upmem_cm::gatherSgCostMs(int num_dpus, int blocks_per_dpu, int block_size) {
  // No transfer beats the fastest rate this sweep measured (1.1209e+07 B/ms);
  // outside its fitted region a leaf's linear model can slope below zero.
  return std::fmax(scatterSgGatherTreeCostMsModel(num_dpus, blocks_per_dpu, block_size), (double)num_dpus * blocks_per_dpu * block_size / 1.1209e+07);
}