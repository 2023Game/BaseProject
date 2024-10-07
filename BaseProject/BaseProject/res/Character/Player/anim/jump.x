xof 0303txt 0032

template XSkinMeshHeader {
  <3cf169ce-ff7c-44ab-93c0-f78f62d172e2>
  WORD nMaxSkinWeightsPerVertex;
  WORD nMaxSkinWeightsPerFace;
  WORD nBones;
}

template SkinWeights {
  <6f0d123b-bad2-4167-a0d0-80224f25fabb>
  STRING transformNodeName;
  DWORD nWeights;
  array DWORD vertexIndices[nWeights];
  array float weights[nWeights];
  Matrix4x4 matrixOffset;
}

Frame Root {
  FrameTransformMatrix {
     1.000000, 0.000000, 0.000000, 0.000000,
     0.000000,-0.000000,-1.000000, 0.000000,
     0.000000, 1.000000,-0.000000, 0.000000,
     0.000000, 0.000000, 0.000000, 1.000000;;
  }
  Frame Armature {
    FrameTransformMatrix {
       0.100000, 0.000000, 0.000000, 0.000000,
       0.000000,-0.000000, 0.100000, 0.000000,
       0.000000,-0.100000,-0.000000, 0.000000,
       0.000000, 0.000000, 0.000000, 1.000000;;
    }
    Frame Armature_mixamorig_Hips {
      FrameTransformMatrix {
         0.996023, 0.070165, 0.054916, 0.000000,
        -0.064853, 0.993536,-0.093168, 0.000000,
        -0.061098, 0.089236, 0.994135, 0.000000,
         0.000591,85.598228, 0.037722, 1.000000;;
      }
      Frame Armature_mixamorig_LeftUpLeg {
        FrameTransformMatrix {
          -0.939442, 0.299333, 0.166871, 0.000000,
           0.187891, 0.042622, 0.981205, 0.000000,
           0.286612, 0.953197,-0.096280, 0.000000,
           9.789100,-5.875899,-0.583700, 1.000000;;
        }
        Frame Armature_mixamorig_LeftLeg {
          FrameTransformMatrix {
             0.991463,-0.110166, 0.069747, 0.000000,
            -0.036692,-0.749029,-0.661520, 0.000000,
             0.125120, 0.653314,-0.746677, 0.000000,
             0.000001,41.702000, 0.000003, 1.000000;;
          }
          Frame Armature_mixamorig_LeftFoot {
            FrameTransformMatrix {
               0.997397,-0.064945,-0.031318, 0.000000,
               0.068455, 0.989344, 0.128505, 0.000000,
               0.022638,-0.130314, 0.991214, 0.000000,
               0.000002,36.981014,-0.000006, 1.000000;;
            }
            Frame Armature_mixamorig_LeftToeBase {
              FrameTransformMatrix {
                 0.944070, 0.021145,-0.329067, 0.000000,
                 0.128784, 0.895041, 0.426985, 0.000000,
                 0.303557,-0.445482, 0.842258, 0.000000,
                 0.000000,16.700531, 0.000000, 1.000000;;
              }
              Frame Armature_mixamorig_LeftToe_End {
                FrameTransformMatrix {
                   1.000000,-0.000000,-0.000000, 0.000000,
                   0.000000, 1.000000, 0.000000, 0.000000,
                  -0.000000,-0.000000, 1.000000, 0.000000,
                  -0.000001, 7.120430,-0.000001, 1.000000;;
                }
              } // End of Armature_mixamorig_LeftToe_End
            } // End of Armature_mixamorig_LeftToeBase
          } // End of Armature_mixamorig_LeftFoot
        } // End of Armature_mixamorig_LeftLeg
      } // End of Armature_mixamorig_LeftUpLeg
      Frame Armature_mixamorig_RightUpLeg {
        FrameTransformMatrix {
          -0.713560,-0.512632,-0.477506, 0.000000,
          -0.677762, 0.332507, 0.655763, 0.000000,
          -0.177402, 0.791611,-0.584673, 0.000000,
          -9.857701,-5.875899,-0.583700, 1.000000;;
        }
        Frame Armature_mixamorig_RightLeg {
          FrameTransformMatrix {
             0.773928, 0.595831,-0.214526, 0.000000,
             0.115155,-0.465520,-0.877514, 0.000000,
            -0.622716, 0.654429,-0.428892, 0.000000,
             0.000005,41.702000, 0.000003, 1.000000;;
          }
          Frame Armature_mixamorig_RightFoot {
            FrameTransformMatrix {
               0.965620, 0.254239, 0.054229, 0.000000,
              -0.231739, 0.747332, 0.622729, 0.000000,
               0.117795,-0.613887, 0.780556, 0.000000,
              -0.000004,36.981030, 0.000001, 1.000000;;
            }
            Frame Armature_mixamorig_RightToeBase {
              FrameTransformMatrix {
                 0.999315,-0.004729,-0.036695, 0.000000,
                 0.025314, 0.810717, 0.584892, 0.000000,
                 0.026984,-0.585420, 0.810281, 0.000000,
                 0.000003,16.700539, 0.000010, 1.000000;;
              }
              Frame Armature_mixamorig_RightToe_End {
                FrameTransformMatrix {
                   1.000000, 0.000000, 0.000000, 0.000000,
                  -0.000000, 1.000000, 0.000000, 0.000000,
                   0.000000,-0.000000, 1.000000, 0.000000,
                   0.000002, 7.120425, 0.000002, 1.000000;;
                }
              } // End of Armature_mixamorig_RightToe_End
            } // End of Armature_mixamorig_RightToeBase
          } // End of Armature_mixamorig_RightFoot
        } // End of Armature_mixamorig_RightLeg
      } // End of Armature_mixamorig_RightUpLeg
      Frame Armature_mixamorig_Spine {
        FrameTransformMatrix {
           0.985194,-0.004660, 0.171377, 0.000000,
          -0.109973, 0.749688, 0.652590, 0.000000,
          -0.131520,-0.661775, 0.738076, 0.000000,
           0.017300,10.574800,-1.117201, 1.000000;;
        }
        Frame Armature_mixamorig_Spine1 {
          FrameTransformMatrix {
             0.999320,-0.024012, 0.027999, 0.000000,
             0.021538, 0.996103, 0.085531, 0.000000,
            -0.029944,-0.084870, 0.995942, 0.000000,
            -0.000000,12.405993, 0.000002, 1.000000;;
          }
          Frame Armature_mixamorig_Spine2 {
            FrameTransformMatrix {
               0.999476,-0.023426, 0.022333, 0.000000,
               0.018640, 0.980719, 0.194531, 0.000000,
              -0.026460,-0.194013, 0.980642, 0.000000,
               0.000000,14.178310,-0.000003, 1.000000;;
            }
            Frame Armature_mixamorig_LeftShoulder {
              FrameTransformMatrix {
                 0.441210, 0.048985,-0.896066, 0.000000,
                 0.894260,-0.107508, 0.434444, 0.000000,
                -0.075053,-0.992997,-0.091239, 0.000000,
                 5.934999,13.670202,-1.662600, 1.000000;;
              }
              Frame Armature_mixamorig_LeftArm {
                FrameTransformMatrix {
                   0.576446,-0.173087, 0.798593, 0.000000,
                   0.126306, 0.984436, 0.122195, 0.000000,
                  -0.807315, 0.030428, 0.589336, 0.000000,
                   0.000000,12.607897,-0.000003, 1.000000;;
                }
                Frame Armature_mixamorig_LeftForeArm {
                  FrameTransformMatrix {
                     0.547421, 0.460362, 0.698854, 0.000000,
                    -0.674966, 0.736565, 0.043505, 0.000000,
                    -0.494723,-0.495518, 0.713940, 0.000000,
                     0.000000,25.574306,-0.000008, 1.000000;;
                  }
                  Frame Armature_mixamorig_LeftHand {
                    FrameTransformMatrix {
                       0.668520, 0.743695,-0.000338, 0.000000,
                      -0.739402, 0.664709, 0.106988, 0.000000,
                       0.079791,-0.071274, 0.994260, 0.000000,
                       0.000007,21.603401,-0.000004, 1.000000;;
                    }
                    Frame Armature_mixamorig_LeftHandIndex1 {
                      FrameTransformMatrix {
                         0.991294, 0.108587, 0.074466, 0.000000,
                        -0.131308, 0.773572, 0.619955, 0.000000,
                         0.009714,-0.624336, 0.781096, 0.000000,
                        -2.341908, 9.909704,-0.291392, 1.000000;;
                      }
                      Frame Armature_mixamorig_LeftHandIndex2 {
                        FrameTransformMatrix {
                           0.997040, 0.076882, 0.000021, 0.000000,
                          -0.057006, 0.739094, 0.671186, 0.000000,
                           0.051586,-0.669201, 0.741289, 0.000000,
                          -0.000006, 3.776703, 0.000006, 1.000000;;
                        }
                        Frame Armature_mixamorig_LeftHandIndex3 {
                          FrameTransformMatrix {
                             0.999424,-0.033708,-0.003977, 0.000000,
                             0.033940, 0.993390, 0.109653, 0.000000,
                             0.000255,-0.109724, 0.993962, 0.000000,
                             0.000000, 2.542999,-0.000011, 1.000000;;
                          }
                          Frame Armature_mixamorig_LeftHandIndex4 {
                            FrameTransformMatrix {
                               1.000000,-0.000000, 0.000000, 0.000000,
                              -0.000000, 1.000000, 0.000000, 0.000000,
                               0.000000,-0.000000, 1.000000, 0.000000,
                               0.000002, 2.529106,-0.000006, 1.000000;;
                            }
                          } // End of Armature_mixamorig_LeftHandIndex4
                        } // End of Armature_mixamorig_LeftHandIndex3
                      } // End of Armature_mixamorig_LeftHandIndex2
                    } // End of Armature_mixamorig_LeftHandIndex1
                    Frame Armature_mixamorig_LeftHandMiddle1 {
                      FrameTransformMatrix {
                         0.999250, 0.037015, 0.011413, 0.000000,
                        -0.035965, 0.777209, 0.628214, 0.000000,
                         0.014383,-0.628153, 0.777957, 0.000000,
                        -0.000002, 9.700804, 0.000010, 1.000000;;
                      }
                      Frame Armature_mixamorig_LeftHandMiddle2 {
                        FrameTransformMatrix {
                           0.996352, 0.085335, 0.000865, 0.000000,
                          -0.054615, 0.629817, 0.774821, 0.000000,
                           0.065575,-0.772041, 0.632180, 0.000000,
                          -0.000004, 4.014606, 0.000008, 1.000000;;
                        }
                        Frame Armature_mixamorig_LeftHandMiddle3 {
                          FrameTransformMatrix {
                             0.999596,-0.026681,-0.009832, 0.000000,
                             0.028422, 0.947885, 0.317343, 0.000000,
                             0.000852,-0.317494, 0.948260, 0.000000,
                             0.000006, 2.996709, 0.000008, 1.000000;;
                          }
                          Frame Armature_mixamorig_LeftHandMiddle4 {
                            FrameTransformMatrix {
                               1.000000,-0.000000, 0.000000, 0.000000,
                               0.000000, 1.000000, 0.000000, 0.000000,
                               0.000000,-0.000000, 1.000000, 0.000000,
                               0.000004, 2.825002,-0.000002, 1.000000;;
                            }
                          } // End of Armature_mixamorig_LeftHandMiddle4
                        } // End of Armature_mixamorig_LeftHandMiddle3
                      } // End of Armature_mixamorig_LeftHandMiddle2
                    } // End of Armature_mixamorig_LeftHandMiddle1
                    Frame Armature_mixamorig_LeftHandPinky1 {
                      FrameTransformMatrix {
                         0.990339, 0.093299, 0.102587, 0.000000,
                        -0.138101, 0.596753, 0.790452, 0.000000,
                         0.012530,-0.796982, 0.603873, 0.000000,
                         4.712700, 8.985507,-0.221591, 1.000000;;
                      }
                      Frame Armature_mixamorig_LeftHandPinky2 {
                        FrameTransformMatrix {
                           0.999998, 0.001412, 0.000977, 0.000000,
                          -0.001341, 0.286580, 0.958055, 0.000000,
                           0.001073,-0.958055, 0.286581, 0.000000,
                           0.000004, 3.562499, 0.000000, 1.000000;;
                        }
                        Frame Armature_mixamorig_LeftHandPinky3 {
                          FrameTransformMatrix {
                             1.000000, 0.000694, 0.000182, 0.000000,
                            -0.000693, 0.870704, 0.491807, 0.000000,
                             0.000183,-0.491807, 0.870704, 0.000000,
                            -0.000004, 2.335207, 0.000001, 1.000000;;
                          }
                          Frame Armature_mixamorig_LeftHandPinky4 {
                            FrameTransformMatrix {
                               1.000000,-0.000000, 0.000000, 0.000000,
                               0.000000, 1.000000, 0.000000, 0.000000,
                               0.000000, 0.000000, 1.000000, 0.000000,
                              -0.000008, 2.037301,-0.000002, 1.000000;;
                            }
                          } // End of Armature_mixamorig_LeftHandPinky4
                        } // End of Armature_mixamorig_LeftHandPinky3
                      } // End of Armature_mixamorig_LeftHandPinky2
                    } // End of Armature_mixamorig_LeftHandPinky1
                    Frame Armature_mixamorig_LeftHandRing1 {
                      FrameTransformMatrix {
                         0.999939,-0.003801, 0.010382, 0.000000,
                        -0.007470, 0.459970, 0.887903, 0.000000,
                        -0.008150,-0.887926, 0.459914, 0.000000,
                         2.241694, 9.308413,-0.143485, 1.000000;;
                      }
                      Frame Armature_mixamorig_LeftHandRing2 {
                        FrameTransformMatrix {
                           0.999744, 0.016952, 0.015008, 0.000000,
                          -0.022610, 0.712828, 0.700975, 0.000000,
                           0.001185,-0.701134, 0.713028, 0.000000,
                          -0.000007, 4.019394, 0.000011, 1.000000;;
                        }
                        Frame Armature_mixamorig_LeftHandRing3 {
                          FrameTransformMatrix {
                             0.999774,-0.021263, 0.000938, 0.000000,
                             0.020219, 0.962567, 0.270288, 0.000000,
                            -0.006650,-0.270208, 0.962779, 0.000000,
                            -0.000003, 2.790807, 0.000001, 1.000000;;
                          }
                          Frame Armature_mixamorig_LeftHandRing4 {
                            FrameTransformMatrix {
                               1.000000, 0.000000, 0.000000, 0.000000,
                               0.000000, 1.000000,-0.000000, 0.000000,
                              -0.000000, 0.000000, 1.000000, 0.000000,
                              -0.000010, 2.540689, 0.000007, 1.000000;;
                            }
                          } // End of Armature_mixamorig_LeftHandRing4
                        } // End of Armature_mixamorig_LeftHandRing3
                      } // End of Armature_mixamorig_LeftHandRing2
                    } // End of Armature_mixamorig_LeftHandRing1
                    Frame Armature_mixamorig_LeftHandThumb1 {
                      FrameTransformMatrix {
                         0.865766, 0.491568,-0.093858, 0.000000,
                        -0.457348, 0.853307, 0.250400, 0.000000,
                         0.203179,-0.173862, 0.963582, 0.000000,
                        -2.370406, 3.545607, 1.908206, 1.000000;;
                      }
                      Frame Armature_mixamorig_LeftHandThumb2 {
                        FrameTransformMatrix {
                           0.954992,-0.274752,-0.111807, 0.000000,
                           0.291755, 0.938075, 0.186800, 0.000000,
                           0.053560,-0.211013, 0.976015, 0.000000,
                           0.000002, 2.808403, 0.000000, 1.000000;;
                        }
                        Frame Armature_mixamorig_LeftHandThumb3 {
                          FrameTransformMatrix {
                             0.618159, 0.106484,-0.778808, 0.000000,
                             0.121190, 0.966025, 0.228273, 0.000000,
                             0.776655,-0.235493, 0.584252, 0.000000,
                             0.000005, 2.378999, 0.000002, 1.000000;;
                          }
                          Frame Armature_mixamorig_LeftHandThumb4 {
                            FrameTransformMatrix {
                               1.000000,-0.000000,-0.000000, 0.000000,
                              -0.000000, 1.000000,-0.000000, 0.000000,
                              -0.000000, 0.000000, 1.000000, 0.000000,
                              -0.000010, 2.445002,-0.000004, 1.000000;;
                            }
                          } // End of Armature_mixamorig_LeftHandThumb4
                        } // End of Armature_mixamorig_LeftHandThumb3
                      } // End of Armature_mixamorig_LeftHandThumb2
                    } // End of Armature_mixamorig_LeftHandThumb1
                  } // End of Armature_mixamorig_LeftHand
                } // End of Armature_mixamorig_LeftForeArm
              } // End of Armature_mixamorig_LeftArm
            } // End of Armature_mixamorig_LeftShoulder
            Frame Armature_mixamorig_Neck {
              FrameTransformMatrix {
                 0.994096,-0.097487,-0.047630, 0.000000,
                 0.094193, 0.993290,-0.067102, 0.000000,
                 0.053852, 0.062219, 0.996608, 0.000000,
                 0.024399,15.862293,-1.675797, 1.000000;;
              }
              Frame Armature_mixamorig_Head {
                FrameTransformMatrix {
                   0.951197,-0.298792,-0.077123, 0.000000,
                   0.230315, 0.853739,-0.466996, 0.000000,
                   0.205378, 0.426442, 0.880890, 0.000000,
                   0.000001, 5.563990, 0.988105, 1.000000;;
                }
                Frame Armature_mixamorig_HeadTop_End {
                  FrameTransformMatrix {
                     1.000000,-0.000000,-0.000000, 0.000000,
                    -0.000000, 1.000000,-0.000000, 0.000000,
                    -0.000000, 0.000000, 1.000000, 0.000000,
                    -0.000001,18.311907, 3.086899, 1.000000;;
                  }
                } // End of Armature_mixamorig_HeadTop_End
                Frame Armature_mixamorig_LeftEye {
                  FrameTransformMatrix {
                    -0.000000,-0.000000,-1.000000, 0.000000,
                     1.000000,-0.000000,-0.000000, 0.000000,
                    -0.000000,-1.000000,-0.000000, 0.000000,
                     2.284398, 7.935709, 7.063698, 1.000000;;
                  }
                } // End of Armature_mixamorig_LeftEye
                Frame Armature_mixamorig_RightEye {
                  FrameTransformMatrix {
                     0.000000, 0.000000, 1.000000, 0.000000,
                    -1.000000, 0.000000,-0.000000, 0.000000,
                    -0.000000,-1.000000,-0.000000, 0.000000,
                    -2.377501, 7.935704, 7.063699, 1.000000;;
                  }
                } // End of Armature_mixamorig_RightEye
              } // End of Armature_mixamorig_Head
            } // End of Armature_mixamorig_Neck
            Frame Armature_mixamorig_RightShoulder {
              FrameTransformMatrix {
                 0.266580,-0.073555, 0.961001, 0.000000,
                -0.941982, 0.191127, 0.275933, 0.000000,
                -0.203970,-0.978805,-0.018337, 0.000000,
                -5.935001,13.670603,-1.662600, 1.000000;;
              }
              Frame Armature_mixamorig_RightArm {
                FrameTransformMatrix {
                   0.903569,-0.122615,-0.410522, 0.000000,
                   0.141391, 0.989832, 0.015562, 0.000000,
                   0.404439,-0.072105, 0.911718, 0.000000,
                   0.000002,12.607899, 0.000015, 1.000000;;
                }
                Frame Armature_mixamorig_RightForeArm {
                  FrameTransformMatrix {
                     0.778126,-0.500934,-0.378926, 0.000000,
                     0.565339, 0.821443, 0.074993, 0.000000,
                     0.273699,-0.272576, 0.922383, 0.000000,
                    -0.000012,25.574301,-0.000003, 1.000000;;
                  }
                  Frame Armature_mixamorig_RightHand {
                    FrameTransformMatrix {
                       0.946628,-0.321000, 0.029233, 0.000000,
                       0.319566, 0.946495, 0.045002, 0.000000,
                      -0.042114,-0.033258, 0.998559, 0.000000,
                      -0.000003,21.603407, 0.000001, 1.000000;;
                    }
                    Frame Armature_mixamorig_RightHandIndex1 {
                      FrameTransformMatrix {
                         0.998628, 0.052352,-0.001276, 0.000000,
                        -0.023919, 0.477655, 0.878222, 0.000000,
                         0.046586,-0.876986, 0.478251, 0.000000,
                         2.341900, 9.909709,-0.290993, 1.000000;;
                      }
                      Frame Armature_mixamorig_RightHandIndex2 {
                        FrameTransformMatrix {
                           0.997036,-0.076931,-0.000082, 0.000000,
                           0.030618, 0.395833, 0.917812, 0.000000,
                          -0.070576,-0.915094, 0.397016, 0.000000,
                          -0.000003, 3.776798, 0.000002, 1.000000;;
                        }
                        Frame Armature_mixamorig_RightHandIndex3 {
                          FrameTransformMatrix {
                             0.999418, 0.007211, 0.033330, 0.000000,
                            -0.032368, 0.508175, 0.860645, 0.000000,
                            -0.010731,-0.861223, 0.508113, 0.000000,
                             0.000001, 2.543011,-0.000003, 1.000000;;
                          }
                          Frame Armature_mixamorig_RightHandIndex4 {
                            FrameTransformMatrix {
                               1.000000,-0.000000,-0.000000, 0.000000,
                              -0.000000, 1.000000,-0.000000, 0.000000,
                              -0.000000, 0.000000, 1.000000, 0.000000,
                               0.000001, 2.529011,-0.000002, 1.000000;;
                            }
                          } // End of Armature_mixamorig_RightHandIndex4
                        } // End of Armature_mixamorig_RightHandIndex3
                      } // End of Armature_mixamorig_RightHandIndex2
                    } // End of Armature_mixamorig_RightHandIndex1
                    Frame Armature_mixamorig_RightHandMiddle1 {
                      FrameTransformMatrix {
                         0.999766, 0.001958,-0.021526, 0.000000,
                         0.019272, 0.370172, 0.928763, 0.000000,
                         0.009787,-0.928961, 0.370048, 0.000000,
                         0.000002, 9.700704, 0.000000, 1.000000;;
                      }
                      Frame Armature_mixamorig_RightHandMiddle2 {
                        FrameTransformMatrix {
                           0.996317,-0.085725,-0.001660, 0.000000,
                           0.009873, 0.095465, 0.995384, 0.000000,
                          -0.085171,-0.991735, 0.095960, 0.000000,
                           0.000006, 4.014697, 0.000002, 1.000000;;
                        }
                        Frame Armature_mixamorig_RightHandMiddle3 {
                          FrameTransformMatrix {
                             0.999608, 0.023207, 0.015694, 0.000000,
                            -0.028007, 0.841138, 0.540095, 0.000000,
                            -0.000666,-0.540322, 0.841458, 0.000000,
                             0.000000, 2.996714, 0.000008, 1.000000;;
                          }
                          Frame Armature_mixamorig_RightHandMiddle4 {
                            FrameTransformMatrix {
                               1.000000, 0.000000, 0.000000, 0.000000,
                              -0.000000, 1.000000, 0.000000, 0.000000,
                               0.000000, 0.000000, 1.000000, 0.000000,
                               0.000008, 2.825002,-0.000004, 1.000000;;
                            }
                          } // End of Armature_mixamorig_RightHandMiddle4
                        } // End of Armature_mixamorig_RightHandMiddle3
                      } // End of Armature_mixamorig_RightHandMiddle2
                    } // End of Armature_mixamorig_RightHandMiddle1
                    Frame Armature_mixamorig_RightHandPinky1 {
                      FrameTransformMatrix {
                         0.997586,-0.002065,-0.069406, 0.000000,
                         0.069412, 0.056400, 0.995992, 0.000000,
                         0.001858,-0.998406, 0.056407, 0.000000,
                        -4.712702, 8.985513,-0.220999, 1.000000;;
                      }
                      Frame Armature_mixamorig_RightHandPinky2 {
                        FrameTransformMatrix {
                           0.999999,-0.001187,-0.000612, 0.000000,
                           0.001188, 0.581894, 0.813264, 0.000000,
                          -0.000610,-0.813264, 0.581895, 0.000000,
                           0.000003, 3.562502,-0.000005, 1.000000;;
                        }
                        Frame Armature_mixamorig_RightHandPinky3 {
                          FrameTransformMatrix {
                             1.000000,-0.000637,-0.000146, 0.000000,
                             0.000637, 0.900092, 0.435699, 0.000000,
                            -0.000146,-0.435699, 0.900092, 0.000000,
                            -0.000000, 2.335205, 0.000015, 1.000000;;
                          }
                          Frame Armature_mixamorig_RightHandPinky4 {
                            FrameTransformMatrix {
                               1.000000,-0.000000, 0.000000, 0.000000,
                               0.000000, 1.000000,-0.000000, 0.000000,
                              -0.000000, 0.000000, 1.000000, 0.000000,
                              -0.000010, 2.037296, 0.000005, 1.000000;;
                            }
                          } // End of Armature_mixamorig_RightHandPinky4
                        } // End of Armature_mixamorig_RightHandPinky3
                      } // End of Armature_mixamorig_RightHandPinky2
                    } // End of Armature_mixamorig_RightHandPinky1
                    Frame Armature_mixamorig_RightHandRing1 {
                      FrameTransformMatrix {
                         0.996658, 0.080283, 0.015083, 0.000000,
                        -0.029915, 0.186897, 0.981924, 0.000000,
                         0.076012,-0.979094, 0.188674, 0.000000,
                        -2.241697, 9.308410,-0.142988, 1.000000;;
                      }
                      Frame Armature_mixamorig_RightHandRing2 {
                        FrameTransformMatrix {
                           0.999713,-0.020224,-0.012822, 0.000000,
                           0.018107, 0.288066, 0.957439, 0.000000,
                          -0.015670,-0.957397, 0.288350, 0.000000,
                           0.000001, 4.019408,-0.000004, 1.000000;;
                        }
                        Frame Armature_mixamorig_RightHandRing3 {
                          FrameTransformMatrix {
                             0.999785, 0.020713,-0.001214, 0.000000,
                            -0.015298, 0.775388, 0.631300, 0.000000,
                             0.014017,-0.631145, 0.775538, 0.000000,
                            -0.000006, 2.790808, 0.000012, 1.000000;;
                          }
                          Frame Armature_mixamorig_RightHandRing4 {
                            FrameTransformMatrix {
                               1.000000, 0.000000, 0.000000, 0.000000,
                              -0.000000, 1.000000, 0.000000, 0.000000,
                               0.000000,-0.000000, 1.000000, 0.000000,
                               0.000006, 2.540691, 0.000008, 1.000000;;
                            }
                          } // End of Armature_mixamorig_RightHandRing4
                        } // End of Armature_mixamorig_RightHandRing3
                      } // End of Armature_mixamorig_RightHandRing2
                    } // End of Armature_mixamorig_RightHandRing1
                    Frame Armature_mixamorig_RightHandThumb1 {
                      FrameTransformMatrix {
                         0.976735,-0.103502, 0.187820, 0.000000,
                        -0.095349, 0.574893, 0.812653, 0.000000,
                        -0.192088,-0.811655, 0.551649, 0.000000,
                         2.370399, 3.545609, 1.909006, 1.000000;;
                      }
                      Frame Armature_mixamorig_RightHandThumb2 {
                        FrameTransformMatrix {
                           0.951149, 0.004181, 0.308702, 0.000000,
                           0.056200, 0.980856,-0.186446, 0.000000,
                          -0.303572, 0.194688, 0.932706, 0.000000,
                           0.000003, 2.807991, 0.000000, 1.000000;;
                        }
                        Frame Armature_mixamorig_RightHandThumb3 {
                          FrameTransformMatrix {
                             0.927103, 0.226657,-0.298509, 0.000000,
                            -0.296321, 0.930930,-0.213454, 0.000000,
                             0.229510, 0.286348, 0.930231, 0.000000,
                             0.000006, 2.379011, 0.000002, 1.000000;;
                          }
                          Frame Armature_mixamorig_RightHandThumb4 {
                            FrameTransformMatrix {
                               1.000000, 0.000000,-0.000000, 0.000000,
                              -0.000000, 1.000000,-0.000000, 0.000000,
                               0.000000, 0.000000, 1.000000, 0.000000,
                              -0.000004, 2.445295,-0.000006, 1.000000;;
                            }
                          } // End of Armature_mixamorig_RightHandThumb4
                        } // End of Armature_mixamorig_RightHandThumb3
                      } // End of Armature_mixamorig_RightHandThumb2
                    } // End of Armature_mixamorig_RightHandThumb1
                  } // End of Armature_mixamorig_RightHand
                } // End of Armature_mixamorig_RightForeArm
              } // End of Armature_mixamorig_RightArm
            } // End of Armature_mixamorig_RightShoulder
          } // End of Armature_mixamorig_Spine2
        } // End of Armature_mixamorig_Spine1
      } // End of Armature_mixamorig_Spine
    } // End of Armature_mixamorig_Hips
  } // End of Armature
} // End of Root
AnimationSet Global {
  Animation {
    {Armature}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.707107, 0.707107, 0.000000, 0.000000;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 0.100000, 0.100000, 0.100000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 0.000000, 0.000000, 0.000000;;;
    }
  }
  Animation {
    {Armature_mixamorig_Hips}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.997960,-0.045694,-0.029063, 0.033824;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 0.000591,85.598228, 0.037722;;;
    }
  }
  Animation {
    {Armature_mixamorig_Spine}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.931794, 0.352644,-0.081267, 0.028255;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 0.017300,10.574800,-1.117201;;;
    }
  }
  Animation {
    {Armature_mixamorig_Spine1}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.998920, 0.042646,-0.014502,-0.011400;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3;-0.000000,12.405993, 0.000002;;;
    }
  }
  Animation {
    {Armature_mixamorig_Spine2}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.995093, 0.097615,-0.012258,-0.010568;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 0.000000,14.178310,-0.000003;;;
    }
  }
  Animation {
    {Armature_mixamorig_Neck}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.997997,-0.032395, 0.025421,-0.048016;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 0.024399,15.862293,-1.675797;;;
    }
  }
  Animation {
    {Armature_mixamorig_Head}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.959925,-0.232684, 0.073574,-0.137799;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 0.000001, 5.563990, 0.988105;;;
    }
  }
  Animation {
    {Armature_mixamorig_HeadTop_End}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-1.000000,-0.000000,-0.000000, 0.000000;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3;-0.000001,18.311907, 3.086899;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftEye}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.500000, 0.500000, 0.500000,-0.500000;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 2.284398, 7.935709, 7.063698;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightEye}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.500000, 0.500000,-0.500000, 0.500000;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3;-2.377501, 7.935704, 7.063699;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftShoulder}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.557329, 0.640304, 0.368280,-0.379163;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 5.934999,13.670202,-1.662600;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftArm}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.887443, 0.025852,-0.452398,-0.084341;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 0.000000,12.607897,-0.000003;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftForeArm}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.865726, 0.155657,-0.344675, 0.327854;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 0.000000,25.574306,-0.000008;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHand}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.912070, 0.048862, 0.021964, 0.406519;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 0.000007,21.603401,-0.000004;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandThumb1}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.959512, 0.110541, 0.077393, 0.247239;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3;-2.370406, 3.545607, 1.908206;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandThumb2}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.983499, 0.101122, 0.042035,-0.144003;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 0.000002, 2.808403, 0.000000;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandThumb3}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.890005, 0.130271, 0.436925,-0.004131;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 0.000005, 2.378999, 0.000002;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandThumb4}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-1.000000,-0.000000,-0.000000,-0.000000;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3;-0.000010, 2.445002,-0.000004;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandIndex1}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.941536, 0.330388,-0.017193, 0.063698;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3;-2.341908, 9.909704,-0.291392;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandIndex2}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.932392, 0.359395, 0.013826, 0.035899;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3;-0.000006, 3.776703, 0.000006;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandIndex3}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.998346, 0.054935, 0.001060,-0.016940;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 0.000000, 2.542999,-0.000011;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandIndex4}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-1.000000, 0.000000,-0.000000,-0.000000;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 0.000002, 2.529106,-0.000006;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandMiddle1}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.942658, 0.333198, 0.000788, 0.019355;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3;-0.000002, 9.700804, 0.000010;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandMiddle2}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.902545, 0.428472, 0.017924, 0.038766;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3;-0.000004, 4.014606, 0.000008;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandMiddle3}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.986882, 0.160819, 0.002707,-0.013959;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 0.000006, 2.996709, 0.000008;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandMiddle4}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-1.000000, 0.000000, 0.000000,-0.000000;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 0.000004, 2.825002,-0.000002;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandRing1}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.854374, 0.519629,-0.005423, 0.001074;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 2.241694, 9.308413,-0.143485;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandRing2}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.925419, 0.378777,-0.003734, 0.010687;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3;-0.000007, 4.019394, 0.000011;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandRing3}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.990596, 0.136407,-0.001915,-0.010469;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3;-0.000003, 2.790807, 0.000001;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandRing4}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-1.000000,-0.000000,-0.000000, 0.000000;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3;-0.000010, 2.540689, 0.000007;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandPinky1}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.893164, 0.444329,-0.025207, 0.064770;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 4.712700, 8.985507,-0.221591;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandPinky2}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.802054, 0.597251, 0.000030, 0.000858;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 0.000004, 3.562499, 0.000000;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandPinky3}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.967136, 0.254260, 0.000000, 0.000359;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3;-0.000004, 2.335207, 0.000001;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandPinky4}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-1.000000, 0.000000,-0.000000,-0.000000;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3;-0.000008, 2.037301,-0.000002;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightShoulder}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.599869, 0.522922,-0.485511, 0.361924;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3;-5.935001,13.670603,-1.662600;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightArm}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.975336, 0.022471, 0.208892,-0.067671;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 0.000002,12.607899, 0.000015;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightForeArm}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.938343, 0.092602, 0.173877,-0.284084;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3;-0.000012,25.574301,-0.000003;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHand}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.986367, 0.019835,-0.018083,-0.162355;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3;-0.000003,21.603407, 0.000001;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandThumb1}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.880806, 0.461029,-0.107830,-0.002314;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 2.370399, 3.545609, 1.909006;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandThumb2}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.982944,-0.096937,-0.155725,-0.013230;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 0.000003, 2.807991, 0.000000;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandThumb3}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.973173,-0.128395, 0.135644, 0.134349;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 0.000006, 2.379011, 0.000002;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandThumb4}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-1.000000,-0.000000, 0.000000, 0.000000;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3;-0.000004, 2.445295,-0.000006;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandIndex1}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.859438, 0.510569, 0.013922, 0.022186;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 2.341900, 9.909709,-0.290993;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandIndex2}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.835148, 0.548677,-0.021102,-0.032195;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3;-0.000003, 3.776798, 0.000002;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandIndex3}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.868290, 0.495764,-0.012686, 0.011396;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 0.000001, 2.543011,-0.000003;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandIndex4}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-1.000000,-0.000000, 0.000000, 0.000000;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 0.000001, 2.529011,-0.000002;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandMiddle1}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.827645, 0.561148, 0.009459,-0.005230;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 0.000002, 9.700704, 0.000000;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandMiddle2}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.739551, 0.671731,-0.028230,-0.032316;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 0.000006, 4.014697, 0.000002;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandMiddle3}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.959453, 0.281519,-0.004263, 0.013345;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 0.000000, 2.996714, 0.000008;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandMiddle4}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-1.000000, 0.000000, 0.000000, 0.000000;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 0.000008, 2.825002,-0.000004;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandRing1}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.770102, 0.636610, 0.019780, 0.035774;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3;-2.241697, 9.308410,-0.142988;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandRing2}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.802516, 0.596510,-0.000887,-0.011941;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 0.000001, 4.019408,-0.000004;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandRing3}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.942166, 0.334985, 0.004042, 0.009555;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3;-0.000006, 2.790808, 0.000012;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandRing4}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-1.000000, 0.000000,-0.000000, 0.000000;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 0.000006, 2.540691, 0.000008;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandPinky1}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.726360, 0.686437, 0.024528,-0.024601;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3;-4.712702, 8.985513,-0.220999;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandPinky2}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.889352, 0.457223, 0.000001,-0.000668;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 0.000003, 3.562502,-0.000005;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandPinky3}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.974703, 0.223504,-0.000000,-0.000327;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3;-0.000000, 2.335205, 0.000015;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandPinky4}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-1.000000,-0.000000,-0.000000,-0.000000;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3;-0.000010, 2.037296, 0.000005;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftUpLeg}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.041550, 0.168967, 0.720885, 0.670856;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 0.999998, 0.999942, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 9.789100,-5.875899,-0.583700;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftLeg}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.352050,-0.933698, 0.039322,-0.052176;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 0.000001,41.702000, 0.000003;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftFoot}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.997241, 0.064884, 0.013526,-0.033442;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 0.000002,36.981014,-0.000006;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftToeBase}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.959345, 0.227360, 0.164858,-0.028050;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 0.000000,16.700531, 0.000000;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftToe_End}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-1.000000, 0.000000,-0.000000,-0.000000;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3;-0.000001, 7.120430,-0.000001;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightUpLeg}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.092577,-0.367065, 0.810913, 0.446221;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 0.999986, 0.999974, 0.999981;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3;-9.857701,-5.875899,-0.583700;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightLeg}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.468913,-0.816753,-0.217626, 0.256271;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 0.000005,41.702000, 0.000003;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightFoot}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.934546, 0.330807, 0.017004, 0.130004;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3;-0.000004,36.981030, 0.000001;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightToeBase}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-0.951356, 0.307538, 0.016734,-0.007895;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 0.000003,16.700539, 0.000010;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightToe_End}
    AnimationKey { // Rotation
      0;
      1;
      0;4;-1.000000, 0.000000,-0.000000, 0.000000;;;
    }
    AnimationKey { // Scale
      1;
      1;
      0;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      1;
      0;3; 0.000002, 7.120425, 0.000002;;;
    }
  }
} // End of AnimationSet Global
