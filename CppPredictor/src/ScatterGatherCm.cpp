
#include <upmem_cost_model/ScatterGatherCm.h>

#include <cmath> // std::log2, std::exp
#include <limits>

double upmem_cm::gatherCostMs(int num_dpus, int block_size) {
  if (num_dpus <= 448) {
    if (num_dpus <= 64) {
      if (block_size <= 992) {
        if (num_dpus <= 7) {
          if (num_dpus <= 2) {
            return 0.04360868 + -0.0001469985 * num_dpus + 1.070834e-05 * block_size + 8.026008e-07 * (double)num_dpus * block_size;
          } else {
            return 0.05406223 + -0.001572857 * num_dpus + 9.801126e-06 * block_size + 5.821997e-07 * (double)num_dpus * block_size;
          }
        } else {
          if (num_dpus <= 8) {
            return 0.06657527 + -0.00259087 * num_dpus + -0.0001618405 * block_size + 2.203062e-05 * (double)num_dpus * block_size;
          } else {
            return 0.04622025 + -1.324306e-05 * num_dpus + 2.103631e-05 * block_size + 1.725223e-06 * (double)num_dpus * block_size;
          }
        }
      } else {
        if (num_dpus <= 31) {
          if (num_dpus <= 15) {
            return 0.05969088 + 0.0007593549 * num_dpus + 4.176116e-06 * block_size + 1.567585e-08 * (double)num_dpus * block_size;
          } else {
            return 0.07873572 + 0.0005910716 * num_dpus + 7.757018e-06 * block_size + 1.804991e-08 * (double)num_dpus * block_size;
          }
        } else {
          if (block_size <= 221184) {
            return 0.1049707 + -8.65499e-05 * num_dpus + 1.540634e-05 * block_size + 1.287595e-08 * (double)num_dpus * block_size;
          } else {
            return -1.469678 + -0.01411323 * num_dpus + 1.922682e-05 * block_size + 1.31618e-07 * (double)num_dpus * block_size;
          }
        }
      }
    } else {
      if (block_size <= 992) {
        if (num_dpus <= 256) {
          if (block_size <= 248) {
            return 0.05724274 + 0.0001778857 * num_dpus + 0.0001242799 * block_size + 9.580303e-07 * (double)num_dpus * block_size;
          } else {
            return 0.06242456 + 0.0003529098 * num_dpus + 0.0001289598 * block_size + 2.615956e-07 * (double)num_dpus * block_size;
          }
        } else {
          if (block_size <= 304) {
            return 0.0005222112 + 0.0003183241 * num_dpus + -0.0001061158 * block_size + 1.486431e-06 * (double)num_dpus * block_size;
          } else {
            return -0.05887982 + 0.0006725595 * num_dpus + 7.169793e-05 * block_size + 3.855289e-07 * (double)num_dpus * block_size;
          }
        }
      } else {
        if (block_size <= 65536) {
          if (num_dpus <= 384) {
            return 0.1078598 + 0.0004375839 * num_dpus + 2.905733e-05 * block_size + 1.176297e-08 * (double)num_dpus * block_size;
          } else {
            return 0.06554729 + 0.000525139 * num_dpus + -2.263173e-05 * block_size + 1.842298e-07 * (double)num_dpus * block_size;
          }
        } else {
          if (num_dpus <= 384) {
            return -1.51061 + 1.688154e-05 * num_dpus + 4.368139e-05 * block_size + 5.840433e-08 * (double)num_dpus * block_size;
          } else {
            return 1.181142 + -0.006098171 * num_dpus + 4.71512e-05 * block_size + 1.033262e-07 * (double)num_dpus * block_size;
          }
        }
      }
    }
  } else {
    if (block_size <= 992) {
      if (block_size <= 248) {
        if (num_dpus <= 1664) {
          if (num_dpus <= 1344) {
            return 0.1495013 + 6.910815e-05 * num_dpus + 0.0005121625 * block_size + 4.673934e-07 * (double)num_dpus * block_size;
          } else {
            return 0.2583237 + 4.348452e-06 * num_dpus + -0.003494037 * block_size + 2.940048e-06 * (double)num_dpus * block_size;
          }
        } else {
          if (num_dpus <= 1856) {
            return -0.01197177 + 0.0001628316 * num_dpus + -0.004125484 * block_size + 2.82431e-06 * (double)num_dpus * block_size;
          } else {
            return -1.243074 + 0.0008164864 * num_dpus + -0.001452847 * block_size + 1.171492e-06 * (double)num_dpus * block_size;
          }
        }
      } else {
        if (num_dpus <= 1664) {
          if (num_dpus <= 1344) {
            return 0.2696028 + 0.0001113416 * num_dpus + 6.667257e-05 * block_size + 3.131644e-07 * (double)num_dpus * block_size;
          } else {
            return -0.667827 + 0.000697094 * num_dpus + -0.0002195745 * block_size + 4.175914e-07 * (double)num_dpus * block_size;
          }
        } else {
          if (num_dpus <= 1856) {
            return -1.133265 + 0.0009016216 * num_dpus + -0.003761082 * block_size + 2.270724e-06 * (double)num_dpus * block_size;
          } else {
            return -0.9707286 + 0.0007389308 * num_dpus + -0.003523866 * block_size + 2.005095e-06 * (double)num_dpus * block_size;
          }
        }
      }
    } else {
      if (block_size <= 22528) {
        if (block_size <= 7936) {
          if (num_dpus <= 1664) {
            return 0.2944864 + 0.00022221 * num_dpus + 3.874119e-05 * block_size + 2.295434e-08 * (double)num_dpus * block_size;
          } else {
            return -0.8370366 + 0.000798366 * num_dpus + -0.0003860646 * block_size + 2.581723e-07 * (double)num_dpus * block_size;
          }
        } else {
          if (num_dpus <= 1344) {
            return 0.5864036 + 0.0003694818 * num_dpus + 2.630959e-05 * block_size + 4.515164e-08 * (double)num_dpus * block_size;
          } else {
            return 2.521448 + -0.000847859 * num_dpus + -0.0002164399 * block_size + 1.843907e-07 * (double)num_dpus * block_size;
          }
        }
      } else {
        if (block_size <= 229376) {
          if (num_dpus <= 1920) {
            return -0.3840652 + -0.001190651 * num_dpus + 6.228176e-05 * block_size + 1.034811e-07 * (double)num_dpus * block_size;
          } else {
            return 9.78391 + -0.005012157 * num_dpus + -0.0004765042 * block_size + 3.435476e-07 * (double)num_dpus * block_size;
          }
        } else {
          if (num_dpus <= 1920) {
            return -18.35936 + 0.02630588 * num_dpus + 0.00012883 * block_size + 6.74495e-09 * (double)num_dpus * block_size;
          } else {
            return 1031.539 + -0.5109315 * num_dpus + -0.003204236 * block_size + 1.71134e-06 * (double)num_dpus * block_size;
          }
        }
      }
    }
  }
}
double upmem_cm::broadcastCostMs(int num_dpus, int block_size) {
  if (block_size <= 512) {
    if (num_dpus <= 448) {
      if (num_dpus <= 64) {
        return 0.04104203 + -1.019551e-06 * num_dpus + -1.081792e-05 * block_size + 1.058815e-06 * (double)num_dpus * block_size;
      } else {
        if (num_dpus <= 384) {
          if (num_dpus <= 368) {
            return 0.05988132 + 8.439184e-05 * num_dpus + 0.0001020184 * block_size + 3.062103e-08 * (double)num_dpus * block_size;
          } else {
            return -0.6742811 + 0.001965389 * num_dpus + 0.002477252 * block_size + -6.083805e-06 * (double)num_dpus * block_size;
          }
        } else {
          if (block_size <= 184) {
            return 0.2274637 + -0.0002258664 * num_dpus + 0.0009973322 * block_size + -1.987409e-06 * (double)num_dpus * block_size;
          } else {
            return 0.6514651 + -0.001176895 * num_dpus + -0.001517203 * block_size + 3.79598e-06 * (double)num_dpus * block_size;
          }
        }
      }
    } else {
      if (block_size <= 80) {
        if (num_dpus <= 1600) {
          if (num_dpus <= 1152) {
            return 0.1244054 + 7.944038e-05 * num_dpus + 0.0002973382 * block_size + -3.036006e-07 * (double)num_dpus * block_size;
          } else {
            return 0.165618 + 5.468011e-05 * num_dpus + -0.0005369521 * block_size + 4.977188e-07 * (double)num_dpus * block_size;
          }
        } else {
          return -0.01105782 + 0.000149915 * num_dpus + -0.001503654 * block_size + 9.874552e-07 * (double)num_dpus * block_size;
        }
      } else {
        if (num_dpus <= 896) {
          if (num_dpus <= 832) {
            return 0.1098913 + 0.0001084481 * num_dpus + 0.0001937392 * block_size + 2.51264e-08 * (double)num_dpus * block_size;
          } else {
            return -0.7749138 + 0.001112534 * num_dpus + -1.265857e-05 * block_size + 2.481867e-07 * (double)num_dpus * block_size;
          }
        } else {
          if (num_dpus <= 1024) {
            return -0.2131537 + 0.0004197611 * num_dpus + -7.662439e-05 * block_size + 3.138712e-07 * (double)num_dpus * block_size;
          } else {
            return 0.04336962 + 0.0001459508 * num_dpus + 0.0003550093 * block_size + -8.732581e-08 * (double)num_dpus * block_size;
          }
        }
      }
    }
  } else {
    if (num_dpus <= 64) {
      if (block_size <= 18432) {
        if (num_dpus <= 31) {
          if (block_size <= 1856) {
            return 0.03824462 + 5.461644e-05 * num_dpus + -2.352695e-06 * block_size + 1.04331e-06 * (double)num_dpus * block_size;
          } else {
            return 0.04837776 + 0.0002219641 * num_dpus + 4.814865e-07 * block_size + 1.538296e-07 * (double)num_dpus * block_size;
          }
        } else {
          if (block_size <= 1856) {
            return 0.007029898 + 0.00110703 * num_dpus + 4.931529e-05 * block_size + -6.55708e-07 * (double)num_dpus * block_size;
          } else {
            return 0.07220434 + -0.0002412967 * num_dpus + 5.109694e-06 * block_size + 8.563434e-08 * (double)num_dpus * block_size;
          }
        }
      } else {
        if (num_dpus <= 31) {
          if (block_size <= 57344) {
            return 0.03097408 + 0.003307072 * num_dpus + 2.069751e-06 * block_size + -1.534612e-08 * (double)num_dpus * block_size;
          } else {
            return 0.09110034 + -0.001910101 * num_dpus + 6.298765e-07 * block_size + 7.559098e-08 * (double)num_dpus * block_size;
          }
        } else {
          if (block_size <= 32768) {
            return 0.1889211 + 0.0001230453 * num_dpus + 9.581345e-08 * block_size + 1.634212e-08 * (double)num_dpus * block_size;
          } else {
            return 0.04251361 + 1.073379e-05 * num_dpus + 4.389015e-06 * block_size + 1.81242e-09 * (double)num_dpus * block_size;
          }
        }
      }
    } else {
      if (num_dpus <= 384) {
        if (block_size <= 15872) {
          if (block_size <= 7936) {
            return 0.1128739 + 0.000111529 * num_dpus + 7.497805e-06 * block_size + 6.479031e-09 * (double)num_dpus * block_size;
          } else {
            return 0.2421178 + 0.000429524 * num_dpus + 4.143807e-06 * block_size + -1.901337e-08 * (double)num_dpus * block_size;
          }
        } else {
          if (num_dpus <= 368) {
            return 0.08995563 + 8.983113e-05 * num_dpus + 9.21968e-06 * block_size + -7.386449e-10 * (double)num_dpus * block_size;
          } else {
            return 1.334216 + -0.003030563 * num_dpus + -8.995509e-05 * block_size + 2.58015e-07 * (double)num_dpus * block_size;
          }
        }
      } else {
        if (num_dpus <= 1664) {
          if (num_dpus <= 448) {
            return 0.1158547 + 0.0002121666 * num_dpus + 2.255454e-05 * block_size + -2.20034e-08 * (double)num_dpus * block_size;
          } else {
            return 0.1854037 + 0.0001334955 * num_dpus + 1.080657e-05 * block_size + 1.402088e-08 * (double)num_dpus * block_size;
          }
        } else {
          if (block_size <= 43008) {
            return 0.4241084 + -6.973216e-06 * num_dpus + -0.0001207811 * block_size + 9.079204e-08 * (double)num_dpus * block_size;
          } else {
            return -8.306335 + 0.004787651 * num_dpus + -4.251167e-05 * block_size + 4.181868e-08 * (double)num_dpus * block_size;
          }
        }
      }
    }
  }
}

