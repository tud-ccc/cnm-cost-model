
#include <upmem_cost_model/ScatterGatherCm.h>

#include <cmath> // std::log2, std::exp
#include <limits>

// Fitted by model_tree.py on 78873 measured configs (upmemcm/scatter_cost/plots/block), relRMSE 4.74%.
namespace {
double scatterBlockCostMsModel(int num_dpus, int block_size) {
  if ((double)num_dpus * block_size <= 38338560) {
    if (num_dpus <= 388) {
      if (block_size <= 1920) {
        if (block_size <= 992) {
          if (num_dpus <= 109) {
            if (num_dpus <= 66) {
              return -0.2927682 + 0.004863236 * num_dpus +
                     0.0004751122 * block_size +
                     -7.137208e-06 * (double)num_dpus * block_size;
            } else {
              return 0.01807351 + 8.870748e-05 * num_dpus +
                     -1.732932e-07 * block_size +
                     1.364751e-07 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 158) {
              return 0.01695674 + 8.791851e-05 * num_dpus +
                     2.272582e-05 * block_size +
                     -5.783877e-08 * (double)num_dpus * block_size;
            } else {
              return 0.01929024 + 5.99574e-05 * num_dpus +
                     1.39774e-05 * block_size +
                     9.430063e-09 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (num_dpus <= 68) {
            return -0.1014727 + 0.001889922 * num_dpus +
                   1.717713e-05 * block_size +
                   -1.179907e-07 * (double)num_dpus * block_size;
          } else {
            if (num_dpus <= 158) {
              return 0.01865825 + 7.396977e-05 * num_dpus +
                     9.280114e-06 * block_size +
                     -2.833065e-09 * (double)num_dpus * block_size;
            } else {
              return 0.01720703 + 6.878579e-05 * num_dpus +
                     9.477626e-06 * block_size +
                     -2.098486e-09 * (double)num_dpus * block_size;
            }
          }
        }
      } else {
        if (block_size <= 110592) {
          if (block_size <= 2048) {
            if (num_dpus <= 97) {
              return 0.7117152 + -0.004767658 * num_dpus +
                     -0.0003398867 * block_size +
                     2.435432e-06 * (double)num_dpus * block_size;
            } else {
              return 0.2847417 + -1.438349e-06 * num_dpus +
                     -0.0001241356 * block_size +
                     2.868538e-08 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 168) {
              return 0.01493913 + 0.0001067613 * num_dpus +
                     4.540488e-06 * block_size +
                     2.197419e-10 * (double)num_dpus * block_size;
            } else {
              return 0.01670013 + 7.385575e-05 * num_dpus +
                     3.992871e-06 * block_size +
                     2.921884e-09 * (double)num_dpus * block_size;
            }
          }
        } else {
          if ((double)num_dpus * block_size <= 29081600) {
            if (block_size <= 229376) {
              return 0.1962265 + -0.002349761 * num_dpus +
                     3.188759e-06 * block_size +
                     2.175979e-08 * (double)num_dpus * block_size;
            } else {
              return -0.683501 + 0.003170815 * num_dpus +
                     6.842887e-06 * block_size +
                     -7.804373e-10 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 128) {
              return -1.895885 + 0.003162349 * num_dpus +
                     7.322402e-06 * block_size +
                     3.654555e-08 * (double)num_dpus * block_size;
            } else {
              return -1.391423 + 0.003015616 * num_dpus +
                     1.293277e-05 * block_size +
                     -9.590898e-09 * (double)num_dpus * block_size;
            }
          }
        }
      }
    } else {
      if (block_size <= 1920) {
        if (block_size <= 992) {
          if (num_dpus <= 640) {
            if (num_dpus <= 496) {
              return 0.01902753 + 6.259639e-05 * num_dpus +
                     2.105924e-05 * block_size +
                     -9.567818e-09 * (double)num_dpus * block_size;
            } else {
              return 0.02938669 + 4.889745e-05 * num_dpus +
                     1.751286e-05 * block_size +
                     -1.552622e-09 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 800) {
              return -0.03598542 + 0.0001471197 * num_dpus +
                     9.071978e-06 * block_size +
                     3.681619e-08 * (double)num_dpus * block_size;
            } else {
              return -0.001436718 + 9.81128e-05 * num_dpus +
                     1.999044e-05 * block_size +
                     1.572094e-08 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (num_dpus <= 640) {
            if (num_dpus <= 568) {
              return 0.01756248 + 6.657479e-05 * num_dpus +
                     3.762287e-06 * block_size +
                     1.304681e-08 * (double)num_dpus * block_size;
            } else {
              return -0.01041119 + 0.0001087754 * num_dpus +
                     3.356011e-06 * block_size +
                     1.304479e-08 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 1344) {
              return 0.007555166 + 8.689701e-05 * num_dpus +
                     1.435572e-05 * block_size +
                     6.56353e-09 * (double)num_dpus * block_size;
            } else {
              return 0.003408934 + 9.332171e-05 * num_dpus +
                     1.228463e-05 * block_size +
                     1.097541e-08 * (double)num_dpus * block_size;
            }
          }
        }
      } else {
        if (num_dpus <= 640) {
          if ((double)num_dpus * block_size <= 15466496) {
            if (block_size <= 2048) {
              return 0.1057557 + 0.0004191609 * num_dpus +
                     -3.738327e-05 * block_size +
                     -1.718133e-07 * (double)num_dpus * block_size;
            } else {
              return 0.01678108 + 6.420404e-05 * num_dpus +
                     6.07092e-06 * block_size +
                     8.542681e-10 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 440) {
              return -0.5130529 + 0.001467086 * num_dpus +
                     1.420073e-05 * block_size +
                     -1.984413e-08 * (double)num_dpus * block_size;
            } else {
              return -0.02001985 + 1.158766e-05 * num_dpus +
                     7.537927e-06 * block_size +
                     2.80153e-09 * (double)num_dpus * block_size;
            }
          }
        } else {
          if ((double)num_dpus * block_size <= 14131200) {
            if (num_dpus <= 1104) {
              return 0.01006613 + 8.403745e-05 * num_dpus +
                     4.271362e-06 * block_size +
                     8.483619e-09 * (double)num_dpus * block_size;
            } else {
              return -0.01310384 + 0.0001021972 * num_dpus +
                     -2.256753e-06 * block_size +
                     1.294935e-08 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 1296) {
              return 0.02932688 + -2.491565e-06 * num_dpus +
                     2.876871e-06 * block_size +
                     1.410391e-08 * (double)num_dpus * block_size;
            } else {
              return -0.06124572 + 8.542895e-05 * num_dpus +
                     3.149776e-06 * block_size +
                     1.526752e-08 * (double)num_dpus * block_size;
            }
          }
        }
      }
    }
  } else {
    if (num_dpus <= 440) {
      if ((double)num_dpus * block_size <= 148242432) {
        if (num_dpus <= 128) {
          if (num_dpus <= 73) {
            if (num_dpus <= 66) {
              return 52.67059 + -0.8558279 * num_dpus +
                     4.382103e-06 * block_size +
                     1.272514e-07 * (double)num_dpus * block_size;
            } else {
              return 0.2416839 + -0.04775207 * num_dpus +
                     -4.46494e-06 * block_size +
                     2.478612e-07 * (double)num_dpus * block_size;
            }
          } else {
            if ((double)num_dpus * block_size <= 59768832) {
              return -5.809847 + 0.01188647 * num_dpus +
                     1.138374e-05 * block_size +
                     7.623526e-08 * (double)num_dpus * block_size;
            } else {
              return -3.024869 + 0.004739557 * num_dpus +
                     1.209384e-05 * block_size +
                     3.529631e-08 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (block_size <= 139264) {
            if (num_dpus <= 388) {
              return -2.678912 + 0.004649614 * num_dpus +
                     2.132401e-05 * block_size +
                     -1.760831e-08 * (double)num_dpus * block_size;
            } else {
              return 0.1195069 + -0.002127231 * num_dpus +
                     4.73183e-06 * block_size +
                     2.428372e-08 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 190) {
              return -3.103472 + 0.007871742 * num_dpus +
                     1.371823e-05 * block_size +
                     1.211375e-08 * (double)num_dpus * block_size;
            } else {
              return -2.622527 + 0.001486358 * num_dpus +
                     1.393049e-05 * block_size +
                     2.600534e-08 * (double)num_dpus * block_size;
            }
          }
        }
      } else {
        if (num_dpus <= 73) {
          if (num_dpus <= 71) {
            if ((double)num_dpus * block_size <= 572522496) {
              return -51.69397 + 0.701255 * num_dpus +
                     3.060014e-05 * block_size +
                     -2.638926e-07 * (double)num_dpus * block_size;
            } else {
              return 430.9076 + -6.488241 * num_dpus +
                     -3.534198e-05 * block_size +
                     7.394843e-07 * (double)num_dpus * block_size;
            }
          } else {
            return -740.5851 + 10.12759 * num_dpus + 8.961352e-05 * block_size +
                   -1.031901e-06 * (double)num_dpus * block_size;
          }
        } else {
          if (num_dpus <= 268) {
            if (num_dpus <= 190) {
              return 0.9680219 + -0.01138848 * num_dpus +
                     1.222573e-05 * block_size +
                     2.177696e-08 * (double)num_dpus * block_size;
            } else {
              return -2.397727 + 0.01387491 * num_dpus +
                     1.44651e-05 * block_size +
                     4.566091e-09 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 428) {
              return -1.293173 + 0.002775317 * num_dpus +
                     1.026526e-05 * block_size +
                     2.295697e-08 * (double)num_dpus * block_size;
            } else {
              return -40.66509 + 0.09511106 * num_dpus +
                     -0.0001062075 * block_size +
                     2.888129e-07 * (double)num_dpus * block_size;
            }
          }
        }
      }
    } else {
      if ((double)num_dpus * block_size <= 86114304) {
        if ((double)num_dpus * block_size <= 54640640) {
          if (num_dpus <= 640) {
            if (block_size <= 86016) {
              return 0.5673546 + -0.001509794 * num_dpus +
                     -3.853483e-06 * block_size +
                     2.992555e-08 * (double)num_dpus * block_size;
            } else {
              return 7.421191 + -0.01691387 * num_dpus +
                     -8.151408e-05 * block_size +
                     2.042185e-07 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 1296) {
              return -0.7139059 + 0.0002202847 * num_dpus +
                     9.833547e-06 * block_size +
                     1.97485e-08 * (double)num_dpus * block_size;
            } else {
              return -0.7222083 + 0.0002199805 * num_dpus +
                     8.108524e-06 * block_size +
                     2.338148e-08 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (num_dpus <= 640) {
            if (num_dpus <= 528) {
              return -3.371428 + 0.002899179 * num_dpus +
                     3.165359e-06 * block_size +
                     5.080395e-08 * (double)num_dpus * block_size;
            } else {
              return -0.6040695 + -0.00237876 * num_dpus +
                     -7.122838e-06 * block_size +
                     6.713498e-08 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 1296) {
              return -2.779923 + 0.0004323246 * num_dpus +
                     1.600797e-05 * block_size +
                     4.68506e-08 * (double)num_dpus * block_size;
            } else {
              return -3.15096 + 0.0005094648 * num_dpus +
                     2.409736e-05 * block_size +
                     4.909941e-08 * (double)num_dpus * block_size;
            }
          }
        }
      } else {
        if ((double)num_dpus * block_size <= 211681280) {
          if (num_dpus <= 1056) {
            if (num_dpus <= 712) {
              return -5.651925 + 0.005321382 * num_dpus +
                     1.103215e-05 * block_size +
                     4.767539e-08 * (double)num_dpus * block_size;
            } else {
              return -5.265884 + 0.004125704 * num_dpus +
                     4.895482e-05 * block_size +
                     1.333053e-09 * (double)num_dpus * block_size;
            }
          } else {
            if ((double)num_dpus * block_size <= 122912768) {
              return 0.6111186 + -0.002410874 * num_dpus +
                     -4.540574e-05 * block_size +
                     1.053208e-07 * (double)num_dpus * block_size;
            } else {
              return 1.1325 + -0.0008461675 * num_dpus +
                     1.729111e-05 * block_size +
                     4.038321e-08 * (double)num_dpus * block_size;
            }
          }
        } else {
          if ((double)num_dpus * block_size <= 521404416) {
            if (num_dpus <= 896) {
              return 4.771834 + -0.005740731 * num_dpus +
                     -5.236691e-06 * block_size +
                     5.679904e-08 * (double)num_dpus * block_size;
            } else {
              return 2.273724 + -0.0008504764 * num_dpus +
                     -7.529192e-07 * block_size +
                     4.062831e-08 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 896) {
              return -10.61858 + 0.006616825 * num_dpus +
                     6.953552e-06 * block_size +
                     5.337677e-08 * (double)num_dpus * block_size;
            } else {
              return -10.0417 + 0.0007973396 * num_dpus +
                     7.319652e-06 * block_size +
                     5.269195e-08 * (double)num_dpus * block_size;
            }
          }
        }
      }
    }
  }
}
} // namespace

double upmem_cm::scatterBlockCostMs(int num_dpus, int block_size) {
  // No transfer beats the fastest rate this sweep measured (8.09163e+07 B/ms);
  // outside its fitted region a leaf's linear model can slope below zero.
  return std::fmax(scatterBlockCostMsModel(num_dpus, block_size), (double)num_dpus * block_size / 8.091634e+07);
}

// Fitted by model_tree.py on 19908 measured configs (upmemcm/scatter_cost/plots/shared), relRMSE 5.28%.
namespace {
double scatterSharedCostMsModel(int num_dpus, int block_size) {
  if (num_dpus <= 640) {
    if (block_size <= 992) {
      if (num_dpus <= 104) {
        if (num_dpus <= 68) {
          return -0.06106769 + 0.001299845 * num_dpus + 0.000169245 * block_size + -2.33662e-06 * (double)num_dpus * block_size;
        } else {
          if (num_dpus <= 88) {
            if (num_dpus <= 80) {
              return 0.001856451 + 0.0003038247 * num_dpus + -4.973494e-06 * block_size + 2.060658e-07 * (double)num_dpus * block_size;
            } else {
              return -0.02367909 + 0.0005725255 * num_dpus + -2.546728e-05 * block_size + 4.523176e-07 * (double)num_dpus * block_size;
            }
          } else {
            if (block_size <= 152) {
              return 0.06148902 + -0.0003630309 * num_dpus + -0.0002391631 * block_size + 2.5364e-06 * (double)num_dpus * block_size;
            } else {
              return 0.0244579 + -2.353243e-06 * num_dpus + -2.954717e-05 * block_size + 4.582793e-07 * (double)num_dpus * block_size;
            }
          }
        }
      } else {
        if (num_dpus <= 116) {
          if (block_size <= 352) {
            return 0.09913398 + -0.0006459667 * num_dpus + -3.944072e-05 * block_size + 4.919248e-07 * (double)num_dpus * block_size;
          } else {
            return 0.1431887 + -0.001030635 * num_dpus + -4.174537e-05 * block_size + 5.138431e-07 * (double)num_dpus * block_size;
          }
        } else {
          if (num_dpus <= 144) {
            if (num_dpus <= 128) {
              return 0.04055281 + -0.0001031363 * num_dpus + -5.027002e-05 * block_size + 5.329408e-07 * (double)num_dpus * block_size;
            } else {
              return -0.02019774 + 0.0003673998 * num_dpus + -4.25234e-05 * block_size + 4.054208e-07 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 192) {
              return 0.008114463 + 0.0001286075 * num_dpus + -1.572473e-05 * block_size + 1.977513e-07 * (double)num_dpus * block_size;
            } else {
              return 0.01950407 + 6.490833e-05 * num_dpus + 1.550813e-05 * block_size + 6.939823e-09 * (double)num_dpus * block_size;
            }
          }
        }
      }
    } else {
      if (num_dpus <= 384) {
        if (block_size <= 11776) {
          if (block_size <= 1920) {
            if (num_dpus <= 128) {
              return 0.02011204 + 5.078534e-05 * num_dpus + 9.899317e-06 * block_size + -2.530832e-09 * (double)num_dpus * block_size;
            } else {
              return 0.02286094 + 4.610612e-05 * num_dpus + 9.124965e-06 * block_size + 8.17974e-09 * (double)num_dpus * block_size;
            }
          } else {
            if (block_size <= 2048) {
              return 0.2667726 + 0.0001826049 * num_dpus + -0.0001156017 * block_size + -5.449581e-08 * (double)num_dpus * block_size;
            } else {
              return 0.02115888 + 6.073493e-05 * num_dpus + 4.455103e-06 * block_size + 4.511143e-09 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (num_dpus <= 160) {
            if (num_dpus <= 128) {
              return 0.02006535 + 0.0001039163 * num_dpus + 4.384996e-06 * block_size + 1.371134e-09 * (double)num_dpus * block_size;
            } else {
              return 0.1129587 + -0.0003794331 * num_dpus + 3.444692e-06 * block_size + 7.200872e-09 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 272) {
              return 0.01606847 + 0.0001373491 * num_dpus + 4.529643e-06 * block_size + 1.317025e-10 * (double)num_dpus * block_size;
            } else {
              return 0.06194821 + 2.460156e-05 * num_dpus + 5.092745e-06 * block_size + -9.662029e-10 * (double)num_dpus * block_size;
            }
          }
        }
      } else {
        if ((double)num_dpus * block_size <= 45875200) {
          if (block_size <= 2048) {
            if ((double)num_dpus * block_size <= 793600) {
              return 0.03011646 + 4.217183e-05 * num_dpus + -2.940025e-06 * block_size + 2.78829e-08 * (double)num_dpus * block_size;
            } else {
              return 0.1291612 + -9.86554e-05 * num_dpus + -5.003558e-05 * block_size + 8.921222e-08 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 496) {
              return 0.01387245 + 7.714918e-05 * num_dpus + 6.243783e-06 * block_size + 1.534074e-10 * (double)num_dpus * block_size;
            } else {
              return 0.002885221 + 9.156806e-05 * num_dpus + 7.013252e-06 * block_size + -1.164902e-09 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (num_dpus <= 480) {
            if (num_dpus <= 432) {
              return -1.841631 + 0.004804351 * num_dpus + 1.644998e-05 * block_size + -2.673802e-08 * (double)num_dpus * block_size;
            } else {
              return -1.218115 + 0.003075193 * num_dpus + 1.430968e-05 * block_size + -1.962739e-08 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 512) {
              return -3.498034 + 0.007168266 * num_dpus + 1.053319e-05 * block_size + -9.85425e-09 * (double)num_dpus * block_size;
            } else {
              return 0.01306391 + 5.44103e-05 * num_dpus + 6.303366e-06 * block_size + 3.347924e-10 * (double)num_dpus * block_size;
            }
          }
        }
      }
    }
  } else {
    if (num_dpus <= 1280) {
      if (block_size <= 992) {
        if (num_dpus <= 1152) {
          if (num_dpus <= 1024) {
            if (num_dpus <= 960) {
              return -0.004228195 + 0.0001106733 * num_dpus + 7.516503e-05 * block_size + -5.331388e-08 * (double)num_dpus * block_size;
            } else {
              return -0.2145001 + 0.0003135643 * num_dpus + 1.371455e-05 * block_size + 1.409972e-08 * (double)num_dpus * block_size;
            }
          } else {
            return 0.2137102 + -9.145754e-05 * num_dpus + -9.261914e-05 * block_size + 1.102962e-07 * (double)num_dpus * block_size;
          }
        } else {
          return -0.1490635 + 0.0002126099 * num_dpus + -0.0001674909 * block_size + 1.614406e-07 * (double)num_dpus * block_size;
        }
      } else {
        if (block_size <= 1920) {
          if (num_dpus <= 960) {
            if ((double)num_dpus * block_size <= 1130496) {
              return 0.01704156 + 8.292168e-05 * num_dpus + 2.273331e-05 * block_size + -4.487769e-09 * (double)num_dpus * block_size;
            } else {
              return -0.0348337 + 0.0001373656 * num_dpus + 4.653362e-05 * block_size + -2.781051e-08 * (double)num_dpus * block_size;
            }
          } else {
            return 0.0580635 + 4.022556e-05 * num_dpus + -1.604297e-05 * block_size + 3.262367e-08 * (double)num_dpus * block_size;
          }
        } else {
          if (num_dpus <= 800) {
            if ((double)num_dpus * block_size <= 2555904) {
              return 0.002997084 + 0.0001252615 * num_dpus + 2.023803e-05 * block_size + -2.109315e-08 * (double)num_dpus * block_size;
            } else {
              return 0.02663878 + 6.733065e-05 * num_dpus + 9.661635e-06 * block_size + 1.783073e-09 * (double)num_dpus * block_size;
            }
          } else {
            if (block_size <= 77824) {
              return 0.03776841 + 5.803173e-05 * num_dpus + 1.062485e-05 * block_size + 1.81895e-09 * (double)num_dpus * block_size;
            } else {
              return 0.02601827 + 6.954764e-05 * num_dpus + 7.310267e-06 * block_size + 4.591892e-09 * (double)num_dpus * block_size;
            }
          }
        }
      }
    } else {
      if (block_size <= 1920) {
        if (block_size <= 992) {
          if (num_dpus <= 1664) {
            if (num_dpus <= 1408) {
              return -0.03552812 + 0.0001197524 * num_dpus + -5.213408e-05 * block_size + 7.503045e-08 * (double)num_dpus * block_size;
            } else {
              return 0.01685358 + 8.809333e-05 * num_dpus + 5.146801e-06 * block_size + 2.583785e-08 * (double)num_dpus * block_size;
            }
          } else {
            if (block_size <= 168) {
              return -0.03738985 + 0.0001201852 * num_dpus + 9.826724e-05 * block_size + -2.368768e-08 * (double)num_dpus * block_size;
            } else {
              return -0.07758531 + 0.0001393592 * num_dpus + -1.179244e-05 * block_size + 3.623377e-08 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (num_dpus <= 1728) {
            return -0.02387306 + 0.0001127758 * num_dpus + 4.461717e-05 * block_size + -9.444681e-09 * (double)num_dpus * block_size;
          } else {
            return 0.08571241 + 4.841823e-05 * num_dpus + -0.0001348087 * block_size + 9.14658e-08 * (double)num_dpus * block_size;
          }
        }
      } else {
        if (num_dpus <= 1856) {
          if (num_dpus <= 1408) {
            if (block_size <= 16384) {
              return -0.03647015 + 0.0001190009 * num_dpus + -1.777326e-05 * block_size + 2.570521e-08 * (double)num_dpus * block_size;
            } else {
              return 0.1628491 + -2.958711e-05 * num_dpus + 1.267516e-05 * block_size + 3.320134e-09 * (double)num_dpus * block_size;
            }
          } else {
            if (block_size <= 22528) {
              return -0.02356722 + 0.0001108991 * num_dpus + 8.735583e-06 * block_size + 6.045343e-09 * (double)num_dpus * block_size;
            } else {
              return -0.2379834 + 0.0002490884 * num_dpus + 1.349475e-05 * block_size + 3.315252e-09 * (double)num_dpus * block_size;
            }
          }
        } else {
          if ((double)num_dpus * block_size <= 10813440) {
            return 0.1747487 + 1.869055e-05 * num_dpus + -7.774435e-05 * block_size + 4.831297e-08 * (double)num_dpus * block_size;
          } else {
            if (block_size <= 63488) {
              return -0.2003397 + 0.0001950443 * num_dpus + 1.731084e-05 * block_size + 3.797918e-09 * (double)num_dpus * block_size;
            } else {
              return 0.01507134 + 7.255821e-05 * num_dpus + 4.623074e-07 * block_size + 1.21575e-08 * (double)num_dpus * block_size;
            }
          }
        }
      }
    }
  }
}
} // namespace

double upmem_cm::scatterSharedCostMs(int num_dpus, int block_size) {
  // No transfer beats the fastest rate this sweep measured (1.09886e+08 B/ms);
  // outside its fitted region a leaf's linear model can slope below zero.
  return std::fmax(scatterSharedCostMsModel(num_dpus, block_size), (double)num_dpus * block_size / 1.098861e+08);
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

// Fitted by model_tree.py on 20080 measured configs (upmemcm/scatter_cost/plots/gather), relRMSE 7.54%.
namespace {
double gatherCostMsModel(int num_dpus, int block_size) {
  if ((double)num_dpus * block_size <= 63700992) {
    if (block_size <= 928) {
      if (num_dpus <= 448) {
        if (num_dpus <= 128) {
          if (num_dpus <= 72) {
            if ((double)num_dpus * block_size <= 16704) {
              return -0.09237899 + 0.001891463 * num_dpus + -4.911007e-05 * block_size + 1.780432e-06 * (double)num_dpus * block_size;
            } else {
              return -0.08673185 + 0.00181376 * num_dpus + -2.112057e-05 * block_size + 1.356446e-06 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 112) {
              return 0.0365699 + 2.64976e-05 * num_dpus + 6.188797e-07 * block_size + 1.03757e-06 * (double)num_dpus * block_size;
            } else {
              return 0.1141685 + -0.0005832857 * num_dpus + 2.531794e-05 * block_size + 8.321395e-07 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (num_dpus <= 384) {
            if (num_dpus <= 336) {
              return 0.03099095 + 8.021548e-05 * num_dpus + 9.270775e-05 * block_size + 1.159126e-07 * (double)num_dpus * block_size;
            } else {
              return 0.05244004 + 3.093292e-05 * num_dpus + 0.0007215566 * block_size + -1.55486e-06 * (double)num_dpus * block_size;
            }
          } else {
            if ((double)num_dpus * block_size <= 153600) {
              return 0.04158106 + 8.44825e-05 * num_dpus + -2.344378e-05 * block_size + 4.772381e-07 * (double)num_dpus * block_size;
            } else {
              return -0.02718846 + 0.000330878 * num_dpus + 8.656412e-05 * block_size + -2.618668e-09 * (double)num_dpus * block_size;
            }
          }
        }
      } else {
        if (block_size <= 224) {
          if (num_dpus <= 992) {
            if ((double)num_dpus * block_size <= 20800) {
              return 0.04227124 + 0.000116061 * num_dpus + 0.0005574041 * block_size + -9.927099e-07 * (double)num_dpus * block_size;
            } else {
              return 0.04960054 + 9.100578e-05 * num_dpus + 0.0002140591 * block_size + 4.724881e-08 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 1664) {
              return 0.05511519 + 7.456811e-05 * num_dpus + 0.0003326474 * block_size + -8.392613e-08 * (double)num_dpus * block_size;
            } else {
              return -0.07793093 + 0.000144797 * num_dpus + 3.736251e-05 * block_size + 1.071721e-07 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (num_dpus <= 640) {
            if (num_dpus <= 576) {
              return -0.009759297 + 0.0002825747 * num_dpus + 0.0002998704 * block_size + -4.355155e-07 * (double)num_dpus * block_size;
            } else {
              return 0.6392536 + -0.0008044793 * num_dpus + -3.316915e-05 * block_size + 2.193577e-07 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 1792) {
              return 0.1481105 + 1.186596e-05 * num_dpus + -1.054829e-05 * block_size + 1.712252e-07 * (double)num_dpus * block_size;
            } else {
              return -0.1260176 + 0.0001718128 * num_dpus + 0.001631195 * block_size + -6.939276e-07 * (double)num_dpus * block_size;
            }
          }
        }
      }
    } else {
      if ((double)num_dpus * block_size <= 25690112) {
        if (block_size <= 1920) {
          if (num_dpus <= 76) {
            return 0.02015017 + 0.0004531651 * num_dpus + -0.0002083948 * block_size + 3.62542e-06 * (double)num_dpus * block_size;
          } else {
            if (block_size <= 1024) {
              return 0.6343111 + 0.0005187448 * num_dpus + -0.0005288147 * block_size + -3.434209e-07 * (double)num_dpus * block_size;
            } else {
              return 0.03925123 + 0.0001018437 * num_dpus + 4.961874e-05 * block_size + 5.886711e-08 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (num_dpus <= 192) {
            if ((double)num_dpus * block_size <= 393216) {
              return 0.03758488 + 0.0002175837 * num_dpus + 2.194162e-05 * block_size + 4.264757e-08 * (double)num_dpus * block_size;
            } else {
              return 0.1090194 + -5.897348e-05 * num_dpus + 1.569923e-05 * block_size + 1.215356e-08 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 640) {
              return 0.1278008 + -3.383688e-06 * num_dpus + 1.507565e-05 * block_size + 3.104695e-08 * (double)num_dpus * block_size;
            } else {
              return 0.1047648 + 6.744191e-05 * num_dpus + 1.181616e-05 * block_size + 4.685435e-08 * (double)num_dpus * block_size;
            }
          }
        }
      } else {
        if (num_dpus <= 640) {
          if ((double)num_dpus * block_size <= 37355520) {
            if (num_dpus <= 336) {
              return -2.008008 + 0.005438831 * num_dpus + 2.182328e-05 * block_size + 1.886274e-08 * (double)num_dpus * block_size;
            } else {
              return 3.629688 + -0.006617202 * num_dpus + -4.380481e-05 * block_size + 1.415078e-07 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 384) {
              return -2.340617 + -0.0004496539 * num_dpus + 1.99946e-05 * block_size + 6.882773e-08 * (double)num_dpus * block_size;
            } else {
              return -0.9217827 + 0.0001252642 * num_dpus + 1.721305e-05 * block_size + 4.905503e-08 * (double)num_dpus * block_size;
            }
          }
        } else {
          if ((double)num_dpus * block_size <= 49020928) {
            if (num_dpus <= 1280) {
              return -0.1428809 + -2.462918e-05 * num_dpus + 2.012912e-05 * block_size + 4.717118e-08 * (double)num_dpus * block_size;
            } else {
              return -0.7241886 + 0.0004995029 * num_dpus + 4.220827e-05 * block_size + 3.564147e-08 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 1280) {
              return -2.485454 + -0.0003960775 * num_dpus + 2.583041e-05 * block_size + 9.5315e-08 * (double)num_dpus * block_size;
            } else {
              return -6.468846 + 0.002598486 * num_dpus + 0.0001496637 * block_size + 1.547298e-08 * (double)num_dpus * block_size;
            }
          }
        }
      }
    }
  } else {
    if (block_size <= 262144) {
      if (num_dpus <= 1920) {
        if ((double)num_dpus * block_size <= 144703488) {
          if (num_dpus <= 800) {
            if (num_dpus <= 640) {
              return -8.186374 + 0.00127672 * num_dpus + 1.265125e-05 * block_size + 1.687017e-07 * (double)num_dpus * block_size;
            } else {
              return -4.403476 + 0.002000434 * num_dpus + 0.0001089346 * block_size + -3.054228e-09 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 1280) {
              return -8.908733 + 0.001519851 * num_dpus + 5.994269e-05 * block_size + 1.337567e-07 * (double)num_dpus * block_size;
            } else {
              return -8.86694 + 0.002037105 * num_dpus + 0.0001160192 * block_size + 8.873731e-08 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (num_dpus <= 896) {
            return -48.94133 + 0.05986829 * num_dpus + 0.0001984229 * block_size + -1.197822e-07 * (double)num_dpus * block_size;
          } else {
            if (num_dpus <= 1408) {
              return -7.399006 + 0.009483069 * num_dpus + 0.0001248875 * block_size + 6.890576e-09 * (double)num_dpus * block_size;
            } else {
              return -1.095638 + 0.0005659294 * num_dpus + 6.750139e-05 * block_size + 8.678535e-08 * (double)num_dpus * block_size;
            }
          }
        }
      } else {
        return -40.07948 + 0.01842197 * num_dpus + 0.002076671 * block_size + -9.132361e-07 * (double)num_dpus * block_size;
      }
    } else {
      if (num_dpus <= 368) {
        if ((double)num_dpus * block_size <= 176160768) {
          if (num_dpus <= 288) {
            if (num_dpus <= 108) {
              return -1.536514 + -0.008023394 * num_dpus + 1.593079e-05 * block_size + 1.191522e-07 * (double)num_dpus * block_size;
            } else {
              return -3.66512 + 0.002582981 * num_dpus + 1.941681e-05 * block_size + 8.288549e-08 * (double)num_dpus * block_size;
            }
          } else {
            return -5.180425 + 0.01799142 * num_dpus + -2.477252e-05 * block_size + 1.82811e-07 * (double)num_dpus * block_size;
          }
        } else {
          if (num_dpus <= 224) {
            if (num_dpus <= 208) {
              return 4.342086 + -0.01318065 * num_dpus + 1.819121e-05 * block_size + 6.393968e-08 * (double)num_dpus * block_size;
            } else {
              return -133.7593 + 0.6171878 * num_dpus + -3.728712e-05 * block_size + 3.150296e-07 * (double)num_dpus * block_size;
            }
          } else {
            if ((double)num_dpus * block_size <= 229113856) {
              return -31.99648 + 0.1264397 * num_dpus + 7.119184e-05 * block_size + -1.551536e-07 * (double)num_dpus * block_size;
            } else {
              return 2.644917 + -0.01829281 * num_dpus + 1.218704e-05 * block_size + 8.507191e-08 * (double)num_dpus * block_size;
            }
          }
        }
      } else {
        if (num_dpus <= 608) {
          if (block_size <= 622592) {
            if (num_dpus <= 496) {
              return -41.81508 + 0.1030833 * num_dpus + 0.0001152606 * block_size + -1.46389e-07 * (double)num_dpus * block_size;
            } else {
              return -155.3574 + 0.2875146 * num_dpus + 0.0004801871 * block_size + -7.356511e-07 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 496) {
              return 20.0849 + -0.06097437 * num_dpus + -4.700079e-06 * block_size + 1.368184e-07 * (double)num_dpus * block_size;
            } else {
              return 213.6846 + -0.3802637 * num_dpus + -0.0001517913 * block_size + 3.874514e-07 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (num_dpus <= 960) {
            if (num_dpus <= 864) {
              return -40.29478 + 0.05327577 * num_dpus + 3.710439e-05 * block_size + 6.824333e-08 * (double)num_dpus * block_size;
            } else {
              return -209.2783 + 0.2308544 * num_dpus + -8.489032e-05 * block_size + 2.095989e-07 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 1024) {
              return -70.90324 + 0.08053857 * num_dpus + -0.0003998882 * block_size + 4.917072e-07 * (double)num_dpus * block_size;
            } else {
              return -25.96767 + 0.02122122 * num_dpus + 8.58382e-05 * block_size + 4.504653e-08 * (double)num_dpus * block_size;
            }
          }
        }
      }
    }
  }
}
} // namespace

double upmem_cm::gatherCostMs(int num_dpus, int block_size) {
  // No transfer beats the fastest rate this sweep measured (1.9204e+07 B/ms);
  // outside its fitted region a leaf's linear model can slope below zero.
  return std::fmax(gatherCostMsModel(num_dpus, block_size), (double)num_dpus * block_size / 1.920401e+07);
}

// Fitted by model_tree.py on 20080 measured configs (upmemcm/scatter_cost/plots/broadcast), relRMSE 4.66%.
namespace {
double broadcastCostMsModel(int num_dpus, int block_size) {
  if (num_dpus <= 640) {
    if (block_size <= 1024) {
      if (num_dpus <= 76) {
        if (num_dpus <= 68) {
          if (block_size <= 256) {
            return -0.003925735 + 0.0003691135 * num_dpus + 4.796555e-05 * block_size + -5.696857e-07 * (double)num_dpus * block_size;
          } else {
            return -0.002789681 + 0.0003474176 * num_dpus + 8.728722e-05 * block_size + -1.128548e-06 * (double)num_dpus * block_size;
          }
        } else {
          if (block_size <= 256) {
            return 0.01344939 + 0.0001523163 * num_dpus + -9.592668e-05 * block_size + 1.517017e-06 * (double)num_dpus * block_size;
          } else {
            return 0.0188319 + 8.466985e-05 * num_dpus + 2.716768e-06 * block_size + 1.09141e-07 * (double)num_dpus * block_size;
          }
        }
      } else {
        if (num_dpus <= 108) {
          if (num_dpus <= 92) {
            if (num_dpus <= 84) {
              return 0.02298817 + -2.663901e-05 * num_dpus + -6.455049e-05 * block_size + 9.310799e-07 * (double)num_dpus * block_size;
            } else {
              return 0.0235908 + 6.957623e-06 * num_dpus + 1.368923e-06 * block_size + 1.046019e-07 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 100) {
              return -0.09483075 + 0.001197989 * num_dpus + 0.0001664356 * block_size + -1.576802e-06 * (double)num_dpus * block_size;
            } else {
              return -0.05739352 + 0.0007822445 * num_dpus + 1.090783e-07 * block_size + 9.658848e-08 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (block_size <= 928) {
            if (num_dpus <= 144) {
              return 0.008298291 + 0.000128657 * num_dpus + 2.881185e-05 * block_size + -1.139453e-07 * (double)num_dpus * block_size;
            } else {
              return 0.01976348 + 3.374135e-05 * num_dpus + 1.399082e-05 * block_size + 7.71681e-09 * (double)num_dpus * block_size;
            }
          } else {
            return 0.1197433 + 0.0001066431 * num_dpus + -8.881906e-05 * block_size + -6.761309e-08 * (double)num_dpus * block_size;
          }
        }
      }
    } else {
      if (num_dpus <= 384) {
        if (block_size <= 29696) {
          if (block_size <= 1920) {
            if (num_dpus <= 112) {
              return 0.02310021 + -1.741432e-05 * num_dpus + 7.779525e-06 * block_size + 1.257231e-08 * (double)num_dpus * block_size;
            } else {
              return 0.01911778 + 3.29544e-05 * num_dpus + 8.471465e-06 * block_size + 5.179878e-09 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 160) {
              return 0.01854989 + 4.9368e-05 * num_dpus + 4.5125e-06 * block_size + 4.772521e-10 * (double)num_dpus * block_size;
            } else {
              return 0.0218747 + 2.342954e-05 * num_dpus + 5.117036e-06 * block_size + 3.321293e-09 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (num_dpus <= 208) {
            if (num_dpus <= 168) {
              return 0.01977754 + 4.5497e-05 * num_dpus + 4.484707e-06 * block_size + 3.604181e-10 * (double)num_dpus * block_size;
            } else {
              return 0.2992618 + -0.001319666 * num_dpus + 3.827287e-06 * block_size + 3.778854e-09 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 224) {
              return -0.2378476 + 0.001490931 * num_dpus + -8.330894e-06 * block_size + 5.891654e-08 * (double)num_dpus * block_size;
            } else {
              return -0.007658919 + 0.0002909605 * num_dpus + 4.028883e-06 * block_size + 1.940787e-09 * (double)num_dpus * block_size;
            }
          }
        }
      } else {
        if ((double)num_dpus * block_size <= 58720256) {
          if (block_size <= 1920) {
            if (num_dpus <= 480) {
              return 0.01031451 + 5.032651e-05 * num_dpus + 2.792058e-05 * block_size + -3.797751e-08 * (double)num_dpus * block_size;
            } else {
              return 0.004250235 + 5.89619e-05 * num_dpus + 2.327838e-05 * block_size + -2.184365e-08 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 416) {
              return 0.03914837 + -1.121206e-05 * num_dpus + 1.295129e-05 * block_size + -1.666789e-08 * (double)num_dpus * block_size;
            } else {
              return 0.01578947 + 3.803565e-05 * num_dpus + 6.243117e-06 * block_size + 2.167737e-10 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (num_dpus <= 416) {
            if ((double)num_dpus * block_size <= 251658240) {
              return 2.55488 + -0.006075771 * num_dpus + 1.101408e-05 * block_size + -1.341404e-08 * (double)num_dpus * block_size;
            } else {
              return 6.906619 + -0.01495583 * num_dpus + 1.645322e-06 * block_size + 7.304271e-09 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 448) {
              return 5.612395 + -0.01236535 * num_dpus + -2.313295e-05 * block_size + 6.548278e-08 * (double)num_dpus * block_size;
            } else {
              return 0.4736967 + -0.0007355903 * num_dpus + 2.398261e-06 * block_size + 6.865112e-09 * (double)num_dpus * block_size;
            }
          }
        }
      }
    }
  } else {
    if (num_dpus <= 1280) {
      if (block_size <= 1024) {
        if (block_size <= 864) {
          if (num_dpus <= 1024) {
            if (num_dpus <= 704) {
              return 0.1010589 + -7.144345e-05 * num_dpus + 4.377318e-05 * block_size + -1.095531e-08 * (double)num_dpus * block_size;
            } else {
              return 0.01421925 + 5.316763e-05 * num_dpus + 3.998411e-05 * block_size + -4.418947e-09 * (double)num_dpus * block_size;
            }
          } else {
            if ((double)num_dpus * block_size <= 428544) {
              return 0.026912 + 4.116555e-05 * num_dpus + 0.0002532363 * block_size + -1.829154e-07 * (double)num_dpus * block_size;
            } else {
              return 0.0006824386 + 6.045721e-05 * num_dpus + 0.0001257305 * block_size + -7.287926e-08 * (double)num_dpus * block_size;
            }
          }
        } else {
          return 0.1457255 + 3.663345e-05 * num_dpus + -0.0001006036 * block_size + 1.251822e-08 * (double)num_dpus * block_size;
        }
      } else {
        if (block_size <= 1920) {
          if (num_dpus <= 896) {
            return 0.01170386 + 5.570849e-05 * num_dpus + 1.791916e-05 * block_size + 2.511109e-09 * (double)num_dpus * block_size;
          } else {
            return 0.02920835 + 3.11493e-05 * num_dpus + 2.362987e-05 * block_size + 2.456701e-09 * (double)num_dpus * block_size;
          }
        } else {
          if ((double)num_dpus * block_size <= 905969664) {
            if (num_dpus <= 896) {
              return -0.003670874 + 8.092835e-05 * num_dpus + 9.143499e-06 * block_size + 2.465954e-09 * (double)num_dpus * block_size;
            } else {
              return 0.04458672 + 2.202985e-05 * num_dpus + 9.181072e-06 * block_size + 2.995692e-09 * (double)num_dpus * block_size;
            }
          } else {
            return -15.92615 + -0.006789277 * num_dpus + -1.351485e-06 * block_size + 3.707461e-08 * (double)num_dpus * block_size;
          }
        }
      }
    } else {
      if ((double)num_dpus * block_size <= 1075838976) {
        if (block_size <= 1920) {
          if (block_size <= 1024) {
            if (num_dpus <= 1600) {
              return -0.05805541 + 0.0001032861 * num_dpus + 8.270431e-05 * block_size + -2.420146e-08 * (double)num_dpus * block_size;
            } else {
              return -0.01028549 + 7.16364e-05 * num_dpus + -5.427174e-05 * block_size + 5.661438e-08 * (double)num_dpus * block_size;
            }
          } else {
            if (num_dpus <= 1728) {
              return -0.002541881 + 6.218259e-05 * num_dpus + 1.767787e-05 * block_size + 9.65427e-09 * (double)num_dpus * block_size;
            } else {
              return 0.1127828 + 2.450923e-06 * num_dpus + -0.0001458827 * block_size + 9.691618e-08 * (double)num_dpus * block_size;
            }
          }
        } else {
          if (num_dpus <= 1856) {
            if (num_dpus <= 1408) {
              return -0.0215512 + 7.40172e-05 * num_dpus + 1.708937e-05 * block_size + 1.175017e-10 * (double)num_dpus * block_size;
            } else {
              return -0.0007004522 + 6.419431e-05 * num_dpus + 1.001127e-05 * block_size + 5.532308e-09 * (double)num_dpus * block_size;
            }
          } else {
            if (block_size <= 13312) {
              return -0.1616268 + 0.0001470247 * num_dpus + 1.157927e-05 * block_size + 5.917734e-09 * (double)num_dpus * block_size;
            } else {
              return 0.2811894 + -7.414187e-05 * num_dpus + -1.102595e-06 * block_size + 1.28523e-08 * (double)num_dpus * block_size;
            }
          }
        }
      } else {
        if (num_dpus <= 1728) {
          return -3.869563 + 3.12057e-05 * num_dpus + 1.583815e-05 * block_size + 7.76875e-09 * (double)num_dpus * block_size;
        } else {
          return -10.26831 + 0.005230164 * num_dpus + 3.449005e-05 * block_size + -4.530885e-09 * (double)num_dpus * block_size;
        }
      }
    }
  }
}
} // namespace

double upmem_cm::broadcastCostMs(int num_dpus, int block_size) {
  // No transfer beats the fastest rate this sweep measured (1.04147e+08 B/ms);
  // outside its fitted region a leaf's linear model can slope below zero.
  return std::fmax(broadcastCostMsModel(num_dpus, block_size), (double)num_dpus * block_size / 1.041472e+08);
}

// Fitted by model_tree.py on 67748 measured configs (upmemcm/scatter_cost/plots/sg), relRMSE 8.07%.
namespace {
double scatterSgTreeCostMsModel(int num_dpus, int blocks_per_dpu, int block_size) {
  if ((double)num_dpus * blocks_per_dpu * block_size <= 40370176) {
    if (num_dpus <= 384) {
      if (num_dpus <= 256) {
        if ((double)blocks_per_dpu * block_size <= 1920) {
          if ((double)blocks_per_dpu * block_size <= 960) {
            if (num_dpus <= 72) {
              return 0.0528489 + -0.0003451669 * num_dpus +
                     0.002896068 * blocks_per_dpu + -2.680392e-05 * block_size +
                     -3.26478e-05 * (double)num_dpus * blocks_per_dpu +
                     4.558021e-07 * (double)num_dpus * block_size +
                     0.0001368423 * (double)blocks_per_dpu * block_size +
                     -1.704379e-06 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return 0.02624105 + 4.10616e-05 * num_dpus +
                     0.0005539885 * blocks_per_dpu + 5.525619e-06 * block_size +
                     2.466294e-06 * (double)num_dpus * blocks_per_dpu +
                     -5.625687e-09 * (double)num_dpus * block_size +
                     1.590468e-05 * (double)blocks_per_dpu * block_size +
                     4.608538e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          } else {
            if ((double)blocks_per_dpu * block_size <= 1040) {
              return 0.08839743 + 0.001849524 * num_dpus +
                     0.0005010446 * blocks_per_dpu +
                     -1.811157e-06 * block_size +
                     1.817613e-06 * (double)num_dpus * blocks_per_dpu +
                     -2.221253e-09 * (double)num_dpus * block_size +
                     -4.625106e-05 * (double)blocks_per_dpu * block_size +
                     -1.757594e-06 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return 0.02520957 + 2.900119e-05 * num_dpus +
                     0.0005050062 * blocks_per_dpu +
                     -8.239019e-07 * block_size +
                     2.934521e-06 * (double)num_dpus * blocks_per_dpu +
                     2.439231e-08 * (double)num_dpus * block_size +
                     1.435276e-05 * (double)blocks_per_dpu * block_size +
                     -4.068874e-09 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          }
        } else {
          if ((double)num_dpus * blocks_per_dpu * block_size <= 21626880) {
            if (num_dpus <= 208) {
              return 0.02269056 + 4.563125e-05 * num_dpus +
                     0.0004977377 * blocks_per_dpu +
                     -8.174117e-07 * block_size +
                     1.892119e-06 * (double)num_dpus * blocks_per_dpu +
                     1.091827e-08 * (double)num_dpus * block_size +
                     7.50514e-06 * (double)blocks_per_dpu * block_size +
                     -2.171522e-09 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return -0.0310345 + 0.0002675548 * num_dpus +
                     0.006129593 * blocks_per_dpu + -8.626763e-07 * block_size +
                     -2.091359e-05 * (double)num_dpus * blocks_per_dpu +
                     1.750735e-08 * (double)num_dpus * block_size +
                     1.523909e-06 * (double)blocks_per_dpu * block_size +
                     2.357384e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          } else {
            if (num_dpus <= 128) {
              return -4.22412 + 0.02853826 * num_dpus +
                     -0.0001357346 * blocks_per_dpu +
                     -2.964252e-08 * block_size +
                     1.536905e-05 * (double)num_dpus * blocks_per_dpu +
                     1.261756e-09 * (double)num_dpus * block_size +
                     1.98841e-05 * (double)blocks_per_dpu * block_size +
                     -6.792419e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return -2.10994 + 0.007199934 * num_dpus +
                     0.007073403 * blocks_per_dpu + 1.327513e-06 * block_size +
                     -3.126279e-05 * (double)num_dpus * blocks_per_dpu +
                     -6.582971e-09 * (double)num_dpus * block_size +
                     2.020499e-05 * (double)blocks_per_dpu * block_size +
                     -3.500524e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          }
        }
      } else {
        if ((double)num_dpus * blocks_per_dpu * block_size <= 21626880) {
          if ((double)blocks_per_dpu * block_size <= 1920) {
            if ((double)blocks_per_dpu * block_size <= 960) {
              return 0.04950043 + 1.219872e-05 * num_dpus +
                     -0.002586715 * blocks_per_dpu +
                     -2.939763e-05 * block_size +
                     1.067727e-05 * (double)num_dpus * blocks_per_dpu +
                     8.884081e-08 * (double)num_dpus * block_size +
                     4.170326e-05 * (double)blocks_per_dpu * block_size +
                     3.948592e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return 0.08398328 + -9.595269e-05 * num_dpus +
                     -0.005905376 * blocks_per_dpu +
                     -9.019205e-05 * block_size +
                     2.050251e-05 * (double)num_dpus * blocks_per_dpu +
                     3.002854e-07 * (double)num_dpus * block_size +
                     3.817786e-05 * (double)blocks_per_dpu * block_size +
                     -3.79078e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          } else {
            if ((double)num_dpus * blocks_per_dpu <= 4480) {
              return 0.005608499 + 0.0001240327 * num_dpus +
                     0.003740741 * blocks_per_dpu + -1.746643e-05 * block_size +
                     -9.042771e-06 * (double)num_dpus * blocks_per_dpu +
                     6.216732e-08 * (double)num_dpus * block_size +
                     1.99469e-05 * (double)blocks_per_dpu * block_size +
                     -1.364458e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return 0.0753389 + -7.133527e-05 * num_dpus +
                     -0.003109645 * blocks_per_dpu + 0.001652809 * block_size +
                     1.124943e-05 * (double)num_dpus * blocks_per_dpu +
                     -4.444574e-06 * (double)num_dpus * block_size +
                     -7.294132e-05 * (double)blocks_per_dpu * block_size +
                     2.425563e-07 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          }
        } else {
          if (blocks_per_dpu <= 20) {
            if ((double)num_dpus * blocks_per_dpu <= 1760) {
              return -0.3849091 + -0.006115543 * num_dpus +
                     -1.612252 * blocks_per_dpu + -5.762456e-05 * block_size +
                     0.005406797 * (double)num_dpus * blocks_per_dpu +
                     1.881747e-07 * (double)num_dpus * block_size +
                     7.204987e-05 * (double)blocks_per_dpu * block_size +
                     -8.39454e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return -3.392478 + 0.004291589 * num_dpus +
                     -0.04377837 * blocks_per_dpu + -0.0002017547 * block_size +
                     9.819926e-05 * (double)num_dpus * blocks_per_dpu +
                     5.415197e-07 * (double)num_dpus * block_size +
                     6.972992e-05 * (double)blocks_per_dpu * block_size +
                     -5.733748e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          } else {
            return 17.91269 + -0.0567442 * num_dpus +
                   -0.3251535 * blocks_per_dpu + -0.001701819 * block_size +
                   0.0009273534 * (double)num_dpus * blocks_per_dpu +
                   4.281742e-06 * (double)num_dpus * block_size +
                   -0.0001016464 * (double)blocks_per_dpu * block_size +
                   4.48202e-07 * (double)num_dpus * blocks_per_dpu * block_size;
          }
        }
      }
    } else {
      if ((double)blocks_per_dpu * block_size <= 1920) {
        if ((double)blocks_per_dpu * block_size <= 960) {
          if (num_dpus <= 512) {
            if (num_dpus <= 448) {
              return 0.04457584 + 6.411336e-05 * num_dpus +
                     -0.001561561 * blocks_per_dpu + 1.747699e-07 * block_size +
                     6.647912e-06 * (double)num_dpus * blocks_per_dpu +
                     1.559835e-08 * (double)num_dpus * block_size +
                     -9.063277e-05 * (double)blocks_per_dpu * block_size +
                     3.894109e-07 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return 0.06874369 + 3.855846e-05 * num_dpus +
                     -0.006309259 * blocks_per_dpu +
                     -4.631373e-05 * block_size +
                     1.612402e-05 * (double)num_dpus * blocks_per_dpu +
                     1.217934e-07 * (double)num_dpus * block_size +
                     -0.0001260236 * (double)blocks_per_dpu * block_size +
                     4.540015e-07 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          } else {
            if (blocks_per_dpu <= 7) {
              return 0.07534909 + 4.351929e-05 * num_dpus +
                     -0.000527347 * blocks_per_dpu + 2.881142e-05 * block_size +
                     9.134086e-07 * (double)num_dpus * blocks_per_dpu +
                     -1.73017e-08 * (double)num_dpus * block_size +
                     0.0001008714 * (double)blocks_per_dpu * block_size +
                     1.251878e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return 0.05272231 + 4.779588e-05 * num_dpus +
                     0.002208787 * blocks_per_dpu + 1.056847e-05 * block_size +
                     2.901514e-07 * (double)num_dpus * blocks_per_dpu +
                     2.139766e-08 * (double)num_dpus * block_size +
                     0.0001192195 * (double)blocks_per_dpu * block_size +
                     -4.88301e-09 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          }
        } else {
          if (num_dpus <= 448) {
            if ((double)num_dpus * block_size <= 135168) {
              return 0.01902689 + 0.0001221287 * num_dpus +
                     -0.00219902 * blocks_per_dpu + 3.369867e-06 * block_size +
                     7.990151e-06 * (double)num_dpus * blocks_per_dpu +
                     -4.204894e-08 * (double)num_dpus * block_size +
                     5.059953e-05 * (double)blocks_per_dpu * block_size +
                     -2.600714e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return -0.244493 + 0.000757065 * num_dpus +
                     0.1679928 * blocks_per_dpu + 0.0004880775 * block_size +
                     -0.0004081529 * (double)num_dpus * blocks_per_dpu +
                     -1.187133e-06 * (double)num_dpus * block_size +
                     -0.0003779296 * (double)blocks_per_dpu * block_size +
                     1.019844e-06 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          } else {
            if (num_dpus <= 1024) {
              return 0.05379194 + 5.659502e-05 * num_dpus +
                     0.00107359 * blocks_per_dpu + -2.421258e-05 * block_size +
                     9.597556e-07 * (double)num_dpus * blocks_per_dpu +
                     5.573394e-08 * (double)num_dpus * block_size +
                     5.463885e-05 * (double)blocks_per_dpu * block_size +
                     -1.225177e-09 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return 0.09599329 + 2.237112e-05 * num_dpus +
                     0.0006471778 * blocks_per_dpu + 2.004074e-05 * block_size +
                     4.987397e-07 * (double)num_dpus * blocks_per_dpu +
                     -1.246407e-08 * (double)num_dpus * block_size +
                     6.912134e-07 * (double)blocks_per_dpu * block_size +
                     4.449696e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          }
        }
      } else {
        if (num_dpus <= 640) {
          if ((double)num_dpus * blocks_per_dpu * block_size <= 21626880) {
            if (num_dpus <= 448) {
              return -0.03379144 + 0.0002120397 * num_dpus +
                     0.003457101 * blocks_per_dpu + -3.051344e-05 * block_size +
                     -5.646969e-06 * (double)num_dpus * blocks_per_dpu +
                     7.630901e-08 * (double)num_dpus * block_size +
                     3.655694e-05 * (double)blocks_per_dpu * block_size +
                     -3.009943e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return 0.04442373 + 4.884543e-05 * num_dpus +
                     2.42211e-05 * blocks_per_dpu + 4.353853e-06 * block_size +
                     3.08355e-06 * (double)num_dpus * blocks_per_dpu +
                     -5.54172e-09 * (double)num_dpus * block_size +
                     1.454609e-05 * (double)blocks_per_dpu * block_size +
                     3.382893e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          } else {
            if (num_dpus <= 448) {
              return -3.94975 + 0.004100355 * num_dpus +
                     0.03922537 * blocks_per_dpu + 5.864821e-06 * block_size +
                     -8.574072e-05 * (double)num_dpus * blocks_per_dpu +
                     -1.295077e-08 * (double)num_dpus * block_size +
                     4.79087e-05 * (double)blocks_per_dpu * block_size +
                     4.502768e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return -3.530828 + 0.002249787 * num_dpus +
                     0.04295763 * blocks_per_dpu + 3.034098e-05 * block_size +
                     -6.293769e-05 * (double)num_dpus * blocks_per_dpu +
                     -5.531238e-08 * (double)num_dpus * block_size +
                     4.974396e-05 * (double)blocks_per_dpu * block_size +
                     7.355317e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          }
        } else {
          if ((double)blocks_per_dpu * block_size <= 30720) {
            if (num_dpus <= 1024) {
              return 0.07319207 + 1.860098e-05 * num_dpus +
                     -0.002428082 * blocks_per_dpu + 1.471209e-05 * block_size +
                     4.204896e-06 * (double)num_dpus * blocks_per_dpu +
                     -1.221532e-08 * (double)num_dpus * block_size +
                     2.766512e-05 * (double)blocks_per_dpu * block_size +
                     1.859975e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return 0.04184126 + 4.033594e-05 * num_dpus +
                     0.001696298 * blocks_per_dpu + 3.883604e-05 * block_size +
                     -8.296314e-09 * (double)num_dpus * blocks_per_dpu +
                     -2.162287e-08 * (double)num_dpus * block_size +
                     1.562074e-05 * (double)blocks_per_dpu * block_size +
                     2.210922e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          } else {
            if (num_dpus <= 768) {
              return -1.309148 + -0.0007230108 * num_dpus +
                     0.2726337 * blocks_per_dpu + 0.0002281192 * block_size +
                     -0.0003662582 * (double)num_dpus * blocks_per_dpu +
                     -2.977724e-07 * (double)num_dpus * block_size +
                     -0.0001397592 * (double)blocks_per_dpu * block_size +
                     3.234784e-07 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return -12.19845 + 0.01104684 * num_dpus +
                     0.09064492 * blocks_per_dpu + 2.598746e-05 * block_size +
                     -8.367699e-05 * (double)num_dpus * blocks_per_dpu +
                     -2.267482e-08 * (double)num_dpus * block_size +
                     0.0003781615 * (double)blocks_per_dpu * block_size +
                     -2.911785e-07 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          }
        }
      }
    }
  } else {
    if (num_dpus <= 384) {
      if (num_dpus <= 256) {
        if ((double)num_dpus * blocks_per_dpu * block_size <= 183500800) {
          if (num_dpus <= 72) {
            if ((double)blocks_per_dpu * block_size <= 917504) {
              return 0.2090974 + -0.05193688 * num_dpus +
                     0.03040631 * blocks_per_dpu + 1.139261e-06 * block_size +
                     -0.0003967404 * (double)num_dpus * blocks_per_dpu +
                     -1.522471e-08 * (double)num_dpus * block_size +
                     3.283293e-05 * (double)blocks_per_dpu * block_size +
                     -2.22543e-07 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return 9.070509 + -0.1878435 * num_dpus +
                     -0.06347531 * blocks_per_dpu + 2.40563e-06 * block_size +
                     0.001111639 * (double)num_dpus * blocks_per_dpu +
                     -3.501848e-08 * (double)num_dpus * block_size +
                     2.361084e-05 * (double)blocks_per_dpu * block_size +
                     -8.093404e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          } else {
            if (num_dpus <= 120) {
              return -3.241881 + -0.009204208 * num_dpus +
                     -0.002011781 * blocks_per_dpu +
                     -5.592264e-08 * block_size +
                     -2.673039e-05 * (double)num_dpus * blocks_per_dpu +
                     -6.063675e-09 * (double)num_dpus * block_size +
                     1.13912e-05 * (double)blocks_per_dpu * block_size +
                     1.041699e-07 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return -3.217608 + 0.00147188 * num_dpus +
                     0.004014747 * blocks_per_dpu + 1.590006e-06 * block_size +
                     -2.718449e-05 * (double)num_dpus * blocks_per_dpu +
                     -9.025288e-09 * (double)num_dpus * block_size +
                     1.535163e-05 * (double)blocks_per_dpu * block_size +
                     5.018715e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          }
        } else {
          if (num_dpus <= 128) {
            if (num_dpus <= 88) {
              return -5.2565 + -0.04552182 * num_dpus +
                     -0.1541359 * blocks_per_dpu + -5.574311e-06 * block_size +
                     0.00218679 * (double)num_dpus * blocks_per_dpu +
                     1.004076e-07 * (double)num_dpus * block_size +
                     1.338676e-05 * (double)blocks_per_dpu * block_size +
                     8.47576e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return 29.98416 + -0.2747076 * num_dpus +
                     0.6100279 * blocks_per_dpu + -2.415115e-07 * block_size +
                     -0.004266564 * (double)num_dpus * blocks_per_dpu +
                     1.237679e-08 * (double)num_dpus * block_size +
                     2.561292e-07 * (double)blocks_per_dpu * block_size +
                     1.636837e-07 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          } else {
            if (num_dpus <= 192) {
              return -0.7919073 + 0.01109236 * num_dpus +
                     0.4339263 * blocks_per_dpu + 1.744419e-06 * block_size +
                     -0.002594079 * (double)num_dpus * blocks_per_dpu +
                     -7.916223e-09 * (double)num_dpus * block_size +
                     1.680822e-05 * (double)blocks_per_dpu * block_size +
                     2.7481e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return 5.287654 + -0.02154375 * num_dpus +
                     0.1306551 * blocks_per_dpu + 7.495719e-06 * block_size +
                     -0.0006013838 * (double)num_dpus * blocks_per_dpu +
                     -3.637324e-08 * (double)num_dpus * block_size +
                     2.213753e-05 * (double)blocks_per_dpu * block_size +
                     4.123249e-09 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          }
        }
      } else {
        if (blocks_per_dpu <= 20) {
          if ((double)num_dpus * blocks_per_dpu * block_size <= 692060160) {
            if (blocks_per_dpu <= 12) {
              return -2.268062 + 0.003102495 * num_dpus +
                     0.04699061 * blocks_per_dpu + -9.234028e-08 * block_size +
                     -0.0001686624 * (double)num_dpus * blocks_per_dpu +
                     -1.308597e-09 * (double)num_dpus * block_size +
                     3.199439e-05 * (double)blocks_per_dpu * block_size +
                     4.0665e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return -15.41213 + 0.0393753 * num_dpus +
                     0.7794268 * blocks_per_dpu + 0.001634281 * block_size +
                     -0.00214307 * (double)num_dpus * blocks_per_dpu +
                     -4.526924e-06 * (double)num_dpus * block_size +
                     -5.038895e-05 * (double)blocks_per_dpu * block_size +
                     2.666201e-07 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          } else {
            if ((double)num_dpus * blocks_per_dpu <= 3840) {
              return 58.5267 + -0.1581512 * num_dpus +
                     -0.7478548 * blocks_per_dpu + 3.51212e-06 * block_size +
                     0.0008336497 * (double)num_dpus * blocks_per_dpu +
                     -1.480846e-08 * (double)num_dpus * block_size +
                     1.52934e-05 * (double)blocks_per_dpu * block_size +
                     8.671938e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return -740.4323 + 2.087869 * num_dpus +
                     56.2718 * blocks_per_dpu + 0.01049436 * block_size +
                     -0.1590071 * (double)num_dpus * blocks_per_dpu +
                     -2.924186e-05 * (double)num_dpus * block_size +
                     -0.000708967 * (double)blocks_per_dpu * block_size +
                     2.107626e-06 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          }
        } else {
          if (num_dpus <= 320) {
            if (blocks_per_dpu <= 24) {
              return -404.9805 + -0.01600804 * num_dpus +
                     16.4883 * blocks_per_dpu + -6.748102e-06 * block_size +
                     0.00170889 * (double)num_dpus * blocks_per_dpu +
                     3.300515e-08 * (double)num_dpus * block_size +
                     -0.0001590308 * (double)blocks_per_dpu * block_size +
                     6.359376e-07 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return 27.43144 + -0.08849954 * num_dpus +
                     -1.107992 * blocks_per_dpu + 0.0004286467 * block_size +
                     0.003429957 * (double)num_dpus * blocks_per_dpu +
                     -1.165213e-06 * (double)num_dpus * block_size +
                     -0.0001780967 * (double)blocks_per_dpu * block_size +
                     6.899622e-07 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          } else {
            if ((double)num_dpus * block_size <= 3604480) {
              return -46.69535 + 0.11327 * num_dpus +
                     1.819531 * blocks_per_dpu + 0.003170921 * block_size +
                     -0.004608956 * (double)num_dpus * blocks_per_dpu +
                     -7.413062e-06 * (double)num_dpus * block_size +
                     -0.0001100657 * (double)blocks_per_dpu * block_size +
                     3.921258e-07 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return -13.68705 + 0.03471284 * num_dpus +
                     0.4298974 * blocks_per_dpu + 5.017189e-05 * block_size +
                     -0.001165025 * (double)num_dpus * blocks_per_dpu +
                     -1.037955e-07 * (double)num_dpus * block_size +
                     3.264423e-05 * (double)blocks_per_dpu * block_size +
                     3.481601e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          }
        }
      }
    } else {
      if (num_dpus <= 448) {
        if ((double)num_dpus * blocks_per_dpu * block_size <= 54525952) {
          return 4.928665 + -0.01778338 * num_dpus +
                 -0.09288879 * blocks_per_dpu + -3.223665e-05 * block_size +
                 0.0002192128 * (double)num_dpus * blocks_per_dpu +
                 7.34642e-08 * (double)num_dpus * block_size +
                 -1.3859e-05 * (double)blocks_per_dpu * block_size +
                 2.103372e-07 * (double)num_dpus * blocks_per_dpu * block_size;
        } else {
          if (blocks_per_dpu <= 7) {
            if ((double)num_dpus * block_size <= 20971520) {
              return 56.0637 + -0.1372142 * num_dpus +
                     -9.073438 * blocks_per_dpu + -0.001867409 * block_size +
                     0.02175692 * (double)num_dpus * blocks_per_dpu +
                     4.47927e-06 * (double)num_dpus * block_size +
                     0.000332886 * (double)blocks_per_dpu * block_size +
                     -6.466819e-07 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return 1.418492 + -0.006045616 * num_dpus +
                     -1.629231 * blocks_per_dpu + -1.76422e-05 * block_size +
                     0.003876236 * (double)num_dpus * blocks_per_dpu +
                     4.230736e-08 * (double)num_dpus * block_size +
                     5.221943e-05 * (double)blocks_per_dpu * block_size +
                     2.581483e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          } else {
            if (blocks_per_dpu <= 10) {
              return -158.6772 + 0.3518992 * num_dpus +
                     16.21805 * blocks_per_dpu + 0.00637876 * block_size +
                     -0.03618049 * (double)num_dpus * blocks_per_dpu +
                     -1.419987e-05 * (double)num_dpus * block_size +
                     -0.0005949346 * (double)blocks_per_dpu * block_size +
                     1.467531e-06 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return 8.710281 + -0.02353225 * num_dpus +
                     -0.5902824 * blocks_per_dpu + -0.0004946397 * block_size +
                     0.001432067 * (double)num_dpus * blocks_per_dpu +
                     1.186889e-06 * (double)num_dpus * block_size +
                     7.242584e-05 * (double)blocks_per_dpu * block_size +
                     -2.27281e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          }
        }
      } else {
        if ((double)blocks_per_dpu * block_size <= 61440) {
          if (num_dpus <= 1280) {
            if ((double)num_dpus * blocks_per_dpu <= 4096) {
              return 2.107897 + -0.003033318 * num_dpus +
                     0.3228505 * blocks_per_dpu + -9.700036e-06 * block_size +
                     -0.0006356344 * (double)num_dpus * blocks_per_dpu +
                     -3.554742e-09 * (double)num_dpus * block_size +
                     2.579655e-05 * (double)blocks_per_dpu * block_size +
                     9.342997e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return -2.13504 + -0.0003939474 * num_dpus +
                     -0.0290965 * blocks_per_dpu + 1.006288e-05 * block_size +
                     5.72276e-05 * (double)num_dpus * blocks_per_dpu +
                     8.745479e-08 * (double)num_dpus * block_size +
                     0.0001275677 * (double)blocks_per_dpu * block_size +
                     -1.750218e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          } else {
            if ((double)num_dpus * blocks_per_dpu * block_size <= 54525952) {
              return -3.610766 + 0.001435355 * num_dpus +
                     -0.009539927 * blocks_per_dpu + 1.568379e-05 * block_size +
                     8.273238e-06 * (double)num_dpus * blocks_per_dpu +
                     -8.759591e-09 * (double)num_dpus * block_size +
                     0.0001033961 * (double)blocks_per_dpu * block_size +
                     2.042512e-09 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return -5.222653 + 0.001571635 * num_dpus +
                     -0.03882563 * blocks_per_dpu + 6.204568e-07 * block_size +
                     2.498636e-05 * (double)num_dpus * blocks_per_dpu +
                     -1.371389e-09 * (double)num_dpus * block_size +
                     9.965745e-05 * (double)blocks_per_dpu * block_size +
                     2.935092e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          }
        } else {
          if (num_dpus <= 832) {
            if ((double)blocks_per_dpu * block_size <= 114688) {
              return -5.964653 + 0.005794971 * num_dpus +
                     -0.01048586 * blocks_per_dpu + -1.939745e-05 * block_size +
                     2.018673e-05 * (double)num_dpus * blocks_per_dpu +
                     3.391083e-08 * (double)num_dpus * block_size +
                     9.388056e-05 * (double)blocks_per_dpu * block_size +
                     1.355669e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return -3.229824 + 0.00433061 * num_dpus +
                     0.05037924 * blocks_per_dpu + 2.37179e-06 * block_size +
                     -9.195564e-05 * (double)num_dpus * blocks_per_dpu +
                     -5.453391e-09 * (double)num_dpus * block_size +
                     5.73529e-05 * (double)blocks_per_dpu * block_size +
                     4.882024e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            }
          } else {
            if (num_dpus <= 1152) {
              return 0.4910587 + -0.001313946 * num_dpus +
                     0.04397318 * blocks_per_dpu + -4.674497e-05 * block_size +
                     -3.765372e-05 * (double)num_dpus * blocks_per_dpu +
                     4.442551e-08 * (double)num_dpus * block_size +
                     8.96167e-05 * (double)blocks_per_dpu * block_size +
                     1.211623e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
            } else {
              return -4.38048 + 0.002595485 * num_dpus +
                     0.05128217 * blocks_per_dpu + 1.159864e-05 * block_size +
                     -2.659998e-05 * (double)num_dpus * blocks_per_dpu +
                     -7.847291e-09 * (double)num_dpus * block_size +
                     8.111758e-05 * (double)blocks_per_dpu * block_size +
                     1.924975e-08 * (double)num_dpus * blocks_per_dpu *
                         block_size;
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

// Fitted by model_tree.py on 29585 measured configs (upmemcm/scatter_cost/plots/sg_gather), relRMSE 3.91%.
namespace {
double scatterSgGatherTreeCostMsModel(int num_dpus, int blocks_per_dpu, int block_size) {
  if ((double)num_dpus * blocks_per_dpu * block_size <= 28835840) {
    if ((double)blocks_per_dpu * block_size <= 960) {
      if (num_dpus <= 640) {
        if (num_dpus <= 80) {
          if (num_dpus <= 64) {
            if (blocks_per_dpu <= 2) {
              return 0.04283828 + -1.323274e-09 * num_dpus + -0.006886004 * blocks_per_dpu + 3.699724e-07 * block_size + -1.090024e-07 * (double)num_dpus * blocks_per_dpu + 5.78079e-09 * (double)num_dpus * block_size + 6.837773e-05 * (double)blocks_per_dpu * block_size + 1.068402e-06 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.02891649 + -6.562181e-09 * num_dpus + 0.0008799582 * blocks_per_dpu + 5.366976e-06 * block_size + 1.857599e-08 * (double)num_dpus * blocks_per_dpu + 8.385902e-08 * (double)num_dpus * block_size + 6.746557e-05 * (double)blocks_per_dpu * block_size + 1.05415e-06 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (blocks_per_dpu <= 2) {
              return 0.1580234 + -0.001680985 * num_dpus + -0.07540786 * blocks_per_dpu + -2.39721e-05 * block_size + 0.001003129 * (double)num_dpus * blocks_per_dpu + 3.324587e-07 * (double)num_dpus * block_size + 9.721806e-05 * (double)blocks_per_dpu * block_size + -1.550004e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.03836774 + -0.0001175584 * num_dpus + -0.002363977 * blocks_per_dpu + 1.95236e-05 * block_size + 3.955252e-05 * (double)num_dpus * blocks_per_dpu + -7.24623e-08 * (double)num_dpus * block_size + 9.34653e-05 * (double)blocks_per_dpu * block_size + -1.489744e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        } else {
          if (num_dpus <= 128) {
            if (num_dpus <= 112) {
              return 0.02335844 + 6.600876e-05 * num_dpus + 0.001003294 * blocks_per_dpu + 4.627026e-05 * block_size + -2.361685e-06 * (double)num_dpus * blocks_per_dpu + -3.965346e-07 * (double)num_dpus * block_size + 1.013981e-05 * (double)blocks_per_dpu * block_size + 9.913608e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.0272452 + 3.739378e-05 * num_dpus + 0.0002591895 * blocks_per_dpu + -3.351501e-05 * block_size + 4.404873e-06 * (double)num_dpus * blocks_per_dpu + 2.868414e-07 * (double)num_dpus * block_size + 0.0002000845 * (double)blocks_per_dpu * block_size + -5.000166e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (num_dpus <= 176) {
              return 0.02617321 + 4.40541e-05 * num_dpus + -0.002101219 * blocks_per_dpu + -1.132026e-05 * block_size + 2.043373e-05 * (double)num_dpus * blocks_per_dpu + 1.28512e-07 * (double)num_dpus * block_size + -1.46523e-05 * (double)blocks_per_dpu * block_size + 8.220174e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.02822518 + 3.992597e-05 * num_dpus + 0.0005485914 * blocks_per_dpu + -2.346712e-06 * block_size + 9.798554e-07 * (double)num_dpus * blocks_per_dpu + 4.963018e-09 * (double)num_dpus * block_size + 0.0001253381 * (double)blocks_per_dpu * block_size + 2.8891e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        }
      } else {
        if (num_dpus <= 1280) {
          if (num_dpus <= 896) {
            if ((double)num_dpus * blocks_per_dpu <= 2816) {
              return 0.04398669 + 3.864445e-05 * num_dpus + -0.007991977 * blocks_per_dpu + -0.0001497151 * block_size + 1.058751e-05 * (double)num_dpus * blocks_per_dpu + 1.911751e-07 * (double)num_dpus * block_size + 0.0003180866 * (double)blocks_per_dpu * block_size + -5.286584e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.03124455 + 4.842341e-05 * num_dpus + -4.829738e-05 * blocks_per_dpu + 5.237488e-05 * block_size + 1.731421e-06 * (double)num_dpus * blocks_per_dpu + 8.590621e-08 * (double)num_dpus * block_size + 0.0003623818 * (double)blocks_per_dpu * block_size + -1.2505e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (num_dpus <= 1024) {
              return 0.05287285 + 2.822738e-05 * num_dpus + -0.001905087 * blocks_per_dpu + 0.0002320268 * block_size + 3.487741e-06 * (double)num_dpus * blocks_per_dpu + -2.26277e-07 * (double)num_dpus * block_size + 0.0002890507 * (double)blocks_per_dpu * block_size + -1.280001e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.06935919 + 1.794326e-05 * num_dpus + -0.00437179 * blocks_per_dpu + -5.942107e-06 * block_size + 4.850544e-06 * (double)num_dpus * blocks_per_dpu + 1.252724e-08 * (double)num_dpus * block_size + 0.0002448462 * (double)blocks_per_dpu * block_size + 1.972949e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        } else {
          if (num_dpus <= 1792) {
            if ((double)num_dpus * blocks_per_dpu * block_size <= 360448) {
              return 0.01055195 + 7.109676e-05 * num_dpus + 4.01751e-05 * blocks_per_dpu + -0.0002151088 * block_size + 1.017904e-06 * (double)num_dpus * blocks_per_dpu + 1.601092e-07 * (double)num_dpus * block_size + 0.0006455454 * (double)blocks_per_dpu * block_size + -1.715613e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.0691399 + 2.886232e-05 * num_dpus + -0.00622197 * blocks_per_dpu + -0.0002788275 * block_size + 4.997511e-06 * (double)num_dpus * blocks_per_dpu + 2.012501e-07 * (double)num_dpus * block_size + 0.0004324217 * (double)blocks_per_dpu * block_size + -9.307768e-09 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if ((double)num_dpus * blocks_per_dpu <= 7680) {
              return -0.1124174 + 0.0001332166 * num_dpus + -0.002991596 * blocks_per_dpu + -0.0006748738 * block_size + 2.194917e-06 * (double)num_dpus * blocks_per_dpu + 3.520936e-07 * (double)num_dpus * block_size + 0.0009739684 * (double)blocks_per_dpu * block_size + -2.20858e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.04452255 + 5.423427e-05 * num_dpus + -0.004999611 * blocks_per_dpu + -1.155559e-05 * block_size + 3.173222e-06 * (double)num_dpus * blocks_per_dpu + -1.223634e-07 * (double)num_dpus * block_size + -0.0001025705 * (double)blocks_per_dpu * block_size + 3.504238e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        }
      }
    } else {
      if (num_dpus <= 640) {
        if ((double)blocks_per_dpu * block_size <= 1920) {
          if (num_dpus <= 320) {
            if (num_dpus <= 176) {
              return 0.02376209 + 5.236077e-05 * num_dpus + 0.0004717097 * blocks_per_dpu + 5.942194e-06 * block_size + 2.943527e-06 * (double)num_dpus * blocks_per_dpu + -4.344306e-08 * (double)num_dpus * block_size + 6.612739e-05 * (double)blocks_per_dpu * block_size + 2.373497e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.02467231 + 4.520873e-05 * num_dpus + 0.000726106 * blocks_per_dpu + 2.498362e-08 * block_size + 3.634549e-07 * (double)num_dpus * blocks_per_dpu + 6.287929e-10 * (double)num_dpus * block_size + 6.760293e-05 * (double)blocks_per_dpu * block_size + 6.980142e-09 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (num_dpus <= 416) {
              return 0.05131133 + -3.729217e-05 * num_dpus + -0.002229635 * blocks_per_dpu + 6.051128e-05 * block_size + 9.315541e-06 * (double)num_dpus * blocks_per_dpu + -1.292195e-07 * (double)num_dpus * block_size + 1.380522e-05 * (double)blocks_per_dpu * block_size + 1.467583e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.01818716 + 4.633314e-05 * num_dpus + 0.001594849 * blocks_per_dpu + 6.335787e-05 * block_size + -7.345542e-07 * (double)num_dpus * blocks_per_dpu + -1.066745e-07 * (double)num_dpus * block_size + 3.214932e-05 * (double)blocks_per_dpu * block_size + 8.235351e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        } else {
          if (num_dpus <= 384) {
            if ((double)num_dpus * blocks_per_dpu <= 144) {
              return -0.08954592 + 0.001595575 * num_dpus + 0.1107033 * blocks_per_dpu + -5.395053e-05 * block_size + -0.001514053 * (double)num_dpus * blocks_per_dpu + 7.575135e-07 * (double)num_dpus * block_size + 8.834405e-05 * (double)blocks_per_dpu * block_size + -7.430086e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.02007472 + 4.998602e-05 * num_dpus + 0.000634742 * blocks_per_dpu + -1.106728e-06 * block_size + 6.759507e-07 * (double)num_dpus * blocks_per_dpu + 7.719755e-09 * (double)num_dpus * block_size + 3.49533e-05 * (double)blocks_per_dpu * block_size + 5.809693e-09 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if ((double)num_dpus * blocks_per_dpu <= 1920) {
              return 0.05693762 + -2.739244e-05 * num_dpus + -0.007437676 * blocks_per_dpu + 1.667618e-05 * block_size + 1.267776e-05 * (double)num_dpus * blocks_per_dpu + -2.941957e-08 * (double)num_dpus * block_size + 2.278008e-05 * (double)blocks_per_dpu * block_size + 3.812816e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.01947275 + 3.026754e-05 * num_dpus + 0.001838025 * blocks_per_dpu + 5.063159e-05 * block_size + -1.010606e-06 * (double)num_dpus * blocks_per_dpu + -8.29204e-08 * (double)num_dpus * block_size + 2.97412e-05 * (double)blocks_per_dpu * block_size + 2.507565e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        }
      } else {
        if ((double)blocks_per_dpu * block_size <= 1920) {
          if (num_dpus <= 1280) {
            if ((double)num_dpus * blocks_per_dpu <= 3584) {
              return -0.01421004 + 7.811629e-05 * num_dpus + 0.02030479 * blocks_per_dpu + 8.365437e-05 * block_size + -1.537601e-05 * (double)num_dpus * blocks_per_dpu + -7.547312e-08 * (double)num_dpus * block_size + 7.675915e-05 * (double)blocks_per_dpu * block_size + 7.603738e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.06874664 + 8.861007e-06 * num_dpus + -0.001527231 * blocks_per_dpu + -0.0002643629 * block_size + 2.578099e-06 * (double)num_dpus * blocks_per_dpu + 2.154305e-07 * (double)num_dpus * block_size + 0.0001354791 * (double)blocks_per_dpu * block_size + 1.868208e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (num_dpus <= 1792) {
              return -0.009201929 + 7.555504e-05 * num_dpus + -6.420491e-05 * blocks_per_dpu + 3.104905e-05 * block_size + 8.191563e-07 * (double)num_dpus * blocks_per_dpu + -1.651415e-08 * (double)num_dpus * block_size + 0.0001542657 * (double)blocks_per_dpu * block_size + 4.973192e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.435813 + -0.000146578 * num_dpus + 0.002082892 * blocks_per_dpu + 0.0002195245 * block_size + 7.818978e-08 * (double)num_dpus * blocks_per_dpu + -1.050542e-07 * (double)num_dpus * block_size + -0.000437245 * (double)blocks_per_dpu * block_size + 3.659804e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        } else {
          if (num_dpus <= 1280) {
            if (num_dpus <= 896) {
              return 0.01640401 + 5.461705e-05 * num_dpus + 0.001021099 * blocks_per_dpu + 1.095252e-05 * block_size + -4.046513e-07 * (double)num_dpus * blocks_per_dpu + -1.201879e-08 * (double)num_dpus * block_size + 6.590666e-05 * (double)blocks_per_dpu * block_size + 1.779951e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.07539843 + -7.498881e-06 * num_dpus + -0.001646501 * blocks_per_dpu + 7.244224e-06 * block_size + 2.44543e-06 * (double)num_dpus * blocks_per_dpu + -5.446244e-09 * (double)num_dpus * block_size + 7.042936e-05 * (double)blocks_per_dpu * block_size + 1.501476e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (num_dpus <= 1664) {
              return -0.08027338 + 0.0001233939 * num_dpus + 0.002143548 * blocks_per_dpu + 3.273076e-05 * block_size + -8.257051e-07 * (double)num_dpus * blocks_per_dpu + -2.251759e-08 * (double)num_dpus * block_size + 5.434968e-05 * (double)blocks_per_dpu * block_size + 4.962194e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.04318978 + 4.445538e-05 * num_dpus + -0.0008960036 * blocks_per_dpu + 1.06812e-05 * block_size + 2.059685e-06 * (double)num_dpus * blocks_per_dpu + -5.559733e-09 * (double)num_dpus * block_size + -8.006837e-05 * (double)blocks_per_dpu * block_size + 1.265474e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        }
      }
    }
  } else {
    if (num_dpus <= 640) {
      if (num_dpus <= 352) {
        if ((double)num_dpus * blocks_per_dpu <= 1152) {
          if ((double)num_dpus * blocks_per_dpu * block_size <= 39321600) {
            if (num_dpus <= 128) {
              return -5.171594 + 0.0217235 * num_dpus + 0.01900855 * blocks_per_dpu + 2.848077e-06 * block_size + -0.0006992312 * (double)num_dpus * blocks_per_dpu + -3.870447e-08 * (double)num_dpus * block_size + 4.531643e-05 * (double)blocks_per_dpu * block_size + 3.344186e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -2.696848 + 0.004061876 * num_dpus + -0.2454064 * blocks_per_dpu + -7.378533e-06 * block_size + 0.001456119 * (double)num_dpus * blocks_per_dpu + 4.305231e-08 * (double)num_dpus * block_size + 5.847004e-05 * (double)blocks_per_dpu * block_size + -4.716425e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if ((double)num_dpus * blocks_per_dpu <= 208) {
              return 1.070446 + -0.03170653 * num_dpus + -1.859926 * blocks_per_dpu + -1.511387e-06 * block_size + 0.03194424 * (double)num_dpus * blocks_per_dpu + 2.66945e-08 * (double)num_dpus * block_size + 4.229698e-05 * (double)blocks_per_dpu * block_size + -2.271105e-09 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -0.03901659 + -0.004645599 * num_dpus + -0.07235567 * blocks_per_dpu + -5.444679e-06 * block_size + 0.0003645604 * (double)num_dpus * blocks_per_dpu + 2.226011e-08 * (double)num_dpus * block_size + 4.108534e-05 * (double)blocks_per_dpu * block_size + 2.833432e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        } else {
          if (num_dpus <= 288) {
            if (num_dpus <= 192) {
              return -0.4270227 + -0.005443768 * num_dpus + -0.07398792 * blocks_per_dpu + -9.537662e-05 * block_size + 0.0004181062 * (double)num_dpus * blocks_per_dpu + 4.735317e-07 * (double)num_dpus * block_size + 4.47414e-05 * (double)blocks_per_dpu * block_size + 2.593235e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -0.1316207 + -0.005354937 * num_dpus + -0.06957613 * blocks_per_dpu + -0.0001065141 * block_size + 0.0002564696 * (double)num_dpus * blocks_per_dpu + 3.580488e-07 * (double)num_dpus * block_size + 4.745774e-05 * (double)blocks_per_dpu * block_size + 1.663662e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if ((double)num_dpus * blocks_per_dpu * block_size <= 235929600) {
              return -7.088706 + 0.01551667 * num_dpus + 0.06835371 * blocks_per_dpu + 0.0001082971 * block_size + -0.0001980069 * (double)num_dpus * blocks_per_dpu + -3.258986e-07 * (double)num_dpus * block_size + 6.856092e-05 * (double)blocks_per_dpu * block_size + -3.788564e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 127.8897 + -0.3965501 * num_dpus + -3.434171 * blocks_per_dpu + -0.0001546276 * block_size + 0.01002001 * (double)num_dpus * blocks_per_dpu + 4.582834e-07 * (double)num_dpus * block_size + 3.363739e-05 * (double)blocks_per_dpu * block_size + 8.28723e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        }
      } else {
        if ((double)num_dpus * blocks_per_dpu * block_size <= 61341696) {
          if ((double)num_dpus * blocks_per_dpu * block_size <= 47710208) {
            if (num_dpus <= 480) {
              return -2.15686 + 0.003591006 * num_dpus + -0.03431229 * blocks_per_dpu + -6.422294e-06 * block_size + 8.806585e-05 * (double)num_dpus * blocks_per_dpu + 2.140747e-08 * (double)num_dpus * block_size + 5.120517e-05 * (double)blocks_per_dpu * block_size + -1.006569e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 1.938014 + -0.003832797 * num_dpus + -0.09557955 * blocks_per_dpu + -3.440517e-05 * block_size + 0.0001550286 * (double)num_dpus * blocks_per_dpu + 5.491595e-08 * (double)num_dpus * block_size + 1.71201e-05 * (double)blocks_per_dpu * block_size + 6.026025e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (block_size <= 26624) {
              return -3.378716 + 0.001703637 * num_dpus + -0.0459267 * blocks_per_dpu + -5.985029e-05 * block_size + 7.741366e-05 * (double)num_dpus * blocks_per_dpu + 1.051483e-07 * (double)num_dpus * block_size + 4.268437e-05 * (double)blocks_per_dpu * block_size + 6.012312e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -7.027757 + 0.01014948 * num_dpus + 1.567432 * blocks_per_dpu + 5.669255e-05 * block_size + -0.00391256 * (double)num_dpus * blocks_per_dpu + -1.341593e-07 * (double)num_dpus * block_size + 5.00984e-06 * (double)blocks_per_dpu * block_size + 1.506525e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        } else {
          if (num_dpus <= 416) {
            if ((double)num_dpus * blocks_per_dpu * block_size <= 301989888) {
              return -11.68792 + 0.02236297 * num_dpus + 0.5683817 * blocks_per_dpu + 4.805526e-06 * block_size + -0.001451446 * (double)num_dpus * blocks_per_dpu + -2.241563e-08 * (double)num_dpus * block_size + -2.920132e-06 * (double)blocks_per_dpu * block_size + 1.747358e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 179.0643 + -0.4705865 * num_dpus + 1.097208 * blocks_per_dpu + 0.0001845449 * block_size + -0.002941637 * (double)num_dpus * blocks_per_dpu + -4.686344e-07 * (double)num_dpus * block_size + -0.0003396618 * (double)blocks_per_dpu * block_size + 1.035201e-06 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (num_dpus <= 512) {
              return 4.571462 + -0.01422872 * num_dpus + -0.08063952 * blocks_per_dpu + 5.1152e-06 * block_size + 0.0001536678 * (double)num_dpus * blocks_per_dpu + -2.127449e-08 * (double)num_dpus * block_size + 6.878707e-06 * (double)blocks_per_dpu * block_size + 1.299749e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -10.04671 + 0.01329125 * num_dpus + 0.04294918 * blocks_per_dpu + -6.599203e-06 * block_size + -0.0001282872 * (double)num_dpus * blocks_per_dpu + 9.111789e-09 * (double)num_dpus * block_size + 7.500351e-05 * (double)blocks_per_dpu * block_size + 1.591936e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        }
      }
    } else {
      if ((double)num_dpus * blocks_per_dpu * block_size <= 70778880) {
        if (num_dpus <= 1280) {
          if ((double)num_dpus * blocks_per_dpu * block_size <= 51904512) {
            if ((double)num_dpus * blocks_per_dpu * block_size <= 39321600) {
              return -0.5527083 + 0.0004124185 * num_dpus + -0.005252382 * blocks_per_dpu + -2.970482e-06 * block_size + 5.539063e-06 * (double)num_dpus * blocks_per_dpu + 3.805484e-09 * (double)num_dpus * block_size + 7.890048e-05 * (double)blocks_per_dpu * block_size + 1.189887e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -1.542135 + 0.0004118454 * num_dpus + 0.002113436 * blocks_per_dpu + -1.911121e-06 * block_size + 1.096478e-08 * (double)num_dpus * blocks_per_dpu + 2.349818e-09 * (double)num_dpus * block_size + 8.047464e-05 * (double)blocks_per_dpu * block_size + 3.447104e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (num_dpus <= 960) {
              return -6.474996 + 0.003640319 * num_dpus + 0.03248662 * blocks_per_dpu + -1.537069e-05 * block_size + -2.937203e-05 * (double)num_dpus * blocks_per_dpu + 2.164386e-08 * (double)num_dpus * block_size + 0.0001226822 * (double)blocks_per_dpu * block_size + 2.285616e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 3.704791 + -0.006647778 * num_dpus + -0.03486296 * blocks_per_dpu + -7.697174e-06 * block_size + 3.634605e-05 * (double)num_dpus * blocks_per_dpu + 9.049659e-09 * (double)num_dpus * block_size + -9.930891e-05 * (double)blocks_per_dpu * block_size + 2.450924e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        } else {
          if ((double)num_dpus * blocks_per_dpu * block_size <= 47710208) {
            if (num_dpus <= 1792) {
              return -1.073828 + 0.0006089155 * num_dpus + 0.007075762 * blocks_per_dpu + 2.996208e-05 * block_size + -3.446253e-06 * (double)num_dpus * blocks_per_dpu + -2.113642e-08 * (double)num_dpus * block_size + 9.689508e-05 * (double)blocks_per_dpu * block_size + 2.923741e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 0.8867592 + -0.000441225 * num_dpus + -0.005928876 * blocks_per_dpu + -2.678863e-05 * block_size + 4.272075e-06 * (double)num_dpus * blocks_per_dpu + 1.356126e-08 * (double)num_dpus * block_size + 4.411821e-05 * (double)blocks_per_dpu * block_size + 6.777881e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (num_dpus <= 1792) {
              return -4.928827 + 0.0007964347 * num_dpus + -0.01204284 * blocks_per_dpu + -5.216649e-06 * block_size + 1.087516e-05 * (double)num_dpus * blocks_per_dpu + -4.067189e-10 * (double)num_dpus * block_size + 0.0001283617 * (double)blocks_per_dpu * block_size + 8.075752e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -1.782706 + -0.0005616172 * num_dpus + -0.08626508 * blocks_per_dpu + -0.0001227731 * block_size + 5.059305e-05 * (double)num_dpus * blocks_per_dpu + 6.314905e-08 * (double)num_dpus * block_size + 0.0001057285 * (double)blocks_per_dpu * block_size + 9.334889e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        }
      } else {
        if (num_dpus <= 1024) {
          if (num_dpus <= 768) {
            if (blocks_per_dpu <= 2) {
              return 18.11833 + -0.0279398 * num_dpus + -11.14599 * blocks_per_dpu + -0.0002125599 * block_size + 0.01547563 * (double)num_dpus * blocks_per_dpu + 2.998454e-07 * (double)num_dpus * block_size + 0.0003971609 * (double)blocks_per_dpu * block_size + -3.739565e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 24.09827 + -0.031998 * num_dpus + 0.9116766 * blocks_per_dpu + 0.001061073 * block_size + -0.001242451 * (double)num_dpus * blocks_per_dpu + -1.471681e-06 * (double)num_dpus * block_size + -0.0003701098 * (double)blocks_per_dpu * block_size + 6.624823e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (blocks_per_dpu <= 3) {
              return 1.631744 + -0.002595575 * num_dpus + 7.963575 * blocks_per_dpu + 0.0002700987 * block_size + -0.009506447 * (double)num_dpus * blocks_per_dpu + -3.134203e-07 * (double)num_dpus * block_size + -0.0002482198 * (double)blocks_per_dpu * block_size + 4.352517e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return 6.972562 + -0.01006491 * num_dpus + -0.2344407 * blocks_per_dpu + -0.0003155652 * block_size + 0.0002869905 * (double)num_dpus * blocks_per_dpu + 3.820755e-07 * (double)num_dpus * block_size + 6.812474e-05 * (double)blocks_per_dpu * block_size + 6.87355e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        } else {
          if ((double)num_dpus * blocks_per_dpu * block_size <= 132120576) {
            if (num_dpus <= 1280) {
              return -14.04412 + 0.006150407 * num_dpus + -0.3424291 * blocks_per_dpu + -9.9119e-05 * block_size + 0.0003048049 * (double)num_dpus * blocks_per_dpu + 8.399047e-08 * (double)num_dpus * block_size + 0.000329698 * (double)blocks_per_dpu * block_size + -7.59477e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -11.47631 + 0.003677774 * num_dpus + -0.03244895 * blocks_per_dpu + 3.335716e-06 * block_size + 2.890731e-05 * (double)num_dpus * blocks_per_dpu + -4.328683e-09 * (double)num_dpus * block_size + 0.0001752359 * (double)blocks_per_dpu * block_size + 8.222136e-08 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          } else {
            if (num_dpus <= 1792) {
              return 1.715123 + -7.410578e-06 * num_dpus + -0.1718968 * blocks_per_dpu + 2.189319e-06 * block_size + 0.0001232558 * (double)num_dpus * blocks_per_dpu + -2.603066e-10 * (double)num_dpus * block_size + -1.329066e-05 * (double)blocks_per_dpu * block_size + 1.346805e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            } else {
              return -26.47098 + 0.01283739 * num_dpus + -3.176658 * blocks_per_dpu + -0.0003126962 * block_size + 0.001560855 * (double)num_dpus * blocks_per_dpu + 1.456222e-07 * (double)num_dpus * block_size + 0.0008988891 * (double)blocks_per_dpu * block_size + -3.020025e-07 * (double)num_dpus * blocks_per_dpu * block_size;
            }
          }
        }
      }
    }
  }
}
} // namespace

double upmem_cm::gatherSgCostMs(int num_dpus, int blocks_per_dpu, int block_size) {
  // No transfer beats the fastest rate this sweep measured (1.38974e+07 B/ms);
  // outside its fitted region a leaf's linear model can slope below zero.
  return std::fmax(scatterSgGatherTreeCostMsModel(num_dpus, blocks_per_dpu, block_size), (double)num_dpus * blocks_per_dpu * block_size / 1.389739e+07);
}