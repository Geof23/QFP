/* -- LICENSE BEGIN --
 *
 * Copyright (c) 2015-2018, Lawrence Livermore National Security, LLC.
 *
 * Produced at the Lawrence Livermore National Laboratory
 *
 * Written by
 *   Michael Bentley (mikebentley15@gmail.com),
 *   Geof Sawaya (fredricflinstone@gmail.com),
 *   and Ian Briggs (ian.briggs@utah.edu)
 * under the direction of
 *   Ganesh Gopalakrishnan
 *   and Dong H. Ahn.
 *
 * LLNL-CODE-743137
 *
 * All rights reserved.
 *
 * This file is part of FLiT. For details, see
 *   https://pruners.github.io/flit
 * Please also read
 *   https://github.com/PRUNERS/FLiT/blob/master/LICENSE
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following
 * conditions are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the disclaimer below.
 *
 * - Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the disclaimer
 *   (as noted below) in the documentation and/or other materials
 *   provided with the distribution.
 *
 * - Neither the name of the LLNS/LLNL nor the names of its
 *   contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL LAWRENCE LIVERMORE NATIONAL
 * SECURITY, LLC, THE U.S. DEPARTMENT OF ENERGY OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Additional BSD Notice
 *
 * 1. This notice is required to be provided under our contract
 *    with the U.S. Department of Energy (DOE). This work was
 *    produced at Lawrence Livermore National Laboratory under
 *    Contract No. DE-AC52-07NA27344 with the DOE.
 *
 * 2. Neither the United States Government nor Lawrence Livermore
 *    National Security, LLC nor any of their employees, makes any
 *    warranty, express or implied, or assumes any liability or
 *    responsibility for the accuracy, completeness, or usefulness of
 *    any information, apparatus, product, or process disclosed, or
 *    represents that its use would not infringe privately-owned
 *    rights.
 *
 * 3. Also, reference herein to any specific commercial products,
 *    process, or services by trade name, trademark, manufacturer or
 *    otherwise does not necessarily constitute or imply its
 *    endorsement, recommendation, or favoring by the United States
 *    Government or Lawrence Livermore National Security, LLC. The
 *    views and opinions of authors expressed herein do not
 *    necessarily state or reflect those of the United States
 *    Government or Lawrence Livermore National Security, LLC, and
 *    shall not be used for advertising or product endorsement
 *    purposes.
 *
 * -- LICENSE END -- */

#include "simple_convex_hull.h"

#include <flit.h>

#include <cmath>
#include <cstdio>
#include <typeinfo>
#include <limits>

std::ostream& operator<< (std::ostream &out, const convex_hull::Point &p) {
  out << "(" << p.id << ", " << p.x << ", " << p.y << ")";
  return out;
}

template <typename T>
class SimpleCHull: public flit::TestBase<T> {
public:
  SimpleCHull(std::string id) : flit::TestBase<T>(std::move(id)) {}

