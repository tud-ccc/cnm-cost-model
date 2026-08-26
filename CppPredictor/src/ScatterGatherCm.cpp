
#include <upmem_cost_model/ScatterGatherCm.h>

#ifndef MLPACK_NO_STD_COUT_PRINT
#define MLPACK_NO_STD_COUT_PRINT
#endif
#include <cmath> // std::log2, std::exp
#include <limits>
#include <mlpack.hpp>

double upmem_cm::gatherCostMs(int num_dpus, int block_size) {
  if (num_dpus <= 64) {
    if (block_size <= 304) {
      if (num_dpus <= 4) {
        if (num_dpus <= 2) {
          return 0.05906109 + -0.005850035 * num_dpus + -1.853638e-05 * block_size + 6.979553e-06 * (double)num_dpus * block_size;
        } else {
          return 0.05449871 + -0.003485829 * num_dpus + 2.729373e-05 * block_size + -1.01812e-05 * (double)num_dpus * block_size;
        }
      } else {
        if (num_dpus <= 5) {
          return 0.005902115 + 0.007535766 * num_dpus + -5.332459e-05 * block_size + 6.042861e-06 * (double)num_dpus * block_size;
        } else {
          return 0.04796294 + -7.181406e-06 * num_dpus + -1.934114e-05 * block_size + 1.291364e-06 * (double)num_dpus * block_size;
        }
      }
    } else {
      if (num_dpus <= 30) {
        if (block_size <= 1664) {
          return 0.04084651 + 6.976145e-05 * num_dpus + -1.1581e-06 * block_size + 1.211593e-06 * (double)num_dpus * block_size;
        } else {
          return 0.03896877 + 0.001109526 * num_dpus + 3.485088e-06 * block_size + 3.898829e-08 * (double)num_dpus * block_size;
        }
      } else {
        if (block_size <= 960) {
          return 0.04357821 + 4.877189e-05 * num_dpus + 2.131569e-05 * block_size + 6.11999e-07 * (double)num_dpus * block_size;
        } else {
          return 0.0944128 + -0.0003284394 * num_dpus + 4.848129e-06 * block_size + 6.068095e-08 * (double)num_dpus * block_size;
        }
      }
    }
  } else {
    if (block_size <= 512) {
      if (num_dpus <= 448) {
        if (num_dpus <= 304) {
          return 0.0723891 + 0.0001211922 * num_dpus + 5.258644e-05 * block_size + 4.783828e-07 * (double)num_dpus * block_size;
        } else {
          return -0.04099943 + 0.0004459722 * num_dpus + -0.0001353315 * block_size + 8.942948e-07 * (double)num_dpus * block_size;
        }
      } else {
        if (num_dpus <= 1472) {
          return 0.1393602 + 0.0001159842 * num_dpus + 0.0002382001 * block_size + 1.373358e-07 * (double)num_dpus * block_size;
        } else {
          return -0.1844995 + 0.0002724733 * num_dpus + 0.0004410875 * block_size + -5.799703e-08 * (double)num_dpus * block_size;
        }
      }
    } else {
      if (num_dpus <= 496) {
        if (num_dpus <= 384) {
          return 0.1147555 + 0.0002826808 * num_dpus + 7.52903e-06 * block_size + 2.280926e-08 * (double)num_dpus * block_size;
        } else {
          return -0.04596156 + 0.0007607857 * num_dpus + 8.316209e-05 * block_size + -1.449911e-07 * (double)num_dpus * block_size;
        }
      } else {
        if (num_dpus <= 768) {
          return 0.2163662 + 0.0002720945 * num_dpus + 5.862079e-05 * block_size + -6.275537e-08 * (double)num_dpus * block_size;
        } else {
          return 0.3595739 + 7.463725e-05 * num_dpus + 3.252239e-05 * block_size + 6.70257e-09 * (double)num_dpus * block_size;
        }
      }
    }
  }
}
double upmem_cm::broadcastCostMs(int num_dpus, int block_size) {
  if (num_dpus <= 64) {
    if (block_size <= 384) {
      if (num_dpus <= 2) {
        return 0.03322624 + 0.007401036 * num_dpus + 1.352941e-06 * block_size + 6.764513e-07 * (double)num_dpus * block_size;
      } else {
        if (num_dpus <= 4) {
          return 0.04077993 + 2.271571e-05 * num_dpus + 3.838972e-06 * block_size + -4.488136e-06 * (double)num_dpus * block_size;
        } else {
          return 0.04135465 + 1.4003e-06 * num_dpus + -1.364451e-05 * block_size + 1.048641e-06 * (double)num_dpus * block_size;
        }
      }
    } else {
      if (block_size <= 960) {
        if (num_dpus <= 2) {
          return 0.04571901 + -0.003334344 * num_dpus + -1.982215e-07 * block_size + 2.828561e-06 * (double)num_dpus * block_size;
        } else {
          return 0.03556956 + 2.36184e-05 * num_dpus + 1.079673e-06 * block_size + 1.058144e-06 * (double)num_dpus * block_size;
        }
      } else {
        if (num_dpus <= 15) {
          return 0.03695837 + 0.0007127151 * num_dpus + 3.616986e-06 * block_size + -1.856278e-08 * (double)num_dpus * block_size;
        } else {
          return 0.05637778 + 0.0002646048 * num_dpus + 8.914184e-07 * block_size + 1.222033e-07 * (double)num_dpus * block_size;
        }
      }
    }
  } else {
    if (block_size <= 464) {
      if (num_dpus <= 384) {
        if (num_dpus <= 76) {
          return 0.03435763 + 0.0005153315 * num_dpus + 5.807842e-05 * block_size + 2.62075e-07 * (double)num_dpus * block_size;
        } else {
          return 0.06110197 + 7.759505e-05 * num_dpus + 0.0001045639 * block_size + 2.339713e-08 * (double)num_dpus * block_size;
        }
      } else {
        if (num_dpus <= 448) {
          return 0.3809485 + -0.0005839684 * num_dpus + -0.0006538199 * block_size + 1.893886e-06 * (double)num_dpus * block_size;
        } else {
          return 0.1155487 + 8.663507e-05 * num_dpus + 0.0001905862 * block_size + 6.005302e-08 * (double)num_dpus * block_size;
        }
      }
    } else {
      if (num_dpus <= 384) {
        if (num_dpus <= 136) {
          return 0.09307586 + 0.0003332122 * num_dpus + 1.355813e-05 * block_size + -4.401865e-08 * (double)num_dpus * block_size;
        } else {
          return 0.09902251 + 0.0001547943 * num_dpus + 6.165412e-06 * block_size + 1.189107e-08 * (double)num_dpus * block_size;
        }
      } else {
        if (num_dpus <= 448) {
          return -0.07003742 + 0.0006553852 * num_dpus + 2.995321e-05 * block_size + -4.799794e-08 * (double)num_dpus * block_size;
        } else {
          return 0.2186063 + 0.0001052928 * num_dpus + -3.423715e-06 * block_size + 2.406086e-08 * (double)num_dpus * block_size;
        }
      }
    }
  }

}