double upmem_cm::scatterBlockCostMs(int num_dpus, int block_size) {
  if (block_size <= 21504) {
    if (num_dpus <= 512) {
      if (block_size <= 512) {
        if (num_dpus <= 64) {
          if (num_dpus <= 2) {
            return 0.05894642 + -0.005648085 * num_dpus + -1.776294e-05 * block_size + 5.274076e-06 * (double)num_dpus * block_size;
          } else {
            return 0.04760898 + -9.363585e-06 * num_dpus + -1.364658e-05 * block_size + 1.218936e-06 * (double)num_dpus * block_size;
          }
        } else {
          if (num_dpus <= 384) {
            return 0.0721115 + 0.0001221398 * num_dpus + 7.061422e-05 * block_size + 3.611663e-07 * (double)num_dpus * block_size;
          } else {
            return 0.03128527 + 0.0003030432 * num_dpus + -8.071424e-05 * block_size + 7.570137e-07 * (double)num_dpus * block_size;
          }
        }
      } else {
        if (num_dpus <= 64) {
          if (num_dpus <= 31) {
            return 0.04195181 + 0.0009242905 * num_dpus + 1.490616e-06 * block_size + 1.293336e-07 * (double)num_dpus * block_size;
          } else {
            return 0.08070553 + -7.968347e-05 * num_dpus + 1.070045e-05 * block_size + -3.285178e-08 * (double)num_dpus * block_size;
          }
        } else {
          if (num_dpus <= 384) {
            return 0.1124188 + 0.0002933404 * num_dpus + 1.032841e-05 * block_size + 1.291954e-08 * (double)num_dpus * block_size;
          } else {
            return 0.01023826 + 0.0006333412 * num_dpus + 2.81528e-05 * block_size + -1.685047e-08 * (double)num_dpus * block_size;
          }
        }
      }
    } else {
      if (block_size <= 992) {
        if (num_dpus <= 1536) {
          if (block_size <= 88) {
            return 0.1811125 + 5.320396e-05 * num_dpus + -0.0002119015 * block_size + 8.216235e-07 * (double)num_dpus * block_size;
          } else {
            return 0.1770907 + 0.0001236383 * num_dpus + 0.000211497 * block_size + 2.061159e-08 * (double)num_dpus * block_size;
          }
        } else {
          if (num_dpus <= 1664) {
            return -0.5838997 + 0.0005310785 * num_dpus + -0.00285782 * block_size + 1.945636e-06 * (double)num_dpus * block_size;
          } else {
            return -0.8046495 + 0.0006088731 * num_dpus + 0.00139159 * block_size + -5.806285e-07 * (double)num_dpus * block_size;
          }
        }
      } else {
        if (block_size <= 7936) {
          if (num_dpus <= 768) {
            return 0.128211 + 0.0003782393 * num_dpus + 0.0001252685 * block_size + -1.538958e-07 * (double)num_dpus * block_size;
          } else {
            return 0.3352641 + 8.294142e-05 * num_dpus + 3.147817e-05 * block_size + 1.029998e-08 * (double)num_dpus * block_size;
          }
        } else {
          if (num_dpus <= 1792) {
            return 0.820316 + -9.054905e-05 * num_dpus + -2.609589e-05 * block_size + 3.981227e-08 * (double)num_dpus * block_size;
          } else {
            return 4.486801 + -0.002030728 * num_dpus + -0.0005518207 * block_size + 3.153422e-07 * (double)num_dpus * block_size;
          }
        }
      }
    }
  } else {
    if (num_dpus <= 448) {
      if (block_size <= 69632) {
        if (num_dpus <= 64) {
          if (num_dpus <= 31) {
            return 0.05077495 + 0.003829138 * num_dpus + 1.717523e-06 * block_size + -1.342263e-08 * (double)num_dpus * block_size;
          } else {
            return 0.1972508 + -0.000427586 * num_dpus + 2.33157e-06 * block_size + 1.724463e-09 * (double)num_dpus * block_size;
          }
        } else {
          if (num_dpus <= 384) {
            return 0.05484015 + 0.000380846 * num_dpus + 8.65037e-06 * block_size + 8.540861e-09 * (double)num_dpus * block_size;
          } else {
            return 4.60142 + -0.01062523 * num_dpus + -9.522063e-05 * block_size + 2.751904e-07 * (double)num_dpus * block_size;
          }
        }
      } else {
        if (num_dpus <= 64) {
          if (num_dpus <= 31) {
            return 0.1095083 + -0.00595864 * num_dpus + 4.187868e-07 * block_size + 1.154562e-07 * (double)num_dpus * block_size;
          } else {
            return -0.8296854 + 0.007660076 * num_dpus + 1.328983e-05 * block_size + -8.044197e-08 * (double)num_dpus * block_size;
          }
        } else {
          if (num_dpus <= 384) {
            return -1.279531 + -0.0003789809 * num_dpus + 1.837949e-05 * block_size + 4.990792e-08 * (double)num_dpus * block_size;
          } else {
            return -2.337615 + 0.002944893 * num_dpus + 2.757875e-05 * block_size + 4.822206e-08 * (double)num_dpus * block_size;
          }
        }
      }
    } else {
      if (num_dpus <= 1600) {
        if (num_dpus <= 960) {
          if (block_size <= 38912) {
            return 1.075073 + -0.001674122 * num_dpus + -3.389339e-05 * block_size + 1.075828e-07 * (double)num_dpus * block_size;
          } else {
            return -2.213471 + 0.001594298 * num_dpus + 4.221674e-05 * block_size + 4.386186e-08 * (double)num_dpus * block_size;
          }
        } else {
          if (block_size <= 28672) {
            return -0.3384697 + 0.0008098937 * num_dpus + -8.142601e-06 * block_size + 2.701955e-08 * (double)num_dpus * block_size;
          } else {
            return -1.274416 + 7.408406e-05 * num_dpus + 5.925208e-05 * block_size + 2.407879e-08 * (double)num_dpus * block_size;
          }
        }
      } else {
        if (block_size <= 155648) {
          if (block_size <= 69632) {
            return -8.173779 + 0.003904497 * num_dpus + 8.474998e-05 * block_size + 1.83745e-08 * (double)num_dpus * block_size;
          } else {
            return -2.94753 + 0.002642839 * num_dpus + 1.546668e-05 * block_size + 4.153087e-08 * (double)num_dpus * block_size;
          }
        } else {
          if (block_size <= 294912) {
            return -34.90296 + 0.02039657 * num_dpus + 0.0001351163 * block_size + -2.826198e-08 * (double)num_dpus * block_size;
          } else {
            return 18.17729 + -0.01535891 * num_dpus + -2.01303e-05 * block_size + 8.056355e-08 * (double)num_dpus * block_size;
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