  virtual size_t getInputsPerRun() { return 1000; }
  virtual std::vector<T> getDefaultInput() {
    return {
      -53.1267929, -39.8255043, 38.1252747, -4.84974098,
      35.5682678, 31.9674683, -21.3283195, 61.1250916,
      64.8973999, 52.0928802, 62.42379, -70.2806168,
      76.3827667, -57.7143555, -0.25641638, 54.9413719,
      -50.949295, -16.9185905, -73.1930237, 69.966568,
      -5.42787361, -84.0428467, 21.3372879, -86.5024948,
      -3.08435488, 24.512928, -66.2378998, -55.5923347,
      31.1993828, 57.7348824, -4.09711742, 25.9350319,
      22.9420891, -47.406559, 42.9041328, -84.3193665,
      6.3797574, -59.8589058, -87.842865, 40.9584427,
      -2.73331881, -20.386364, 53.8916779, -7.7852726,
      73.0152817, 58.6679688, 52.188797, -59.3687439,
      63.5682297, 0.814619899, 32.4166794, -23.2943649,
      -18.5796299, -46.2460251, 76.670578, 0.154865563,
      -16.7003956, 58.2950974, 44.5625381, -85.5010071,
      -26.7997475, 10.146698, -37.7471313, -68.5062408,
      -15.4410152, 10.1897087, -69.6117706, -22.5938282,
      -63.201767, 29.0127907, -63.0701141, 17.2787704,
      73.2750092, 12.6404228, 74.1420898, 64.8555603,
      -28.6916046, -51.8502693, 10.5195246, -46.5580978,
      32.178215, 64.7550507, -48.0336075, -86.4014282,
      23.5417328, 33.6696625, -81.2138519, -74.5933838,
      10.5300388, 46.5625381, 26.373024, -29.8022804,
      78.5281067, 71.8397369, -15.0946655, -1.56150866,
      -17.9705448, -62.8875847, -19.1226292, -76.1396179,
      -28.8420887, 17.8072586, 24.3530197, -55.5670853,
      0.128965259, -44.3346291, 14.3211842, 71.437355,
      -12.9710464, 46.6595535, 29.9119644, -45.4414177,
      -48.2012634, -13.0889454, 37.8384476, 58.5543289,
      -60.8540039, 56.6245995, 5.77977943, 14.3246136,
      -56.4287338, -49.2819214, 24.9064522, 43.9181938,
      -42.0907516, -90.1882172, 12.0379734, -19.6771774,
      16.6054878, 76.1292038, -31.1682014, 52.4119759,
      -84.2446899, 15.0036612, 18.6637383, -0.901874363,
      53.8828888, 38.0176315, -53.7289734, 62.7306976,
      -58.1525574, 76.1829987, -64.1454544, -41.7052383,
      -36.9059563, -39.8395767, -47.7994995, -14.5461035,
      38.6038666, 76.5455475, -13.7540646, 46.8237076,
      -37.3849449, -88.8476105, -68.8739624, 71.6403198,
      7.43161011, 26.377861, 38.4305611, 45.8559418,
      37.8584824, -38.8209381, -61.3479614, -41.3535118,
      -18.7845726, 57.3157768, 57.7446136, -46.3364182,
      -4.66659069, 68.6642227, -65.0404434, 20.3947144,
      -14.7686615, 21.9301109, 0.508324027, 31.5392399,
      3.90937996, 35.9226837, 3.46056843, -38.9214783,
      -65.7129211, 11.5253544, 12.9349422, 66.5834351,
      -72.2895355, 27.274828, -39.9574127, -43.039093,
      -10.9958868, -47.6101456, 7.84955645, 48.6814423,
      47.898777, 11.1501808, 71.9765015, -35.5343781,
      -12.9687614, -29.8947563, 87.8105011, -12.6026459,
      -25.8791199, 9.23748207, -5.74050522, 7.21465588,
      -33.4809875, 77.9816742, 25.2213287, 53.6422501,
      -45.7115211, 50.5007401, 19.753479, -28.2105713,
      -54.7600479, -48.7463074, 24.8402863, 59.4178314,
      78.5285187, -61.2004318, 21.4114456, 72.5653458,
      25.5192871, -52.1737251, -56.9343719, 8.76947975,
      42.1903152, -66.3925934, -21.7321968, 15.6889811,
      3.71265411, -33.9216995, -78.3483887, -52.4851761,
      93.8810577, 34.476387, -23.4516811, 46.8674927,
      -47.1578217, -79.6650696, -23.7547073, -45.006897,
      54.0495186, 79.212616, -51.3986282, 21.108326,
      -69.5336761, -21.5256329, -79.0897064, 14.0275612,
      68.3899384, 6.97031069, -56.2368279, 47.8259239,
      38.0104446, -29.9573345, 43.0628281, 15.5521812,
      54.7660904, -43.3179474, -50.1935577, 80.2975922,
      5.97420835, 57.9254799, -53.6223221, -78.325882,
      97.4345703, -12.4254112, 8.92573261, -49.7232513,
      -87.057785, 33.0334625, 5.26985216, -11.1894112,
      12.2460899, -41.0960693, 93.1327744, -16.9034576,
      2.02687669, 78.6916351, -16.4084702, -53.8476334,
      4.2272253, 10.5685596, 58.6268692, -22.4109116,
      -32.9213486, 20.2549782, -20.3912964, -59.5899887,
      60.1508865, 12.6289978, -60.7271271, 87.9439545,
      57.0219116, 36.7665672, 14.6507616, 73.0217438,
      29.3738632, 71.4389343, -93.487648, 42.3160782,
      -38.357338, 62.8982162, -50.3080482, 73.8887558,
      26.8348541, 29.2921486, 26.6665783, -52.5729942,
      -51.6320953, -54.3904724, -23.2067757, 52.5951385,
      -57.3544769, 21.8875275, 35.2169266, -7.06197262,
      -39.2922211, -66.6091003, 33.3480453, 42.6775208,
      -67.5126648, -40.9116554, -64.3458252, 72.7230835,
      17.6737671, -44.6623764, 45.7448387, -34.3870964,
      48.5954132, -25.9239578, 29.7478313, 74.8866577,
      -9.10904312, -34.0927887, -46.1918869, -82.2742004,
      0.232057601, 80.4746857, 34.8340988, 48.5999641,
      31.1169224, 33.4461746, -41.6346359, 56.9762955,
      60.3664017, 58.2308731, 36.381752, 7.54161644,
      -21.9107895, 91.5486526, 72.038002, -6.20960808,
      -30.7977276, -73.742569, -76.3162537, -8.09125519,
      68.0624771, -8.75255966, -20.6595039, 16.6578979,
      48.5373421, 14.1210318, -25.2415924, 61.2471466,
      -14.9390593, -49.6146393, -15.9943371, -79.3555756,
      -50.5746651, -45.8088074, -8.93676758, -32.9903069,
      -42.6813545, 32.6424561, 10.4534149, -82.314949,
      9.43860626, -17.8134117, 25.2266655, 70.7416611,
      -43.0509109, 15.8299246, 86.3509064, -68.8159256,
      -36.093811, -3.49791384, 23.0928192, 1.64996493,
      70.9633789, -62.2152557, -63.1268616, -40.1151581,
      -61.6268005, -66.5496063, -60.3027191, 23.4341393,
      -32.950386, -11.6484795, 27.2924175, 67.59095,
      -52.4245911, 40.1744919, -30.0479336, -11.8920956,
      -43.9691925, 63.619339, 92.260376, 30.1179943,
      32.2733612, -42.1288261, -41.9700699, 29.6065826,
      38.349678, 14.0621223, 44.0045166, 7.28857517,
      -70.8705139, -75.7324066, -72.4961853, -81.3418427,
      -3.61780024, 15.49296, 65.0104141, 17.9692535,
      13.5919342, -59.9408836, -40.0313263, 2.46039629,
      -66.5566406, 9.07993317, -29.9486427, -35.767395,
      -32.180294, -54.9638748, -61.1920624, 23.8505135,
      30.4743137, 17.5357418, 75.7873535, -18.6870461,
      75.4069214, 3.49856925, -70.5151825, -45.8592834,
      -47.0878868, -4.69182968, 26.0778656, -17.9584064,
      -15.7756519, -24.599472, 87.16716, -84.042038,
      -90.5412369, 87.529007, -25.688652, -55.1304588,
      -2.73058414, -43.9011269, -47.6373558, 30.712759,
      29.8273811, -72.5532837, -0.0219341293, -15.8854866,
      -27.5171661, 3.43458343, 29.7838745, 7.98985386,
      -60.4644012, -89.3960571, -75.3457794, -66.492218,
      14.1025009, 23.8203163, 38.7645187, 67.0146103,
      5.59591675, -59.4220734, 18.7374954, -56.263031,
      -0.807692111, 24.4699402, 29.376215, -69.5300751,
      -47.6169128, 68.3361511, 45.020752, 49.6524963,
      -75.5649872, 80.5972595, 85.3979492, -23.9187584,
      -56.6046143, -14.6239738, -34.7715416, 85.5595093,
      53.4591904, 59.6609192, 12.1146288, 33.3789139,
      70.2648468, -76.7637177, -46.6458664, 2.93262625,
      -47.9106903, -53.9646492, 74.9799423, 57.6852188,
      -48.7381516, 12.2827187, -12.110384, -44.5131264,
      58.5715065, 39.0846786, 55.6380692, -2.57798243,
      -52.1950531, -58.9570503, 33.5419388, 58.7073898,
      -78.3597946, 37.5051804, -81.9975281, 16.1515923,
      9.34863377, -16.7690544, -57.904789, -18.6268997,
      83.2759857, -59.322731, 36.5708618, -6.07505131,
      -52.8725891, -5.04228973, 61.5061569, 85.6841583,
      5.64164209, -6.34362984, 61.934639, -38.0638008,
      70.587677, 54.8569603, -42.1928101, -30.4566994,
      12.5069199, 78.0938492, 31.6138992, 24.9604492,
      -62.2979279, 19.0725403, -16.3321609, 45.8097076,
      43.0451469, -57.9455452, 66.993988, -29.0409431,
      -88.2471771, -4.44336128, -42.6351433, 59.6773796,
      19.4477673, 41.7981644, -46.3976669, 66.5751648,
      58.5747223, 55.4926186, -42.7079735, -17.2183609,
      -67.6371613, -62.2080536, -50.2494583, 43.3346329,
      -20.8836632, 58.6737404, 77.5265121, 56.271843,
      55.3328629, -33.6893158, -61.5974426, 14.8537827,
      -9.58407402, -72.8968964, 30.3447685, -83.3250732,
      -66.1938629, -45.4909668, -29.1521606, -71.2271881,
      33.2795258, 79.3287048, -6.51710129, -11.9212856,
      -38.489006, 47.085228, 24.3351784, -44.5628662,
      -57.0380249, -83.0213623, -92.0999451, -24.6751823,
      24.4518604, 44.1180229, 5.12687635, 68.2167664,
      56.7084618, -95.5277557, -18.341116, -47.5745544,
      -10.6517992, 3.27529979, -27.6880627, 27.6265659,
      -21.7591629, 67.3052826, -50.665802, -82.9203186,
      -42.8342628, -57.999115, -70.9336548, 55.0938454,
      -60.1051331, 22.549242, 48.2052612, 87.8733063,
      74.6671906, 7.89186382, -56.6895752, -47.0194283,
      94.5517731, 20.8917675, 79.4213943, 19.0036278,
      -16.4249344, -75.0676117, -55.6093292, -41.1511955,
      -70.5953674, -68.9177399, -24.0771637, 53.0827103,
      68.6874237, 12.8833504, -78.906601, 87.3123932,
      -44.0758209, -29.5724049, 69.0406494, -22.2615051,
      98.8959045, 19.9258423, 37.8650513, 25.2582016,
      47.5077972, 72.537735, 13.1315022, -56.0061798,
      -36.3565407, -38.5253754, 83.4418335, 44.6626663,
      65.5802383, 81.4284973, -95.949585, -15.4932613,
      71.009491, -68.345047, 26.5694027, 5.44683075,
      32.4184914, 7.52493858, -6.11904144, 36.4573441,
      -79.5917206, -1.81178999, 42.3350143, -40.4536743,
      68.6157913, -66.805481, -76.247757, 50.725563,
      36.3342133, -33.3499985, -83.632103, -58.9877243,
      -60.8122559, 12.7132568, -50.3453217, -75.3499451,
      57.401741, -48.3382225, 52.5265732, 22.9819946,
      19.5577164, 75.1422729, 90.7025757, -87.6139526,
      28.6160679, -60.9091644, 17.8328819, -38.965435,
      11.2643585, 59.5762749, 2.5246129, -63.2946472,
      -23.6702347, 31.3270607, 82.7191086, 66.7644119,
      47.7354279, 11.5040646, -42.125885, -40.1948166,
      -35.3785057, -61.1094131, -34.5339584, -12.9768972,
      -26.5773087, -79.8465729, -83.294136, -69.1755753,
      -44.970932, -8.9487133, -41.1608582, -3.59436941,
      88.0123978, 19.2230148, -7.99446392, -78.3388367,
      41.5277061, -71.5962982, 69.1631622, 39.2594986,
      -6.98732662, -9.74694443, 16.3489723, -8.8387146,
      -43.0684586, -17.7180595, 27.6069832, -8.86561394,
      -1.1812923, 7.29994106, 50.9395752, 63.4402008,
      29.4043884, -13.9130993, 55.4960098, -10.7054892,
      -10.5458155, 72.201889, 3.33280325, -33.6979027,
      49.7205887, -32.7953568, -32.2595711, 56.2982712,
      -78.2209167, 42.9598236, 77.9594498, 63.3067856,
      -42.1690598, 65.6878891, -57.0495834, -27.337534,
      42.408371, 7.16182327, 47.0375977, 63.9884872,
      29.8278885, -68.1851425, -26.3118458, -71.3534088,
      22.3286591, 29.6604309, -2.8804934, 38.2004738,
      -65.6873779, -47.3844833, -18.5883179, -35.8490829,
      11.2848196, 6.56333256, -83.0795593, -3.64316821,
      56.9818382, -32.125267, -28.7796211, 43.4094925,
      -53.8140259, 70.9986725, -52.8995934, 73.6981964,
      -41.4945908, -45.3005943, -35.0740585, 65.5623627,
      2.24535728, 33.7823944, -30.0650291, -67.9267578,
      48.8111191, 62.1884003, 47.1872711, -28.8602238,
      -86.3323288, -55.693222, 31.1590958, -33.4544373,
      -19.8638496, 77.2193604, 81.8124847, -73.2276154,
      53.463974, -82.7018051, -36.4866524, 45.7972336,
      -31.6132069, -43.4474258, -70.4091415, -63.6083832,
      75.413681, -58.6601486, -71.3449097, -66.0809021,
      65.7205429, -41.7703896, 39.8655777, -32.0340958,
      -2.95528412, -71.6341782, 69.7204285, -54.144165,
      -22.9783401, -42.7081757, -77.9716949, -26.0968094,
      -15.1875467, 1.04985034, 5.09733868, -13.2325516,
      -81.3466644, -72.7060623, -64.6413269, 72.1173096,
      -55.4078522, -53.2655334, -41.7013245, 12.97894,
      -13.4991026, -28.8966141, 32.584404, 48.3820114,
      -4.34290695, -52.2930794, 22.6852226, 83.1964874,
      -89.0307617, -54.2353363, 56.1950874, 8.01395035,
      -61.220932, 44.4808006, 18.5183678, -62.3804245,
      66.4211884, -54.42062, -5.26337147, 73.0524902,
      46.6292343, 4.86667156, -40.1800461, -12.8985844,
      32.160614, -56.9589272, 64.251442, 59.966629,
      76.2429657, -58.8846245, 77.9782715, -37.2561302,
      -23.1326523, -49.0531921, 51.510006, -41.0081329,
      -36.6976929, -25.8047714, 11.8696384, 60.7389679,
      19.9598904, -13.3699913, -48.033226, -19.4421921,
      52.929657, -7.69600964, 1.39123881, -23.478878,
      37.8833771, 1.16057181, -50.4263725, -75.1032562,
      -58.6213379, 79.0748444, 11.9981499, -21.428009,
      -42.5326462, -23.7504196, -42.8961029, 38.7430344,
      -60.8161926, 21.5495987, 6.51960897, -36.0864105,
      -41.036171, -41.9703903, -55.2756882, -72.7011642,
      15.4386978, 56.5939484, 71.2516632, 40.431282,
      -21.4246235, -41.4301376, 7.45652199, 17.9724655,
      34.0876999, 13.8804674, 77.7074432, -9.46364594,
      -84.9589691, -37.3675156, -62.7480621, 26.1009865,
      -58.2926483, -28.9310703, 69.3215485, 68.8560028,
      30.5323734, -3.8932662, -9.18712234, -43.816391,
      39.4751816, 2.36519003, -58.0839539, -82.9957275,
      78.960083, -30.1457825, -55.6968765, -48.4309616,
      12.9155979, -65.8799438, -21.532238, 74.7048187,
      43.8059273, -60.1590195, -66.9385681, -81.7222366,
      -41.2458496, -53.8797264, -69.3670502, -42.9909554,
      -8.03337669, -32.1151047, -30.4225407, -61.2933273,
      3.60240555, 43.9317169, 55.4251022, 20.6022072,
      -24.6101303, 68.056839, 95.3510895, 19.8977547,
      -72.4077148, -44.1675797, 36.9020386, -93.4476318,
      -87.845932, -13.7621355, 27.8026886, 43.6349411,
      71.4739227, -39.812851, -41.2760963, 33.8451309,
      51.1441345, -43.5661049, 52.122879, 74.546875,
      -14.231967, -12.2114601, 53.3747711, 60.9485092,
      20.3220139, 27.9849319, -13.8773832, -57.5103035,
      -28.0833549, -23.1008644, -31.8753891, 30.5203781
    };
  }

protected:
  virtual flit::Variant run_impl(const std::vector<T>& ti) {
    FLIT_UNUSED(ti);

    // write inputs to a temporary file
    flit::fsutil::TempFile tmpfile;
    std::vector<float> towrite(ti.begin(), ti.end());
    auto writer = fopen(tmpfile.name.c_str(), "w+");
    fwrite(towrite.data(), sizeof(float), towrite.size(), writer);
    fflush(writer);

    // read points from that temporary file
    convex_hull::CHullEdges.clear();
    convex_hull::PointList.clear();
    convex_hull::ReadInputs(writer);

    // perform the convex hull
    convex_hull::SimpleComputeConvexhull<T>();
    return convex_hull::getEdgeCount();
  }

protected:
  using flit::TestBase<T>::id;
};

REGISTER_TYPE(SimpleCHull)