double upmem_cm::scatterBlockCostMs(int num_dpus, int block_size) {
  if (num_dpus <= 64) {
    if (block_size <= 416) {
      if (num_dpus <= 2) {
        return 0.04464552 + 0.0002555342 * num_dpus + 1.020002e-05 * block_size + -5.063032e-06 * (double)num_dpus * block_size;
      } else {
        if (num_dpus <= 4) {
          return 0.0233827 + 0.00704713 * num_dpus + -2.459798e-06 * block_size + 2.897615e-06 * (double)num_dpus * block_size;
        } else {
          return 0.04714619 + -4.229908e-05 * num_dpus + 2.01544e-06 * block_size + 2.235611e-06 * (double)num_dpus * block_size;
        }
      }
    } else {
      if (num_dpus <= 15) {
        if (num_dpus <= 2) {
          return 0.04771209 + -0.0004835731 * num_dpus + 8.273215e-06 * block_size + -4.819548e-08 * (double)num_dpus * block_size;
        } else {
          return 0.04032868 + 0.001928076 * num_dpus + 9.482876e-06 * block_size + -2.794943e-07 * (double)num_dpus * block_size;
        }
      } else {
        if (block_size <= 1984) {
          return 0.02402642 + 0.001299618 * num_dpus + 6.636813e-05 * block_size + -6.05774e-07 * (double)num_dpus * block_size;
        } else {
          return 0.05421051 + 0.0007071535 * num_dpus + 9.584342e-06 * block_size + 1.163743e-07 * (double)num_dpus * block_size;
        }
      }
    }
  } else {
    if (block_size <= 448) {
      if (num_dpus <= 416) {
        if (num_dpus <= 232) {
          return 0.05326328 + 0.0002200965 * num_dpus + 0.0001296131 * block_size + 8.518087e-07 * (double)num_dpus * block_size;
        } else {
          return 0.03657578 + 0.0002272072 * num_dpus + 8.807599e-05 * block_size + 8.190519e-07 * (double)num_dpus * block_size;
        }
      } else {
        if (num_dpus <= 1216) {
          return 0.1322513 + 0.0001052422 * num_dpus + 0.000418701 * block_size + 4.063692e-07 * (double)num_dpus * block_size;
        } else {
          return 0.06307513 + 0.0001403594 * num_dpus + 0.0004291426 * block_size + 2.014805e-07 * (double)num_dpus * block_size;
        }
      }
    } else {
      if (num_dpus <= 448) {
        if (block_size <= 960) {
          return 0.07511821 + 0.0003708258 * num_dpus + 0.000113876 * block_size + 1.89999e-07 * (double)num_dpus * block_size;
        } else {
          return 0.1608596 + 0.0003215246 * num_dpus + 8.768438e-06 * block_size + 6.608216e-08 * (double)num_dpus * block_size;
        }
      } else {
        if (num_dpus <= 1664) {
          return 0.3298322 + 0.0002198918 * num_dpus + 2.887847e-05 * block_size + 2.724243e-08 * (double)num_dpus * block_size;
        } else {
          return -0.6753042 + 0.0007053253 * num_dpus + -0.0003571449 * block_size + 2.428582e-07 * (double)num_dpus * block_size;
        }
      }
    }
  }
}

