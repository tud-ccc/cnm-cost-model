
#include <upmem_cost_model/ScatterGatherCm.h>

#include <cmath> // std::log2, std::exp
#include <limits>

// Fitted by model_tree.py on 34345 measured configs (upmemcm/scatter_cost/plots/block), relRMSE 6.90%.
namespace {
double scatterBlockCostMsModel(int num_dpus, int block_size) {
  if (block_size <= 29696) {
    if (num_dpus <= 60) {
      if (num_dpus <= 1) {
        if (block_size <= 992) {
          return 0.02445235 + -0.008213792 * num_dpus + 2.978938e-07 * block_size + 7.102343e-14 * (double)num_dpus * block_size;
        } else {
          return 0.01619632 + -1.281508e-16 * num_dpus + 8.605126e-07 * block_size + 2.051622e-13 * (double)num_dpus * block_size;
        }
      } else {
        if (num_dpus <= 2) {
          if (block_size <= 992) {
            return 0.01748827 + -0.0007018879 * num_dpus + 3.151592e-07 * block_size + 1.502796e-13 * (double)num_dpus * block_size;
          } else {
            return 0.01601812 + 1.956107e-15 * num_dpus + 8.710994e-07 * block_size + 4.153725e-13 * (double)num_dpus * block_size;
          }
        } else {
          if (num_dpus <= 2) {
            if (block_size <= 992) {
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
      if (num_dpus <= 384) {
        if (num_dpus <= 64) {
          if (block_size <= 992) {
            return 0.01465229 + 8.575594e-05 * num_dpus + 1.451604e-05 * block_size + 2.214971e-10 * (double)num_dpus * block_size;
          } else {
            return 0.01868029 + 5.791616e-05 * num_dpus + 4.12662e-06 * block_size + 6.296722e-11 * (double)num_dpus * block_size;
          }
        } else {
          if (block_size <= 320) {
            if (num_dpus <= 76) {
              return 0.0125651 + 0.0002627977 * num_dpus + -7.625204e-06 * block_size + 2.668404e-07 * (double)num_dpus * block_size;
            } else {
              return 0.02409778 + 6.964125e-05 * num_dpus + 1.91525e-05 * block_size + 2.155361e-08 * (double)num_dpus * block_size;
            }
          } else {
            if (block_size <= 1984) {
              return 0.02648893 + 8.158031e-05 * num_dpus + 1.653404e-05 * block_size + 1.184939e-09 * (double)num_dpus * block_size;
            } else {
              return 0.02623402 + 6.280551e-05 * num_dpus + 7.59238e-06 * block_size + 8.752472e-09 * (double)num_dpus * block_size;
            }
          }
        }
      } else {
        if (num_dpus <= 448) {
          if (block_size <= 640) {
            return 0.1739477 + -0.000249877 * num_dpus + -7.947799e-05 * block_size + 2.912719e-07 * (double)num_dpus * block_size;
          } else {
            if ((double)num_dpus * block_size <= 683008) {
              return 0.1393603 + -0.0001042337 * num_dpus + -2.566391e-05 * block_size + 8.340381e-08 * (double)num_dpus * block_size;
            } else {
              return 0.05227525 + 3.680158e-05 * num_dpus + 6.284668e-05 * block_size + -1.100116e-07 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (block_size <= 1984) {
            if (block_size <= 640) {
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
      if ((double)num_dpus * block_size <= 1.50733e+07) {
        if (num_dpus <= 31) {
          if (num_dpus <= 8) {
            if (num_dpus <= 2) {
              return 0.007984273 + 0.0005475631 * num_dpus + 1.047497e-06 * block_size + -5.108047e-09 * (double)num_dpus * block_size;
            } else {
              return 0.01000012 + 0.0006663873 * num_dpus + 9.162231e-07 * block_size + 3.095321e-08 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 15) {
              return -0.02207953 + 0.002129061 * num_dpus + 2.315521e-06 * block_size + -3.987082e-08 * (double)num_dpus * block_size;
            } else {
              return 0.001548618 + 0.0002808099 * num_dpus + 1.640711e-06 * block_size + 4.204662e-08 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (block_size <= 51200) {
            if (num_dpus <= 42) {
              return 0.01096417 + 0.0001678436 * num_dpus + 4.82042e-06 * block_size + -7.986688e-09 * (double)num_dpus * block_size;
            } else {
              return 0.01653396 + 3.266797e-05 * num_dpus + 4.529555e-06 * block_size + -1.86937e-10 * (double)num_dpus * block_size;
            }
          } else {
            if (block_size <= 172032) {
              return -0.01678643 + -1.715905e-05 * num_dpus + 5.042328e-06 * block_size + 1.146744e-09 * (double)num_dpus * block_size;
            } else {
              return -0.0194627 + -0.002935186 * num_dpus + 4.658506e-06 * block_size + 2.346645e-08 * (double)num_dpus * block_size;
            }
          }
        }
      } else {
        if (num_dpus <= 31) {
          if (num_dpus <= 15) {
            if (num_dpus <= 8) {
              return -5.591451 + 0.2130193 * num_dpus + 1.109639e-06 * block_size + 2.812052e-07 * (double)num_dpus * block_size;
            } else {
              return -1.905059 + 0.006773202 * num_dpus + 2.936591e-06 * block_size + 2.502031e-08 * (double)num_dpus * block_size;
            }
          } else {
            if ((double)num_dpus * block_size <= 2.53952e+07) {
              return -3.007257 + 0.02857975 * num_dpus + 4.032636e-06 * block_size + 9.366381e-08 * (double)num_dpus * block_size;
            } else {
              return -4.167603 + 0.07856616 * num_dpus + 5.959101e-06 * block_size + 2.77907e-08 * (double)num_dpus * block_size;
            }
          }
        } else {
          if ((double)num_dpus * block_size <= 3.01466e+07) {
            if ((double)num_dpus * block_size <= 1.83501e+07) {
              return -1.460508 + 0.0001560188 * num_dpus + 5.839205e-06 * block_size + 8.499632e-08 * (double)num_dpus * block_size;
            } else {
              return -5.051979 + 0.02671476 * num_dpus + 1.064442e-05 * block_size + 1.062803e-07 * (double)num_dpus * block_size;
            }
          } else {
            if (block_size <= 4.1943e+06) {
              return -2.028238 + 0.0107707 * num_dpus + 1.10426e-05 * block_size + 4.306077e-08 * (double)num_dpus * block_size;
            } else {
              return -9.446037 + 0.0717639 * num_dpus + 1.25899e-05 * block_size + 3.215797e-08 * (double)num_dpus * block_size;
            }
          }
        }
      }
    } else {
      if ((double)num_dpus * block_size <= 2.53952e+07) {
        if ((double)num_dpus * block_size <= 1.37134e+07) {
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
          if (block_size <= 155648) {
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
            if (num_dpus <= 400) {
              return -2.662838 + 0.003274289 * num_dpus + -8.093045e-05 * block_size + 3.215821e-07 * (double)num_dpus * block_size;
            } else {
              return -1.33595 + -0.0009195327 * num_dpus + 3.483799e-05 * block_size + 4.514479e-08 * (double)num_dpus * block_size;
            }
          }
        } else {
          if ((double)num_dpus * block_size <= 2.49561e+08) {
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

// Fitted by model_tree.py on 34212 measured configs (upmemcm/scatter_cost/plots/gather), relRMSE 7.78%.
namespace {
double gatherCostMsModel(int num_dpus, int block_size) {
  if (num_dpus <= 64) {
    if ((double)num_dpus * block_size <= 5.57056e+06) {
      if (num_dpus <= 14) {
        if (num_dpus <= 1) {
          if (block_size <= 992) {
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
            if (block_size <= 992) {
              return 0.02994615 + -0.004240095 * num_dpus + 4.672222e-06 * block_size + 2.281358e-09 * (double)num_dpus * block_size;
            } else {
              return 0.02039203 + -2.877088e-16 * num_dpus + 4.07329e-06 * block_size + 1.988911e-09 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 2) {
              return 0.01804256 + 0.001152638 * num_dpus + 3.970825e-06 * block_size + 2.906662e-09 * (double)num_dpus * block_size;
            } else {
              return 0.02146288 + 0.0002854237 * num_dpus + 4.223511e-06 * block_size + 1.566628e-08 * (double)num_dpus * block_size;
            }
          }
        }
      } else {
        if (block_size <= 1984) {
          if (num_dpus <= 14) {
            return 0.02349903 + 0 * num_dpus + 8.047635e-06 * block_size + 2.947132e-08 * (double)num_dpus * block_size;
          } else {
            if (num_dpus <= 15) {
              return 0.02452969 + 0 * num_dpus + 8.734861e-06 * block_size + 3.412055e-08 * (double)num_dpus * block_size;
            } else {
              return 0.0171645 + 0.0002357056 * num_dpus + 1.944119e-05 * block_size + 3.349908e-07 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (num_dpus <= 31) {
            if (num_dpus <= 15) {
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
      if (num_dpus <= 31) {
        if (num_dpus <= 15) {
          if ((double)num_dpus * block_size <= 2.43794e+07) {
            if ((double)num_dpus * block_size <= 1.3566e+07) {
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
          if ((double)num_dpus * block_size <= 1.6384e+07) {
            if (block_size <= 425984) {
              return 0.1242969 + -0.007950161 * num_dpus + 7.716706e-06 * block_size + 4.030752e-08 * (double)num_dpus * block_size;
            } else {
              return 0.4591393 + -0.06128193 * num_dpus + 6.582891e-06 * block_size + 1.76986e-07 * (double)num_dpus * block_size;
            }
          } else {
            if (block_size <= 1.04858e+06) {
              return -0.4923229 + -0.0879438 * num_dpus + 7.320605e-06 * block_size + 2.457676e-07 * (double)num_dpus * block_size;
            } else {
              return -7.004457 + 0.1349104 * num_dpus + 1.325378e-05 * block_size + 2.864367e-08 * (double)num_dpus * block_size;
            }
          }
        }
      } else {
        if ((double)num_dpus * block_size <= 1.49422e+07) {
          if ((double)num_dpus * block_size <= 1.11411e+07) {
            if (block_size <= 131072) {
              return -1.727686 + 0.03224061 * num_dpus + 3.08291e-05 * block_size + -2.614022e-07 * (double)num_dpus * block_size;
            } else {
              return 0.09409118 + -0.002892203 * num_dpus + 1.61152e-05 * block_size + 2.082676e-08 * (double)num_dpus * block_size;
            }
          } else {
            return -0.8403893 + 0.002920683 * num_dpus + 1.819476e-05 * block_size + 3.505336e-08 * (double)num_dpus * block_size;
          }
        } else {
          if (block_size <= 1.04858e+06) {
            if ((double)num_dpus * block_size <= 2.43794e+07) {
              return -3.549847 + 0.01041811 * num_dpus + 2.078501e-05 * block_size + 1.347533e-07 * (double)num_dpus * block_size;
            } else {
              return -6.874934 + 0.05861217 * num_dpus + 2.770389e-05 * block_size + 4.06132e-08 * (double)num_dpus * block_size;
            }
          } else {
            if ((double)num_dpus * block_size <= 4.09993e+08) {
              return -7.838779 + 0.0685446 * num_dpus + 2.663744e-05 * block_size + 4.087748e-08 * (double)num_dpus * block_size;
            } else {
              return 8.364624 + -0.02184495 * num_dpus + 2.582475e-05 * block_size + 3.497096e-08 * (double)num_dpus * block_size;
            }
          }
        }
      }
    }
  } else {
    if ((double)num_dpus * block_size <= 2.43794e+07) {
      if (block_size <= 992) {
        if (num_dpus <= 416) {
          if (num_dpus <= 76) {
            if (num_dpus <= 72) {
              return -0.01393102 + 0.0007819741 * num_dpus + 5.944326e-06 * block_size + 9.727544e-07 * (double)num_dpus * block_size;
            } else {
              return 0.1019767 + -0.000810456 * num_dpus + 6.026312e-05 * block_size + 2.636605e-07 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 112) {
              return 0.0244609 + 0.0001561971 * num_dpus + 3.476525e-07 * block_size + 1.044791e-06 * (double)num_dpus * block_size;
            } else {
              return 0.02839603 + 9.51941e-05 * num_dpus + 9.366046e-05 * block_size + 1.306287e-07 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (num_dpus <= 448) {
            if (block_size <= 320) {
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
          if (block_size <= 1984) {
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
          if (block_size <= 1984) {
            if (block_size <= 992) {
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
            if ((double)num_dpus * block_size <= 9.75176e+07) {
              return 5.556363 + -0.1300075 * num_dpus + 2.512719e-05 * block_size + 2.590751e-07 * (double)num_dpus * block_size;
            } else {
              return -11.28636 + 0.0930933 * num_dpus + 4.708983e-05 * block_size + -6.890673e-08 * (double)num_dpus * block_size;
            }
          } else {
            if ((double)num_dpus * block_size <= 3.99114e+07) {
              return -3.059682 + 0.001458298 * num_dpus + 4.796057e-05 * block_size + 8.298355e-08 * (double)num_dpus * block_size;
            } else {
              return -2.207048 + 0.003350901 * num_dpus + 5.280952e-05 * block_size + 3.199724e-08 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (block_size <= 131072) {
            return -5.836775 + 0.007168429 * num_dpus + 0.0001194531 * block_size + -2.726254e-08 * (double)num_dpus * block_size;
          } else {
            if ((double)num_dpus * block_size <= 4.09993e+08) {
              return 1.979561 + -0.007289729 * num_dpus + 6.158121e-05 * block_size + 7.796775e-08 * (double)num_dpus * block_size;
            } else {
              return 16.52689 + -0.02878572 * num_dpus + 4.661622e-05 * block_size + 9.874663e-08 * (double)num_dpus * block_size;
            }
          }
        }
      } else {
        if ((double)num_dpus * block_size <= 1.44441e+08) {
          if (block_size <= 43008) {
            if (num_dpus <= 1024) {
              return 0.9708035 + -0.002426013 * num_dpus + 2.681059e-05 * block_size + 1.250414e-07 * (double)num_dpus * block_size;
            } else {
              return 0.01098418 + -0.0002163832 * num_dpus + 2.792781e-05 * block_size + 6.719725e-08 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 896) {
              return -4.29597 + 0.003384822 * num_dpus + 0.0001134858 * block_size + 4.203855e-08 * (double)num_dpus * block_size;
            } else {
              return 0.5119057 + -0.003202554 * num_dpus + 2.823688e-05 * block_size + 1.314367e-07 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (block_size <= 262144) {
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
  if (num_dpus <= 60) {
    if (num_dpus <= 1) {
      if (block_size <= 992) {
        return 0.01780013 + 0 * num_dpus + 3.628914e-07 * block_size + 8.859652e-11 * (double)num_dpus * block_size;
      } else {
        if (block_size <= 57344) {
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
        if (block_size <= 992) {
          return 0.01847631 + -0.0007467947 * num_dpus + 1.843712e-06 * block_size + 9.002501e-10 * (double)num_dpus * block_size;
        } else {
          if (block_size <= 55296) {
            return 0.01806694 + 7.259424e-16 * num_dpus + 8.506462e-07 * block_size + 4.153546e-10 * (double)num_dpus * block_size;
          } else {
            if (block_size <= 884736) {
              return 0.009228773 + 0 * num_dpus + 1.031195e-06 * block_size + 5.03513e-10 * (double)num_dpus * block_size;
            } else {
              return -0.05167651 + 0 * num_dpus + 1.140372e-06 * block_size + 5.568222e-10 * (double)num_dpus * block_size;
            }
          }
        }
      } else {
        if (num_dpus <= 2) {
          if (block_size <= 3328) {
            return 0.01756121 + 0 * num_dpus + 1.118218e-06 * block_size + 8.190071e-10 * (double)num_dpus * block_size;
          } else {
            if (block_size <= 55296) {
              return 0.01783924 + -1.138197e-10 * num_dpus + 8.602433e-07 * block_size + 6.300608e-10 * (double)num_dpus * block_size;
            } else {
              return 0.002414629 + 0 * num_dpus + 1.079803e-06 * block_size + 7.908703e-10 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (num_dpus <= 3) {
            if (block_size <= 992) {
              return 0.02881024 + -0.002744338 * num_dpus + 6.227067e-07 * block_size + 6.081119e-10 * (double)num_dpus * block_size;
            } else {
              return 0.0108129 + 0.001561918 * num_dpus + 1.025948e-06 * block_size + 1.001902e-09 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 4) {
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
        if (block_size <= 992) {
          return 0.01521409 + 6.370091e-05 * num_dpus + 7.584957e-06 * block_size + 1.18515e-07 * (double)num_dpus * block_size;
        } else {
          return 0.00537172 + 0.0002518778 * num_dpus + 2.22631e-06 * block_size + 3.478608e-08 * (double)num_dpus * block_size;
        }
      } else {
        if (num_dpus <= 384) {
          if (block_size <= 320) {
            if (num_dpus <= 76) {
              return 0.04624317 + -0.0002294026 * num_dpus + -2.812516e-05 * block_size + 5.422748e-07 * (double)num_dpus * block_size;
            } else {
              return 0.02576444 + 3.364138e-05 * num_dpus + 2.045434e-05 * block_size + 1.713649e-08 * (double)num_dpus * block_size;
            }
          } else {
            if (block_size <= 992) {
              return 0.02535217 + 3.007508e-05 * num_dpus + 2.276512e-05 * block_size + 4.571158e-08 * (double)num_dpus * block_size;
            } else {
              return 0.02875496 + 5.328071e-05 * num_dpus + 8.167961e-06 * block_size + 3.875303e-09 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (block_size <= 1984) {
            if (block_size <= 320) {
              return 0.04748377 + 7.148324e-06 * num_dpus + 0.000170978 * block_size + -3.142785e-07 * (double)num_dpus * block_size;
            } else {
              return 0.0724927 + -2.548268e-05 * num_dpus + 2.189041e-05 * block_size + -4.047818e-09 * (double)num_dpus * block_size;
            }
          } else {
            if (block_size <= 8704) {
              return 0.1063095 + -0.0001344529 * num_dpus + -4.869006e-06 * block_size + 4.43856e-08 * (double)num_dpus * block_size;
            } else {
              return 0.1553033 + -0.0002648572 * num_dpus + 8.008523e-06 * block_size + 1.385415e-08 * (double)num_dpus * block_size;
            }
          }
        }
      }
    } else {
      if (num_dpus <= 448) {
        if (block_size <= 992) {
          return -0.03129656 + 0.0001849194 * num_dpus + -3.47711e-05 * block_size + 1.997605e-07 * (double)num_dpus * block_size;
        } else {
          if (block_size <= 90112) {
            if (block_size <= 3968) {
              return -0.007423468 + 0.0001850367 * num_dpus + 0.0001017837 * block_size + -2.076565e-07 * (double)num_dpus * block_size;
            } else {
              return 0.2581581 + -0.0004680944 * num_dpus + 9.493931e-06 * block_size + 1.723949e-08 * (double)num_dpus * block_size;
            }
          } else {
            if (block_size <= 344064) {
              return -5.619917 + 0.01314745 * num_dpus + 4.241446e-05 * block_size + -6.684184e-08 * (double)num_dpus * block_size;
            } else {
              return 1.985483 + -0.004558749 * num_dpus + 1.407915e-05 * block_size + -8.823235e-10 * (double)num_dpus * block_size;
            }
          }
        }
      } else {
        if (block_size <= 992) {
          if (num_dpus <= 768) {
            return 0.03986232 + 5.180502e-05 * num_dpus + 5.04218e-05 * block_size + 2.864704e-08 * (double)num_dpus * block_size;
          } else {
            if (num_dpus <= 1408) {
              return 0.04156218 + 4.455405e-05 * num_dpus + 8.637348e-05 * block_size + -1.814406e-08 * (double)num_dpus * block_size;
            } else {
              return -0.02216266 + 8.502255e-05 * num_dpus + 7.121874e-05 * block_size + -5.039265e-09 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (block_size <= 1984) {
            if (block_size <= 992) {
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

double upmem_cm::scatterSgCostMs(int num_dpus, int block_size,
                                 int blocks_per_dpu) {
  if (blocks_per_dpu <= 28) {
    if (blocks_per_dpu <= 7) {
      if ((double)num_dpus * block_size <= 124416) {
        if (block_size <= 864) {
          if (num_dpus <= 60) {
            if (num_dpus <= 2) {
              return 0.04965499 + -0.003670288 * num_dpus +
                     0.001053712 * blocks_per_dpu + -5.900087e-06 * block_size +
                     -8.107639e-09 * (double)num_dpus * blocks_per_dpu +
                     5.830334e-06 * (double)num_dpus * block_size +
                     1.661055e-06 * (double)blocks_per_dpu * block_size +
                     1.986024e-12 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return 0.0437957 + 0.0004193133 * num_dpus +
                     0.0006825864 * blocks_per_dpu + 1.007736e-05 * block_size +
                     2.407493e-09 * (double)num_dpus * blocks_per_dpu +
                     1.347219e-06 * (double)num_dpus * block_size +
                     2.959965e-09 * (double)blocks_per_dpu * block_size +
                     3.509235e-13 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          } else {
            if (num_dpus <= 384) {
              return 0.08723239 + 2.429084e-05 * num_dpus +
                     0.002474669 * blocks_per_dpu + 9.399932e-05 * block_size +
                     9.47437e-08 * (double)num_dpus * blocks_per_dpu +
                     6.644004e-07 * (double)num_dpus * block_size +
                     7.215478e-09 * (double)blocks_per_dpu * block_size +
                     4.229009e-13 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return 0.09152196 + 8.650534e-05 * num_dpus +
                     0.001776923 * blocks_per_dpu + 0.0001728379 * block_size +
                     2.974378e-07 * (double)num_dpus * blocks_per_dpu +
                     1.013569e-06 * (double)num_dpus * block_size +
                     2.330795e-09 * (double)blocks_per_dpu * block_size +
                     5.971599e-13 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          }
        } else {
          if (blocks_per_dpu <= 2) {
            if (blocks_per_dpu <= 1) {
              return 0.04297853 + 0.00146397 * num_dpus +
                     -3.897185e-10 * blocks_per_dpu + 3.59448e-06 * block_size +
                     8.725941e-11 * (double)num_dpus * blocks_per_dpu +
                     3.88522e-09 * (double)num_dpus * block_size +
                     2.142293e-13 * (double)blocks_per_dpu * block_size +
                     2.315767e-16 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return 0.04228036 + 0.001059999 * num_dpus +
                     -1.109584e-10 * blocks_per_dpu +
                     6.374582e-06 * block_size +
                     1.263615e-10 * (double)num_dpus * blocks_per_dpu +
                     3.489056e-07 * (double)num_dpus * block_size +
                     7.599172e-13 * (double)blocks_per_dpu * block_size +
                     4.15928e-14 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          } else {
            if ((double)num_dpus * block_size <= 22848) {
              return 0.0386862 + -0.0002501491 * num_dpus +
                     0.002687461 * blocks_per_dpu + 6.659121e-07 * block_size +
                     -5.669642e-08 * (double)num_dpus * blocks_per_dpu +
                     1.597155e-06 * (double)num_dpus * block_size +
                     2.05359e-06 * (double)blocks_per_dpu * block_size +
                     8.212567e-13 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return 0.005067751 + 0.001117995 * num_dpus +
                     0.01049384 * blocks_per_dpu + 7.180006e-06 * block_size +
                     6.77563e-08 * (double)num_dpus * blocks_per_dpu +
                     7.135381e-07 * (double)num_dpus * block_size +
                     4.147913e-07 * (double)blocks_per_dpu * block_size +
                     2.255221e-12 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          }
        }
      } else {
        if (blocks_per_dpu <= 2) {
          if (blocks_per_dpu <= 1) {
            if (num_dpus <= 64) {
              return 0.08061349 + -9.527868e-05 * num_dpus +
                     -2.164597e-10 * blocks_per_dpu +
                     -8.238467e-08 * block_size +
                     -5.67912e-12 * (double)num_dpus * blocks_per_dpu +
                     2.909739e-07 * (double)num_dpus * block_size +
                     -4.917805e-15 * (double)blocks_per_dpu * block_size +
                     1.734339e-14 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return 0.1569549 + 0.0001730293 * num_dpus +
                     -1.455192e-11 * blocks_per_dpu +
                     5.248311e-06 * block_size +
                     1.031334e-11 * (double)num_dpus * blocks_per_dpu +
                     5.178748e-08 * (double)num_dpus * block_size +
                     3.128239e-13 * (double)blocks_per_dpu * block_size +
                     3.086775e-15 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          } else {
            if (num_dpus <= 400) {
              return 0.1934051 + 5.888737e-05 * num_dpus +
                     -1.360822e-07 * blocks_per_dpu +
                     -1.214991e-06 * block_size +
                     -2.256267e-12 * (double)num_dpus * blocks_per_dpu +
                     1.266178e-07 * (double)num_dpus * block_size +
                     -3.96659e-12 * (double)blocks_per_dpu * block_size +
                     1.810235e-14 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return 0.2496154 + 0.0001417385 * num_dpus +
                     -8.498318e-09 * blocks_per_dpu + 2.32204e-05 * block_size +
                     1.309036e-11 * (double)num_dpus * blocks_per_dpu +
                     8.162154e-08 * (double)num_dpus * block_size +
                     3.228451e-12 * (double)blocks_per_dpu * block_size +
                     1.157068e-14 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          }
        } else {
          if (blocks_per_dpu <= 4) {
            if (num_dpus <= 400) {
              return 0.2430537 + -0.0001273727 * num_dpus +
                     -1.008666e-07 * blocks_per_dpu +
                     -2.850442e-06 * block_size +
                     -4.21447e-11 * (double)num_dpus * blocks_per_dpu +
                     2.167949e-07 * (double)num_dpus * block_size +
                     -2.604632e-12 * (double)blocks_per_dpu * block_size +
                     5.743539e-14 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return 0.2518245 + 0.0001392849 * num_dpus +
                     -3.783498e-08 * blocks_per_dpu +
                     4.537708e-05 * block_size +
                     2.230413e-11 * (double)num_dpus * blocks_per_dpu +
                     1.496661e-07 * (double)num_dpus * block_size +
                     9.852372e-12 * (double)blocks_per_dpu * block_size +
                     3.907248e-14 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          } else {
            if (num_dpus <= 152) {
              return 0.04185468 + -0.0001387972 * num_dpus +
                     0.01881541 * blocks_per_dpu + -1.647913e-05 * block_size +
                     2.935839e-07 * (double)num_dpus * blocks_per_dpu +
                     8.647062e-07 * (double)num_dpus * block_size +
                     1.227894e-06 * (double)blocks_per_dpu * block_size +
                     1.582217e-11 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return 0.1727637 + 0.0001951699 * num_dpus +
                     -7.133349e-08 * blocks_per_dpu +
                     5.089706e-06 * block_size +
                     2.834559e-11 * (double)num_dpus * blocks_per_dpu +
                     3.091922e-07 * (double)num_dpus * block_size +
                     2.019869e-12 * (double)blocks_per_dpu * block_size +
                     1.258603e-13 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          }
        }
      }
    } else {
      if (num_dpus <= 64) {
        if ((double)num_dpus * blocks_per_dpu * block_size <= 8) {
          if (num_dpus <= 9) {
            if (block_size <= 464) {
              return 0.04375141 + 0.0007229678 * num_dpus +
                     0.0001042808 * blocks_per_dpu +
                     -6.463259e-06 * block_size +
                     -2.619496e-08 * (double)num_dpus * blocks_per_dpu +
                     1.925938e-06 * (double)num_dpus * block_size +
                     1.536526e-06 * (double)blocks_per_dpu * block_size +
                     3.945162e-12 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return 0.04848711 + -0.001312959 * num_dpus +
                     0.0005114254 * blocks_per_dpu +
                     -3.076828e-06 * block_size +
                     -5.310129e-08 * (double)num_dpus * blocks_per_dpu +
                     4.459467e-06 * (double)num_dpus * block_size +
                     1.493666e-06 * (double)blocks_per_dpu * block_size +
                     3.89848e-12 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          } else {
            if (num_dpus <= 31) {
              return 0.01875428 + 0.001252417 * num_dpus +
                     0.0008274112 * blocks_per_dpu +
                     -4.381348e-06 * block_size +
                     -2.461546e-07 * (double)num_dpus * blocks_per_dpu +
                     1.783022e-06 * (double)num_dpus * block_size +
                     2.405025e-06 * (double)blocks_per_dpu * block_size +
                     1.02899e-11 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return 0.0002498552 + 0.001034852 * num_dpus +
                     0.002126767 * blocks_per_dpu + 0.0001604209 * block_size +
                     -1.441912e-06 * (double)num_dpus * blocks_per_dpu +
                     -2.478939e-06 * (double)num_dpus * block_size +
                     7.716031e-06 * (double)blocks_per_dpu * block_size +
                     7.816931e-11 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          }
        } else {
          if (num_dpus <= 31) {
            if (num_dpus <= 15) {
              return 0.153046 + -0.004468206 * num_dpus +
                     0.001302686 * blocks_per_dpu + -6.313765e-06 * block_size +
                     -2.210245e-08 * (double)num_dpus * blocks_per_dpu +
                     1.203361e-06 * (double)num_dpus * block_size +
                     8.470552e-07 * (double)blocks_per_dpu * block_size +
                     2.58548e-12 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return 0.07153928 + 0.001822531 * num_dpus +
                     0.0009368201 * blocks_per_dpu + -7.9802e-06 * block_size +
                     -1.734764e-08 * (double)num_dpus * blocks_per_dpu +
                     1.566594e-07 * (double)num_dpus * block_size +
                     3.164163e-06 * (double)blocks_per_dpu * block_size +
                     1.831985e-11 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          } else {
            if (block_size <= 704) {
              return 0.372591 + -0.006104519 * num_dpus +
                     -0.00538161 * blocks_per_dpu + -0.0004263711 * block_size +
                     0.0001452134 * (double)num_dpus * blocks_per_dpu +
                     8.62762e-06 * (double)num_dpus * block_size +
                     1.347412e-05 * (double)blocks_per_dpu * block_size +
                     1.291319e-09 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return 0.04219273 + 0.001863924 * num_dpus +
                     0.001957968 * blocks_per_dpu + 2.04117e-06 * block_size +
                     4.886727e-08 * (double)num_dpus * blocks_per_dpu +
                     -1.971097e-07 * (double)num_dpus * block_size +
                     6.525981e-06 * (double)blocks_per_dpu * block_size +
                     7.59774e-11 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          }
        }
      } else {
        if (blocks_per_dpu <= 16) {
          if ((double)num_dpus * blocks_per_dpu * block_size <= 2.60014e+07) {
            if (block_size <= 240) {
              return 0.0477325 + 9.515874e-05 * num_dpus +
                     0.004531308 * blocks_per_dpu + 9.346134e-05 * block_size +
                     3.199566e-07 * (double)num_dpus * blocks_per_dpu +
                     1.324917e-06 * (double)num_dpus * block_size +
                     7.106459e-09 * (double)blocks_per_dpu * block_size +
                     1.467178e-12 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return -0.07841679 + 0.0003458657 * num_dpus +
                     0.01700513 * blocks_per_dpu + 0.0001355209 * block_size +
                     1.110991e-06 * (double)num_dpus * blocks_per_dpu +
                     1.872361e-07 * (double)num_dpus * block_size +
                     2.263335e-07 * (double)blocks_per_dpu * block_size +
                     1.355734e-11 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          } else {
            if (num_dpus <= 832) {
              return -8.028829 + 0.0009876653 * num_dpus +
                     0.3272963 * blocks_per_dpu + -8.027658e-05 * block_size +
                     5.041896e-05 * (double)num_dpus * blocks_per_dpu +
                     1.870453e-06 * (double)num_dpus * block_size +
                     2.0103e-05 * (double)blocks_per_dpu * block_size +
                     2.984776e-09 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return -7.528921 + 0.004021707 * num_dpus +
                     0.2473772 * blocks_per_dpu + 0.000497704 * block_size +
                     -0.0001363306 * (double)num_dpus * blocks_per_dpu +
                     -2.391236e-07 * (double)num_dpus * block_size +
                     8.05637e-05 * (double)blocks_per_dpu * block_size +
                     1.898449e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          }
        } else {
          if ((double)num_dpus * blocks_per_dpu * block_size <= 3.42917e+07) {
            if (block_size <= 152) {
              return 0.06831001 + 1.743654e-05 * num_dpus +
                     0.002817065 * blocks_per_dpu + 4.779292e-05 * block_size +
                     3.890323e-06 * (double)num_dpus * blocks_per_dpu +
                     2.033928e-06 * (double)num_dpus * block_size +
                     2.259282e-10 * (double)blocks_per_dpu * block_size +
                     6.49514e-12 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return -0.1477483 + 0.0003542552 * num_dpus +
                     0.01245742 * blocks_per_dpu + 0.0002723254 * block_size +
                     8.30225e-07 * (double)num_dpus * blocks_per_dpu +
                     4.642034e-07 * (double)num_dpus * block_size +
                     9.890319e-08 * (double)blocks_per_dpu * block_size +
                     6.677071e-12 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          } else {
            if (num_dpus <= 832) {
              return 7.348664 + -0.01561046 * num_dpus +
                     -0.3022083 * blocks_per_dpu + -0.002131235 * block_size +
                     0.0005026446 * (double)num_dpus * blocks_per_dpu +
                     3.695101e-06 * (double)num_dpus * block_size +
                     6.981089e-05 * (double)blocks_per_dpu * block_size +
                     3.774697e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return -6.936147 + 0.003565175 * num_dpus +
                     0.07623959 * blocks_per_dpu + 0.0009152949 * block_size +
                     -4.307628e-05 * (double)num_dpus * blocks_per_dpu +
                     -6.404365e-07 * (double)num_dpus * block_size +
                     8.739657e-05 * (double)blocks_per_dpu * block_size +
                     2.854722e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          }
        }
      }
    }
  } else {
    if ((double)num_dpus * block_size <= 4480) {
      if (num_dpus <= 31) {
        if (block_size <= 80) {
          if ((double)num_dpus * blocks_per_dpu * block_size <= 683008) {
            if ((double)num_dpus * blocks_per_dpu <= 1488) {
              return 0.04465673 + 0.0004289939 * num_dpus +
                     6.787333e-06 * blocks_per_dpu +
                     -4.153357e-05 * block_size +
                     4.270446e-05 * (double)num_dpus * blocks_per_dpu +
                     -2.656303e-06 * (double)num_dpus * block_size +
                     1.78978e-06 * (double)blocks_per_dpu * block_size +
                     2.128917e-11 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return 0.03586535 + 0.0004131331 * num_dpus +
                     -1.920302e-05 * blocks_per_dpu +
                     -0.0001689927 * block_size +
                     4.628022e-05 * (double)num_dpus * blocks_per_dpu +
                     -3.757006e-07 * (double)num_dpus * block_size +
                     3.801863e-06 * (double)blocks_per_dpu * block_size +
                     2.048757e-11 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          } else {
            if (num_dpus <= 15) {
              return 0.08550912 + 0.005963362 * num_dpus +
                     1.135343e-05 * blocks_per_dpu + -0.001480927 * block_size +
                     2.229867e-05 * (double)num_dpus * blocks_per_dpu +
                     -1.081709e-07 * (double)num_dpus * block_size +
                     7.906323e-06 * (double)blocks_per_dpu * block_size +
                     2.865855e-11 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return 0.1218897 + 0.007917561 * num_dpus +
                     0.0001997091 * blocks_per_dpu + -0.004348334 * block_size +
                     1.138882e-05 * (double)num_dpus * blocks_per_dpu +
                     -1.283282e-07 * (double)num_dpus * block_size +
                     1.8847e-05 * (double)blocks_per_dpu * block_size +
                     1.145871e-10 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          }
        } else {
          if ((double)num_dpus * blocks_per_dpu * block_size <= 8) {
            if ((double)blocks_per_dpu * block_size <= 30720) {
              return 0.0416079 + -0.0008148323 * num_dpus +
                     -0.0001361892 * blocks_per_dpu +
                     -3.555694e-05 * block_size +
                     5.939898e-05 * (double)num_dpus * blocks_per_dpu +
                     5.349867e-06 * (double)num_dpus * block_size +
                     2.815822e-06 * (double)blocks_per_dpu * block_size +
                     1.367663e-10 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return 0.06689296 + -0.0009381655 * num_dpus +
                     -3.853199e-05 * blocks_per_dpu +
                     -1.054957e-05 * block_size +
                     7.35024e-05 * (double)num_dpus * blocks_per_dpu +
                     8.020174e-06 * (double)num_dpus * block_size +
                     1.460731e-06 * (double)blocks_per_dpu * block_size +
                     1.315089e-10 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          } else {
            if (num_dpus <= 15) {
              return 0.1682623 + -0.01839836 * num_dpus +
                     -6.169279e-05 * blocks_per_dpu +
                     -0.0002240118 * block_size +
                     0.0001010722 * (double)num_dpus * blocks_per_dpu +
                     5.055004e-05 * (double)num_dpus * block_size +
                     1.399035e-06 * (double)blocks_per_dpu * block_size +
                     7.253143e-10 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return -0.03108048 + -0.0069129 * num_dpus +
                     0.0005775777 * blocks_per_dpu +
                     -0.0004658476 * block_size +
                     5.476876e-05 * (double)num_dpus * blocks_per_dpu +
                     6.265096e-05 * (double)num_dpus * block_size +
                     5.216257e-06 * (double)blocks_per_dpu * block_size +
                     6.207861e-10 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          }
        }
      } else {
        if ((double)blocks_per_dpu * block_size <= 16384) {
          if (num_dpus <= 64) {
            if (block_size <= 32) {
              return 0.006675974 + 0.001307226 * num_dpus +
                     0.0004788367 * blocks_per_dpu + 0.001327651 * block_size +
                     3.35037e-05 * (double)num_dpus * blocks_per_dpu +
                     -3.072055e-05 * (double)num_dpus * block_size +
                     -5.934518e-08 * (double)blocks_per_dpu * block_size +
                     -1.483147e-10 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return 0.04568526 + 0.0008474144 * num_dpus +
                     -0.0001630695 * blocks_per_dpu +
                     -0.0001939991 * block_size +
                     3.179324e-05 * (double)num_dpus * blocks_per_dpu +
                     -8.013108e-06 * (double)num_dpus * block_size +
                     1.775333e-05 * (double)blocks_per_dpu * block_size +
                     1.721781e-10 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          } else {
            if (num_dpus <= 128) {
              return 0.02096802 + 0.001081429 * num_dpus +
                     0.0005090234 * blocks_per_dpu + 0.00276193 * block_size +
                     1.567513e-05 * (double)num_dpus * blocks_per_dpu +
                     -4.213849e-05 * (double)num_dpus * block_size +
                     1.85687e-05 * (double)blocks_per_dpu * block_size +
                     1.860272e-10 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return 0.14296 + -9.706421e-05 * num_dpus +
                     0.00190657 * blocks_per_dpu + -0.002294149 * block_size +
                     2.985226e-06 * (double)num_dpus * blocks_per_dpu +
                     1.387944e-05 * (double)num_dpus * block_size +
                     -5.180282e-07 * (double)blocks_per_dpu * block_size +
                     5.802528e-11 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          }
        } else {
          if (num_dpus <= 76) {
            if ((double)blocks_per_dpu * block_size <= 30720) {
              return -0.8480723 + 0.01237555 * num_dpus +
                     0.001353116 * blocks_per_dpu + 0.0006678186 * block_size +
                     1.880875e-06 * (double)num_dpus * blocks_per_dpu +
                     -2.479385e-06 * (double)num_dpus * block_size +
                     3.71554e-05 * (double)blocks_per_dpu * block_size +
                     3.929597e-10 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return 0.2880564 + 0.004123141 * num_dpus +
                     -3.09865e-05 * blocks_per_dpu + -0.00724423 * block_size +
                     1.829293e-05 * (double)num_dpus * blocks_per_dpu +
                     2.107177e-07 * (double)num_dpus * block_size +
                     3.957149e-05 * (double)blocks_per_dpu * block_size +
                     4.392397e-10 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          } else {
            if (blocks_per_dpu <= 2048) {
              return 3.22646 + -0.04520426 * num_dpus +
                     0.0003269082 * blocks_per_dpu + -0.09204438 * block_size +
                     2.102992e-05 * (double)num_dpus * blocks_per_dpu +
                     0.001107514 * (double)num_dpus * block_size +
                     5.344691e-05 * (double)blocks_per_dpu * block_size +
                     6.692797e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return -1.381104 + 0.002668501 * num_dpus +
                     0.002223413 * blocks_per_dpu + -0.03360272 * block_size +
                     4.007106e-06 * (double)num_dpus * blocks_per_dpu +
                     0.0003513977 * (double)num_dpus * block_size +
                     6.68209e-05 * (double)blocks_per_dpu * block_size +
                     6.694786e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          }
        }
      }
    } else {
      if ((double)num_dpus * block_size <= 74240) {
        if (num_dpus <= 36) {
          if (block_size <= 608) {
            if ((double)blocks_per_dpu * block_size <= 20736) {
              return 0.2304443 + -0.006516051 * num_dpus +
                     -0.005376543 * blocks_per_dpu +
                     -0.0006971405 * block_size +
                     0.0001864656 * (double)num_dpus * blocks_per_dpu +
                     1.788239e-05 * (double)num_dpus * block_size +
                     1.650305e-05 * (double)blocks_per_dpu * block_size +
                     7.950073e-10 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return 0.008172929 + 0.002143639 * num_dpus +
                     -3.870111e-05 * blocks_per_dpu +
                     -4.319082e-06 * block_size +
                     9.889072e-05 * (double)num_dpus * blocks_per_dpu +
                     4.698095e-06 * (double)num_dpus * block_size +
                     1.979261e-06 * (double)blocks_per_dpu * block_size +
                     6.1068e-10 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          } else {
            if (num_dpus <= 15) {
              return 0.01205051 + 0.006677141 * num_dpus +
                     0.001549377 * blocks_per_dpu + 1.85628e-05 * block_size +
                     4.431648e-09 * (double)num_dpus * blocks_per_dpu +
                     2.324869e-07 * (double)num_dpus * block_size +
                     7.082573e-07 * (double)blocks_per_dpu * block_size +
                     1.190937e-12 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return -0.1800953 + 0.01312958 * num_dpus +
                     0.0017601 * blocks_per_dpu + 7.927556e-05 * block_size +
                     3.09971e-07 * (double)num_dpus * blocks_per_dpu +
                     -3.291522e-06 * (double)num_dpus * block_size +
                     2.437617e-06 * (double)blocks_per_dpu * block_size +
                     7.532101e-12 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          }
        } else {
          if ((double)blocks_per_dpu * block_size <= 12288) {
            if ((double)blocks_per_dpu * block_size <= 8320) {
              return 0.02538615 + 1.759202e-05 * num_dpus +
                     0.002675906 * blocks_per_dpu + 0.0003762264 * block_size +
                     2.418778e-06 * (double)num_dpus * blocks_per_dpu +
                     4.585453e-06 * (double)num_dpus * block_size +
                     1.206277e-08 * (double)blocks_per_dpu * block_size +
                     1.512273e-11 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return -0.03975812 + 0.0006105327 * num_dpus +
                     0.0009585051 * blocks_per_dpu + -0.000535963 * block_size +
                     4.043588e-06 * (double)num_dpus * blocks_per_dpu +
                     1.380148e-06 * (double)num_dpus * block_size +
                     3.847921e-05 * (double)blocks_per_dpu * block_size +
                     1.181788e-09 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          } else {
            if (block_size <= 72) {
              return -0.917891 + -0.0012212 * num_dpus +
                     0.002637995 * blocks_per_dpu + 0.005873528 * block_size +
                     5.821892e-06 * (double)num_dpus * blocks_per_dpu +
                     3.432073e-05 * (double)num_dpus * block_size +
                     4.307641e-05 * (double)blocks_per_dpu * block_size +
                     4.871938e-09 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return 0.246118 + -0.001871527 * num_dpus +
                     0.00319019 * blocks_per_dpu + -0.0005115433 * block_size +
                     1.525108e-05 * (double)num_dpus * blocks_per_dpu +
                     8.396661e-06 * (double)num_dpus * block_size +
                     6.633367e-06 * (double)blocks_per_dpu * block_size +
                     1.779317e-10 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          }
        }
      } else {
        if ((double)num_dpus * blocks_per_dpu * block_size <= 3.42917e+07) {
          if ((double)blocks_per_dpu * block_size <= 30720) {
            if (block_size <= 104) {
              return -0.8221902 + 0.0001404152 * num_dpus +
                     0.009135809 * blocks_per_dpu + 0.006609229 * block_size +
                     3.140437e-06 * (double)num_dpus * blocks_per_dpu +
                     2.013825e-06 * (double)num_dpus * block_size +
                     3.722048e-08 * (double)blocks_per_dpu * block_size +
                     1.153949e-11 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return 0.01881123 + 3.033178e-05 * num_dpus +
                     0.006199704 * blocks_per_dpu + 0.0001653256 * block_size +
                     7.165194e-06 * (double)num_dpus * blocks_per_dpu +
                     1.274478e-06 * (double)num_dpus * block_size +
                     -1.885346e-08 * (double)blocks_per_dpu * block_size +
                     1.877193e-11 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          } else {
            if ((double)num_dpus * blocks_per_dpu * block_size <= 1.30007e+07) {
              return 0.6921968 + -0.00623205 * num_dpus +
                     0.002104064 * blocks_per_dpu + -0.0005834565 * block_size +
                     3.817235e-05 * (double)num_dpus * blocks_per_dpu +
                     6.279879e-06 * (double)num_dpus * block_size +
                     9.532769e-06 * (double)blocks_per_dpu * block_size +
                     2.688991e-10 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return -1.454536 + 0.001494844 * num_dpus +
                     0.000455868 * blocks_per_dpu + -0.0004567564 * block_size +
                     1.620818e-05 * (double)num_dpus * blocks_per_dpu +
                     2.019607e-06 * (double)num_dpus * block_size +
                     3.077273e-05 * (double)blocks_per_dpu * block_size +
                     1.661918e-09 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          }
        } else {
          if (num_dpus <= 512) {
            if (num_dpus <= 448) {
              return -6.910083 + 0.0131948 * num_dpus +
                     0.007580758 * blocks_per_dpu + 0.0008931678 * block_size +
                     -6.08624e-06 * (double)num_dpus * blocks_per_dpu +
                     -2.193542e-06 * (double)num_dpus * block_size +
                     1.410202e-05 * (double)blocks_per_dpu * block_size +
                     1.078319e-07 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return -6.191259 + 0.00908399 * num_dpus +
                     0.002415871 * blocks_per_dpu + 0.0001822843 * block_size +
                     7.409598e-06 * (double)num_dpus * blocks_per_dpu +
                     -2.918488e-07 * (double)num_dpus * block_size +
                     7.916387e-05 * (double)blocks_per_dpu * block_size +
                     9.43819e-09 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          } else {
            if (num_dpus <= 1792) {
              return -1.661118 + 0.0001992154 * num_dpus +
                     0.004868188 * blocks_per_dpu + -0.000251567 * block_size +
                     2.307643e-06 * (double)num_dpus * blocks_per_dpu +
                     4.378967e-07 * (double)num_dpus * block_size +
                     8.20124e-05 * (double)blocks_per_dpu * block_size +
                     2.2728e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return -9.629723 + 0.004976255 * num_dpus +
                     0.01397794 * blocks_per_dpu + -0.0036997 * block_size +
                     -2.181444e-06 * (double)num_dpus * blocks_per_dpu +
                     1.935051e-06 * (double)num_dpus * block_size +
                     6.691691e-05 * (double)blocks_per_dpu * block_size +
                     3.106506e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          }
        }
      }
    }
  }
}