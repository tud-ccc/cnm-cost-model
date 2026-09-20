
#include <upmem_cost_model/ScatterGatherCm.h>

#include <cmath> // std::log2, std::exp
#include <limits>

double upmem_cm::gatherCostMs(int num_dpus, int block_size) {
  if (num_dpus <= 448) {
    if (num_dpus <= 64) {
      if (num_dpus <= 31) {
        if (block_size <= 1856) {
          if (num_dpus <= 2) {
            return 0.02248676 + -0.0003508566 * num_dpus +
                   2.221183e-06 * block_size +
                   6.191067e-07 * (double)num_dpus * block_size;
          } else {
            return 0.02349029 + -2.3259e-05 * num_dpus +
                   -4.552319e-06 * block_size +
                   1.251196e-06 * (double)num_dpus * block_size;
          }
        } else {
          if (num_dpus <= 15) {
            return 0.02229384 + 0.0001203107 * num_dpus +
                   4.195826e-06 * block_size +
                   1.92705e-08 * (double)num_dpus * block_size;
          } else {
            return 0.01919509 + 0.0001589527 * num_dpus +
                   8.147421e-06 * block_size +
                   1.810556e-08 * (double)num_dpus * block_size;
          }
        }
      } else {
        if (block_size <= 992) {
          if (num_dpus <= 36) {
            return -0.00188905 + 0.0008006234 * num_dpus +
                   6.456045e-06 * block_size +
                   9.672808e-07 * (double)num_dpus * block_size;
          } else {
            return 0.01957532 + 0.0001254547 * num_dpus +
                   5.627007e-06 * block_size +
                   9.606898e-07 * (double)num_dpus * block_size;
          }
        } else {
          if (block_size <= 221184) {
            return 0.03073515 + 0.0001389165 * num_dpus +
                   1.519047e-05 * block_size +
                   1.932547e-08 * (double)num_dpus * block_size;
          } else {
            return -1.219345 + -0.03046805 * num_dpus +
                   1.785783e-05 * block_size +
                   1.953265e-07 * (double)num_dpus * block_size;
          }
        }
      }
    } else {
      if (block_size <= 57344) {
        if (block_size <= 992) {
          if (num_dpus <= 128) {
            return 0.03507364 + 5.127627e-05 * num_dpus +
                   2.994784e-06 * block_size +
                   1.014561e-06 * (double)num_dpus * block_size;
          } else {
            return 0.02374975 + 0.0001142556 * num_dpus +
                   7.122531e-05 * block_size +
                   2.181334e-07 * (double)num_dpus * block_size;
          }
        } else {
          if (num_dpus <= 384) {
            return 0.04942478 + 0.0001889709 * num_dpus +
                   2.754571e-05 * block_size +
                   2.974381e-08 * (double)num_dpus * block_size;
          } else {
            return -0.2161405 + 0.0008229727 * num_dpus +
                   0.0001751945 * block_size +
                   -2.796378e-07 * (double)num_dpus * block_size;
          }
        }
      } else {
        if (num_dpus <= 384) {
          if (block_size <= 131072) {
            return 0.6637976 + -0.007315668 * num_dpus +
                   1.868633e-05 * block_size +
                   1.441839e-07 * (double)num_dpus * block_size;
          } else {
            return -4.578747 + 0.01113985 * num_dpus +
                   5.33386e-05 * block_size +
                   3.324201e-08 * (double)num_dpus * block_size;
          }
        } else {
          if (block_size <= 114688) {
            return -4.152264 + 0.003015803 * num_dpus +
                   9.675287e-05 * block_size +
                   2.890901e-08 * (double)num_dpus * block_size;
          } else {
            return 2.609453 + -0.008847785 * num_dpus +
                   6.08022e-05 * block_size +
                   7.992529e-08 * (double)num_dpus * block_size;
          }
        }
      }
    }
  } else {
    if (block_size <= 27648) {
      if (block_size <= 992) {
        if (num_dpus <= 1792) {
          if (num_dpus <= 512) {
            return 0.03162742 + 0.0001273981 * num_dpus +
                   0.0001637757 * block_size +
                   1.555806e-07 * (double)num_dpus * block_size;
          } else {
            return 0.0757526 + 5.616628e-05 * num_dpus +
                   0.0002604215 * block_size +
                   3.972928e-09 * (double)num_dpus * block_size;
          }
        } else {
          if (num_dpus <= 1920) {
            return -0.3592972 + 0.0002906561 * num_dpus +
                   0.005518858 * block_size +
                   -2.742566e-06 * (double)num_dpus * block_size;
          } else {
            return -0.247229 + 0.0002283101 * num_dpus +
                   0.000343926 * block_size +
                   -4.544934e-08 * (double)num_dpus * block_size;
          }
        }
      } else {
        if (block_size <= 1856) {
          if (num_dpus <= 1024) {
            return 0.01738584 + 0.0001406266 * num_dpus +
                   0.0001399608 * block_size +
                   6.426645e-09 * (double)num_dpus * block_size;
          } else {
            return 0.1360472 + 7.207285e-06 * num_dpus +
                   -1.497858e-05 * block_size +
                   1.268983e-07 * (double)num_dpus * block_size;
          }
        } else {
          if (num_dpus <= 992) {
            return 0.09806079 + 9.44706e-05 * num_dpus +
                   4.848439e-05 * block_size +
                   5.081727e-08 * (double)num_dpus * block_size;
          } else {
            return 0.3155459 + -7.986139e-05 * num_dpus +
                   1.535097e-05 * block_size +
                   5.706529e-08 * (double)num_dpus * block_size;
          }
        }
      }
    } else {
      if (block_size <= 327680) {
        if (num_dpus <= 1920) {
          if (num_dpus <= 1024) {
            return -2.03549 + 0.0003823432 * num_dpus +
                   9.657328e-05 * block_size +
                   6.212474e-08 * (double)num_dpus * block_size;
          } else {
            return 0.2402291 + -0.002406224 * num_dpus +
                   8.803938e-06 * block_size +
                   1.388721e-07 * (double)num_dpus * block_size;
          }
        } else {
          return -57.48988 + 0.02694143 * num_dpus + 0.002406904 * block_size +
                 -1.07336e-06 * (double)num_dpus * block_size;
        }
      } else {
        if (num_dpus <= 1600) {
          if (num_dpus <= 1024) {
            return -13.0307 + 0.0196579 * num_dpus + 0.0001198051 * block_size +
                   2.14933e-08 * (double)num_dpus * block_size;
          } else {
            return -96.83723 + 0.08972868 * num_dpus +
                   0.0001990085 * block_size +
                   -5.53705e-08 * (double)num_dpus * block_size;
          }
        } else {
          if (num_dpus <= 1856) {
            return -340.8113 + 0.2038779 * num_dpus +
                   0.0004508452 * block_size +
                   -1.672147e-07 * (double)num_dpus * block_size;
          } else {
            return 699.4207 + -0.3376448 * num_dpus + -0.00140208 * block_size +
                   7.885878e-07 * (double)num_dpus * block_size;
          }
        }
      }
    }
  }
}