double upmem_cm::scatterSgCostMs(int num_dpus, int block_size,
                                 int blocks_per_dpu) {
  if (blocks_per_dpu <= 28) {
    if (blocks_per_dpu <= 7) {
      if ((double)num_dpus * block_size <= 124416) {
        if (block_size <= 864) {
          if (num_dpus <= 60) {
            if (num_dpus <= 2) {
              return 0.04965499 + -0.003670288 * num_dpus + 0.001053712 * blocks_per_dpu + -5.900087e-06 * block_size + -8.107639e-09 * (double)num_dpus * blocks_per_dpu + 5.830334e-06 * (double)num_dpus * block_size + 1.661055e-06 * (double)blocks_per_dpu * block_size + 1.986024e-12 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.0437957 + 0.0004193133 * num_dpus + 0.0006825864 * blocks_per_dpu + 1.007736e-05 * block_size + 2.407493e-09 * (double)num_dpus * blocks_per_dpu + 1.347219e-06 * (double)num_dpus * block_size + 2.959965e-09 * (double)blocks_per_dpu * block_size + 3.509235e-13 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (num_dpus <= 384) {
              return 0.08723239 + 2.429084e-05 * num_dpus + 0.002474669 * blocks_per_dpu + 9.399932e-05 * block_size + 9.47437e-08 * (double)num_dpus * blocks_per_dpu + 6.644004e-07 * (double)num_dpus * block_size + 7.215478e-09 * (double)blocks_per_dpu * block_size + 4.229009e-13 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.09152196 + 8.650534e-05 * num_dpus + 0.001776923 * blocks_per_dpu + 0.0001728379 * block_size + 2.974378e-07 * (double)num_dpus * blocks_per_dpu + 1.013569e-06 * (double)num_dpus * block_size + 2.330795e-09 * (double)blocks_per_dpu * block_size + 5.971599e-13 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        } else {
          if (blocks_per_dpu <= 2) {
            if (blocks_per_dpu <= 1) {
              return 0.04297853 + 0.00146397 * num_dpus + -3.897185e-10 * blocks_per_dpu + 3.59448e-06 * block_size + 8.725941e-11 * (double)num_dpus * blocks_per_dpu + 3.88522e-09 * (double)num_dpus * block_size + 2.142293e-13 * (double)blocks_per_dpu * block_size + 2.315767e-16 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.04228036 + 0.001059999 * num_dpus + -1.109584e-10 * blocks_per_dpu + 6.374582e-06 * block_size + 1.263615e-10 * (double)num_dpus * blocks_per_dpu + 3.489056e-07 * (double)num_dpus * block_size + 7.599172e-13 * (double)blocks_per_dpu * block_size + 4.15928e-14 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if ((double)num_dpus * block_size <= 22848) {
              return 0.0386862 + -0.0002501491 * num_dpus + 0.002687461 * blocks_per_dpu + 6.659121e-07 * block_size + -5.669642e-08 * (double)num_dpus * blocks_per_dpu + 1.597155e-06 * (double)num_dpus * block_size + 2.05359e-06 * (double)blocks_per_dpu * block_size + 8.212567e-13 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.005067751 + 0.001117995 * num_dpus + 0.01049384 * blocks_per_dpu + 7.180006e-06 * block_size + 6.77563e-08 * (double)num_dpus * blocks_per_dpu + 7.135381e-07 * (double)num_dpus * block_size + 4.147913e-07 * (double)blocks_per_dpu * block_size + 2.255221e-12 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        }
      } else {
        if (blocks_per_dpu <= 2) {
          if (blocks_per_dpu <= 1) {
            if (num_dpus <= 64) {
              return 0.08061349 + -9.527868e-05 * num_dpus + -2.164597e-10 * blocks_per_dpu + -8.238467e-08 * block_size + -5.67912e-12 * (double)num_dpus * blocks_per_dpu + 2.909739e-07 * (double)num_dpus * block_size + -4.917805e-15 * (double)blocks_per_dpu * block_size + 1.734339e-14 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.1569549 + 0.0001730293 * num_dpus + -1.455192e-11 * blocks_per_dpu + 5.248311e-06 * block_size + 1.031334e-11 * (double)num_dpus * blocks_per_dpu + 5.178748e-08 * (double)num_dpus * block_size + 3.128239e-13 * (double)blocks_per_dpu * block_size + 3.086775e-15 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (num_dpus <= 400) {
              return 0.1934051 + 5.888737e-05 * num_dpus + -1.360822e-07 * blocks_per_dpu + -1.214991e-06 * block_size + -2.256267e-12 * (double)num_dpus * blocks_per_dpu + 1.266178e-07 * (double)num_dpus * block_size + -3.96659e-12 * (double)blocks_per_dpu * block_size + 1.810235e-14 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.2496154 + 0.0001417385 * num_dpus + -8.498318e-09 * blocks_per_dpu + 2.32204e-05 * block_size + 1.309036e-11 * (double)num_dpus * blocks_per_dpu + 8.162154e-08 * (double)num_dpus * block_size + 3.228451e-12 * (double)blocks_per_dpu * block_size + 1.157068e-14 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        } else {
          if (blocks_per_dpu <= 4) {
            if (num_dpus <= 400) {
              return 0.2430537 + -0.0001273727 * num_dpus + -1.008666e-07 * blocks_per_dpu + -2.850442e-06 * block_size + -4.21447e-11 * (double)num_dpus * blocks_per_dpu + 2.167949e-07 * (double)num_dpus * block_size + -2.604632e-12 * (double)blocks_per_dpu * block_size + 5.743539e-14 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.2518245 + 0.0001392849 * num_dpus + -3.783498e-08 * blocks_per_dpu + 4.537708e-05 * block_size + 2.230413e-11 * (double)num_dpus * blocks_per_dpu + 1.496661e-07 * (double)num_dpus * block_size + 9.852372e-12 * (double)blocks_per_dpu * block_size + 3.907248e-14 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (num_dpus <= 152) {
              return 0.04185468 + -0.0001387972 * num_dpus + 0.01881541 * blocks_per_dpu + -1.647913e-05 * block_size + 2.935839e-07 * (double)num_dpus * blocks_per_dpu + 8.647062e-07 * (double)num_dpus * block_size + 1.227894e-06 * (double)blocks_per_dpu * block_size + 1.582217e-11 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.1727637 + 0.0001951699 * num_dpus + -7.133349e-08 * blocks_per_dpu + 5.089706e-06 * block_size + 2.834559e-11 * (double)num_dpus * blocks_per_dpu + 3.091922e-07 * (double)num_dpus * block_size + 2.019869e-12 * (double)blocks_per_dpu * block_size + 1.258603e-13 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        }
      }
    } else {
      if (num_dpus <= 64) {
        if ((double)num_dpus * blocks_per_dpu * block_size <= 8) {
          if (num_dpus <= 9) {
            if (block_size <= 464) {
              return 0.04375141 + 0.0007229678 * num_dpus + 0.0001042808 * blocks_per_dpu + -6.463259e-06 * block_size + -2.619496e-08 * (double)num_dpus * blocks_per_dpu + 1.925938e-06 * (double)num_dpus * block_size + 1.536526e-06 * (double)blocks_per_dpu * block_size + 3.945162e-12 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.04848711 + -0.001312959 * num_dpus + 0.0005114254 * blocks_per_dpu + -3.076828e-06 * block_size + -5.310129e-08 * (double)num_dpus * blocks_per_dpu + 4.459467e-06 * (double)num_dpus * block_size + 1.493666e-06 * (double)blocks_per_dpu * block_size + 3.89848e-12 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (num_dpus <= 31) {
              return 0.01875428 + 0.001252417 * num_dpus + 0.0008274112 * blocks_per_dpu + -4.381348e-06 * block_size + -2.461546e-07 * (double)num_dpus * blocks_per_dpu + 1.783022e-06 * (double)num_dpus * block_size + 2.405025e-06 * (double)blocks_per_dpu * block_size + 1.02899e-11 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.0002498552 + 0.001034852 * num_dpus + 0.002126767 * blocks_per_dpu + 0.0001604209 * block_size + -1.441912e-06 * (double)num_dpus * blocks_per_dpu + -2.478939e-06 * (double)num_dpus * block_size + 7.716031e-06 * (double)blocks_per_dpu * block_size + 7.816931e-11 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        } else {
          if (num_dpus <= 31) {
            if (num_dpus <= 15) {
              return 0.153046 + -0.004468206 * num_dpus + 0.001302686 * blocks_per_dpu + -6.313765e-06 * block_size + -2.210245e-08 * (double)num_dpus * blocks_per_dpu + 1.203361e-06 * (double)num_dpus * block_size + 8.470552e-07 * (double)blocks_per_dpu * block_size + 2.58548e-12 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.07153928 + 0.001822531 * num_dpus + 0.0009368201 * blocks_per_dpu + -7.9802e-06 * block_size + -1.734764e-08 * (double)num_dpus * blocks_per_dpu + 1.566594e-07 * (double)num_dpus * block_size + 3.164163e-06 * (double)blocks_per_dpu * block_size + 1.831985e-11 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (block_size <= 704) {
              return 0.372591 + -0.006104519 * num_dpus + -0.00538161 * blocks_per_dpu + -0.0004263711 * block_size + 0.0001452134 * (double)num_dpus * blocks_per_dpu + 8.62762e-06 * (double)num_dpus * block_size + 1.347412e-05 * (double)blocks_per_dpu * block_size + 1.291319e-09 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.04219273 + 0.001863924 * num_dpus + 0.001957968 * blocks_per_dpu + 2.04117e-06 * block_size + 4.886727e-08 * (double)num_dpus * blocks_per_dpu + -1.971097e-07 * (double)num_dpus * block_size + 6.525981e-06 * (double)blocks_per_dpu * block_size + 7.59774e-11 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        }
      } else {
        if (blocks_per_dpu <= 16) {
          if ((double)num_dpus * blocks_per_dpu * block_size <= 2.60014e+07) {
            if (block_size <= 240) {
              return 0.0477325 + 9.515874e-05 * num_dpus + 0.004531308 * blocks_per_dpu + 9.346134e-05 * block_size + 3.199566e-07 * (double)num_dpus * blocks_per_dpu + 1.324917e-06 * (double)num_dpus * block_size + 7.106459e-09 * (double)blocks_per_dpu * block_size + 1.467178e-12 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -0.07841679 + 0.0003458657 * num_dpus + 0.01700513 * blocks_per_dpu + 0.0001355209 * block_size + 1.110991e-06 * (double)num_dpus * blocks_per_dpu + 1.872361e-07 * (double)num_dpus * block_size + 2.263335e-07 * (double)blocks_per_dpu * block_size + 1.355734e-11 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (num_dpus <= 832) {
              return -8.028829 + 0.0009876653 * num_dpus + 0.3272963 * blocks_per_dpu + -8.027658e-05 * block_size + 5.041896e-05 * (double)num_dpus * blocks_per_dpu + 1.870453e-06 * (double)num_dpus * block_size + 2.0103e-05 * (double)blocks_per_dpu * block_size + 2.984776e-09 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -7.528921 + 0.004021707 * num_dpus + 0.2473772 * blocks_per_dpu + 0.000497704 * block_size + -0.0001363306 * (double)num_dpus * blocks_per_dpu + -2.391236e-07 * (double)num_dpus * block_size + 8.05637e-05 * (double)blocks_per_dpu * block_size + 1.898449e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        } else {
          if ((double)num_dpus * blocks_per_dpu * block_size <= 3.42917e+07) {
            if (block_size <= 152) {
              return 0.06831001 + 1.743654e-05 * num_dpus + 0.002817065 * blocks_per_dpu + 4.779292e-05 * block_size + 3.890323e-06 * (double)num_dpus * blocks_per_dpu + 2.033928e-06 * (double)num_dpus * block_size + 2.259282e-10 * (double)blocks_per_dpu * block_size + 6.49514e-12 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -0.1477483 + 0.0003542552 * num_dpus + 0.01245742 * blocks_per_dpu + 0.0002723254 * block_size + 8.30225e-07 * (double)num_dpus * blocks_per_dpu + 4.642034e-07 * (double)num_dpus * block_size + 9.890319e-08 * (double)blocks_per_dpu * block_size + 6.677071e-12 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (num_dpus <= 832) {
              return 7.348664 + -0.01561046 * num_dpus + -0.3022083 * blocks_per_dpu + -0.002131235 * block_size + 0.0005026446 * (double)num_dpus * blocks_per_dpu + 3.695101e-06 * (double)num_dpus * block_size + 6.981089e-05 * (double)blocks_per_dpu * block_size + 3.774697e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -6.936147 + 0.003565175 * num_dpus + 0.07623959 * blocks_per_dpu + 0.0009152949 * block_size + -4.307628e-05 * (double)num_dpus * blocks_per_dpu + -6.404365e-07 * (double)num_dpus * block_size + 8.739657e-05 * (double)blocks_per_dpu * block_size + 2.854722e-08 * (double)num_dpus * blocks_per_dpu * block_size;
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
              return 0.04465673 + 0.0004289939 * num_dpus + 6.787333e-06 * blocks_per_dpu + -4.153357e-05 * block_size + 4.270446e-05 * (double)num_dpus * blocks_per_dpu + -2.656303e-06 * (double)num_dpus * block_size + 1.78978e-06 * (double)blocks_per_dpu * block_size + 2.128917e-11 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.03586535 + 0.0004131331 * num_dpus + -1.920302e-05 * blocks_per_dpu + -0.0001689927 * block_size + 4.628022e-05 * (double)num_dpus * blocks_per_dpu + -3.757006e-07 * (double)num_dpus * block_size + 3.801863e-06 * (double)blocks_per_dpu * block_size + 2.048757e-11 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (num_dpus <= 15) {
              return 0.08550912 + 0.005963362 * num_dpus + 1.135343e-05 * blocks_per_dpu + -0.001480927 * block_size + 2.229867e-05 * (double)num_dpus * blocks_per_dpu + -1.081709e-07 * (double)num_dpus * block_size + 7.906323e-06 * (double)blocks_per_dpu * block_size + 2.865855e-11 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.1218897 + 0.007917561 * num_dpus + 0.0001997091 * blocks_per_dpu + -0.004348334 * block_size + 1.138882e-05 * (double)num_dpus * blocks_per_dpu + -1.283282e-07 * (double)num_dpus * block_size + 1.8847e-05 * (double)blocks_per_dpu * block_size + 1.145871e-10 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        } else {
          if ((double)num_dpus * blocks_per_dpu * block_size <= 8) {
            if ((double)blocks_per_dpu * block_size <= 30720) {
              return 0.0416079 + -0.0008148323 * num_dpus + -0.0001361892 * blocks_per_dpu + -3.555694e-05 * block_size + 5.939898e-05 * (double)num_dpus * blocks_per_dpu + 5.349867e-06 * (double)num_dpus * block_size + 2.815822e-06 * (double)blocks_per_dpu * block_size + 1.367663e-10 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.06689296 + -0.0009381655 * num_dpus + -3.853199e-05 * blocks_per_dpu + -1.054957e-05 * block_size + 7.35024e-05 * (double)num_dpus * blocks_per_dpu + 8.020174e-06 * (double)num_dpus * block_size + 1.460731e-06 * (double)blocks_per_dpu * block_size + 1.315089e-10 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (num_dpus <= 15) {
              return 0.1682623 + -0.01839836 * num_dpus + -6.169279e-05 * blocks_per_dpu + -0.0002240118 * block_size + 0.0001010722 * (double)num_dpus * blocks_per_dpu + 5.055004e-05 * (double)num_dpus * block_size + 1.399035e-06 * (double)blocks_per_dpu * block_size + 7.253143e-10 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -0.03108048 + -0.0069129 * num_dpus + 0.0005775777 * blocks_per_dpu + -0.0004658476 * block_size + 5.476876e-05 * (double)num_dpus * blocks_per_dpu + 6.265096e-05 * (double)num_dpus * block_size + 5.216257e-06 * (double)blocks_per_dpu * block_size + 6.207861e-10 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        }
      } else {
        if ((double)blocks_per_dpu * block_size <= 16384) {
          if (num_dpus <= 64) {
            if (block_size <= 32) {
              return 0.006675974 + 0.001307226 * num_dpus + 0.0004788367 * blocks_per_dpu + 0.001327651 * block_size + 3.35037e-05 * (double)num_dpus * blocks_per_dpu + -3.072055e-05 * (double)num_dpus * block_size + -5.934518e-08 * (double)blocks_per_dpu * block_size + -1.483147e-10 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.04568526 + 0.0008474144 * num_dpus + -0.0001630695 * blocks_per_dpu + -0.0001939991 * block_size + 3.179324e-05 * (double)num_dpus * blocks_per_dpu + -8.013108e-06 * (double)num_dpus * block_size + 1.775333e-05 * (double)blocks_per_dpu * block_size + 1.721781e-10 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (num_dpus <= 128) {
              return 0.02096802 + 0.001081429 * num_dpus + 0.0005090234 * blocks_per_dpu + 0.00276193 * block_size + 1.567513e-05 * (double)num_dpus * blocks_per_dpu + -4.213849e-05 * (double)num_dpus * block_size + 1.85687e-05 * (double)blocks_per_dpu * block_size + 1.860272e-10 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.14296 + -9.706421e-05 * num_dpus + 0.00190657 * blocks_per_dpu + -0.002294149 * block_size + 2.985226e-06 * (double)num_dpus * blocks_per_dpu + 1.387944e-05 * (double)num_dpus * block_size + -5.180282e-07 * (double)blocks_per_dpu * block_size + 5.802528e-11 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        } else {
          if (num_dpus <= 76) {
            if ((double)blocks_per_dpu * block_size <= 30720) {
              return -0.8480723 + 0.01237555 * num_dpus + 0.001353116 * blocks_per_dpu + 0.0006678186 * block_size + 1.880875e-06 * (double)num_dpus * blocks_per_dpu + -2.479385e-06 * (double)num_dpus * block_size + 3.71554e-05 * (double)blocks_per_dpu * block_size + 3.929597e-10 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.2880564 + 0.004123141 * num_dpus + -3.09865e-05 * blocks_per_dpu + -0.00724423 * block_size + 1.829293e-05 * (double)num_dpus * blocks_per_dpu + 2.107177e-07 * (double)num_dpus * block_size + 3.957149e-05 * (double)blocks_per_dpu * block_size + 4.392397e-10 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (blocks_per_dpu <= 2048) {
              return 3.22646 + -0.04520426 * num_dpus + 0.0003269082 * blocks_per_dpu + -0.09204438 * block_size + 2.102992e-05 * (double)num_dpus * blocks_per_dpu + 0.001107514 * (double)num_dpus * block_size + 5.344691e-05 * (double)blocks_per_dpu * block_size + 6.692797e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -1.381104 + 0.002668501 * num_dpus + 0.002223413 * blocks_per_dpu + -0.03360272 * block_size + 4.007106e-06 * (double)num_dpus * blocks_per_dpu + 0.0003513977 * (double)num_dpus * block_size + 6.68209e-05 * (double)blocks_per_dpu * block_size + 6.694786e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        }
      }
    } else {
      if ((double)num_dpus * block_size <= 74240) {
        if (num_dpus <= 36) {
          if (block_size <= 608) {
            if ((double)blocks_per_dpu * block_size <= 20736) {
              return 0.2304443 + -0.006516051 * num_dpus + -0.005376543 * blocks_per_dpu + -0.0006971405 * block_size + 0.0001864656 * (double)num_dpus * blocks_per_dpu + 1.788239e-05 * (double)num_dpus * block_size + 1.650305e-05 * (double)blocks_per_dpu * block_size + 7.950073e-10 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.008172929 + 0.002143639 * num_dpus + -3.870111e-05 * blocks_per_dpu + -4.319082e-06 * block_size + 9.889072e-05 * (double)num_dpus * blocks_per_dpu + 4.698095e-06 * (double)num_dpus * block_size + 1.979261e-06 * (double)blocks_per_dpu * block_size + 6.1068e-10 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (num_dpus <= 15) {
              return 0.01205051 + 0.006677141 * num_dpus + 0.001549377 * blocks_per_dpu + 1.85628e-05 * block_size + 4.431648e-09 * (double)num_dpus * blocks_per_dpu + 2.324869e-07 * (double)num_dpus * block_size + 7.082573e-07 * (double)blocks_per_dpu * block_size + 1.190937e-12 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -0.1800953 + 0.01312958 * num_dpus + 0.0017601 * blocks_per_dpu + 7.927556e-05 * block_size + 3.09971e-07 * (double)num_dpus * blocks_per_dpu + -3.291522e-06 * (double)num_dpus * block_size + 2.437617e-06 * (double)blocks_per_dpu * block_size + 7.532101e-12 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        } else {
          if ((double)blocks_per_dpu * block_size <= 12288) {
            if ((double)blocks_per_dpu * block_size <= 8320) {
              return 0.02538615 + 1.759202e-05 * num_dpus + 0.002675906 * blocks_per_dpu + 0.0003762264 * block_size + 2.418778e-06 * (double)num_dpus * blocks_per_dpu + 4.585453e-06 * (double)num_dpus * block_size + 1.206277e-08 * (double)blocks_per_dpu * block_size + 1.512273e-11 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -0.03975812 + 0.0006105327 * num_dpus + 0.0009585051 * blocks_per_dpu + -0.000535963 * block_size + 4.043588e-06 * (double)num_dpus * blocks_per_dpu + 1.380148e-06 * (double)num_dpus * block_size + 3.847921e-05 * (double)blocks_per_dpu * block_size + 1.181788e-09 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (block_size <= 72) {
              return -0.917891 + -0.0012212 * num_dpus + 0.002637995 * blocks_per_dpu + 0.005873528 * block_size + 5.821892e-06 * (double)num_dpus * blocks_per_dpu + 3.432073e-05 * (double)num_dpus * block_size + 4.307641e-05 * (double)blocks_per_dpu * block_size + 4.871938e-09 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.246118 + -0.001871527 * num_dpus + 0.00319019 * blocks_per_dpu + -0.0005115433 * block_size + 1.525108e-05 * (double)num_dpus * blocks_per_dpu + 8.396661e-06 * (double)num_dpus * block_size + 6.633367e-06 * (double)blocks_per_dpu * block_size + 1.779317e-10 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        }
      } else {
        if ((double)num_dpus * blocks_per_dpu * block_size <= 3.42917e+07) {
          if ((double)blocks_per_dpu * block_size <= 30720) {
            if (block_size <= 104) {
              return -0.8221902 + 0.0001404152 * num_dpus + 0.009135809 * blocks_per_dpu + 0.006609229 * block_size + 3.140437e-06 * (double)num_dpus * blocks_per_dpu + 2.013825e-06 * (double)num_dpus * block_size + 3.722048e-08 * (double)blocks_per_dpu * block_size + 1.153949e-11 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.01881123 + 3.033178e-05 * num_dpus + 0.006199704 * blocks_per_dpu + 0.0001653256 * block_size + 7.165194e-06 * (double)num_dpus * blocks_per_dpu + 1.274478e-06 * (double)num_dpus * block_size + -1.885346e-08 * (double)blocks_per_dpu * block_size + 1.877193e-11 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if ((double)num_dpus * blocks_per_dpu * block_size <= 1.30007e+07) {
              return 0.6921968 + -0.00623205 * num_dpus + 0.002104064 * blocks_per_dpu + -0.0005834565 * block_size + 3.817235e-05 * (double)num_dpus * blocks_per_dpu + 6.279879e-06 * (double)num_dpus * block_size + 9.532769e-06 * (double)blocks_per_dpu * block_size + 2.688991e-10 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -1.454536 + 0.001494844 * num_dpus + 0.000455868 * blocks_per_dpu + -0.0004567564 * block_size + 1.620818e-05 * (double)num_dpus * blocks_per_dpu + 2.019607e-06 * (double)num_dpus * block_size + 3.077273e-05 * (double)blocks_per_dpu * block_size + 1.661918e-09 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        } else {
          if (num_dpus <= 512) {
            if (num_dpus <= 448) {
              return -6.910083 + 0.0131948 * num_dpus + 0.007580758 * blocks_per_dpu + 0.0008931678 * block_size + -6.08624e-06 * (double)num_dpus * blocks_per_dpu + -2.193542e-06 * (double)num_dpus * block_size + 1.410202e-05 * (double)blocks_per_dpu * block_size + 1.078319e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -6.191259 + 0.00908399 * num_dpus + 0.002415871 * blocks_per_dpu + 0.0001822843 * block_size + 7.409598e-06 * (double)num_dpus * blocks_per_dpu + -2.918488e-07 * (double)num_dpus * block_size + 7.916387e-05 * (double)blocks_per_dpu * block_size + 9.43819e-09 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (num_dpus <= 1792) {
              return -1.661118 + 0.0001992154 * num_dpus + 0.004868188 * blocks_per_dpu + -0.000251567 * block_size + 2.307643e-06 * (double)num_dpus * blocks_per_dpu + 4.378967e-07 * (double)num_dpus * block_size + 8.20124e-05 * (double)blocks_per_dpu * block_size + 2.2728e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -9.629723 + 0.004976255 * num_dpus + 0.01397794 * blocks_per_dpu + -0.0036997 * block_size + -2.181444e-06 * (double)num_dpus * blocks_per_dpu + 1.935051e-06 * (double)num_dpus * block_size + 6.691691e-05 * (double)blocks_per_dpu * block_size + 3.106506e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        }
      }
    }
  }
}