double upmem_cm::broadcastCostMs(int num_dpus, int block_size) {
  if (num_dpus <= 448) {
    if (num_dpus <= 64) {
      if (num_dpus <= 2) {
        if (block_size <= 272) {
          return 0.02020152 + -0.001863613 * num_dpus +
                 -1.219464e-05 * block_size +
                 8.948234e-06 * (double)num_dpus * block_size;
        } else {
          if (block_size <= 1088) {
            return 0.017189 + 3.378262e-05 * num_dpus +
                   1.186017e-06 * block_size +
                   9.99631e-08 * (double)num_dpus * block_size;
          } else {
            return 0.0169076 + 0.0001413237 * num_dpus +
                   9.319842e-07 * block_size +
                   1.876492e-08 * (double)num_dpus * block_size;
          }
        }
      } else {
        if (num_dpus <= 2) {
          if (block_size <= 1088) {
            return 0.01774284 + 4.993394e-13 * num_dpus +
                   5.182521e-07 * block_size +
                   3.706828e-13 * (double)num_dpus * block_size;
          } else {
            return 0.01735177 + 2.109539e-12 * num_dpus +
                   9.525816e-07 * block_size +
                   6.813398e-13 * (double)num_dpus * block_size;
          }
        } else {
          if (num_dpus <= 3) {
            return 0.01765376 + 0 * num_dpus + 9.721826e-07 * block_size +
                   9.271458e-13 * (double)num_dpus * block_size;
          } else {
            return 0.01713847 + 6.273789e-05 * num_dpus +
                   8.026522e-07 * block_size +
                   6.79944e-08 * (double)num_dpus * block_size;
          }
        }
      }
    } else {
      if (num_dpus <= 384) {
        if (block_size <= 272) {
          if (num_dpus <= 72) {
            return 0.03269053 + -3.691242e-05 * num_dpus +
                   -8.186034e-06 * block_size +
                   2.915316e-07 * (double)num_dpus * block_size;
          } else {
            return 0.02488625 + 3.66717e-05 * num_dpus +
                   2.298704e-05 * block_size +
                   1.901517e-08 * (double)num_dpus * block_size;
          }
        } else {
          if (block_size <= 992) {
            return 0.02420219 + 3.665781e-05 * num_dpus +
                   2.519808e-05 * block_size +
                   3.235253e-08 * (double)num_dpus * block_size;
          } else {
            return 0.02709683 + 5.94391e-05 * num_dpus +
                   8.20171e-06 * block_size +
                   3.779329e-09 * (double)num_dpus * block_size;
          }
        }
      } else {
        if (block_size <= 992) {
          if (block_size <= 120) {
            return 0.04343705 + 1.790075e-05 * num_dpus +
                   -1.503305e-05 * block_size +
                   1.168423e-07 * (double)num_dpus * block_size;
          } else {
            return 0.03887921 + 2.587213e-05 * num_dpus +
                   -2.461424e-05 * block_size +
                   1.75893e-07 * (double)num_dpus * block_size;
          }
        } else {
          if (block_size <= 27648) {
            return 0.07115225 + -2.412489e-05 * num_dpus +
                   -2.575795e-05 * block_size +
                   9.414677e-08 * (double)num_dpus * block_size;
          } else {
            return -1.573753 + 0.004006437 * num_dpus +
                   1.82379e-05 * block_size +
                   -1.131662e-08 * (double)num_dpus * block_size;
          }
        }
      }
    }
  } else {
    if (block_size <= 992) {
      if (num_dpus <= 1408) {
        if (num_dpus <= 496) {
          if (block_size <= 184) {
            return 0.05932093 + 1.423412e-05 * num_dpus +
                   0.0001158195 * block_size +
                   -1.384599e-07 * (double)num_dpus * block_size;
          } else {
            return 0.08902539 + -5.133002e-05 * num_dpus +
                   -7.157214e-07 * block_size +
                   1.370083e-07 * (double)num_dpus * block_size;
          }
        } else {
          if (num_dpus <= 768) {
            return 0.0392736 + 5.265321e-05 * num_dpus +
                   4.774961e-05 * block_size +
                   3.289655e-08 * (double)num_dpus * block_size;
          } else {
            return 0.03989524 + 4.606751e-05 * num_dpus +
                   8.68294e-05 * block_size +
                   -1.840442e-08 * (double)num_dpus * block_size;
          }
        }
      } else {
        if (num_dpus <= 1856) {
          if (num_dpus <= 1600) {
            return 0.1149062 + -4.190985e-06 * num_dpus +
                   6.862866e-05 * block_size +
                   -4.712389e-09 * (double)num_dpus * block_size;
          } else {
            return 0.07430745 + 2.856988e-05 * num_dpus +
                   0.0001249172 * block_size +
                   -3.292836e-08 * (double)num_dpus * block_size;
          }
        } else {
          if (block_size <= 272) {
            return 0.02821244 + 6.383184e-05 * num_dpus +
                   -4.335442e-05 * block_size +
                   3.990398e-08 * (double)num_dpus * block_size;
          } else {
            return 0.105269 + 1.976817e-05 * num_dpus +
                   -6.669589e-05 * block_size +
                   6.816724e-08 * (double)num_dpus * block_size;
          }
        }
      }
    } else {
      if (block_size <= 1856) {
        if (num_dpus <= 1216) {
          if (num_dpus <= 512) {
            return -0.01459553 + 0.0001607903 * num_dpus +
                   6.787981e-05 * block_size +
                   -5.70786e-08 * (double)num_dpus * block_size;
          } else {
            return 0.04915658 + 3.387183e-05 * num_dpus +
                   3.552583e-05 * block_size +
                   5.010834e-09 * (double)num_dpus * block_size;
          }
        } else {
          if (num_dpus <= 1536) {
            return 0.05187333 + 3.503011e-05 * num_dpus +
                   2.629584e-05 * block_size +
                   6.648246e-09 * (double)num_dpus * block_size;
          } else {
            return 0.002844974 + 6.812591e-05 * num_dpus +
                   5.289661e-06 * block_size +
                   1.91772e-08 * (double)num_dpus * block_size;
          }
        }
      } else {
        if (num_dpus <= 992) {
          if (num_dpus <= 512) {
            return 0.1440855 + -0.0001438406 * num_dpus +
                   3.883417e-05 * block_size +
                   -4.07662e-08 * (double)num_dpus * block_size;
          } else {
            return 0.07012787 + 1.316579e-05 * num_dpus +
                   1.496996e-05 * block_size +
                   8.664156e-09 * (double)num_dpus * block_size;
          }
        } else {
          if (num_dpus <= 1408) {
            return 0.08875071 + 4.501246e-06 * num_dpus +
                   1.783213e-05 * block_size +
                   4.025237e-09 * (double)num_dpus * block_size;
          } else {
            return 0.002594918 + 6.86615e-05 * num_dpus +
                   1.515301e-05 * block_size +
                   5.18925e-09 * (double)num_dpus * block_size;
          }
        }
      }
    }
  }
}

double upmem_cm::scatterBlockCostMs(int num_dpus, int block_size) {
  if (block_size <= 18432) {
    if (num_dpus <= 448) {
      if (num_dpus <= 60) {
        if (block_size <= 224) {
          if (num_dpus <= 2) {
            return 0.01642097 + 8.191531e-05 * num_dpus +
                   -1.661087e-06 * block_size +
                   -3.79323e-07 * (double)num_dpus * block_size;
          } else {
            return 0.01631339 + 4.205606e-05 * num_dpus +
                   -4.42109e-06 * block_size +
                   3.025826e-07 * (double)num_dpus * block_size;
          }
        } else {
          if (block_size <= 1856) {
            return 0.01435283 + 0.0001284796 * num_dpus +
                   3.040699e-06 * block_size +
                   9.648993e-08 * (double)num_dpus * block_size;
          } else {
            return 0.01602894 + 5.000367e-05 * num_dpus +
                   6.60596e-07 * block_size +
                   7.199795e-08 * (double)num_dpus * block_size;
          }
        }
      } else {
        if (num_dpus <= 384) {
          if (block_size <= 248) {
            return 0.02336572 + 6.97836e-05 * num_dpus +
                   1.887131e-05 * block_size +
                   4.36137e-08 * (double)num_dpus * block_size;
          } else {
            return 0.03102744 + 8.293248e-05 * num_dpus +
                   6.180571e-06 * block_size +
                   8.588405e-09 * (double)num_dpus * block_size;
          }
        } else {
          if (block_size <= 928) {
            return 0.1215938 + -0.0001307987 * num_dpus +
                   -1.302205e-05 * block_size +
                   1.418304e-07 * (double)num_dpus * block_size;
          } else {
            return 0.02657178 + 0.0001270489 * num_dpus +
                   6.347127e-05 * block_size +
                   -1.155806e-07 * (double)num_dpus * block_size;
          }
        }
      }
    } else {
      if (block_size <= 1856) {
        if (block_size <= 992) {
          if (num_dpus <= 1024) {
            return 0.04987618 + 6.757654e-05 * num_dpus +
                   6.377176e-05 * block_size +
                   4.594504e-09 * (double)num_dpus * block_size;
          } else {
            return 0.003615648 + 0.0001018296 * num_dpus +
                   8.974793e-05 * block_size +
                   -1.720882e-08 * (double)num_dpus * block_size;
          }
        } else {
          if (num_dpus <= 992) {
            return 0.04906602 + 6.534039e-05 * num_dpus +
                   3.062465e-05 * block_size +
                   1.180678e-08 * (double)num_dpus * block_size;
          } else {
            return 0.01116873 + 9.508695e-05 * num_dpus +
                   3.281086e-05 * block_size +
                   3.926751e-09 * (double)num_dpus * block_size;
          }
        }
      } else {
        if (block_size <= 4864) {
          if (block_size <= 1984) {
            return 1.02723 + 9.4137e-05 * num_dpus + -0.000460616 * block_size +
                   -7.102947e-09 * (double)num_dpus * block_size;
          } else {
            return 0.0351554 + 7.871682e-05 * num_dpus +
                   2.305993e-05 * block_size +
                   5.603818e-10 * (double)num_dpus * block_size;
          }
        } else {
          if (num_dpus <= 992) {
            return 0.07936792 + -1.100726e-05 * num_dpus +
                   1.223244e-05 * block_size +
                   2.0919e-08 * (double)num_dpus * block_size;
          } else {
            return -0.004796296 + 6.543189e-05 * num_dpus +
                   2.520882e-05 * block_size +
                   5.450615e-09 * (double)num_dpus * block_size;
          }
        }
      }
    }
  } else {
    if (num_dpus <= 448) {
      if (block_size <= 69632) {
        if (num_dpus <= 64) {
          if (num_dpus <= 31) {
            return 0.01610156 + -8.938296e-05 * num_dpus +
                   7.200142e-07 * block_size +
                   7.280617e-08 * (double)num_dpus * block_size;
          } else {
            return 0.01555231 + 4.101995e-05 * num_dpus +
                   4.51302e-06 * block_size +
                   3.190317e-10 * (double)num_dpus * block_size;
          }
        } else {
          if (num_dpus <= 384) {
            return 0.01342303 + -3.977994e-05 * num_dpus +
                   8.382111e-06 * block_size +
                   1.075536e-08 * (double)num_dpus * block_size;
          } else {
            return 0.8328312 + -0.002279083 * num_dpus +
                   4.006894e-05 * block_size +
                   -3.68348e-08 * (double)num_dpus * block_size;
          }
        }
      } else {
        if (num_dpus <= 72) {
          if (block_size <= 327680) {
            return 0.03147042 + -0.001633669 * num_dpus +
                   5.172478e-07 * block_size +
                   9.514496e-08 * (double)num_dpus * block_size;
          } else {
            return 0.45688 + -0.05537946 * num_dpus +
                   -1.122891e-06 * block_size +
                   2.65056e-07 * (double)num_dpus * block_size;
          }
        } else {
          if (num_dpus <= 384) {
            return -1.392915 + -0.001660896 * num_dpus +
                   1.829238e-05 * block_size +
                   6.459573e-08 * (double)num_dpus * block_size;
          } else {
            return -1.316036 + -0.0003880576 * num_dpus +
                   3.181251e-05 * block_size +
                   5.250035e-08 * (double)num_dpus * block_size;
          }
        }
      }
    } else {
      if (block_size <= 43008) {
        if (block_size <= 28672) {
          if (num_dpus <= 1600) {
            return 0.3220294 + -0.0004180038 * num_dpus +
                   8.349434e-06 * block_size +
                   2.93754e-08 * (double)num_dpus * block_size;
          } else {
            return 4.039044 + -0.002650941 * num_dpus +
                   -0.0001984947 * block_size +
                   1.523236e-07 * (double)num_dpus * block_size;
          }
        } else {
          if (num_dpus <= 1024) {
            return 1.822967 + -0.003994736 * num_dpus +
                   -4.70618e-05 * block_size +
                   1.55827e-07 * (double)num_dpus * block_size;
          } else {
            return 0.3412205 + -0.001633683 * num_dpus +
                   -9.29636e-06 * block_size +
                   8.250002e-08 * (double)num_dpus * block_size;
          }
        }
      } else {
        if (block_size <= 114688) {
          if (num_dpus <= 992) {
            return -3.159456 + 0.001558329 * num_dpus +
                   6.107674e-05 * block_size +
                   3.895121e-08 * (double)num_dpus * block_size;
          } else {
            return -2.126949 + 0.0001398935 * num_dpus +
                   5.453635e-05 * block_size +
                   3.939934e-08 * (double)num_dpus * block_size;
          }
        } else {
          if (num_dpus <= 1920) {
            return -2.653044 + 0.002898698 * num_dpus +
                   6.422422e-05 * block_size +
                   1.508808e-08 * (double)num_dpus * block_size;
          } else {
            return 190.471 + -0.09469353 * num_dpus +
                   -0.0005781592 * block_size +
                   3.409309e-07 * (double)num_dpus * block_size;
          }
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